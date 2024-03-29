#include "pch.h"
#include "bot_initializer.h"
#include "detours.h"
#include "..\window\main_window.h"
#include "..\window\waiting_window.h"
#include "..\window\loading_window.h"
#include "..\window\addresses_offsets_window.h"
#include "../utils/string_utls.h"
#include "options/option_utils.h"
#include "../utils/trace.h"
#include "crash_handler.h"

#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/progressbar.h"
#include "gwinguiv2/message_box.h"
#include "gwinguiv2/gui.h"

#include "gwinmem/process_memory_internal.h"
#include "gwinmem/utils/safe_handle.h"

#include "logging.h"

namespace bot {

BotInitializer::BotInitializer() : has_removed_exception_handler_( false ) {}

ServerType DetectServer( const HWND target_window ) {
  const auto window_title = gwingui::control::GetText( target_window );

  std::wstring window_title_lower =
      stringutils::WideStringToLower( window_title );

  if ( window_title_lower.find( TEXT( "mazey" ) ) != std::wstring::npos ) {
    return ServerType::MazeyFlyff;
  } else if ( window_title_lower.find( TEXT( "ignite" ) ) !=
              std::wstring::npos ) {
    return ServerType::IgniteFlyff;
  } else if ( window_title_lower.find( TEXT( "dragon crusade" ) ) !=
              std::wstring::npos ) {
    return ServerType::DragonCrusade;
  } else if ( window_title_lower.find( TEXT( "eclipse flyff" ) ) !=
              std::wstring::npos ) {
    return ServerType::EclipseFlyff;
  }

  return ServerType::Unknown;
}

typedef struct _UNICODE_STRING {
  WORD Length;
  WORD MaximumLength;
  WORD* Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _LDR_DATA_TABLE_ENTRY {
  LIST_ENTRY InLoadOrderLinks;
  LIST_ENTRY InMemoryOrderLinks;
  LIST_ENTRY InInitializationOrderLinks;
  PVOID DllBase;
  PVOID EntryPoint;
  ULONG SizeOfImage;
  UNICODE_STRING FullDllName;
  UNICODE_STRING BaseDllName;
  ULONG Flags;
  WORD LoadCount;
  WORD TlsIndex;
  union {
    LIST_ENTRY HashLinks;
    struct {
      PVOID SectionPointer;
      ULONG CheckSum;
    };
  };
  union {
    ULONG TimeDateStamp;
    PVOID LoadedImports;
  };
  _ACTIVATION_CONTEXT* EntryPointActivationContext;
  PVOID PatchInformation;
  LIST_ENTRY ForwarderLinks;
  LIST_ENTRY ServiceTagLinks;
  LIST_ENTRY StaticLinks;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

bool GetClientServerTypeAndWindowHandle( ServerType* server_type,
                                         HWND* window_handle ) {
  std::vector<gwinmem::SearchConditionCallback_t> window_search_conditions = {
    []( const HWND window_handle ) -> bool {
      std::wstring classname( MAX_PATH, '\0' );
      GetClassName( window_handle, &classname[ 0 ], MAX_PATH );
      classname.resize( wcslen( classname.c_str() ) );
      if ( classname == TEXT( "D3D Window" ) ||
           classname == TEXT( "IgniteWindowClass" ) )
        return true;

      return false;
    },
  };

  const auto window_handles =
      gwinmem::CurrentProcess().FindWindowsCreatedByProcess(
          window_search_conditions );

  if ( window_handles.size() <= 0 )
    return false;

  // If the flyff server has more than one window, find out which one is correct
  for ( const auto window_handle_ : window_handles ) {
    const ServerType server_type_ = DetectServer( window_handle_ );

    if ( server_type_ != ServerType::Unknown ) {
      *window_handle = window_handle_;
      *server_type = server_type_;

      return true;
    }
  }

  *window_handle = 0;
  *server_type = ServerType::Unknown;

  return false;
}

void OpenConsole() {
  // Create the console after we have detected the server, otherwise they can
  // collide and read the title of console by mistake
  static bool allocate_console = []() -> bool {
    AllocConsole();
    freopen( "CONOUT$", "w", stdout );
    return true;
  }();
}

// Why do I declare as global? Because ManualMapHandleStaticTlsData requires the same address of the entry
gwinmem::LDR_DATA_TABLE_ENTRY g_entry;

int BotInitializer::Load( HINSTANCE instance_handle,
                          const uint32_t reserved_value ) {
  try {
    g_entry.DllBase = instance_handle;

    // Initializing the static tls on a manual mapped dll fixes the crashing issues with using the static keyword
    // Previously I was required to use /GL (Whole Program Optimization)
    //
    auto static_tls_succeeded =
        gwinmem::CurrentProcess().ManualMapHandleStaticTlsData( &g_entry );

    if ( !static_tls_succeeded ) {
      gwingui::messagebox::Error(
          TEXT( "Failed to initialize static tls data" ) );
      return -1;
    }

    static std::vector<int> test_static_tls_data_temp_var = { 1337, 1338 };

    // If the static tls initialization call above does not work properly, inform us about that shits
    //
    if ( test_static_tls_data_temp_var.size() == 0 ) {
      gwingui::messagebox::Error(
          TEXT( "Static tls data is not working properly, have you initialized "
                "it?" ) );
      return -1;
    }

#if ENABLE_PROTECTION

    // Check if the value in the dll header is 1
    uint8_t* dll_header_data = reinterpret_cast<uint8_t*>( instance_handle );

    auto dos_headers = reinterpret_cast<IMAGE_DOS_HEADER*>( dll_header_data );

    auto nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(
        dll_header_data + dos_headers->e_lfanew );

    DWORD old_protection;
    VirtualProtect( &nt_headers->OptionalHeader.CheckSum,
                    sizeof( nt_headers->OptionalHeader.CheckSum ),
                    PAGE_READWRITE, &old_protection );

    // If the checksum is not 0, then the dll has been dumped
    if ( nt_headers->OptionalHeader.CheckSum != 0 ) {
      botcore_.bad_boy_present_ = true;
    }

    // Modify a dll header to indicate that we are running, if someone dumps the
    // dll and tries to inject it, it won't work =)
    nt_headers->OptionalHeader.CheckSum = 1;

    VirtualProtect( &nt_headers->OptionalHeader.CheckSum,
                    sizeof( nt_headers->OptionalHeader.CheckSum ),
                    old_protection, &old_protection );

    // When we manual map out dll, we use the value 1 for the reserved parameter
    // in dllmain. If the reserved value is not 1, then someone tries to inject
    // without my injector.
    if ( reserved_value != 1 ) {
      botcore_.bad_boy_present_ = true;
    }
#endif

    ServerType server_type;
    HWND target_window_handle;

    if ( !GetClientServerTypeAndWindowHandle( &server_type,
                                              &target_window_handle ) ) {
      gwingui::messagebox::Error( TEXT(
          "Cannot identify the server type or get the target window. Is the "
          "server supported?" ) );
      return -1;
    }

    botcore_.client_ = flyff_client_factory::CreateFlyffClient( server_type );
    botcore_.target_window_handle_ = target_window_handle;

    botcore_.kb_hook_handle_ = SetWindowsHookEx(
        WH_KEYBOARD_LL, BotCore::KeyboardCallback, NULL, NULL );

    if ( !botcore_.kb_hook_handle_ ) {
      gwingui::messagebox::Error(
          TEXT( "Failed to hook keyboard with error code: " ) +
          std::to_wstring( GetLastError() ) );
      return -1;
    }

    assert( botcore_.client_ != nullptr );

    // Initialize the flyff client e.g. ignite flyff has integrity checks to be
    // bypassed
    if ( !botcore_.client_->Initialize() ) {
      gwingui::messagebox::Error(
          TEXT( "The flyff client failed to initialize properly." ) );
      return -1;
    }

#if ENABLE_DEBUGGING
    OpenConsole();

    GWIN_TRACE( "Console had been opened" );
#endif

    instance_handle_ = instance_handle;

    const auto main_window_thread_id =
        GetWindowThreadProcessId( target_window_handle, 0 );

    if ( !main_window_thread_id ) {
      gwingui::messagebox::Error(
          TEXT( "The main window thread is 0, with the target handle: " ) +
          std::to_wstring(
              reinterpret_cast<uintptr_t>( target_window_handle ) ) +
          TEXT( "WindowText: " ) +
          gwingui::control::GetText( target_window_handle ) );

      return -1;
    }

    HMODULE crash_rpt_module = GetModuleHandle( TEXT( "CrashRpt1403.dll" ) );

    // TODO: do it properly
    if ( !crash_rpt_module ) {
      crash_rpt_module = GetModuleHandle( TEXT( "CrashRpt1402.dll" ) );
    }

    // TODO: Find a better way to unregister their exception handler, probably by thread hijacking
    // TODO: instead of checking if the module is there, iterate all the modules and find any exports that has "crUninstall"
    // can be as simple as iterate modules, and then call getprocaddress on each module to see if crUninstall exists
    // if so, unregister it

    // Check if the target client has a known crash reporter module
    if ( crash_rpt_module ) {
      // Unregister the existing exception handler, that is required to make my
      // handlers catch everything.
      if ( !UnregisterExceptionHandler( main_window_thread_id ) ) {
        gwingui::messagebox::Error(
            TEXT( "Unable to un-register the existing exception handler." ) );
        return -1;
      }
    }

    // Bypasses the RtlIsValidHandler check, it should always let any exception
    // handler through.
    if ( !gwinmem::CurrentProcess().ManualMapFixExceptionHandling() ) {
      gwingui::messagebox::Error(
          TEXT( "Unable to fix the exception handling." ) );
      return -1;
    }

    GWIN_TRACE( "Starting MM free thread\n" );

    // Crashes in debug mode without Whole Program Optimization = On
    // Start a thread that run until this thread is finished, then it free's the
    // memory
    gwinmem::CurrentProcess().ManualMapStartFreeDllThread(
        reinterpret_cast<uintptr_t>( instance_handle ) );

    GWIN_TRACE( "Replacing exception handler\n" );
  } catch ( ... ) {
    gwingui::messagebox::Error(
        TEXT( "An exception occured during initialization before our exception "
              "handler has been added." ) );
  }

  // Why not SetUnhandledExceptionFilter()?
  // Because it has worse performance..and I need the performance because I
  // use the exception handler to catch the breakpoints I put out
  // On the other hand:
  // Why only use AddVectoredExceptionHandler() for breakpoint hooks?
  // Because it catches everything BEFORE seh meaning that we cannot use
  // basic try, catch to handle exceptions because veh is called immediately
  prev_filter_ =
      AddVectoredExceptionHandler( 1, crash_handler::MainExceptionHandlerBp );
  prev_filter_2_ =
      SetUnhandledExceptionFilter( crash_handler::MainExceptionHandler );

  GWIN_TRACE( "Prev Filter: %d\n", reinterpret_cast<int>( prev_filter_2_ ) );

  gwingui::Gui gui( instance_handle );

  const auto loading_window_handle =
      gui.AddWindow( DIALOG_LOADING, nullptr, new LoadingWindow );

  const HWND mainwindow_handle =
      gui.AddWindow( DIALOG_MAIN, nullptr, new MainWindow );

  gui.AddWindow( DIALOG_WAITING, mainwindow_handle, new WaitingWindow );

  gui.AddWindow( DIALOG_ADDRESSES, nullptr, new AddressesOffsetsWindow );

  PostGuiCreation( loading_window_handle );

  // Begin the window message loop
  gui.Run();

  // Once the message loop has stopped, unload the bot
  if ( !bot::Initializer().Unload(
           mainwindow_handle,
           reinterpret_cast<uintptr_t>( instance_handle ) ) ) {
    gwingui::messagebox::Error(
        TEXT( "Error while attempting to unload the bot." ) );
    return -1;
  }

  return 0;
}

bool BotInitializer::Unload( const HWND mainwindow_handle,
                             const uintptr_t dllbase ) {
  // Wait for all the hooked functions to not execute
  std::lock_guard<std::mutex> hooks_lock_guard( g_hooks_mutex );

  // Free the static tls data, otherwise the process will get fucked (crash) when unloading the bot
  //
  const auto free_static_tls_result =
      gwinmem::CurrentProcess().ManualMapFreeStaticTlsData( &g_entry );

  if ( !free_static_tls_result ) {
    gwingui::messagebox::Error( TEXT( "Failed to free static tls data: " ) );
    return false;
  }

  // Give the hooked functions a chance to completely stop executing
  // without this it might crash
  Sleep( 500 );

  UnhookWindowsHookEx( botcore_.kb_hook_handle_ );

  LogQueue().Exit();

  botcore_.GetFlyffClient()->PreHookDeInitialization();

  // Unload stuff specific to choosen server
  botcore_.GetFlyffClient()->OnExit();

  botcore_.GetFlyffClient()->RemoveDefaultHooks();

  botcore_.GetFlyffClient()->PostHookDeinitialization();

  // Call this in the thread that we called ManualMapFixExceptionHandling() in
  gwinmem::CurrentProcess().ManualMapResetExceptionHandling();

  RemoveVectoredExceptionHandler( prev_filter_ );
  SetUnhandledExceptionFilter( prev_filter_2_ );

  // Wait for the detoured funtions to finish before removing the code
  Sleep( 100 );

  return true;
}

void BotInitializer::PostGuiCreation( const HWND loading_window_handle ) {
  const auto& flyff_client = botcore_.client_;

  if ( !flyff_client->Search( loading_window_handle ) )
    gwingui::messagebox::Error( TEXT( "Unable to search for addresses" ) );

  flyff_client->PreHookInitialization();

  flyff_client->InitializeDefaultHooks();

  flyff_client->PostHookInitialization();

  const auto account_name_addr =
      flyff_client->GetClientVar( MemoryContants::kAccountNameAddress );

  std::string account_name = "";

  try {
    account_name =
        gwinmem::CurrentProcess().ReadString( account_name_addr, 200 );
  } catch ( gwinmem::BadMemoryException ) {
    gwingui::messagebox::Error( TEXT( "Failed to read the account name." ) );
  }

  auto local_player = flyff_client->CreateLocalPlayer();

  std::string character_name = "";

  if ( local_player ) {
    if ( !local_player->IsDeletedOrInvalidMemory() )
      character_name = local_player->GetName();
  }

  std::wstring title = stringutils::AnsiToWide( character_name ) +
                       TEXT( " - " ) + stringutils::AnsiToWide( account_name ) +
                       TEXT( " - RIMJOB" );
  // TEXT( " - RIMJOB - RIghteously Mighty JOllygood Bot" );

  auto main_window_handle = GWH( DIALOG_MAIN );
  gwingui::control::SetText( main_window_handle, title );

  ShowWindow( loading_window_handle, SW_HIDE );

  ShowWindow( main_window_handle, SW_SHOW );
  SetForegroundWindow( main_window_handle );

  auto& bot_options = botcore_.GetBotOptions();
  bot_options.LoadOptions( character_name );
}

bool BotInitializer::UnregisterExceptionHandler(
    uint32_t thread_id_of_ex_handlers ) {
  thread_id_of_ex_handlers_ = thread_id_of_ex_handlers;

  DetourTransactionBegin();

  DetourUpdateThread( GetCurrentThread() );

  auto user32 = GetModuleHandle( TEXT( "user32.dll" ) );

  if ( !user32 ) {
    gwingui::messagebox::Error(
        TEXT( "user32.dll is not loaded into memory." ) );
    return false;
  }

  //copy_rect_original_ = reinterpret_cast<decltype( CopyRect )*>(
  //    GetProcAddress( user32, "CopyRect" ) );
  //def_window_proc_a_original_ = reinterpret_cast<decltype( DefWindowProcA )*>(
  //    GetProcAddress( user32, "DefWindowProcA" ) );
  peek_message_a_original_ = reinterpret_cast<decltype( PeekMessageA )*>(
      GetProcAddress( user32, "PeekMessageA" ) );

  //if ( !copy_rect_original_ )
  //  return false;

  if ( !peek_message_a_original_ )
    return false;

  //DetourAttach( reinterpret_cast<PVOID*>( &copy_rect_original_ ),
  //              BotInitializer::CopyRectHooked );
  DetourAttach( reinterpret_cast<PVOID*>( &peek_message_a_original_ ),
                BotInitializer::PeekMessageAHooked );

  if ( DetourTransactionCommit() != NO_ERROR ) {
    gwingui::messagebox::Error(
        TEXT( "Error while trying to hook NtQueryInformationProcess." ) );
    return false;
  }

  while ( !has_removed_exception_handler_ )
    Sleep( 50 );

  return true;
}

LRESULT WINAPI BotInitializer::DefWindowProcAHooked( HWND window_handle,
                                                     UINT message,
                                                     WPARAM wparam,
                                                     LPARAM lparam ) {
  if ( GetCurrentThreadId() == Initializer().thread_id_of_ex_handlers_ ) {
    HMODULE crash_rpt_module = GetModuleHandle( TEXT( "CrashRpt1403.dll" ) );

    // TODO: do it properly
    if ( !crash_rpt_module ) {
      crash_rpt_module = GetModuleHandle( TEXT( "CrashRpt1402.dll" ) );
    }

    if ( crash_rpt_module ) {
      using tCrUninstall = int ( * )();
      using tCrGetLastErrorMsgW = int ( * )( LPWSTR pszBuffer, UINT uBuffSize );

      const auto cr_uninstall = reinterpret_cast<tCrUninstall>(
          GetProcAddress( crash_rpt_module, "crUninstall" ) );

      /*
      const auto cr_get_last_error_msg_w =
          reinterpret_cast<tCrGetLastErrorMsgW>(
              GetProcAddress( crash_rpt_module, "crGetLastErrorMsgW" ) );
      */

      const auto result = cr_uninstall();

      /*
      if ( result != 0 ) {
        wchar_t error_message[ 256 ];
        cr_get_last_error_msg_w( error_message, sizeof( error_message ) );

        gwingui::messagebox::Error(
            TEXT( "Failed to remove crash handler.\ncrUninstall returned "
                  "with " ) +
            std::to_wstring( result ) + TEXT( "\nError message: " ) +
            std::wstring( error_message ) );
      }
      */

      GWIN_TRACE( "Removed the crash handler with return code: %d\n", result );
    } else {
      GWIN_TRACE( "Crash handler is not found.\n" );
    }

    DetourTransactionBegin();

    DetourUpdateThread( GetCurrentThread() );

    auto user32 = GetModuleHandle( TEXT( "user32.dll" ) );

    if ( !user32 ) {
      gwingui::messagebox::Error(
          TEXT( "user32.dll is not loaded into memory." ) );
      return -1;
    }

    DetourDetach(
        reinterpret_cast<PVOID*>( &Initializer().def_window_proc_a_original_ ),
        BotInitializer::DefWindowProcAHooked );

    if ( DetourTransactionCommit() != NO_ERROR ) {
      gwingui::messagebox::Error( TEXT( "Error while trying to hook." ) );
      return false;
    }

    Initializer().has_removed_exception_handler_ = true;
  }

  return Initializer().def_window_proc_a_original_( window_handle, message,
                                                    wparam, lparam );
}

BOOL WINAPI BotInitializer::PeekMessageAHooked( LPMSG message,
                                                HWND window_handle,
                                                UINT msg_filter_min,
                                                UINT msg_filter_max,
                                                UINT remove_msg ) {
  if ( GetCurrentThreadId() == Initializer().thread_id_of_ex_handlers_ ) {
    HMODULE crash_rpt_module = GetModuleHandle( TEXT( "CrashRpt1403.dll" ) );

    // TODO: do it properly
    if ( !crash_rpt_module ) {
      crash_rpt_module = GetModuleHandle( TEXT( "CrashRpt1402.dll" ) );
    }

    if ( crash_rpt_module ) {
      using tCrUninstall = int ( * )();
      using tCrGetLastErrorMsgW = int ( * )( LPWSTR pszBuffer, UINT uBuffSize );

      const auto cr_uninstall = reinterpret_cast<tCrUninstall>(
          GetProcAddress( crash_rpt_module, "crUninstall" ) );

      /*
      const auto cr_get_last_error_msg_w =
          reinterpret_cast<tCrGetLastErrorMsgW>(
              GetProcAddress( crash_rpt_module, "crGetLastErrorMsgW" ) );
      */

      const auto result = cr_uninstall();

      /*
      if ( result != 0 ) {
        wchar_t error_message[ 256 ];
        cr_get_last_error_msg_w( error_message, sizeof( error_message ) );

        gwingui::messagebox::Error(
            TEXT( "Failed to remove crash handler.\ncrUninstall returned "
                  "with " ) +
            std::to_wstring( result ) + TEXT( "\nError message: " ) +
            std::wstring( error_message ) );
      }
      */

      GWIN_TRACE( "Removed the crash handler with return code: %d\n", result );
    } else {
      GWIN_TRACE( "Crash handler is not found.\n" );
    }

    DetourTransactionBegin();

    DetourUpdateThread( GetCurrentThread() );

    auto user32 = GetModuleHandle( TEXT( "user32.dll" ) );

    if ( !user32 ) {
      gwingui::messagebox::Error(
          TEXT( "user32.dll is not loaded into memory." ) );
      return -1;
    }

    DetourDetach(
        reinterpret_cast<PVOID*>( &Initializer().peek_message_a_original_ ),
        BotInitializer::PeekMessageAHooked );

    if ( DetourTransactionCommit() != NO_ERROR ) {
      gwingui::messagebox::Error( TEXT( "Error while trying to hook." ) );
      return false;
    }

    Initializer().has_removed_exception_handler_ = true;
  }

  return Initializer().peek_message_a_original_(
      message, window_handle, msg_filter_min, msg_filter_max, remove_msg );
}

BOOL WINAPI BotInitializer::CopyRectHooked( LPRECT destination,
                                            const RECT* source ) {
  gwingui::messagebox::Warning(
      std::to_wstring( GetCurrentThreadId() ) + TEXT( " : " ) +
      std::to_wstring( Initializer().thread_id_of_ex_handlers_ ) );

  if ( GetCurrentThreadId() == Initializer().thread_id_of_ex_handlers_ ) {
    HMODULE crash_rpt_module = GetModuleHandle( TEXT( "CrashRpt1403.dll" ) );

    // TODO: do it properly
    if ( !crash_rpt_module ) {
      crash_rpt_module = GetModuleHandle( TEXT( "CrashRpt1402.dll" ) );
    }

    if ( crash_rpt_module ) {
      using tCrUninstall = int ( * )();
      using tCrGetLastErrorMsgW = int ( * )( LPWSTR pszBuffer, UINT uBuffSize );

      const auto cr_uninstall = reinterpret_cast<tCrUninstall>(
          GetProcAddress( crash_rpt_module, "crUninstall" ) );

      /*
      const auto cr_get_last_error_msg_w =
          reinterpret_cast<tCrGetLastErrorMsgW>(
              GetProcAddress( crash_rpt_module, "crGetLastErrorMsgW" ) );
      */

      const auto result = cr_uninstall();

      /*
      if ( result != 0 ) {
        wchar_t error_message[ 256 ];
        cr_get_last_error_msg_w( error_message, sizeof( error_message ) );

        gwingui::messagebox::Error(
            TEXT( "Failed to remove crash handler.\ncrUninstall returned "
                  "with " ) +
            std::to_wstring( result ) + TEXT( "\nError message: " ) +
            std::wstring( error_message ) );
      }
      */

      GWIN_TRACE( "Removed the crash handler with return code: %d\n", result );
    } else {
      GWIN_TRACE( "Crash handler is not found.\n" );
    }

    DetourTransactionBegin();

    DetourUpdateThread( GetCurrentThread() );

    auto user32 = GetModuleHandle( TEXT( "user32.dll" ) );

    if ( !user32 ) {
      gwingui::messagebox::Error(
          TEXT( "user32.dll is not loaded into memory." ) );
      return -1;
    }

    DetourDetach(
        reinterpret_cast<PVOID*>( &Initializer().copy_rect_original_ ),
        BotInitializer::CopyRectHooked );

    if ( DetourTransactionCommit() != NO_ERROR ) {
      gwingui::messagebox::Error( TEXT( "Error while trying to hook." ) );
      return false;
    }

    Initializer().has_removed_exception_handler_ = true;
  } else {
    gwingui::messagebox::Error(
        TEXT( "Oopsi doopsie, something went horribly wrong. Please restart "
              "the game and try again." ) );
  }

  return Initializer().copy_rect_original_( destination, source );
}

BotInitializer g_init;

BotInitializer& BotInitializer::Instance() {
  return g_init;
}

HINSTANCE BotInitializer::GetInstance() {
  return instance_handle_;
}

BotCore* BotInitializer::GetBotCore() {
  return &botcore_;
}

}  // namespace bot
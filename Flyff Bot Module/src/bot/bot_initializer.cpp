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

  if ( window_title_lower.find( TEXT( "mazey" ) ) != std::wstring::npos )
    return ServerType::MazeyFlyff;
  else if ( window_title_lower.find( TEXT( "ignite" ) ) != std::wstring::npos )
    return ServerType::IgniteFlyff;
  else if ( window_title_lower.find( TEXT( "dragon crusade" ) ) !=
            std::wstring::npos )
    return ServerType::DragonCrusade;

  return ServerType::Unknown;
}

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

int BotInitializer::Load( HINSTANCE instance_handle,
                          const uint32_t reserved_value ) {
  // Currently not used because not needed. I tried this to fix the crash issues
  // when using static without /GL (Whole Program Optimization). It still
  // crashed. fuckin assshit.
#if 0
  const auto status = LOL( reinterpret_cast<uintptr_t>( instance_handle ) );

  gwingui::messagebox::Error( std::to_wstring( status ) );

  if ( status >= 0 ) {
    gwingui::messagebox::Error( TEXT( "success" ) );
  } else {
    gwingui::messagebox::Error( TEXT( "failed" ) );
  }
#endif

#if ENABLE_PROTECTION

  // Check if the value in the dll header is 1
  uint8_t* dll_header_data = reinterpret_cast<uint8_t*>( instance_handle );

  auto dos_headers = reinterpret_cast<IMAGE_DOS_HEADER*>( dll_header_data );

  auto nt_headers = reinterpret_cast<IMAGE_NT_HEADERS*>(
      dll_header_data + dos_headers->e_lfanew );

  DWORD old_protection;
  VirtualProtect( &nt_headers->OptionalHeader.CheckSum,
                  sizeof( nt_headers->OptionalHeader.CheckSum ), PAGE_READWRITE,
                  &old_protection );

  // If the checksum is not 0, then the dll has been dumped
  if ( nt_headers->OptionalHeader.CheckSum != 0 ) {
    botcore_.bad_boy_present_ = true;
  }

  // Modify a dll header to indicate that we are running, if someone dumps the
  // dll and tries to inject it, it won't work =)
  nt_headers->OptionalHeader.CheckSum = 1;

  VirtualProtect( &nt_headers->OptionalHeader.CheckSum,
                  sizeof( nt_headers->OptionalHeader.CheckSum ), old_protection,
                  &old_protection );

  // When we manual map out dll, we use the value 1 for the reserved parameter
  // in dllmain. If the reserved value is not 1, then someone tries to inject
  // without my injector.
  if ( reserved_value != 1 ) {
    botcore_.bad_boy_present_ = true;
  }
#endif

  // TODO: modify most of my code with std functions

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

  botcore_.kb_hook_handle_ =
      SetWindowsHookEx( WH_KEYBOARD_LL, BotCore::KeyboardCallback, NULL, NULL );

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
#endif

  instance_handle_ = instance_handle;

  const auto main_window_thread_id =
      GetWindowThreadProcessId( target_window_handle, 0 );

  if ( !main_window_thread_id ) {
    gwingui::messagebox::Error(
        TEXT( "The main window thread is 0, with the target handle: " ) +
        std::to_wstring( reinterpret_cast<uintptr_t>( target_window_handle ) ) +
        TEXT( "WindowText: " ) +
        gwingui::control::GetText( target_window_handle ) );

    return -1;
  }

  const HMODULE crash_rpt_module =
      GetModuleHandle( TEXT( "CrashRpt1403.dll" ) );

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

  // Crashes in debug mode without Whole Program Optimization = On
  // Start a thread that run until this thread is finished, then it free's the
  // memory
  gwinmem::CurrentProcess().ManualMapStartFreeDllThread(
      reinterpret_cast<uintptr_t>( instance_handle ) );

  prev_filter_ =
      SetUnhandledExceptionFilter( crash_handler::MainExceptionHandler );

  GWIN_TRACE( "Prev Filter: %d\n", reinterpret_cast<int>( prev_filter_ ) );

  // gwinmem::ProcessInternal().ManualMapFixStaticTls((uintptr_t)instance_handle);

  // TODO: Fix static tls, easiest with LdrpHandleTlsData
  // https://github.com/DarthTon/Blackbone/blob/ae70e7059a1fbc6c1f101e59d5fe201c93676c99/src/BlackBone/Symbols/PatternLoader.cpp#L70
  // __stdcall turned into __thiscall
  // https://www.unknowncheats.me/forum/general-programming-and-reversing/232300-fixing-static-tls.html
  // use pdb to get the address:
  // https://www.unknowncheats.me/forum/c-and-c-/248123-pdbparse.html

  gwingui::Gui gui( instance_handle );

  const auto loading_window_handle =
      gui.AddWindow( DIALOG_LOADING, nullptr, new LoadingWindow );

  const HWND mainwindow_handle =
      gui.AddWindow( DIALOG_MAIN, nullptr, new MainWindow );

  gui.AddWindow( DIALOG_WAITING, mainwindow_handle, new WaitingWindow );

  gui.AddWindow( DIALOG_ADDRESSES, nullptr, new AddressesOffsetsWindow );

  try {
    PostGuiCreation( loading_window_handle );

    // Begin the window message loop
    gui.Run();

    // Once the message loop has stopped, unload the bot
    if ( !bot::Initializer().Unload( mainwindow_handle ) ) {
      gwingui::messagebox::Error(
          TEXT( "Error while attempting to unload the bot." ) );
      return -1;
    }
  } catch ( std::exception ex ) {
    gwingui::messagebox::Error( stringutils::AnsiToWide( ex.what() ) );
    return -1;
  } catch ( ... ) {
    return -1;
  }

  return 0;
}

bool BotInitializer::Unload( const HWND mainwindow_handle ) {
  // Wait for all the hooked functions to not execute
  std::lock_guard<std::mutex> hooks_lock_guard( g_hooks_mutex );

  // Give the hooked functions a chance to completely stop executing
  // without this it might crash
  Sleep( 500 );

  UnhookWindowsHookEx( botcore_.kb_hook_handle_ );

  LogQueue().Exit();

  botcore_.GetFlyffClient()->PreHookDeInitialization();

  // Unload stuff specific to choosen server
  botcore_.GetFlyffClient()->OnExit();

  botcore_.GetFlyffClient()->RemoveDefaultHooks();

  /*
  DetourTransactionBegin();

  DetourUpdateThread( GetCurrentThread() );

  // TODO: consider making a hook_manager class to keep all hooks in one list
  // that a FlyffClient can add onto

  DetourDetach( reinterpret_cast<PVOID*>( &botcore_.endscene_original_ ),
                BotCore::EndSceneHooked );
  DetourDetach( reinterpret_cast<PVOID*>( &botcore_.get_cursor_pos_original_ ),
                BotCore::GetCursorPosHooked );
  DetourDetach( reinterpret_cast<PVOID*>( &botcore_.get_key_state_original_ ),
                BotCore::GetKeyStateHooked );

  if ( DetourTransactionCommit() != NO_ERROR ) {
    gwingui::messagebox::Error(
        TEXT( "Error while trying to detach all hooks." ) );
    return false;
  }
  */

  botcore_.GetFlyffClient()->PostHookDeinitialization();

  // Call this in the thread that we called ManualMapFixExceptionHandling() in
  gwinmem::CurrentProcess().ManualMapResetExceptionHandling();

  SetUnhandledExceptionFilter( prev_filter_ );

  // Wait for the detoured funtions to finish before removing the code
  Sleep( 100 );

  return true;
}

// LPDIRECT3DDEVICE9 CreateD3DDevice() {
//  D3DPRESENT_PARAMETERS present_params = { 0 };
//
//  present_params.BackBufferWidth = 1;
//  present_params.BackBufferHeight = 1;
//  present_params.BackBufferFormat = D3DFMT_UNKNOWN;
//
//  present_params.Windowed = TRUE;
//  present_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
//
//  present_params.EnableAutoDepthStencil = TRUE;
//  present_params.AutoDepthStencilFormat = D3DFMT_D16;
//
//  LPDIRECT3DDEVICE9 device_temp = NULL;
//
//  Direct3DCreate9( D3D_SDK_VERSION )
//      ->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 0,
//                      D3DCREATE_HARDWARE_VERTEXPROCESSING, &present_params,
//                      &device_temp );
//
//  return device_temp;
//}

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

  copy_rect_original_ = reinterpret_cast<decltype( CopyRect )*>(
      GetProcAddress( user32, "CopyRect" ) );

  if ( !copy_rect_original_ )
    return false;

  DetourAttach( reinterpret_cast<PVOID*>( &copy_rect_original_ ),
                BotInitializer::CopyRectHooked );

  if ( DetourTransactionCommit() != NO_ERROR ) {
    gwingui::messagebox::Error(
        TEXT( "Error while trying to hook NtQueryInformationProcess." ) );
    return false;
  }

  while ( !has_removed_exception_handler_ )
    Sleep( 50 );

  return true;
}

BOOL WINAPI BotInitializer::CopyRectHooked( LPRECT destination,
                                            const RECT* source ) {
  if ( GetCurrentThreadId() == Initializer().thread_id_of_ex_handlers_ ) {
    HMODULE crash_rpt_module = GetModuleHandle( TEXT( "CrashRpt1403.dll" ) );

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
      gwingui::messagebox::Error(
          TEXT( "Error while trying to hook NtQueryInformationProcess." ) );
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

struct PdbInfo {
  DWORD signature;
  GUID guid;
  DWORD age;
  char pdb_filename[ 1 ];
};

std::string GuidToString( const GUID& guid ) {
  char guid_string[ 37 ];  // 32 hex chars + 4 hyphens + null terminator
  snprintf( guid_string, sizeof( guid_string ),
            "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x", guid.Data1,
            guid.Data2, guid.Data3, guid.Data4[ 0 ], guid.Data4[ 1 ],
            guid.Data4[ 2 ], guid.Data4[ 3 ], guid.Data4[ 4 ], guid.Data4[ 5 ],
            guid.Data4[ 6 ], guid.Data4[ 7 ] );
  return guid_string;
}

PdbInfo* GetPdbFromModule( const uintptr_t module_address ) {
  const auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>( module_address );

  if ( dos_header->e_magic != IMAGE_DOS_SIGNATURE )
    return nullptr;

  const auto nt_header = reinterpret_cast<IMAGE_NT_HEADERS*>(
      module_address + dos_header->e_lfanew );

  const auto debug_data_directory =
      &nt_header->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_DEBUG ];

  if ( !debug_data_directory->Size )
    return nullptr;

  const auto debug_directory = reinterpret_cast<IMAGE_DEBUG_DIRECTORY*>(
      module_address + debug_data_directory->VirtualAddress );

  if ( IMAGE_DEBUG_TYPE_CODEVIEW == debug_directory->Type ) {
    const auto pdb_info = reinterpret_cast<PdbInfo*>(
        module_address + debug_directory->AddressOfRawData );

    return pdb_info;
  }

  return nullptr;
}

NTSTATUS LOL( const uintptr_t dllbase ) {
  const auto ntdll = LoadLibrary( TEXT( "ntdll.dll" ) );

  const auto pdb_info =
      GetPdbFromModule( reinterpret_cast<uintptr_t>( ntdll ) );

  printf( "PDB path: %s\n", pdb_info->pdb_filename );
  printf( "dicks: %s", GuidToString( pdb_info->guid ).c_str() );

  std::string pdb_path = std::string( pdb_info->pdb_filename ) + "/" +
                         GuidToString( pdb_info->guid ) +
                         std::to_string( pdb_info->age ) + "/" +
                         std::string( pdb_info->pdb_filename );

  const auto symbol_server_url = "http://msdl.microsoft.com/download/symbols/";

  const auto result =
      URLDownloadToFileA( nullptr, ( symbol_server_url + pdb_path ).c_str(),
                          "D:\\test.pdb", 0, nullptr );

  if ( result != S_OK )
    return 1;

  // Get better debugging error messages
  // SymSetOptions( SymGetOptions() | SYMOPT_DEBUG );

  if ( !SymInitialize( GetCurrentProcess(), NULL, FALSE ) )
    return 1;

  const auto kDummyBaseAddress = 0x10000000;

  const auto pdb_file_handle = CreateFile( TEXT( "D:\\test.pdb" ), GENERIC_READ,
                                           0, NULL, OPEN_EXISTING, 0, NULL );

  const auto pdb_file_size = GetFileSize( pdb_file_handle, NULL );

  CloseHandle( pdb_file_handle );

  const auto addr =
      SymLoadModuleEx( GetCurrentProcess(), NULL, "D:\\test.pdb", NULL,
                       kDummyBaseAddress, pdb_file_size, NULL, 0 );

  if ( !addr )
    return 1;

  char lol[ MAX_SYM_NAME ];

  SYMBOL_INFO sym_info;

  sym_info.SizeOfStruct = sizeof( SYMBOL_INFO );
  sym_info.MaxNameLen = MAX_SYM_NAME;
  *reinterpret_cast<char*>( sym_info.Name ) = lol[ 0 ];

  if ( !SymFromName( GetCurrentProcess(), "LdrpHandleTlsData", &sym_info ) )
    return 1;

  printf( "addr: %I64d\n", sym_info.Address );

  SymUnloadModule64( GetCurrentProcess(), addr );

  if ( !SymCleanup( GetCurrentProcess() ) )
    return 1;

  using LdrpHandleTlsData_stdcall_t =
      NTSTATUS( __stdcall* )( PLDR_DATA_TABLE_ENTRY ModuleEntry );

  using LdrpHandleTlsData_thiscall_t =
      NTSTATUS( __thiscall* )( PLDR_DATA_TABLE_ENTRY ModuleEntry );

  const auto rva = sym_info.Address - kDummyBaseAddress;

  const auto func = reinterpret_cast<uint64_t>( ntdll ) + rva;

  const auto func_typed =
      reinterpret_cast<LdrpHandleTlsData_thiscall_t>( func );

  LDR_DATA_TABLE_ENTRY entr = {};

  entr.DllBase = reinterpret_cast<PVOID>( dllbase );

  // TODO: either use the manual mapped dll loader... or only fill dllbase..?

  return func_typed( &entr );
}

HINSTANCE BotInitializer::GetInstance() {
  return instance_handle_;
}

BotCore* BotInitializer::GetBotCore() {
  return &botcore_;
}

}  // namespace bot
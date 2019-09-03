#include "loader_core.h"

// #include "gwin\file_management\file_management_simple.h"
// #include "gwin\win32\gui\controls\ControlC.h"
// #include "gwin\win32\net\HttpWebRequest.h"
// #include "gwin\win32\process\ProcessEnumerator.h"
// #include "gwin\win32\process\ProcessMemory.h"

#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/message_box.h"

#include "gwinmem/processes.h"
#include "gwinmem/process_memory_external.h"
#include "gwinmem/utils/safe_handle.h"

#include <string>
#include <assert.h>

namespace loader {

std::vector<Process> GetRunningProcessesByNames(
    const std::initializer_list<std::wstring> process_names ) {
  const auto processes = gwinmem::processes::GetProcesses();

  std::vector<Process> processes_out;

  for ( auto& process : processes ) {
    for ( auto& name : process_names ) {
      if ( std::wstring( process.szExeFile ) == name ) {
        auto process_memory =
            gwinmem::processmemoryexternal::OpenExternalProcess(
                process.th32ProcessID );

        if ( process_memory.IsOpen() ) {
          loader::Process proc;

          proc.process_id = process.th32ProcessID;
          proc.process_name = process.szExeFile;

          std::vector<gwinmem::SearchConditionCallback_t>
              window_search_conditions{
                []( HWND window_handle ) -> bool {
                  std::wstring classname( MAX_PATH, '\0' );
                  GetClassName( window_handle, &classname[ 0 ], MAX_PATH );
                  classname.resize( wcslen( classname.c_str() ) );
                  if ( classname == TEXT( "D3D Window" ) ||
                       classname == TEXT( "IgniteWindowClass" ) )
                    return true;

                  return false;
                },
              };

          const auto target_windows =
              process_memory.FindWindowsCreatedByProcess(
                  window_search_conditions );

          proc.window_name = TEXT( "" );

          for ( const auto window_handle : target_windows ) {
            proc.window_name += gwingui::control::GetText( window_handle );
          }

          processes_out.push_back( proc );
        } else
          gwingui::messagebox::Error(
              TEXT( "Unable to open process " ) +
              std::to_wstring( process.th32ProcessID ) );
      }
    }
  }

  return processes_out;
}

void SuspendOrResumeProcess( const uint32_t process_id, const bool suspend ) {
  gwinmem::SafeHandle snapshot_handle =
      CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 );

  THREADENTRY32 thread;
  thread.dwSize = sizeof( thread );

  assert( Thread32First( snapshot_handle.GetValue(), &thread ) );

  do {
    if ( thread.th32OwnerProcessID == process_id ) {
      gwinmem::SafeHandle thread_handle =
          OpenThread( THREAD_ALL_ACCESS, FALSE, thread.th32ThreadID );

      assert( thread_handle.GetValue() );

      if ( suspend )
        SuspendThread( thread_handle.GetValue() );
      else
        ResumeThread( thread_handle.GetValue() );
    }
  } while ( Thread32Next( snapshot_handle.GetValue(), &thread ) );
}

bool ReadFileData( const std::wstring& filename,
                   std::vector<uint8_t>* file_data ) {
  gwinmem::SafeHandle handle =
      CreateFile( filename.c_str(), GENERIC_READ, FILE_SHARE_READ, 0,
                  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

  if ( handle.GetValue() == INVALID_HANDLE_VALUE )
    return false;

  const DWORD file_size = GetFileSize( handle.GetValue(), 0 );

  std::vector<uint8_t> buf;

  buf.resize( file_size );

  DWORD bytes_read = 0;

  if ( !ReadFile( handle.GetValue(), &buf[ 0 ], file_size, &bytes_read, 0 ) )
    return false;

  if ( bytes_read != file_size )
    return {};

  *file_data = buf;

  return true;
}

void Inject( const uint32_t process_id ) {
  auto process_memory =
      gwinmem::processmemoryexternal::OpenExternalProcess( process_id );

#define USE_MANUAL_MAP TRUE
#define DOWNLOAD_BOT FALSE

  if ( process_memory.IsOpen() ) {
#if USE_MANUAL_MAP
#if !DOWNLOAD_BOT
    wchar_t current_exe_dir[ MAX_PATH ];

    if ( !GetModuleFileName( NULL, current_exe_dir, MAX_PATH ) )
      gwingui::messagebox::Error(
          TEXT( "Unable to get current working directory" ) );

    auto current_exe_dir_str = std::wstring( current_exe_dir );
    size_t exe_name_offset = current_exe_dir_str.find_last_of( L"\\" );
    std::wstring current_working_dir =
        current_exe_dir_str.substr( 0, exe_name_offset );

    auto ha = current_working_dir + TEXT( "\\RIMJOB.module.dll" );

    std::vector<uint8_t> dll_data;
    ReadFileData( ha, &dll_data );
#else
    gWin::HttpWebRequest http_request( TEXT( "http://94.46.240.138" ),
                                       TEXT( "8080" ), TEXT( "/v2/bot.php" ) );

    HttpWebResponseRawVector response( 5000 );

    wchar_t post_data[ 500 ] = { 0 };

    if ( !http_request.GetRequestRawVector( &response ) ) {
      gWin::logger::DisplayError( TEXT( "Unable to send http post request." ) );
      return;
    }

    char* buffer = new char[ response.sourceLength ]{ 0 };
    response.DecryptSource( buffer );

    std::vector<uint8_t> dll_data( buffer, buffer + response.sourceLength );
#endif

    // TODO: Fix the manual mapping, it works with the old Flyff Bot V2, not the
    // new manual map. Figure out what is wrong.
    uint64_t addr = process_memory.ManualMapDll( TEXT( "dicky" ), dll_data );

    if ( !addr )
      gwingui::messagebox::Error( TEXT( "Unable to inject the dll." ) );

#else

    // wchar_t current_working_dir_buf[MAX_PATH];

    // if (!GetModuleFileName(NULL, current_working_dir_buf, MAX_PATH))
    //  gWin::logger::DisplayError(TEXT("Unable to get current working
    //  directory"));

    // const auto current_exe_dir_str = std::wstring(current_working_dir_buf);
    // size_t exe_name_offset = current_exe_dir_str.find_last_of(L"\\");
    // std::wstring current_working_dir = current_exe_dir_str.substr(0,
    // exe_name_offset);

    // auto ha = current_working_dir + TEXT("\\RIMJOB.module.dll");

    // if (!procMem.InjectDll(gWin::StringUtils::WideStringToString(ha)))
    //  gWin::logger::DisplayError(TEXT("Unable to attach to the client"));

    wchar_t current_exe_dir[ MAX_PATH ];

    if ( !GetModuleFileName( NULL, current_exe_dir, MAX_PATH ) )
      gwingui::messagebox::Error(
          TEXT( "Unable to get current working directory" ) );

    auto current_exe_dir_str = std::wstring( current_exe_dir );
    size_t exe_name_offset = current_exe_dir_str.find_last_of( L"\\" );
    std::wstring current_working_dir =
        current_exe_dir_str.substr( 0, exe_name_offset );

    auto ha = current_working_dir + TEXT( "\\RIMJOB.module.dll" );

    const auto ddrr = process_memory.RemoteLoadLibrary( ha );

    // LPVOID loadlib_addr = GetProcAddress(
    //    GetModuleHandle( TEXT( "kernel32.dll" ) ), "LoadLibraryA" );

    // auto ha_s = gWin::StringUtils::WideStringToString( ha );

    // char* buf = new char[ ha_s.size() ];
    // memcpy( buf, ha_s.c_str(), ha_s.size() );

    // uint32_t dir_addr =
    //    static_cast<uint32_t>( process_memory.CommitMemory( buf, ha_s.size() )
    //    );

    // gWin::SafeHandle thread_handle = CreateRemoteThread(
    //    procMem.GetProcessHandle(), 0, 0,
    //    ( LPTHREAD_START_ROUTINE )loadlib_addr, ( LPVOID )dir_addr, 0, 0 );
    // WaitForSingleObject( thread_handle.GetValue(), INFINITE );
#endif
  } else
    gwingui::messagebox::Error( TEXT( "Unable to open the selected process" ) );

  // Sleep(1500);

  // Resume the process
  // SuspendOrResumeProcess(process_id, false);
}

void InjectHwidBypass( const uint32_t process_id ) {
  // Suspend the process
  // SuspendOrResumeProcess(process_id, true);

  auto process_memory =
      gwinmem::processmemoryexternal::OpenExternalProcess( process_id );

  if ( process_memory.IsOpen() ) {
#if DOWNLOAD_BOT
    gWin::HttpWebRequest http_request( TEXT( "http://94.46.240.138" ),
                                       TEXT( "8080" ),
                                       TEXT( "/v2/hwid_bypass.php" ) );

    HttpWebResponseRawVector response( 5000 );

    wchar_t post_data[ 500 ] = { 0 };

    if ( !http_request.GetRequestRawVector( &response ) ) {
      gWin::logger::DisplayError( TEXT( "Unable to send http post request." ) );
      return;
    }

    char* buffer = new char[ response.sourceLength ]{ 0 };
    response.DecryptSource( buffer );

    std::vector<uint8_t> dll_data( buffer, buffer + response.sourceLength );
#else
    wchar_t current_exe_dir[ MAX_PATH ];

    if ( !GetModuleFileName( NULL, current_exe_dir, MAX_PATH ) )
      gwingui::messagebox::Error(
          TEXT( "Unable to get current working directory" ) );

    auto current_exe_dir_str = std::wstring( current_exe_dir );
    size_t exe_name_offset = current_exe_dir_str.find_last_of( L"\\" );
    std::wstring current_working_dir =
        current_exe_dir_str.substr( 0, exe_name_offset );

    auto ha = current_working_dir + TEXT( "\\HWID Bypasser.dll" );

    std::vector<uint8_t> dll_data;
    ReadFileData( ha, &dll_data );
#endif

    uint64_t addr = process_memory.ManualMapDll( TEXT( "dicky2" ), dll_data );

    if ( !addr )
      gwingui::messagebox::Error( TEXT( "Unable to attach the client" ) );
  } else
    gwingui::messagebox::Error( TEXT( "Unable to open the selected process" ) );
}

}  // namespace loader
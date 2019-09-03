#include "pch.h"
#include "crash_handler.h"

#include "bot_initializer.h"
#include "options/option_utils.h"

#include "gwinmem/utils/safe_handle.h"
#include "gwinmem/process_memory_internal.h"

bool WriteDump( EXCEPTION_POINTERS* ex_pointers,
                const std::wstring path,
                int flags ) {
  gwinmem::SafeHandle dump_file_handle = CreateFile(
      path.c_str(), GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0 );

  if ( dump_file_handle.GetValue() == INVALID_HANDLE_VALUE ) {
    printf( "CreateFile failed with error code %d\n", GetLastError() );
    return false;
  }

  MINIDUMP_EXCEPTION_INFORMATION ex_info;
  ex_info.ThreadId = GetCurrentThreadId();
  ex_info.ExceptionPointers = ex_pointers;
  ex_info.ClientPointers = TRUE;

  if ( !MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
                           dump_file_handle.GetValue(),
                           static_cast<MINIDUMP_TYPE>( flags ), &ex_info, NULL,
                           NULL ) ) {
    printf( "MiniDumpWriteDump failed with error code %d\n", GetLastError() );
    return false;
  }

  return true;
}

DWORD WINAPI CrashDumpWriter( LPVOID param ) {
  const std::wstring dumps_folder_name = TEXT( "dumps" );

  time_t t = time( 0 );
  tm now;
  localtime_s( &now, &t );

  std::wstring dump_name =
      TEXT( "rimjob-v2-" ) + std::to_wstring( now.tm_year ) +
      std::to_wstring( now.tm_mon ) + std::to_wstring( now.tm_mday ) +
      std::to_wstring( now.tm_hour ) + std::to_wstring( now.tm_min ) +
      std::to_wstring( now.tm_sec );

  std::wstring crash_dump_folder_path = bot::optionutils::GetGreyb1tDirectory();

  CreateDirectory( crash_dump_folder_path.c_str(), NULL );

  std::wstring crash_dump_folder_path_v2 =
      bot::optionutils::GetOptionsDirectory();

  CreateDirectory( crash_dump_folder_path_v2.c_str(), NULL );

  std::wstring crash_dump_path =
      crash_dump_folder_path_v2 + TEXT( "\\" ) + dumps_folder_name;

  CreateDirectory( crash_dump_path.c_str(), NULL );

  crash_dump_path =
      crash_dump_path + TEXT( "\\" ) + dump_name + TEXT( ".crashdump" );

  EXCEPTION_POINTERS* ex_pointers =
      reinterpret_cast<EXCEPTION_POINTERS*>( param );

  if ( !WriteDump( ex_pointers, crash_dump_path,
                   MiniDumpWithFullMemory | MiniDumpWithThreadInfo |
                       MiniDumpWithFullMemoryInfo ) )
    printf( "Unable to write the full dump\n" );

  if ( !WriteDump( ex_pointers, crash_dump_path + TEXT( ".tiny" ),
                   MiniDumpNormal ) )
    printf( "Unable to write the tiny dump\n" );

  return TRUE;
}

bool has_allocated_console = false;

LONG WINAPI crash_handler::MainExceptionHandler( EXCEPTION_POINTERS* ex ) {
  // Add the manual mapped module (this one) as a dll to make it easier when
  // debugging the dump
  gwinmem::CurrentProcess().ManualMapAddLoaderDll(
      reinterpret_cast<uintptr_t>( bot::Initializer().GetInstance() ),
      TEXT( "C:\\crerrpt.dll" ) );

  if ( !has_allocated_console ) {
    AllocConsole();
    freopen( "CONOUT$", "w", stdout );

    has_allocated_console = true;
  }

  printf( "Exception occured\n" );

  DWORD thread_id;
  gwinmem::SafeHandle thread_handle =
      CreateThread( 0, 0, CrashDumpWriter, ex, CREATE_SUSPENDED, &thread_id );

  if ( thread_handle.GetValue() ) {
    printf( "Creating the crash dump... \n" );

    if ( ResumeThread( thread_handle.GetValue() ) == -1 )
      printf( "R FAILED ResumeThread FAILED.\n" );

    printf( "Resumed the crash dump writer thread...\n" );

    WaitForSingleObject( thread_handle.GetValue(), INFINITE );

    DWORD thread_exit_code;
    GetExitCodeThread( thread_handle.GetValue(), &thread_exit_code );

    if ( !thread_exit_code )
      printf( "Writer thread failed on the thread %d\n", thread_id );
  } else {
    printf( "CreateThread failed with error code %d\n", GetLastError() );
  }

  printf( "Successfully created and saved a crashdump\n" );

  Sleep( 2000 );

  TerminateProcess( GetCurrentProcess(), 0 );

  return EXCEPTION_CONTINUE_EXECUTION;
}
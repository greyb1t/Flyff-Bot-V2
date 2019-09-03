#include "process_selector_window.h"
#include "loader_core.h"
#include "res\resource.h"

#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/listview.h"
#include "gwinguiv2/message_box.h"

#include <ctime>

void ProcessSelectorWindow::Initialize( const HWND window_handle ) {
  srand( static_cast<unsigned int>( time( 0 ) ) );

  control::SetText( GetHandle(), TEXT( "RIMJOB" ) );
  ShowWindow( GetHandle(), SW_SHOW );

  const HWND listview_process_handle =
      control::GetWindowHandle( LIST_PROCESSES );

  listview::AddColumn( listview_process_handle, TEXT( "Window Name" ), 0 );
  listview::AddColumn( listview_process_handle, TEXT( "Process Id" ), 0 );
  listview::AddColumn( listview_process_handle, TEXT( "Name" ), 0 );

  listview::SetColumnWidth( listview_process_handle, 0, 110 );
  listview::SetColumnWidth( listview_process_handle, 1, 70 );
  listview::SetColumnWidthAutoSize( listview_process_handle, 2 );

  std::vector<loader::Process> processes = loader::GetRunningProcessesByNames( {
      TEXT( "Kingdom FlyFF Bot.exe" ),
      TEXT( "neuz_patched.exe" ),
      TEXT( "Neuz_patched.exe" ),
      TEXT( "Neuz.exe" ),
      TEXT( "Code Tester.exe" ),
      TEXT( "data.dll" ),
  } );

  for ( int i = 0, size = processes.size(); i < size; ++i ) {
    const auto process = processes[ i ];

    const int inserted_item_index =
        listview::AddItem( listview_process_handle, process.process_name, i );

    listview::AddSubitem( listview_process_handle,
                          std::to_wstring( process.process_id ),
                          inserted_item_index, 2 );
    listview::AddSubitem( listview_process_handle, process.window_name,
                          inserted_item_index, 3 );
  }
}

std::wstring GetRandomString( uint32_t length ) {
  std::wstring s = TEXT( "" );

  std::wstring ascii = TEXT( "abcdef1234567890" );

  for ( uint32_t i = 0; i < length; ++i ) {
    uint32_t random_value = rand() % ( ascii.size() - 1 );
    s += ascii[ random_value ];
  }

  return s;
}

std::wstring GetRandomStringLettersOnly( uint32_t length ) {
  std::wstring s = TEXT( "" );

  std::wstring ascii = TEXT( "abcdefghijklmnopqrstuvwxyz" );

  for ( uint32_t i = 0; i < length; ++i ) {
    uint32_t random_value = rand() % ( ascii.size() - 1 );
    s += ascii[ random_value ];
  }

  return s;
}

std::wstring WideStringToUpper( const std::wstring& s ) {
  std::wstring new_string;

  for ( auto c : s ) {
    new_string += toupper( c );
  }

  return new_string;
}

void ProcessSelectorWindow::OnButtonClick( const HWND button_handle,
                                           const uint32_t button_id ) {
  switch ( button_id ) {
    case BUTTON_INJECT: {
      const HWND listview_process_handle =
          control::GetWindowHandle( LIST_PROCESSES );

      NMITEMACTIVATE item;
      item.iItem = listview::GetSelectedItem( listview_process_handle );
      item.hdr.idFrom = LIST_PROCESSES;
      item.hdr.hwndFrom = listview_process_handle;

      OnListViewDoubleClick( &item );
    } break;

    case BUTTON_BYPASS_CLIENT_LIMIT: {
      const HWND listview_process_handle =
          control::GetWindowHandle( LIST_PROCESSES );

      const auto sel_index =
          listview::GetSelectedItem( listview_process_handle );

      if ( sel_index == -1 )
        return;

      const std::wstring process_id_str =
          listview::GetItemText( listview_process_handle, sel_index, 1 );

      const uint32_t process_id = std::stoul( process_id_str );

      loader::InjectHwidBypass( process_id );

      gwingui::messagebox::Warning( TEXT( "Bypassed!" ) );

      return;

      /*
        Ignite Flyff has banned computers using the MachineGuid value in the
        registry. After me and my friends was banned, they noticed that I was
        able to bypass it. So they upgraded protection.
      */

      //////////////////////////////////////////////////////////////////////////
      wchar_t machine_guid[ MAX_PATH ] = { 0 };
      DWORD machine_guid_buf_size = sizeof( machine_guid );
      DWORD machine_guid_type = REG_SZ;
      HKEY key_handle;

      RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                    TEXT( "SOFTWARE\\Microsoft\\Cryptography" ), 0,
                    KEY_READ | KEY_WRITE | KEY_WOW64_64KEY, &key_handle );

      RegQueryValueEx( key_handle, TEXT( "MachineGuid" ), 0, &machine_guid_type,
                       reinterpret_cast<LPBYTE>( &machine_guid ),
                       &machine_guid_buf_size );

      std::wstring new_machine_guid =
          GetRandomString( 8 ) + TEXT( "-" ) + GetRandomString( 4 ) +
          TEXT( "-" ) + GetRandomString( 4 ) + TEXT( "-" ) +
          GetRandomString( 4 ) + TEXT( "-" ) + GetRandomString( 12 );

      RegSetValueEx( key_handle, TEXT( "MachineGuid" ), 0, REG_SZ,
                     reinterpret_cast<const BYTE*>(
                         /*machine_guid*/ new_machine_guid.c_str() ),
                     machine_guid_buf_size );

      RegCloseKey( key_handle );
      //////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      wchar_t hdd_id[ MAX_PATH ] = { 0 };
      DWORD hdd_buf_size = sizeof( hdd_id );
      DWORD hdd_key_type = REG_SZ;
      HKEY hdd_key_handle;

      RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                    TEXT( "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 1\\Scsi Bus "
                          "1\\Target Id 0\\Logical Unit Id 0" ),
                    0, KEY_READ | KEY_WRITE | KEY_WOW64_64KEY,
                    &hdd_key_handle );

      RegQueryValueEx( hdd_key_handle, TEXT( "SerialNumber" ), 0, &hdd_key_type,
                       reinterpret_cast<LPBYTE>( &hdd_id ), &hdd_buf_size );

      std::wstring new_hdd_id =
          WideStringToUpper( GetRandomStringLettersOnly( 2 ) ) + TEXT( "-" ) +
          WideStringToUpper( GetRandomString( 10 ) );

      RegSetValueEx( hdd_key_handle, TEXT( "SerialNumber" ), 0, REG_SZ,
                     reinterpret_cast<const BYTE*>( new_hdd_id.c_str() ),
                     hdd_buf_size );

      RegCloseKey( hdd_key_handle );
      //////////////////////////////////////////////////////////////////////////

      // gWin::logger::DisplayError(
      //     TEXT( "Successfully bypassed the client limit restriction." ) );
    } break;

    default:
      break;
  }
}

void ProcessSelectorWindow::OnListViewDoubleClick( NMITEMACTIVATE* item ) {
  if ( item->iItem != -1 ) {
    const std::wstring process_id_str =
        listview::GetItemText( item->hdr.hwndFrom, item->iItem, 1 );
    const uint32_t process_id = std::stoul( process_id_str );

    loader::Inject( process_id );

    ExitProcess( 0 );
  }
}
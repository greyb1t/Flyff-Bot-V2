#include "pch.h"
#include "option_utils.h"

#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/combobox.h"

namespace bot {

namespace optionutils {

void FillComboBoxFKeys( uint32_t control_identifier ) {
  const auto combobox_handle = GWH( control_identifier );

  for ( int i = 1; i <= 9; ++i )
    gwingui::combobox::AddString( combobox_handle,
                                  TEXT( "F" ) + std::to_wstring( i ) );
}

void FillComboBoxNumbers( uint32_t control_identifier ) {
  const auto combobox_handle = GWH( control_identifier );

  for ( int i = '1'; i <= '9'; ++i ) {
    std::wstring lol = TEXT( " " );
    char c = i;
    lol[ 0 ] = c;
    gwingui::combobox::AddString( combobox_handle, lol );
  }
}

void FillComboBoxAlphabet( uint32_t control_identifier ) {
  const auto combobox_handle = GWH( control_identifier );

  for ( int i = 'A'; i < 'Z'; ++i ) {
    std::wstring lol = TEXT( " " );
    char c = i;
    lol[ 0 ] = c;
    gwingui::combobox::AddString( combobox_handle, lol );
  }
}

uint32_t AsciiKeyToVirtualKeyCode( const std::wstring& key ) {
  if ( key.size() == 1 )
    return key[ 0 ];
  else if ( key.size() == 2 ) {
    if ( key[ 0 ] == 'F' ) {
      uint32_t f_value = key[ 1 ] - '0';
      return ( VK_F1 - 1 ) + f_value;
    }
  }

  return -1;
}

std::wstring VirtualKeyCodeToAsciiKey( const uint32_t key_code ) {
  std::wstring key = L"";

  if ( ( key_code >= 'A' && key_code <= 'Z' ) ||
       key_code >= '1' && key_code <= '9' )
    key += key_code;
  else if ( key_code >= VK_F1 && key_code <= VK_F9 )
    key += L"F" + std::to_wstring( ( key_code - VK_F1 + 1 ) );

  return key;
}

std::wstring GetGreyb1tDirectory() {
  PWSTR appdata_directory_temp[ MAX_PATH ];
  SHGetKnownFolderPath( FOLDERID_RoamingAppData, 0, 0, appdata_directory_temp );

  std::wstring appdata_directory = appdata_directory_temp[ 0 ];
  std::wstring options_directory = appdata_directory + TEXT( "\\greyb1t" );

  return options_directory;
}

std::wstring GetOptionsDirectory() {
  return GetGreyb1tDirectory() + TEXT( "\\v2" );
}

}  // namespace bot_options

}  // namespace bot
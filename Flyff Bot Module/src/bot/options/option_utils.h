#pragma once

namespace bot {

namespace optionutils {

void FillComboBoxFKeys( uint32_t control_identifier );
void FillComboBoxNumbers( uint32_t control_identifier );
void FillComboBoxAlphabet( uint32_t control_identifier );

uint32_t AsciiKeyToVirtualKeyCode( const std::wstring& key );
std::wstring VirtualKeyCodeToAsciiKey( const uint32_t key_code );

std::wstring GetGreyb1tDirectory();
std::wstring GetOptionsDirectory();

}  // namespace bot_options

}  // namespace bot
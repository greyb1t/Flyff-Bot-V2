#pragma once

#include <string>
#include <algorithm>
#include <codecvt>

namespace stringutils {

inline std::wstring WideStringToLower( const std::wstring& str ) {
  std::wstring strcopy;
  strcopy.resize( str.size() );
  std::transform( str.begin(), str.end(), strcopy.begin(),
                  []( wchar_t c ) { return towlower( c ); } );
  return strcopy;
}

inline std::wstring AnsiToWide( const std::string& str ) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
  return conv.from_bytes( str );
}

inline std::string WideToAnsi( const std::wstring& str ) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
  return conv.to_bytes( str );
}

}  // namespace stringutils
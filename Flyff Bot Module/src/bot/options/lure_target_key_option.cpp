#include "pch.h"
#include "lure_target_key_option.h"
#include "..\bot_options.h"
#include "../../res/resource.h"
#include "option_utils.h"

#include "gwinguiv2/controls/combobox.h"

namespace bot {

const std::string LureTargetKeyOption::kLureTargetKeyCodeJsonName = "key_code";

json LureTargetKeyOption::GetJson() {
  auto j = Option::GetJson();

  j[ kLureTargetKeyCodeJsonName ] = lure_key_code_;

  return j;
}

void LureTargetKeyOption::LoadJson( const json& json_parent ) {
  Option::LoadJson( json_parent );

  if ( JsonExists( json_parent, name_ ) ) {
    auto json_child = json_parent.at( name_ );

    if ( JsonExists( json_child, kLureTargetKeyCodeJsonName ) )
      lure_key_code_ =
          json_child.at( kLureTargetKeyCodeJsonName ).get<uint16_t>();
  }
}

void LureTargetKeyOption::RefreshControls() {
  std::wstring key_code_s =
      optionutils::VirtualKeyCodeToAsciiKey( lure_key_code_ );

  const auto combobox_target_lure_key_handle = GWH( COMBO_TARGET_LURE_KEY );

  int key_code_index = gwingui::combobox::FindString(
      combobox_target_lure_key_handle, 0, key_code_s );

  gwingui::combobox::SetSelectedIndex( combobox_target_lure_key_handle,
                                       key_code_index );
}

}  // namespace bot
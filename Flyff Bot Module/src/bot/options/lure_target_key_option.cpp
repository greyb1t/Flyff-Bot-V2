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

uint16_t LureTargetKeyOption::GetLureKeyCode() {
  return lure_key_code_;
}

void LureTargetKeyOption::SetLureKeyCode( uint16_t bot_mode ) {
  lure_key_code_ = bot_mode;
}

bool LureTargetKeyOption::TryApplyOption() {
  const auto combo_target_lure_key = GWH( COMBO_TARGET_LURE_KEY );

  const auto lure_key_index =
      gwingui::combobox::GetSelectedIndex( combo_target_lure_key );

  if ( lure_key_index == -1 ) {
    gwingui::messagebox::Error(
        TEXT( "You have to choose a key to lure the target." ) );
    return false;
  }

  const auto lure_key_str =
      gwingui::combobox::GetString( combo_target_lure_key, lure_key_index );
  const auto lure_key = optionutils::AsciiKeyToVirtualKeyCode( lure_key_str );

  SetLureKeyCode( lure_key );

  return true;
}

}  // namespace bot
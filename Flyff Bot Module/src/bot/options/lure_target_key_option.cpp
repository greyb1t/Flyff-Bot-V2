#include "pch.h"
#include "lure_target_key_option.h"
#include "..\bot_options.h"
#include "../../res/resource.h"
#include "option_utils.h"

#include "gwinguiv2/controls/combobox.h"
#include "gwinguiv2/controls/hotkey.h"

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
  const auto lure_key_hotkey_handle = GWH( HOTKEY_TARGET_LURE );

  gwingui::hotkey::SetCurrentHotkey( lure_key_hotkey_handle, lure_key_code_ );
}

uint16_t LureTargetKeyOption::GetLureKeyCode() {
  return lure_key_code_;
}

void LureTargetKeyOption::SetLureKeyCode( uint16_t bot_mode ) {
  lure_key_code_ = bot_mode;
}

bool LureTargetKeyOption::TryApplyOption() {
  const auto lure_key_hotkey_handle = GWH( HOTKEY_TARGET_LURE );

  const auto lure_virtual_keycode =
      gwingui::hotkey::GetCurrentHotkey( lure_key_hotkey_handle );

  if ( !lure_virtual_keycode ) {
    gwingui::messagebox::Error(
        TEXT( "You have to choose a key to lure the target." ) );
    return false;
  }

  SetLureKeyCode( lure_virtual_keycode );

  return true;
}

}  // namespace bot
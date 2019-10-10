#include "pch.h"
#include "auto_healing_pills_option.h"
#include "../../res/resource.h"
#include "option_utils.h"

#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/editcontrol.h"
#include "gwinguiv2/controls/combobox.h"
#include "gwinguiv2/controls/hotkey.h"

namespace bot {

void AutoHealingPillsOption::RefreshControls() {
  Option::RefreshControls();

  const auto editcontrol_auto_pills_hp_limit = GWH( EDIT_AUTO_PILLS_HP_LIMIT );

  if ( health_limit_ != -1 )
    gwingui::editcontrol::SetInt( editcontrol_auto_pills_hp_limit,
                                  health_limit_ );
  else
    gwingui::editcontrol::SetText( editcontrol_auto_pills_hp_limit,
                                   TEXT( "" ) );

  const auto hotkey_auto_health_pill_handle =
      GWH( HOTKEY_AUTO_HEALTH_PILLS_KEY );

  gwingui::hotkey::SetCurrentHotkey( hotkey_auto_health_pill_handle,
                                     heal_key_code_ );
}

void AutoHealingPillsOption::EnableOrDisableControls( bool enable ) {
  gwingui::control::EnableOrDisable( GWH( HOTKEY_AUTO_HEALTH_PILLS_KEY ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_AUTO_PILLS_HP_LIMIT ), enable );
}

bool AutoHealingPillsOption::TryApplyOption() {
  const auto checkbox_auto_health_pills = GWH( CHECK_AUTO_HEALTH_PILLS );

  if ( gwingui::checkbox::IsChecked( checkbox_auto_health_pills ) ) {
    SetStatus( true );

    const auto hotkey_auto_health_pills_key_handle =
        GWH( HOTKEY_AUTO_HEALTH_PILLS_KEY );

    const auto heal_virtual_keycode = gwingui::hotkey::GetCurrentHotkey(
        hotkey_auto_health_pills_key_handle );

    if ( !heal_virtual_keycode ) {
      gwingui::messagebox::Error(
          TEXT( "You have to choose a key to use the pills." ) );
      return false;
    }

    SetHealKeyCode( heal_virtual_keycode );

    const auto editcontrol_auto_pills_hp_limit =
        GWH( EDIT_AUTO_PILLS_HP_LIMIT );
    auto health_limit =
        gwingui::editcontrol::GetInt( editcontrol_auto_pills_hp_limit );

    if ( !health_limit ) {
      gwingui::messagebox::Error( TEXT( "You enter the health limit." ) );
      return false;
    }

    SetHealthLimit( health_limit );
  } else
    SetStatus( false );

  return true;
}

}  // namespace bot
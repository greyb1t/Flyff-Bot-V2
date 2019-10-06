#include "pch.h"
#include "auto_healing_pills_option.h"
#include "../../res/resource.h"
#include "option_utils.h"

#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/editcontrol.h"
#include "gwinguiv2/controls/combobox.h"

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

  std::wstring key_code_s =
      optionutils::VirtualKeyCodeToAsciiKey( heal_key_code_ );

  if ( !key_code_s.empty() ) {
    const auto combobox_auto_health_pills_key =
        GWH( COMBO_AUTO_HEALTH_PILLS_KEY );

    int key_code_index = gwingui::combobox::FindString(
        combobox_auto_health_pills_key, 0, key_code_s );

    gwingui::combobox::SetSelectedIndex( combobox_auto_health_pills_key,
                                         key_code_index );
  }
}

void AutoHealingPillsOption::EnableOrDisableControls( bool enable ) {
  gwingui::control::EnableOrDisable( GWH( COMBO_AUTO_HEALTH_PILLS_KEY ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_AUTO_PILLS_HP_LIMIT ), enable );
}

bool AutoHealingPillsOption::TryApplyOption() {
  const auto checkbox_auto_health_pills = GWH( CHECK_AUTO_HEALTH_PILLS );

  if ( gwingui::checkbox::IsChecked( checkbox_auto_health_pills ) ) {
    SetStatus( true );

    const auto combo_auto_health_pills_key = GWH( COMBO_AUTO_HEALTH_PILLS_KEY );

    const auto heal_key_index =
        gwingui::combobox::GetSelectedIndex( combo_auto_health_pills_key );

    if ( heal_key_index == -1 ) {
      gwingui::messagebox::Error(
          TEXT( "You have to choose a key to use the pills." ) );
      return false;
    }

    const auto heal_key_str = gwingui::combobox::GetString(
        combo_auto_health_pills_key, heal_key_index );
    const auto heal_key = optionutils::AsciiKeyToVirtualKeyCode( heal_key_str );

    SetHealKeyCode( heal_key );

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
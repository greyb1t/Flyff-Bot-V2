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

    MessageBox( 0, key_code_s.c_str(), TEXT( "" ), 0 );

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

}  // namespace bot
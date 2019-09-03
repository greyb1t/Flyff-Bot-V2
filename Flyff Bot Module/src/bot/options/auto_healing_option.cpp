#include "pch.h"
#include "auto_healing_option.h"
#include "../../res/resource.h"
#include "option_utils.h"

#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/editcontrol.h"
#include "gwinguiv2/controls/combobox.h"

namespace bot {

const std::string AutoHealingOption::kHealKeyCodeJsonName = "heal_key_code";
const std::string AutoHealingOption::kHealthLimitJsonName = "health_limit";

void AutoHealingOption::SetHealKeyCode( uint16_t key_code ) {
  heal_key_code_ = key_code;
}

uint16_t AutoHealingOption::GetHealCode() const {
  return heal_key_code_;
}

void AutoHealingOption::SetHealthLimit( int32_t health ) {
  health_limit_ = health;
}

int32_t AutoHealingOption::GetHealthLimit() const {
  return health_limit_;
}

json AutoHealingOption::GetJson() {
  auto j = Option::GetJson();

  j[ kHealKeyCodeJsonName ] = heal_key_code_;
  j[ kHealthLimitJsonName ] = health_limit_;

  return j;
}

void AutoHealingOption::LoadJson( const json& json_parent ) {
  Option::LoadJson( json_parent );

  if ( JsonExists( json_parent, name_ ) ) {
    auto json_child = json_parent.at( name_ );

    if ( JsonExists( json_child, kHealKeyCodeJsonName ) &&
         JsonExists( json_child, kHealthLimitJsonName ) ) {
      heal_key_code_ = json_child.at( kHealKeyCodeJsonName ).get<uint16_t>();
      health_limit_ = json_child.at( kHealthLimitJsonName ).get<uint32_t>();
    }
  }
}

void AutoHealingOption::RefreshControls() {
  Option::RefreshControls();

  const auto editcontrol_auto_food_hp_limit = GWH( EDIT_AUTO_FOOD_HP_LIMIT );

  if ( health_limit_ != -1 )
    gwingui::editcontrol::SetInt( editcontrol_auto_food_hp_limit,
                                  health_limit_ );
  else
    gwingui::editcontrol::SetText( editcontrol_auto_food_hp_limit, TEXT( "" ) );

  std::wstring key_code_s =
      optionutils::VirtualKeyCodeToAsciiKey( heal_key_code_ );

  if ( !key_code_s.empty() ) {
    const auto combobox_auto_health_food_key =
        GWH( COMBO_AUTO_HEALTH_FOOD_KEY );

    int key_code_index = gwingui::combobox::FindString(
        combobox_auto_health_food_key, 0, key_code_s );

    gwingui::combobox::SetSelectedIndex( combobox_auto_health_food_key,
                                         key_code_index );
  }
}

void AutoHealingOption::EnableOrDisableControls( bool enable ) {
  gwingui::control::EnableOrDisable( GWH( COMBO_AUTO_HEALTH_FOOD_KEY ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_AUTO_FOOD_HP_LIMIT ), enable );
}

}  // namespace bot
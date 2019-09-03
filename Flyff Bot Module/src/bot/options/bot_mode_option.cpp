#include "pch.h"
#include "bot_mode_option.h"

namespace bot {

const std::string BotModeOption::kBotModeJsonName = "botmode";

json BotModeOption::GetJson() {
  auto j = Option::GetJson();

  j[ kBotModeJsonName ] = bot_mode_;

  return j;
}

void BotModeOption::LoadJson( const json& json_parent ) {
  Option::LoadJson( json_parent );

  if ( JsonExists( json_parent, name_ ) ) {
    auto json_child = json_parent.at( name_ );

    if ( JsonExists( json_child, kBotModeJsonName ) )
      bot_mode_ = json_child.at( kBotModeJsonName ).get<BotMode>();
  }
}

void BotModeOption::RefreshControls() {
  const auto combobox_handle = GWH( COMBO_BOT_MODE );
  gwingui::combobox::SetSelectedIndex( combobox_handle,
                                       static_cast<int>( bot_mode_ ) );
}

}  // namespace bot
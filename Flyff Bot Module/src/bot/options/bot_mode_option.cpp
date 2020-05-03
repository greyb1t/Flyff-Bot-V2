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

void BotModeOption::RefreshControls( bot::BotOptions* bot_options ) {
  const auto combobox_handle = GWH( COMBO_BOT_MODE );
  gwingui::combobox::SetSelectedIndex( combobox_handle,
                                       static_cast<int>( bot_mode_ ) );
}

BotMode BotModeOption::GetBotMode() const {
  return bot_mode_;
}

void BotModeOption::SetBotMode( BotMode bot_mode ) {
  bot_mode_ = bot_mode;
}

bool BotModeOption::TryApplyOption() {
  const auto combo_bot_mode = GWH( COMBO_BOT_MODE );

  const auto bot_mode_index =
      gwingui::combobox::GetSelectedIndex( combo_bot_mode );

  if ( bot_mode_index == -1 ) {
    gwingui::messagebox::Error( TEXT( "You have to choose the bot mode." ) );
    return false;
  }

  const auto bot_mode_str =
      gwingui::combobox::GetString( combo_bot_mode, bot_mode_index );

  if ( bot_mode_str == TEXT( "OneVsOne" ) )
    SetBotMode( BotMode::kBotModeOneVsOne );
  else
    SetBotMode( BotMode::kBotModeNone );

  return true;
}

}  // namespace bot
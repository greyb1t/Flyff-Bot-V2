#include "pch.h"
#include "attack_sequence_option.h"
#include "option_utils.h"
#include "../../res/resource.h"

#include "gwinguiv2/controls/listview.h"

namespace bot {

void AttackSequenceOption::RefreshControls(
    bot::BotOptions* bot_options ) {
  Option::RefreshControls( bot_options );

  const auto listview_attacks = GWH( LISTVIEW_ATTACKS );

  for ( auto& value : values_ ) {
    const auto listview_attacks_count =
        gwingui::listview ::GetItemCount( listview_attacks );

    const auto item_index = gwingui::listview::AddItem(
        listview_attacks,
        optionutils::VirtualKeyCodeToAsciiKey( value.key_code ),
        listview_attacks_count );

    gwingui::listview::AddSubitem(
        listview_attacks, std::to_wstring( value.time_ms_after_last_attack ),
        item_index, 2 );
  }
}

void AttackSequenceOption::EnableOrDisableControls(
    bool enable,
    bot::BotOptions* bot_options ) {
  gwingui::control::EnableOrDisable( GWH( LISTVIEW_ATTACKS ), enable );
  gwingui::control::EnableOrDisable( GWH( HOTKEY_ATTACK_KEY ), enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_ATTACK_KEY_INTERVAL ), enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_ADD_ATTACK ), enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_REMOVE_ATTACK ), enable );
}

bool AttackSequenceOption::TryApplyOption() {
  Clear();

  const auto checkbox_attack_sequence = GWH( CHECK_ATTACK_SEQUENCE );

  if ( gwingui::checkbox::IsChecked( checkbox_attack_sequence ) ) {
    SetStatus( true );

    const auto listview_attacks = GWH( LISTVIEW_ATTACKS );

    for ( int i = 0,
              count = gwingui::listview::GetItemCount( listview_attacks );
          i < count; ++i ) {
      const auto row = gwingui::listview::GetItemRowText( listview_attacks, i );

      Key key;

      key.key_code = optionutils::AsciiKeyToVirtualKeyCode( row[ 0 ] );
      key.time_ms_after_last_attack = std::stoi( row[ 1 ] );

      AddValue( key );
    }
  } else
    SetStatus( false );

  return true;
}

}  // namespace bot
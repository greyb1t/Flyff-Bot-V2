#include "pch.h"
#include "character_rebuff.h"
#include "option_utils.h"
#include "../../res/resource.h"

#include "gwinguiv2/controls/listview.h"
#include "gwinguiv2/controls/editcontrol.h"

namespace bot {

const std::string CharacterRebuffOption::kJSON_NAME = "Rebuff Sequence";
const std::string CharacterRebuffOption::kRebuffIntervalJsonName =
    "rebuff_interval";

uint32_t CharacterRebuffOption::GetRebuffInterval() const {
  return rebuff_interval_ms_;
}

void CharacterRebuffOption::SetRebuffInterval( uint32_t interval ) {
  rebuff_interval_ms_ = interval;
}

json CharacterRebuffOption::GetJson() {
  json j = KeySequenceOption::GetJson();

  j[ kRebuffIntervalJsonName ] = rebuff_interval_ms_;

  return j;
}

void CharacterRebuffOption::LoadJson( const json& json_parent ) {
  if ( JsonExists( json_parent, Option::kOptionJsonName ) &&
       JsonExists( json_parent, kRebuffIntervalJsonName ) ) {
    enabled_ = json_parent.at( Option::kOptionJsonName ).get<bool>();
    rebuff_interval_ms_ =
        json_parent.at( kRebuffIntervalJsonName ).get<uint32_t>();
  }

  KeySequenceOption::LoadJsonKeySequenceObject( json_parent );
}

void CharacterRebuffOption::RefreshControls() {
  Option::RefreshControls();

  const auto edit_rebuff_seq_interval = GWH( EDIT_REBUFF_SEQUENCE_INTERVAL );

  if ( rebuff_interval_ms_ != -1 )
    gwingui::editcontrol::SetInt( edit_rebuff_seq_interval,
                                  rebuff_interval_ms_ );
  else
    gwingui::editcontrol::SetText( edit_rebuff_seq_interval, TEXT( "" ) );

  const auto listview_rebuff_key_seq = GWH( LISTVIEW_REBUFF_KEY_SEQUENCE );

  gwingui::listview::ClearItems( listview_rebuff_key_seq );

  for ( auto& value : values_ ) {
    const auto listview_count =
        gwingui::listview::GetItemCount( listview_rebuff_key_seq );

    const auto item_index = gwingui::listview::AddItem(
        listview_rebuff_key_seq,
        optionutils::VirtualKeyCodeToAsciiKey( value.key_code ),
        listview_count );

    gwingui::listview::AddSubitem(
        listview_rebuff_key_seq,
        std::to_wstring( value.time_ms_after_last_attack ), item_index, 2 );
  }
}

void CharacterRebuffOption::EnableOrDisableControls( bool enable ) {
  gwingui::control::EnableOrDisable( GWH( LISTVIEW_REBUFF_KEY_SEQUENCE ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( HOTKEY_REBUFF_KEY ), enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_REBUFF_KEY_INTERVAL ), enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_REBUFF_SEQUENCE_INTERVAL ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_REBUFF_KEY_ADD ), enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_REBUFF_KEY_REMOVE ), enable );
  gwingui::control::EnableOrDisable(
      GWH( BUTTON_APPLY_SELECTED_REBUFF_SEQUENCE ), enable );
}

CharacterRebuffReturnValue CharacterRebuffOption::Update() {
  bool change_state = false;

  character_rebuff_timer_.DoEachIntervalBefore(
      rebuff_interval_ms_, [&]() { change_state = true; } );

  if ( change_state )
    return CharacterRebuffReturnValue::kCharRebuffReady;
  else
    return CharacterRebuffReturnValue::kCharRebuffNotReady;
}

void CharacterRebuffOption::ResetRebuffTimer() {
  character_rebuff_timer_.SetStartTime( GetTickCount64() );
}

void CharacterRebuffOption::ResetRebuffTimer20() {
  character_rebuff_timer_.Reset();
}

}  // namespace bot
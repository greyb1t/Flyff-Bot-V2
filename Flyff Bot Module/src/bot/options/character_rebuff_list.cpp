#include "pch.h"
#include "character_rebuff_list.h"
#include "character_rebuff.h"
#include "../../res/resource.h"

#include "option_utils.h"

#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/listbox.h"
#include "gwinguiv2/controls/listview.h"
#include "gwinguiv2/controls/editcontrol.h"

namespace bot {

const std::string CharacterRebuffListOption::kRebuffListJsonName =
    "rebuff_list";

void CharacterRebuffListOption::AddRebuffSequence(
    const CharacterRebuffOption& rebuff ) {
  rebuff_list_.push_back( rebuff );
}

void CharacterRebuffListOption::ReplaceRebuffSequence(
    uint32_t index,
    const CharacterRebuffOption& rebuff ) {
  if ( !RebuffExists( index ) )
    assert( false && "no exists rebuff" );

  rebuff_list_[ index ] = rebuff;
}

void CharacterRebuffListOption::RemoveRebuffSequence( const uint32_t index ) {
  rebuff_list_.erase( rebuff_list_.begin() + index );
}

CharacterRebuffOption& CharacterRebuffListOption::GetCharacterRebuff(
    uint32_t index ) {
  if ( index < 0 || index >= rebuff_list_.size() )
    assert( false && "index out of range char rebuff" );

  return rebuff_list_[ index ];
}

bool CharacterRebuffListOption::RebuffExists( uint32_t index ) {
  return index < rebuff_list_.size();
}

std::vector<CharacterRebuffOption>& CharacterRebuffListOption::GetRebuffList() {
  return rebuff_list_;
}

json CharacterRebuffListOption::GetJson() {
  auto j = Option::GetJson();

  j[ kRebuffListJsonName ] = json::array();

  for ( auto& rebuff : rebuff_list_ ) {
    // rebuff.GetJson();

    // j[rebuff.GetName()] = rebuff.GetJson();

    // TODO: make GetJson const!!!

    j[ kRebuffListJsonName ].push_back( rebuff.GetJson() );
  }

  return j;
}

void CharacterRebuffListOption::LoadJson( const json& json_parent ) {
  Option::LoadJson( json_parent );

  if ( JsonExists( json_parent, name_ ) ) {
    const auto json_child = json_parent.at( name_ );

    if ( JsonExists( json_child, kRebuffListJsonName ) ) {
      const auto json_char_rebuff_list = json_child.at( kRebuffListJsonName );

      int counter = 0;

      for ( auto& rebuff : json_char_rebuff_list ) {
        CharacterRebuffOption rebuff_temp(
            CharacterRebuffOption::kJSON_NAME + std::to_string( counter ), 0 );

        rebuff_temp.LoadJson( rebuff );

        rebuff_list_.push_back( rebuff_temp );
        ++counter;
      }
    }
  }
}

void CharacterRebuffListOption::RefreshControls() {
  Option::RefreshControls();

  for ( auto& value : rebuff_list_ ) {
    const auto listbox_rebuff_sequence_handle = GWH( LISTBOX_REBUFF_SEQUENCES );
    int count = gwingui::listbox::GetCount( listbox_rebuff_sequence_handle );
    gwingui::listbox::AddString(
        listbox_rebuff_sequence_handle,
        TEXT( "Rebuff Sequence " ) + std::to_wstring( count + 1 ) );
  }
}

void CharacterRebuffListOption::EnableOrDisableControls( bool enable ) {
  gwingui::control::EnableOrDisable( GWH( LISTBOX_REBUFF_SEQUENCES ), enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_ADD_REBUFF_SEQUENCE ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_REMOVE_REBUFF_SEQUENCE ),
                                     enable );
}

CharacterRebuffOption CharacterRebuffListOption::ReadCharacterRebuffOptions(
    uint32_t index ) {
  CharacterRebuffOption rebuff_sequence_option(
      CharacterRebuffOption::kJSON_NAME + std::to_string( index ), 0 );

  rebuff_sequence_option.Clear();

  rebuff_sequence_option.SetStatus( true );

  const auto editcontrol_rebuff_sequence_interval =
      GWH( EDIT_REBUFF_SEQUENCE_INTERVAL );

  int sequence_interval =
      gwingui::editcontrol::GetInt( editcontrol_rebuff_sequence_interval );

  rebuff_sequence_option.SetRebuffInterval( sequence_interval );

  const auto listview_rebuff_key_sequence = GWH( LISTVIEW_REBUFF_KEY_SEQUENCE );

  for ( int i = 0, count = gwingui::listview::GetItemCount(
                       listview_rebuff_key_sequence );
        i < count; ++i ) {
    const auto row =
        gwingui::listview::GetItemRowText( listview_rebuff_key_sequence, i );

    Key key;

    key.key_code = optionutils::AsciiKeyToVirtualKeyCode( row[ 0 ] );
    key.time_ms_after_last_attack = std::stoi( row[ 1 ] );

    rebuff_sequence_option.AddValue( key );
  }

  return rebuff_sequence_option;
}

bool CharacterRebuffListOption::TryApplyOption() {
  // Save the currently selected sequence in case it has not been saved
  // the reason is because the user might have changed something without
  // selecting another sequence in the list that is the way it is saved
  const auto listbox_rebuff_sequence = GWH( LISTBOX_REBUFF_SEQUENCES );

  const auto selected_index =
      gwingui::listbox::GetSelectedIndex( listbox_rebuff_sequence );

  if ( selected_index != -1 ) {
    const auto charcter_rebuff_of_options =
        ReadCharacterRebuffOptions( selected_index );

    ReplaceRebuffSequence( selected_index, charcter_rebuff_of_options );
  }

  const auto checkbox_rebuff_sequence = GWH( CHECK_REBUFF_SEQUENCES );

  if ( gwingui::checkbox::IsChecked( checkbox_rebuff_sequence ) )
    SetStatus( true );
  else
    SetStatus( false );

  return true;
}

}  // namespace bot
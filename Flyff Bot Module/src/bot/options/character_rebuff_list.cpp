#include "pch.h"
#include "character_rebuff_list.h"
#include "../../res/resource.h"

#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/listbox.h"

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

}  // namespace bot
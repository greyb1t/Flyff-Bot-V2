#include "pch.h"
#include "key_sequence_option.h"

namespace bot {

const std::string KeySequenceOption::kKeysJsonName = "keys";

json KeySequenceOption::GetJson() {
  auto j = Option::GetJson();

  j[ kKeysJsonName ] = json::array();

  for ( const auto& value : values_ ) {
    j[ kKeysJsonName ].push_back(
        { value.key_code, value.time_ms_after_last_attack } );
  }

  return j;
}

void KeySequenceOption::LoadJson( const json& json_parent ) {
  Option::LoadJson( json_parent );

  if ( JsonExists( json_parent, name_ ) ) {
    const auto json_child = json_parent.at( name_ );
    LoadJsonKeySequenceObject( json_child );
  }
}

void KeySequenceOption::LoadJsonKeySequenceObject(
    const json& json_key_sequence_object ) {
  if ( JsonExists( json_key_sequence_object, kKeysJsonName ) ) {
    const auto json_keys = json_key_sequence_object.at( kKeysJsonName );

    for ( auto& elem : json_keys ) {
      Key key;
      key.key_code = elem[ 0 ].get<uint16_t>();
      key.time_ms_after_last_attack = elem[ 1 ].get<int32_t>();
      values_.push_back( key );
    }
  }
}

}  // namespace bot
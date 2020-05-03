#pragma once

#include "value_list_option.h"

namespace bot {

struct Key {
  uint16_t key_code;
  int32_t time_ms_after_last_attack;

  Key() {}

  bool operator==( const Key& other ) {
    return key_code == other.key_code &&
           time_ms_after_last_attack == other.time_ms_after_last_attack;
  }
};

class KeySequenceOption : public ValueListOption<Key> {
 public:
  KeySequenceOption( const std::string& name,
                     const uint32_t control_identifier )
      : ValueListOption<Key>( name, control_identifier ) {}

  json GetJson() override;

  void LoadJson( const json& json_parent ) override;
  void LoadJsonKeySequenceObject( const json& json_key_sequence_object );

  void EnableOrDisableControls( bool enable,
                                bot::BotOptions* bot_options ) override {}

 private:
  const static std::string kKeysJsonName;
};

}  // namespace bot
#pragma once

#include "key_sequence_option.h"
#include "../timer_on_interval.h"

namespace bot {

enum class CharacterRebuffReturnValue { kCharRebuffReady, kCharRebuffNotReady };

class CharacterRebuffOption : public KeySequenceOption {
 public:
  CharacterRebuffOption()
      : KeySequenceOption( "", 0 ), rebuff_interval_ms_( -1 ) {}

  CharacterRebuffOption( const std::string& name,
                         const uint32_t control_identifier )
      : KeySequenceOption( name, control_identifier ),
        rebuff_interval_ms_( -1 ) {}

  uint32_t GetRebuffInterval() const;

  void SetRebuffInterval( uint32_t interval );

  json GetJson() override;

  void LoadJson( const json& json_parent ) override;

  void RefreshControls() override;

  void EnableOrDisableControls( bool enable ) override;

  CharacterRebuffReturnValue Update();

  void ResetRebuffTimer();
  void ResetRebuffTimer20();

  static const std::string kJSON_NAME;

 private:
  uint32_t rebuff_interval_ms_;
  TimerOnInterval character_rebuff_timer_;

  static const std::string kRebuffIntervalJsonName;
};

}  // namespace bot
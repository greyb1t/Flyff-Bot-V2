#pragma once

#include "key_sequence_option.h"

namespace bot {

class AttackSequenceOption : public KeySequenceOption {
 public:
  AttackSequenceOption( const std::string& name,
                        const uint32_t control_identifier )
      : KeySequenceOption( name, control_identifier ) {}

  void RefreshControls( bot::BotOptions* bot_options ) override;

  void EnableOrDisableControls( bool enable,
                                bot::BotOptions* bot_options ) override;

  bool TryApplyOption() override;
};

}  // namespace bot
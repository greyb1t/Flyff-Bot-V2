#pragma once

#include "auto_healing_option.h"

namespace bot {

class AutoHealingPillsOption : public AutoHealingOption {
 public:
  AutoHealingPillsOption( const std::string& name,
                          const uint32_t control_identifier )
      : AutoHealingOption( name, control_identifier ) {}

  void RefreshControls( bot::BotOptions* bot_options ) override;

  void EnableOrDisableControls( bool enable,
                                bot::BotOptions* bot_options ) override;

  virtual bool TryApplyOption() override;
};

}  // namespace bot
#pragma once

#include "whitelisted_names_option.h"

namespace bot {

class WhitelistedPlayerNamesOption : public WhitelistedNamesOption {
 public:
  WhitelistedPlayerNamesOption( const std::string& name,
                                const uint32_t control_identifier )
      : WhitelistedNamesOption( name, control_identifier ) {}

  void RefreshControls( bot::BotOptions* bot_options ) override;

  void EnableOrDisableControls( bool enable,
                                bot::BotOptions* bot_options ) override;

  bool TryApplyOption() override;
};

}  // namespace bot
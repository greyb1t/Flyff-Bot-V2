#pragma once

#include "option.h"
#include "option_utils.h"
#include "../../res/resource.h"

#include "gwinguiv2/controls/combobox.h"

namespace bot {

enum class BotMode {
  kBotModeOneVsOne,

  kBotModeNone = -1
};

class BotModeOption : public Option {
 public:
  BotModeOption( const std::string& name, const uint32_t control_identifier )
      : Option( name, control_identifier ),
        bot_mode_( BotMode::kBotModeNone ) {}

  json GetJson() override;

  void LoadJson( const json& json_parent ) override;

  void RefreshControls() override;

  void EnableOrDisableControls( bool enable ) override {}

  BotMode GetBotMode() const;

  void SetBotMode( BotMode bot_mode );

  virtual bool TryApplyOption() override;

 private:
  BotMode bot_mode_;

  const static std::string kBotModeJsonName;
};

}  // namespace bot
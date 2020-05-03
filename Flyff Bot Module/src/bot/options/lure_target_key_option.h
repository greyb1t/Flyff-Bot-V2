#pragma once

#include "option.h"

namespace bot {

class LureTargetKeyOption : public Option {
 public:
  LureTargetKeyOption( const std::string& name,
                       const uint32_t control_identifier )
      : Option( name, control_identifier ) {}

  json GetJson() override;

  void LoadJson( const json& json_parent ) override;

  void RefreshControls( bot::BotOptions* bot_options ) override;

  void EnableOrDisableControls( bool enable,
                                bot::BotOptions* bot_options ) override {}

  uint16_t GetLureKeyCode();

  void SetLureKeyCode( uint16_t bot_mode );

  virtual bool TryApplyOption() override;

 private:
  uint16_t lure_key_code_;

  const static std::string kLureTargetKeyCodeJsonName;
};

}  // namespace bot
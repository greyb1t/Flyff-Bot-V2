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

  void RefreshControls() override;

  void EnableOrDisableControls( bool enable ) override {}

  uint16_t GetLureKeyCode() {
    return lure_key_code_;
  }

  void SetLureKeyCode( uint16_t bot_mode ) {
    lure_key_code_ = bot_mode;
  }

 private:
  uint16_t lure_key_code_;

  const static std::string kLureTargetKeyCodeJsonName;
};

}  // namespace bot
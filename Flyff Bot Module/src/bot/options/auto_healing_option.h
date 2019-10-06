#pragma once

#include "option.h"

namespace bot {

class AutoHealingOption : public Option {
 public:
  AutoHealingOption( const std::string& name,
                     const uint32_t control_identifier )
      : Option( name, control_identifier ),
        heal_key_code_( -1 ),
        health_limit_( -1 ) {}

  void SetHealKeyCode( uint16_t key_code );
  uint16_t GetHealCode() const;

  void SetHealthLimit( int32_t health );
  int32_t GetHealthLimit() const;

  json GetJson() override;

  void LoadJson( const json& json_parent ) override;

  void RefreshControls() override;

  void EnableOrDisableControls( bool enable ) override;

  virtual bool TryApplyOption() override;

 protected:
  uint16_t heal_key_code_;
  uint32_t health_limit_;

  const static std::string kHealKeyCodeJsonName;
  const static std::string kHealthLimitJsonName;
};

}  // namespace bot
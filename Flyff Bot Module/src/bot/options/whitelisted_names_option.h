#pragma once

#include "value_list_option.h"

namespace bot {

class WhitelistedNamesOption : public ValueListOption<std::string> {
 public:
  WhitelistedNamesOption( const std::string& name,
                          const uint32_t control_identifier )
      : ValueListOption<std::string>( name, control_identifier ) {}

  json GetJson() override;

  void LoadJson( const json& json_parent ) override;

  void RefreshControls( bot::BotOptions* bot_options ) override;

  void EnableOrDisableControls( bool enable,
                                bot::BotOptions* bot_options ) override;

  virtual bool IsEntityAllowed( const Entity& entity ) const;

  bool TryApplyOption() override;

 private:
  const static std::string kWhitelistedNamesJsonName;
};

}  // namespace bot
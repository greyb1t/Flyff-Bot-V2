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

  void RefreshControls() override;

  void EnableOrDisableControls( bool enable ) override;

  virtual bool IsEntityAllowed( const std::unique_ptr<Entity>& entity ) const;

  bool TryApplyOption() override;

 private:
  const static std::string kWhitelistedNamesJsonName;
};

}  // namespace bot
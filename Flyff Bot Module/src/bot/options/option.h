#pragma once

#include "../entity_filter.h"

#include "ext/json.hpp"

#include "gwinguiv2/controls/checkbox.h"
#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/message_box.h"

using namespace nlohmann;

class Option : public bot::EntityFilter {
 public:
  Option( const std::string& option_name, const uint32_t control_identifier )
      : enabled_( false ),
        name_( option_name ),
        control_identifier_( control_identifier ) {}

  // Option( const Option& option ) = delete;

  virtual bool TryApplyOption() {
    return true;
  }

  void SetStatus( bool enabled );

  bool IsEnabled() const;

  virtual json GetJson();

  virtual void LoadJson( const json& json_parent );

  bool JsonExists( const json& j, const std::string& key );

  virtual void RefreshControls();

  std::string GetName() const;

  virtual void EnableOrDisableControls( bool enable ) {}

  virtual bool IsEntityAllowed(
      const std::unique_ptr<bot::Entity>& entity ) const;

 protected:
  bool enabled_;
  std::string name_;
  uint32_t control_identifier_;

  const static std::string kOptionJsonName;
};
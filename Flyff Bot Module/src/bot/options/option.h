#pragma once

#include "../entity_filter.h"

#include "ext/json.hpp"

#include "gwinguiv2/controls/checkbox.h"
#include "gwinguiv2/controls/control.h"

using namespace nlohmann;

class Option : public bot::EntityFilter {
 public:
  Option( const std::string& option_name, const uint32_t control_identifier )
      : enabled_( false ),
        name_( option_name ),
        control_identifier_( control_identifier ) {}

  void SetStatus( bool enabled ) {
    enabled_ = enabled;
  }

  bool IsEnabled() const {
    return enabled_;
  }

  virtual json GetJson();

  virtual void LoadJson( const json& json_parent );

  bool JsonExists( const json& j, const std::string& key );

  virtual void RefreshControls() {
    if ( control_identifier_ ) {
      const auto checkbox_handle = GWH( control_identifier_ );
      gwingui::checkbox::SetCheck( checkbox_handle, enabled_ );
    }

    EnableOrDisableControls( enabled_ );
  }

  std::string GetName() const {
    return name_;
  }

  virtual void EnableOrDisableControls( bool enable ) {}

  virtual bool IsEntityAllowed( const bot::UniquePtrEntity& entity ) const {
    assert( false && "unhandled filter" );
    return false;
  }

 protected:
  bool enabled_;
  std::string name_;
  uint32_t control_identifier_;

  const static std::string kOptionJsonName;
};
#pragma once

#include "option.h"

#include "../../res/resource.h"

class AvoidEngagedMonsterOption : public Option {
 public:
  AvoidEngagedMonsterOption( const std::string& name,
                             const uint32_t control_identifier )
      : Option( name, control_identifier ) {}

  bool TryApplyOption() override {
    const auto checkbox_avoid_engaged_entities =
        GWH( CHECK_AVOID_ENGAGED_ENTITIES );

    if ( gwingui::checkbox::IsChecked( checkbox_avoid_engaged_entities ) )
      SetStatus( true );
    else
      SetStatus( false );

    return true;
  }

  virtual bool IsEntityAllowed( const bot::Entity& entity ) const {
    if ( IsEnabled() ) {
      if ( entity.IsEngaged() ) {
        return false;
      }
    }

    return true;
  }
};
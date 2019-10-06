#pragma once

#include "option.h"

#include "../../res/resource.h"

class UpdateCharPosOption : public Option {
 public:
  UpdateCharPosOption( const std::string& name,
                       const uint32_t control_identifier )
      : Option( name, control_identifier ) {}

  bool TryApplyOption() override {
    const auto checkbox_update_pos_each_kill =
        GWH( CHECK_UPDATE_POS_EACH_KILL );

    if ( gwingui::checkbox::IsChecked( checkbox_update_pos_each_kill ) )
      SetStatus( true );
    else
      SetStatus( false );

    return true;
  }
};
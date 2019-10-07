#include "pch.h"
#include "average_y_pos_option.h"

#include "../../res/resource.h"

void bot::AverageYPosOption::SetYPos( float y ) {
  average_y_pos_ = y;
}

float bot::AverageYPosOption::GetYPos() const {
  return average_y_pos_;
}

bool bot::AverageYPosOption::IsEntityAllowed( const Entity& entity ) const {
  if ( IsEnabled() )
    return entity.GetPosition().y >= average_y_pos_;
  else
    return true;
}

bool bot::AverageYPosOption::TryApplyOption() {
  const auto checkbox_average_y_pos = GWH( CHECK_AVERAGE_Y_POSITION );

  if ( gwingui::checkbox::IsChecked( checkbox_average_y_pos ) )
    SetStatus( true );
  else
    SetStatus( false );

  return true;
}

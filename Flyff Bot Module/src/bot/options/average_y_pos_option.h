#pragma once

#include "option.h"

namespace bot {

class AverageYPosOption : public Option {
 public:
  AverageYPosOption( const std::string& name,
                     const uint32_t control_identifier )
      : Option( name, control_identifier ) {}

  void SetYPos( float y ) {
    average_y_pos_ = y;
  }

  float GetYPos() const {
    return average_y_pos_;
  }

  virtual bool IsEntityAllowed( const bot::UniquePtrEntity& entity ) const {
    if ( IsEnabled() )
      return entity->GetPosition().y >= average_y_pos_;
    else
      return true;
  }

 private:
  float average_y_pos_;
};

}  // namespace bot
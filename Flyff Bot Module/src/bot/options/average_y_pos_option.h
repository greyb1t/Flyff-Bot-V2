#pragma once

#include "option.h"

namespace bot {

class AverageYPosOption : public Option {
 public:
  AverageYPosOption( const std::string& name,
                     const uint32_t control_identifier )
      : Option( name, control_identifier ) {}

  void SetYPos( float y );

  float GetYPos() const;

  virtual bool IsEntityAllowed( const Entity& entity ) const;

  bool TryApplyOption() override;

 private:
  float average_y_pos_;
};

}  // namespace bot
#pragma once

#include "option.h"

class RemoveAllObstacleOption : public Option {
 public:
  RemoveAllObstacleOption( const std::string& name,
                           const uint32_t control_identifier )
      : Option( name, control_identifier ) {}
  bool TryApplyOption() override;
};
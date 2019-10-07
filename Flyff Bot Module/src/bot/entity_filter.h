#pragma once

#include "entity.h"

namespace bot {

class EntityFilter {
 public:
  virtual bool IsEntityAllowed( const Entity& entity ) const = 0;
};

}  // namespace bot
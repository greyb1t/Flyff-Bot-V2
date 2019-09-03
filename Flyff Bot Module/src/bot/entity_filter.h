#pragma once

#include "entity.h"

namespace bot {

class EntityFilter {
 public:
  virtual bool IsEntityAllowed( const UniquePtrEntity& entity ) const = 0;
};

}
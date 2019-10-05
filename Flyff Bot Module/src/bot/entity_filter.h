#pragma once

#include "entity.h"

namespace bot {

class EntityFilter {
 public:
  virtual bool IsEntityAllowed( const std::unique_ptr<Entity>& entity ) const = 0;
};

}
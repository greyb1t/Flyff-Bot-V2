#pragma once

#include "../local_player.h"

namespace bot {

class LocalPlayerEclipse : public LocalPlayer {
 public:
  LocalPlayerEclipse( FlyffClient* client ) : LocalPlayer( client ) {}

  virtual std::unique_ptr<Entity> GetSelectedEntity();
};

}  // namespace bot
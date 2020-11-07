#pragma once

#include "../local_player.h"

namespace bot {

class LocalPlayerIgnite : public LocalPlayer {
 public:
  LocalPlayerIgnite( FlyffClient* client ) : LocalPlayer( client ) {}

  bool IsStandingStill() const override;
  bool IsRunning() const override;

  virtual std::unique_ptr<Entity> GetSelectedEntity();
  // bool IsEntitySelected() override;

  void MoveForward() const override;
  bool IsWalkingBackwards() const override;
};

}  // namespace bot
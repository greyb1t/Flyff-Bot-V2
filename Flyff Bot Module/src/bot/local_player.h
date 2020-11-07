#pragma once

#include "entity.h"
#include "back_camera.h"

namespace bot {

class LocalPlayer : public Entity {
 public:
  LocalPlayer( FlyffClient* client );
  virtual ~LocalPlayer();

  virtual std::unique_ptr<Entity> GetSelectedEntity();

  BackCamera GetCamera() const;

  void SetCameraRotX( float degrees ) const;
  void SetCameraRotY( float degrees ) const;

  float GetCameraRotX() const;
  float GetCameraRotY() const;

  void SetCameraMatrix( const D3DXMATRIX& mat ) const;

  float GetScrollDistance() const;
  void SetScrollDistance( float value ) const;

  bool IsEntitySelected();

  void SetMotion( int value ) const;
  virtual void MoveForward() const;
  virtual bool IsWalkingBackwards() const;
};

}  // namespace bot
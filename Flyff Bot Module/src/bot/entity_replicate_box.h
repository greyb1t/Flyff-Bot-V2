#pragma once

#include "entity.h"
#include "local_player.h"

namespace bot {

class EntityReplicateBox : public Entity {
 public:
  EntityReplicateBox( FlyffClient* client,
                      const UniquePtrLocalPlayer& local_player )
      : Entity( client, 1337 ),
        pos_( local_player->GetPosition() ),
        world_matrix_( local_player->GetWorldMatrix() ) {
    bound_box_.pos[ 0 ] = { -0.25f, 0.25f, -0.25f };
    bound_box_.pos[ 1 ] = { 0.25f, 0.25f, -0.25f };
    bound_box_.pos[ 2 ] = { 0.25f, 0.25f, 0.25f };
    bound_box_.pos[ 3 ] = { -0.25f, 0.25f, 0.25f };
    bound_box_.pos[ 4 ] = { -0.25f, -0.25f, -0.25f };
    bound_box_.pos[ 5 ] = { 0.25f, -0.25f, -0.25f };
    bound_box_.pos[ 6 ] = { 0.25f, -0.25f, 0.25f };
    bound_box_.pos[ 7 ] = { -0.25f, -0.25f, 0.25f };
  }

  D3DXVECTOR3 GetPosition() const override {
    return pos_;
  }

  void SetPosition( const D3DXVECTOR3 position ) override {
    // Required to modify these values to worldmatrix with world2screen
    // (D3DVec3Project)
    world_matrix_._41 = position.x;
    world_matrix_._42 = position.y;
    world_matrix_._43 = position.z;

    pos_ = position;
  }

  BOUND_BOX GetBoundBox() const override {
    return bound_box_;
  }

  void SetBoundBox( const BOUND_BOX& bound_box ) override {
    bound_box_ = bound_box;
  }

  void SetWorldMatrix( const D3DXMATRIX& matrix ) const override {
    // world_matrix_ = matrix;
    memcpy( ( void* )&world_matrix_, matrix, sizeof( matrix ) );
  }

  D3DXMATRIX GetWorldMatrix() const override {
    return world_matrix_;
  }

  bool IsMonster() const override {
    return true;
  }

  bool IsAlive() const override {
    return true;
  }

  virtual std::string GetName() const {
    return "";
  }

 private:
  D3DXVECTOR3 pos_;
  BOUND_BOX bound_box_;
  D3DXMATRIX world_matrix_;
};

}  // namespace bot
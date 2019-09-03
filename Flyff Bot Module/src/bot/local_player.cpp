#include "pch.h"
#include "local_player.h"
#include "botcore.h"
#include "../utils/trace.h"

#include "gwinmem/process_memory_internal.h"

namespace bot {

LocalPlayer::LocalPlayer( FlyffClient* client ) : Entity( client, 0 ) {
  const auto player_base_address =
      client->GetClientVar( MemoryContants::kPlayerBaseAddress );

  address_ptr_ =
      gwinmem::CurrentProcess().Read<uint32_t>( player_base_address );
}

LocalPlayer::~LocalPlayer() {}

UniquePtrEntity LocalPlayer::GetSelectedEntity() {
  const auto selected_entity_address =
      client_->GetClientVar( MemoryContants::kSelectedEntityAddress );
  const auto selected_entity_offset =
      client_->GetClientVar( MemoryContants::kSelectedEntityOffset );
  uint32_t selected_entity_ptr_address =
      gwinmem::CurrentProcess().Read<uint32_t>( selected_entity_address,
                                                { selected_entity_offset } );

  return client_->CreateEntity( selected_entity_ptr_address );
}

BackCamera LocalPlayer::GetCamera() const {
  const auto camera_address =
      client_->GetClientVar( MemoryContants::kCameraAddress );

  BackCamera camera =
      gwinmem::CurrentProcess().Read<BackCamera>( camera_address );

  return camera;
}

void LocalPlayer::SetCameraRotX( const float& degrees ) const {
  const auto camera_address =
      client_->GetClientVar( MemoryContants::kCameraAddress );
  gwinmem::CurrentProcess().Write(
      camera_address + offsetof( BackCamera, m_fRotx ), degrees );
}

void LocalPlayer::SetCameraRotY( const float& degrees ) const {
  const auto camera_address =
      client_->GetClientVar( MemoryContants::kCameraAddress );
  gwinmem::CurrentProcess().Write(
      camera_address + offsetof( BackCamera, m_fRoty ), degrees );
}

float LocalPlayer::GetCameraRotX() const {
  const auto camera_address =
      client_->GetClientVar( MemoryContants::kCameraAddress );
  float value = gwinmem::CurrentProcess().Read<float>(
      camera_address + offsetof( BackCamera, m_fRotx ) );
  return value;
}

float LocalPlayer::GetCameraRotY() const {
  const auto camera_address =
      client_->GetClientVar( MemoryContants::kCameraAddress );
  float value = gwinmem::CurrentProcess().Read<float>(
      camera_address + offsetof( BackCamera, m_fRoty ) );
  return value;
}

void LocalPlayer::SetCameraMatrix( const D3DXMATRIX& mat ) const {
  const auto camera_address =
      client_->GetClientVar( MemoryContants::kCameraAddress );
  gwinmem::CurrentProcess().Write(
      camera_address + offsetof( BackCamera, m_matView ), mat );
}

float LocalPlayer::GetScrollDistance() const {
  const auto scroll_distance_address =
      client_->GetClientVar( MemoryContants::kScrollDistanceAddress );
  float value =
      gwinmem::CurrentProcess().Read<float>( scroll_distance_address );
  return value;
}

void LocalPlayer::SetScrollDistance( float value ) const {
  const auto scroll_distance_address =
      client_->GetClientVar( MemoryContants::kScrollDistanceAddress );
  gwinmem::CurrentProcess().Write( scroll_distance_address, value );
}

bool LocalPlayer::IsEntitySelected() {
  const auto selected_entity_address =
      client_->GetClientVar( MemoryContants::kSelectedEntityAddress );
  const auto selected_entity_offset =
      client_->GetClientVar( MemoryContants::kSelectedEntityOffset );
  int val = gwinmem::CurrentProcess().Read<int>( selected_entity_address,
                                                 { selected_entity_offset } );
  return !!val;
}

void LocalPlayer::SetMotion( int value ) const {
  const auto movement_base_address =
      client_->GetClientVar( MemoryContants::kMovementOffset );
  const auto move_offset = client_->GetClientVar( MemoryContants::kMoveOffset );
  gwinmem::CurrentProcess().Write( address_ptr_ + movement_base_address,
                                   { move_offset }, value );
}

void LocalPlayer::MoveForward() const {
  SetMotion( 4 );
}

bool LocalPlayer::IsWalkingBackwards() const {
  return GetMotion() == 5;
}

}  // namespace bot
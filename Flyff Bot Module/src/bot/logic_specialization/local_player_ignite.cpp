#include "pch.h"
#include "local_player_ignite.h"
#include "../servers/flyff_client.h"
#include "..\entity_list.h"

#include "gwinmem/process_memory_internal.h"

namespace bot {

bool LocalPlayerIgnite::IsStandingStill() const {
  return GetMotion() == -2;
}

bool LocalPlayerIgnite::IsRunning() const {
  return GetMotion() == -5;
}

std::unique_ptr<Entity> LocalPlayerIgnite::GetSelectedEntity() {
  // Read the id from the entity class
  const auto entity_id_offset =
      client_->GetClientVar( MemoryContants::kIgniteEntityIdOffset );

  const auto selected_entity_id =
      LocalPlayer::GetSelectedEntity()->GetPointerAddress();

  EntityList entity_list( client_ );
  auto& entities = entity_list.GetMoverEntities();

  // Loop through all mover entities around me
  for ( auto& entity : entities ) {
    // Read the entity id
    uint32_t entity_id = gwinmem::CurrentProcess().Read<uint32_t>(
        entity->GetPointerAddress() + entity_id_offset );

    if ( entity_id == selected_entity_id )
      return std::move( entity );
  }

  return client_->CreateEntity( 0 );
}

void LocalPlayerIgnite::MoveForward() const {
  SetMotion( -5 );
}

bool LocalPlayerIgnite::IsWalkingBackwards() const {
  return GetMotion() == -6;
}

}  // namespace bot
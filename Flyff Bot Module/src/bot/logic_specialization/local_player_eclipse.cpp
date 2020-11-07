#include "pch.h"
#include "local_player_eclipse.h"
#include "../servers/flyff_client.h"
#include "..\entity_list.h"
#include "../utils/trace.h"

#include "gwinmem/process_memory_internal.h"

namespace bot {

std::unique_ptr<Entity> LocalPlayerEclipse::GetSelectedEntity() {
  const auto selected_entity_ptr =
      client_->GetClientVar( MemoryContants::kSelectedEntityAddress );

  return client_->CreateEntity( selected_entity_ptr );
}

}  // namespace bot
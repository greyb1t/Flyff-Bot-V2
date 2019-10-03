#include "pch.h"
#include "entity_list.h"
#include "botcore.h"
#include "../utils/trace.h"

#include "logging.h"

#include "gwinmem/process_memory_internal.h"

namespace bot {

EntityList::EntityList( FlyffClient* client ) : client_( client ) {
  ReadEntityList();
}

std::vector<UniquePtrEntity>& EntityList::GetMoverEntities() {
  return GetEntitiesOfType( ObjectType::kObjectTypeMover );
}

std::vector<UniquePtrEntity>& EntityList::GetCommonCtrlEntities() {
  return GetEntitiesOfType( ObjectType::kObjectTypeCommonCtrl );
}

std::vector<UniquePtrEntity>& EntityList::GetObjectEntities() {
  return GetEntitiesOfType( ObjectType::kObjectTypeObject );
}

std::vector<UniquePtrEntity>& EntityList::GetEntitiesOfType(
    const ObjectType object_type ) {
  return entitylist_list_[ static_cast<int>( object_type ) ];
}

void EntityList::ReadEntityList() {
  std::vector<EntityPointerData> entity_ptrs;

  auto entity_list_address =
      client_->GetClientVar( MemoryContants::kEntityListAddress );

  const short kEntityListPadding = 0x4;

  uint32_t entity_ptr_addr = 0;

  do {
    entity_ptr_addr =
        gwinmem::CurrentProcess().Read<uint32_t>( entity_list_address );

    // If the end has been reached
    if ( !entity_ptr_addr )
      break;

    const EntityPointerData current_entity_ptr_data = { entity_list_address,
                                                        entity_ptr_addr };

    // Check to see if the pointer to the entity already exists
    auto find_result =
        std::find_if( entity_ptrs.begin(), entity_ptrs.end(),
                      [&]( const EntityPointerData& entity_ptr_data ) -> bool {
                        return current_entity_ptr_data.entity_ptr_addr ==
                               entity_ptr_data.entity_ptr_addr;
                      } );

    // Was it not found?
    if ( find_result == entity_ptrs.end() )
      entity_ptrs.push_back( current_entity_ptr_data );

    // entity_ptrs.insert({ entity_list_address, entity_ptr_addr });

    entity_list_address = entity_list_address + kEntityListPadding;
  } while ( entity_ptr_addr );

  int value = 0;

  for ( int i = 0; i < entity_ptrs.size(); ++i ) {
    const auto& entity_ptr = entity_ptrs[ i ];

    // It is inefficient allocating an entity, then destroying it later if we
    // don't want it. I know...I'll fix it later, can't bother now.
    auto entity = client_->CreateEntity( entity_ptr.entity_ptr_addr );

    // if the entity is marked as deleted by the game, we treat it as such by
    // ignoring it
    if ( entity->IsDeleted() ) {
      continue;
    }

    // Is the entity invalid?
    // Due to us searching the entities in a linear order (beginning -> end),
    // we can use break to avoid continuing in the entity array to invalid
    // entities.
    if ( entity->IsInvalidMemory() ) {
      break;
    }

    const auto object_flags = entity->GetFlags();

    // Check if the object flags are invalid, fixes possible crashes
    if ( object_flags > OBJ_FLAG_MAX || object_flags == 0 ) {
      continue;
    }

    const ObjectType object_type = entity->GetObjectType();

    // Avoid reading all the different types of objects, the reason being that
    // the objects in flyff is using polymorphism and each object have different
    // sizes based on their types, if we do not filter, we'll risk reading /
    // writing bad memory and crashing the client or bot.
    if ( object_type >= ObjectType::kObjectTypeObject &&
         object_type < ObjectType::kMaxObjectTypes ) {
      entitylist_list_[ static_cast<int>( object_type ) ].push_back(
          std::move( entity ) );
    }
  }

  // std::array<std::vector<UniquePtrEntity>, ObjectType::kMaxObjectTypes - 1>
  // entitylist_list;

  // std::set<EntityPointerData> entity_ptrs;

  // auto entity_list_address =
  // client->GetClientVar(MemoryContants::kEntityListAddress);

  // const short kEntityListPadding = 0x4;

  // uint32_t entity_ptr_addr = 0;

  // do {
  //  entity_ptr_addr = 0;
  //  bool read_ret = gWin::ProcessInternal().Read(entity_list_address,
  //  &entity_ptr_addr);

  //  // If the end has been reached
  //  if (!entity_ptr_addr || !read_ret)
  //    break;

  //  // STD::SET, DOES IT OVERWRITE THE FIRST VALUE BECAUSE IT CANNOT EXISTS
  //  TWO OF THEM?

  //  entity_ptrs.insert({ entity_list_address, entity_ptr_addr });
  //  entity_list_address = entity_list_address + kEntityListPadding;
  //} while (entity_ptr_addr);

  // for (auto &entity_ptr : entity_ptrs) {

  //  // It is inefficient allocating an entity, then destroying it later if we
  //  don't want it.
  //  // I know...I'll fix it later, can't bother now.
  //  auto entity = client->CreateEntity(entity_ptr.entity_ptr_addr);

  //  // Is the entity invalid?
  //  if (entity->IsDeletedOrInvalidMemory())
  //    continue;

  //  const ObjectType object_type = entity->GetObjectType();

  //  // Avoid reading all the different types of objects, the reason being that
  //  // the objects in flyff is using polymorphism and each object have
  //  different sizes
  //  // based on their types, if we do not filter, we'll risk reading / writing
  //  bad memory
  //  // and crashing the client or bot.
  //  if (object_type >= ObjectType::kObjectTypeObject &&
  //    object_type <= ObjectType::kMaxObjectTypes - 1) {
  //    entitylist_list[object_type].push_back(std::move(entity));
  //  }
  //}

  // return entitylist_list;
}

}  // namespace bot
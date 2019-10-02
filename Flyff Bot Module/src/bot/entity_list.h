#pragma once

#include "entity.h"
#include "local_player.h"

namespace bot {

class EntityList {
 public:
  EntityList( FlyffClient* client );

  std::vector<UniquePtrEntity>& GetMoverEntities();
  std::vector<UniquePtrEntity>& GetCommonCtrlEntities();
  std::vector<UniquePtrEntity>& GetObjectEntities();

  std::vector<UniquePtrEntity>& GetEntitiesOfType(
      const ObjectType object_type );

 private:
  void ReadEntityList( );

 private:
  FlyffClient* client_;
  std::array<std::vector<UniquePtrEntity>,
             static_cast<int>( ObjectType::kMaxObjectTypes )>
      entitylist_list_;
};

}  // namespace bot
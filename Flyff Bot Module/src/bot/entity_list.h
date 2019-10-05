#pragma once

#include "entity.h"
#include "local_player.h"

namespace bot {

class EntityList {
 public:
  EntityList( FlyffClient* client );

  std::vector<std::unique_ptr<Entity>>& GetMoverEntities();
  std::vector<std::unique_ptr<Entity>>& GetCommonCtrlEntities();
  std::vector<std::unique_ptr<Entity>>& GetObjectEntities();

  std::vector<std::unique_ptr<Entity>>& GetEntitiesOfType(
      const ObjectType object_type );

 private:
  void ReadEntityList();

 private:
  FlyffClient* client_;
  std::array<std::vector<std::unique_ptr<Entity>>,
             static_cast<int>( ObjectType::kMaxObjectTypes )>
      entitylist_list_;
};

}  // namespace bot
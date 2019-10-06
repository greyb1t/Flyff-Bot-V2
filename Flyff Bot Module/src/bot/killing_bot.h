#pragma once

#include "bot.h"

namespace bot {

class KillingBot : public Bot {
 public:
  KillingBot( BotCore* botcore,
              int origin_state,
              const std::wstring& bot_name );
  virtual ~KillingBot() {}

  // Returns true or false based on whether an entity was found
  std::unique_ptr<Entity> FindNearestMonster(
      std::vector<std::unique_ptr<Entity>>& entities,
      const LocalPlayer& local_player,
      std::vector<const EntityFilter*>& entity_filters );

  bool IsEntityBlacklisted( const Entity& entity );

  void RestoreBlockedBoundBoxes();
  void RestoreSavedBoundBoxes();

 protected:
  std::vector<Entity> blacklisted_entities_temporary_;

  std::vector<float> last_entity_y_positions_;

  int entities_not_found_counter_;

  std::vector<std::unique_ptr<Entity>> saved_selection_blocked_entities_;
  std::vector<std::unique_ptr<Entity>> saved_bound_box_changed_entities_;

  uint32_t monster_kill_count_;
};

}  // namespace bot
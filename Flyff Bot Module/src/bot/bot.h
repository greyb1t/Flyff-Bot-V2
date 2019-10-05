#pragma once

#include <string>

#include "local_player.h"
#include "state_machine.h"
#include "../utils/stopwatch.h"

#define DO_ONCE( lambda_to_execute )                     \
  {                                                      \
    auto id_value = __COUNTER__;                         \
    auto value = execute_once_map_.find( id_value );     \
                                                         \
    if ( value == execute_once_map_.end() ) {            \
      lambda_to_execute();                               \
      execute_once_map_[ id_value ] = lambda_to_execute; \
    }                                                    \
  }

class DoOnce;

namespace bot {

class EntityFilter;
class BotCore;
class Entity;
class LocalPlayer;

enum class OneVsOneStates;

struct EntityPointerData {
  uint32_t entity_addr;
  uint32_t entity_ptr_addr;

  bool operator==( const EntityPointerData& rhs ) const {
    return entity_ptr_addr == rhs.entity_ptr_addr;
  }

  bool operator<( const EntityPointerData& rhs ) const {
    return entity_ptr_addr < rhs.entity_ptr_addr;
  }
};

class Bot : public StateMachine {
 public:
  Bot( BotCore* botcore, int origin_state, const std::wstring& bot_name );
  virtual ~Bot();

  virtual void Update() = 0;

  void SortEntitiesByDistanceToEntity(
      const Entity& entity,
      std::vector<std::unique_ptr<Entity>>& entities );

  // Returns true or false based on whether an entity was found
  std::unique_ptr<Entity> FindNearestMonster(
      std::vector<std::unique_ptr<Entity>>& entities,
      const LocalPlayer& local_player,
      std::vector<const EntityFilter*>& entity_filters );

  void AdjustCameraTowardsEntity( const LocalPlayer* local_player,
                                  const Entity& entity );

  bool IsEntityBlacklisted( const Entity& entity );

  void DeSelectEntity();

  bool IsEntityValid( const Entity& entity );

  void LogOnce( DoOnce& do_once, const std::wstring& text );
  // bool IsEntityWhitelisted( const UniquePtrEntity& entity );

  // Returns the entity if found, nullptr otherwise
  std::unique_ptr<Entity> IsNonWhitelistedPlayerFound(
      const std::vector<std::unique_ptr<Entity>>& entities,
      const LocalPlayer& local_player );

  // bool IsEntityAboveAverageYPosition( const UniquePtrEntity& entity );

  void RestoreBlockedBoundBoxes();
  void RestoreSavedBoundBoxes();

  // NOTE: Deprecated
  // Returns false if the entity is outside of the view (outside window)
  bool ClickEntity( const Entity& entity );

  // Returns true if the entity is visible and false if not.
  bool GetEntityScreenPosition( const Entity& entity,
                                POINT& entity_screen_pos );

  void OnStateChanging() override;

  // Getters
  BotCore* GetBotCore();
  std::unique_ptr<LocalPlayer>& GetLocalPlayer();
  bool IsStateStopped();

  template <typename T>
  T GetCurrentTargetSelectionState() {
    return static_cast<T>( current_target_selection_state_ );
  }

  // Setters
  void SetIsStopped( bool value );

  template <typename T>
  void SetNextTargetSelectionState( T next_state );

 protected:
  BotCore* botcore_;

  // TODO: See if I can remove the unique_ptr from local_player_, just like
  // WhitelistedNames is...
  std::unique_ptr<LocalPlayer> local_player_;
  bool is_state_stopped_;

  std::vector<Entity> blacklisted_entities_temporary_;
  std::vector<Entity> blacklisted_entities_permanent_;

  // float average_y_pos_;
  std::vector<float> last_entity_y_positions_;

  int entities_not_found_counter_;

  std::vector<std::unique_ptr<Entity>> saved_selection_blocked_entities_;
  std::vector<std::unique_ptr<Entity>> saved_bound_box_changed_entities_;

  std::map<int, std::function<void()>> execute_once_map_;

  std::wstring bot_name_;
  uint32_t monster_kill_count_;
  Stopwatch bot_duration_stopwatch_;

 private:
  int current_target_selection_state_;
};

template <typename T>
void Bot::SetNextTargetSelectionState( T next_state ) {
  current_target_selection_state_ = static_cast<T>( next_state );
}

}  // namespace bot
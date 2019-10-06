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

namespace bot {

class EntityFilter;
class BotCore;
class Entity;
class LocalPlayer;
class WhitelistedPlayerNamesOption;

enum class BaseBotStates { Running, Stopped };

class Bot : public StateMachine {
 public:
  Bot( BotCore* botcore, int origin_state, const std::wstring& bot_name );
  virtual ~Bot();

  void Update();
  virtual void OnStop() = 0;

  void SortEntitiesByDistanceToEntity(
      const Entity& entity,
      std::vector<std::unique_ptr<Entity>>& entities );

  void AdjustCameraTowardsEntity( const LocalPlayer* local_player,
                                  const Entity& entity );

  void DeSelectEntity();

  bool IsEntityValid( const Entity& entity );

  // Returns the entity if found, nullptr otherwise
  std::unique_ptr<Entity> IsNonWhitelistedPlayerFound(
      const WhitelistedPlayerNamesOption& whitelisted_names_option,
      const std::vector<std::unique_ptr<Entity>>& entities,
      const LocalPlayer& local_player );

  // Returns true if the entity is visible and false if not.
  bool GetEntityScreenPosition( const Entity& entity,
                                POINT& entity_screen_pos );

  void OnStateChanging() override;

  BotCore* GetBotCore();
  std::unique_ptr<LocalPlayer>& GetLocalPlayer();

  void SetInternalState( BaseBotStates state );
  BaseBotStates GetInternalState() const;

 protected:
  virtual void UpdateInternal() = 0;

  BotCore* botcore_;

  std::unique_ptr<LocalPlayer> local_player_;

  std::map<int, std::function<void()>> execute_once_map_;

  std::wstring bot_name_;
  Stopwatch bot_duration_stopwatch_;

 private:
  // We use an internal state machine for the overall bot state, running or
  // stopped, to avoid having to handle this in every implementation of every
  // possible bot
  StateMachine internal_bot_state_machine_;
};

}  // namespace bot
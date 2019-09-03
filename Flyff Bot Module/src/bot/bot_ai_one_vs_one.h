#pragma once

#include "bot.h"
#include "key_sequence.h"
#include "blocked_state_machine.h"
#include "walk_to_position_state_machine.h"
#include "idle_state_machine.h"
#include "options/character_rebuff.h"

namespace bot {

class LocalPlayer;

enum class OneVsOneStates {
  kFindingTarget,
  kFocusOnTarget,
  kSelectTarget,
  kVerifyTarget,
  kLureTarget,
  kBeginAttackTarget,
  kStartedAttackingTarget,
  kStartedHittingTarget,
  kBlockedTypeRunningIntoObstacle,
  kBlockedTypeShootThroughObstacle,
  kBlockedSelectingPlayerInsideChar,
  kWaitUntilPlayerLeaves,
  kCheckAroundForPlayers,
  kRebuffCharacter,
  kUpdateCharacterPosition,
  kIdle,
  kCharacterDied,
  kManualStop
};

class BotAIOneVsOne : public Bot {
 public:
  BotAIOneVsOne( BotCore* botcore );
  virtual ~BotAIOneVsOne();

  void Update() override;

  void OnStateChanging() override;

 private:
  UniquePtrEntity current_target_entity_;
  int32_t nearest_entity_hp_when_selected_;
  int32_t selected_entity_hp_when_started_hitting_;

  bool has_stopped_running_;
  bool has_started_running_;
  bool has_started_standing_still_;

  int has_selected_unintended_target_count_;

  TimerOnInterval begin_target_attack_not_moving_timer;
  TimerOnInterval running_target_blocked_timer;
  TimerOnInterval block_check_while_attacking_timer;
  TimerOnInterval block_check_while_attacking_timer2;
  TimerOnInterval blocked_move_forward_timer;
  TimerOnInterval wait_until_player_leaves_timer_;

  CharacterRebuffOption* current_rebuff_;

  std::vector<CharacterRebuffOption> rebuff_list_;

  // UniquePtrEntity pet_entity_;

  int check_around_counter_;

  bool has_checked_hp_;

  bool started_walking_backwards_;

  int32_t local_player_health_start_;

  FocusTargetStateMachine focus_target_machine_;
  SelectTargetStateMachine select_target_machine_;
  SimulationStateMachine simulation_machine_;
  KeySequence attack_sequence_;
  BlockedStateMachine blocked_state_machine_;
  WalkToPositionStateMachine walk_to_pos_machine_;
  KeySequence rebuff_sequence_;
  IdleStateMachine idle_machine_;

  std::vector<const EntityFilter*> default_filter_;
};

}  // namespace bot
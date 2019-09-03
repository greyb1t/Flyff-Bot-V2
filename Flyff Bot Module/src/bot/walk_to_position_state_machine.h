#pragma once

#include "state_machine.h"
#include "entity.h"
#include "focus_target_state_machine.h"
#include "select_target_state_machine.h"

namespace bot {

class SimulationStateMachine;

enum class WalkToStateMachineStates {
  kFocusTargetPosition,
  kClickTargetPosition,
  kWaitUntilPositionReached,
};

class Bot;

class WalkToPositionStateMachine : public StateMachine {
 public:
  WalkToPositionStateMachine( Bot* bot )
      : StateMachine( static_cast<int>(
            WalkToStateMachineStates::kFocusTargetPosition ) ),
        focus_target_machine_( bot ),
        select_target_machine_( bot ),
        bot_( bot ) {}

  StateStatusReturnValue WalkToPosition(
      const UniquePtrLocalPlayer& local_player,
      SimulationStateMachine& simulation,
      const D3DXVECTOR3& target_position );

 private:
  Bot* bot_;
  UniquePtrEntity target_box_;

  FocusTargetStateMachine focus_target_machine_;
  SelectTargetStateMachine select_target_machine_;
};

}  // namespace bot
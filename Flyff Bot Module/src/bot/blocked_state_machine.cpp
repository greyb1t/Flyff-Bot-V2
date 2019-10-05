#include "pch.h"
#include "blocked_state_machine.h"
#include "local_player.h"
#include "walk_to_position_state_machine.h"

namespace bot {

StateStatusReturnValue BlockedStateMachine::Update(
    const LocalPlayer& local_player,
    SimulationStateMachine& simulation,
    WalkToPositionStateMachine& walk_to_pos_machine ) {
  const auto current_state = GetCurrentState<BlockedStates>();

  switch ( current_state ) {
    case BlockedStates::kWalkBackwards: {
      // instead of Getposition, calculate the position backwards from the
      // character with math
      const auto walk_pos_status = walk_to_pos_machine.WalkToPosition(
          local_player, simulation, local_player.GetPosition() );

      switch ( walk_pos_status ) {
        case StateStatusReturnValue::kSucceeded: {
        } break;

        default:
          break;
      }
    } break;

    case BlockedStates::kWalkLeft: {
      // instead of Getposition, calculate the position backwards from the
      // character with math
      const auto walk_pos_status = walk_to_pos_machine.WalkToPosition(
          local_player, simulation, local_player.GetPosition() );

      switch ( walk_pos_status ) {
        case StateStatusReturnValue::kSucceeded: {
        } break;

        default:
          break;
      }
    } break;

    default:
      break;
  }

  return StateStatusReturnValue::kFailed;
}

}  // namespace bot
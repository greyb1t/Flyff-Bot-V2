#include "pch.h"
#include "walk_to_position_state_machine.h"
#include "entity_replicate_box.h"
#include "botcore.h"

namespace bot {

StateStatusReturnValue WalkToPositionStateMachine::WalkToPosition(
    const UniquePtrLocalPlayer& local_player,
    SimulationStateMachine& simulation,
    const D3DXVECTOR3& target_position ) {
  // TODO:
  // If gotten stuck, walk backwards, then sideways
  // Try get the monster again
  // If stuck again, repeat first step.
  // Check the distance to the box we placed on the ground to know when to try
  // get the monster again. or by checking if the player is standing still or
  // both to determine whether is has been blocked when trying to unblock

  const auto current_state = GetCurrentState<WalkToStateMachineStates>();

  switch ( current_state ) {
    case WalkToStateMachineStates::kFocusTargetPosition: {
      const auto botcore = bot_->GetBotCore();
      target_box_ = std::make_unique<EntityReplicateBox>(
          botcore->GetFlyffClient(), local_player );

      // THE PROBLEM IS THAT SETPOSITION USES THE "Entity" class version of the
      // function and not the replicatebox version
      target_box_->SetPosition( target_position );

      UniquePtrEntity boxx = std::make_unique<EntityReplicateBox>(
          botcore->GetFlyffClient(), local_player );

      boxx->SetPosition( target_position );

      botcore->AddEntityToDraw( boxx );

      const auto focus_status = focus_target_machine_.Focus( target_box_ );

      switch ( focus_status ) {
        case StateStatusReturnValue::kInProgress:
          break;

        case StateStatusReturnValue::kSucceeded: {
          SetNextState( WalkToStateMachineStates::kClickTargetPosition );
        } break;

        default:
          break;
      }
    } break;

    case WalkToStateMachineStates::kClickTargetPosition: {
      const auto select_target_status =
          select_target_machine_.Select( target_box_ );

      switch ( select_target_status ) {
        case StateStatusReturnValue::kSucceeded: {
          SetNextState( WalkToStateMachineStates::kWaitUntilPositionReached );
        } break;

        case StateStatusReturnValue::kFailed:
          assert( false && "HOW DO I HANDLE THIS SHIT FUCK DICK WHORE BITCH" );
          return StateStatusReturnValue::kFailed;
          break;

        case StateStatusReturnValue::kInProgress:
          break;

        default:
          break;
      }
    } break;

    case WalkToStateMachineStates::kWaitUntilPositionReached: {
      // TODO: Change the DistanceTo parameter to UniquePtrEntity
      if ( local_player->DistanceTo( target_box_.get() ) <= 0.1f )
        return StateStatusReturnValue::kSucceeded;
    } break;

    default:
      break;
  }

  return StateStatusReturnValue::kInProgress;
}

}  // namespace bot
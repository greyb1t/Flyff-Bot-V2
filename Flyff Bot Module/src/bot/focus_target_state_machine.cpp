#include "pch.h"
#include "focus_target_state_machine.h"
#include "bot.h"
#include "logging.h"

namespace bot {

StateStatusReturnValue
FocusTargetStateMachine::Focus( const
                                UniquePtrEntity &entity ) {
  const TargetFocusStates target_focus_state =
    GetCurrentState<TargetFocusStates>();

  switch ( target_focus_state ) {

    case TargetFocusStates::AdjustCamera: {
      logging::Log(
        TEXT( "Adjusting the camera towards the entity.\n" ) );

      const auto &local_player = bot_->GetLocalPlayer();
      bot_->AdjustCameraTowardsEntity(
        local_player.get(), entity );

      // Wait for the camera to adjust before selecting the entity
      // bot_->GetBotCore()->SkipUpdateForFrames(5);

      SetNextState( TargetFocusStates::kWaitForCamera );
    }
    break;

    case TargetFocusStates::kWaitForCamera: {
      wait_timer_.DoEachIntervalAfter( 200, [&]() {
        SetNextState(
          TargetFocusStates::CheckEntityVisibility );
      } );
    }
    break;

    case TargetFocusStates::CheckEntityVisibility: {
      POINT entity_screen_pos;

      // If the entity is visible, then continue to next state
      if ( bot_->GetEntityScreenPosition( entity,
                                          entity_screen_pos ) ) {

        // Reset the state for the next time
        SetNextState( TargetFocusStates::AdjustCamera );

        return StateStatusReturnValue::kSucceeded;
      } else {
        logging::Log(
          TEXT( "The entity was outside the screen, adjusting the camera accordingly.\n" ) );
        SetNextState( TargetFocusStates::AdjustCamera );
      }
    }
    break;

    default:
      break;
  }

  return StateStatusReturnValue::kInProgress;
}

}
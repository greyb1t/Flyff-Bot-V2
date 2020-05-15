#include "pch.h"
#include "focus_target_state_machine.h"
#include "bot.h"
#include "logging.h"

namespace bot {

StateStatusReturnValue FocusTargetStateMachine::Focus( const Entity& entity ) {
  const TargetFocusStates target_focus_state =
      GetCurrentState<TargetFocusStates>();

  switch ( target_focus_state ) {
    case TargetFocusStates::AdjustCamera: {
      logging::Log( TEXT( "Adjusting the camera towards the entity.\n" ) );

      const auto& local_player = bot_->GetLocalPlayer();
      bot_->AdjustCameraTowardsEntity( local_player.get(), entity );

      SetNextState( TargetFocusStates::CheckEntityVisibility );
    } break;

    case TargetFocusStates::CheckEntityVisibility: {
      POINT entity_screen_pos;

      // If the entity is visible
      if ( bot_->GetEntityScreenPosition( entity, entity_screen_pos ) ) {
        if ( prev_entity_screen_pos_.x == 0 &&
             prev_entity_screen_pos_.y == 0 ) {
          prev_entity_screen_pos_ = entity_screen_pos;
        } else {
          // Since flyff has smoothed out their camera movement when changing the
          // rotation value, we have to wait for the camera to almost stop
          // If we do not, there is a chance the camera moves so quickly, the bot
          // misses the entity because it has an old screen position of the monster

          // Compare the change made on the position of the entity on the screen to
          // determine whether or not the camera has not fully adjusted yet
          const auto prev_entity_screen_pos_dist =
              sqrt( prev_entity_screen_pos_.x * prev_entity_screen_pos_.x +
                    prev_entity_screen_pos_.y * prev_entity_screen_pos_.y );

          const auto current_entity_screen_pos_dist =
              sqrt( entity_screen_pos.x * entity_screen_pos.x +
                    entity_screen_pos.y * entity_screen_pos.y );

          const auto dist_delta = abs( current_entity_screen_pos_dist -
                                       prev_entity_screen_pos_dist );

          const auto kMinimumDistanceDelta = 10;

          // If the delta is so low, that almost no changes were made, we are ready to click the entity
          if ( dist_delta <= kMinimumDistanceDelta ) {
            // Reset the state for the next time
            SetNextState( TargetFocusStates::AdjustCamera );

            // Reset the previous entity pos
            prev_entity_screen_pos_ = { 0 };

            return StateStatusReturnValue::kSucceeded;
          } else {
            // If the camera still moves a lot, save the recent entity pos to compare on the next frame
            prev_entity_screen_pos_ = entity_screen_pos;
          }
        }
      }
    } break;

    default:
      break;
  }

  return StateStatusReturnValue::kInProgress;
}

}  // namespace bot
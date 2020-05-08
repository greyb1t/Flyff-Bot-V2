#include "pch.h"
#include "select_target_state_machine.h"
#include "botcore.h"
#include "logging.h"

namespace bot {

StateStatusReturnValue SelectTargetStateMachine::Select(
    const Entity& entity ) {
  /*
    Eclipse Flyff added protection against bots by checking if the 
    cursor is on a monster when the user selects it.

    That ruined the bot because I reset the cursor pos IMMEDIATELY 
    after I have clicked on the monster. 
    Therefore, the game registers it as I never had my mouse on the monster.

    Simply fixed by never reseting the cursor positin after having selected an entity.
  */

  const auto target_selection_state = GetCurrentState<TargetSelectionStates>();

  const auto botcore = bot_->GetBotCore();

  switch ( target_selection_state ) {
    case TargetSelectionStates::HoverCursor: {
      logging::Log( TEXT( "Hovering the cursor over the entity: " ) +
                    std::to_wstring( entity.GetPointerAddress() ) +
                    TEXT( ".\n" ) );

      POINT entity_screen_pos;

      // If the entity is visible
      if ( bot_->GetEntityScreenPosition( entity, entity_screen_pos ) ) {
        logging::Log( TEXT(
            "The entity is visible and now trying to hover over it.\n" ) );

        // Save the current cursor position
        GetCursorPos( &old_cursor_pos_ );

        // Force the target process to return the following position from
        // GetCursorPos
        botcore->SetSimulatedCursorPos( entity_screen_pos );

        SetNextState( TargetSelectionStates::Click );
      } else {
        logging::Log(
            TEXT( "The entity could not be selected because not visible and "
                  "has been blacklisted temporarily." ) );

        return StateStatusReturnValue::kFailed;
      }
    } break;

    case TargetSelectionStates::Click: {
      const auto simulation_status = simulation_machine_.SimulateMouseclick(
          MouseClickButtons::kLeftMouseButton );

      switch ( simulation_status ) {
        case StateStatusReturnValue::kSucceeded: {
          logging::Log( TEXT( "Clicked on the entity.\n" ) );

          // Set both the simulated and real cursor pos, because simulated
          // cursor pos is not enough.
          //botcore->SetSimulatedCursorPos( old_cursor_pos_ );

          // If the cursor is inside the flyff window, then set the real cursor
          // pos to the previous one If we always do it, the cursor lags when
          // doing other stuff
          if ( WindowFromPoint( old_cursor_pos_ ) ==
               botcore->GetTargetWindow() ) {
            //SetCursorPos( old_cursor_pos_.x, old_cursor_pos_.y );
          }

          SetNextState( TargetSelectionStates::ResetCursor );
        } break;

        default:
          break;
      }

      // simulation::SendVirtualMouseClick(botcore->GetTargetWindow(), 50);
    } break;

    case TargetSelectionStates::ResetCursor: {
      // Prevent the bot from locking the cursor
      //POINT invalid_screen_pos{ -1, -1 };
      //botcore->SetSimulatedCursorPos( invalid_screen_pos );

      // Reset the TargetSelectionState for next entity
      SetNextState( TargetSelectionStates::HoverCursor );

      return StateStatusReturnValue::kSucceeded;
    } break;

    default:
      break;
  }

  return StateStatusReturnValue::kInProgress;
}

}  // namespace bot
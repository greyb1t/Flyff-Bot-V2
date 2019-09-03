#include "pch.h"
#include "simluation_state_machine.h"
#include "botcore.h"
#include "../utils/simulation.h"
#include "logging.h"

namespace bot {

StateStatusReturnValue SimulationStateMachine::KeyPress(
    const uint16_t key_code,
    const uint32_t ms_delay ) {
  // Add a keypress with the condition of the time has to pass the ms_delay
  // then return true to tell the function that we want a key up now.
  return KeyPress( key_code, [&]() -> bool {
    bool ret = false;

    wait_timer_.DoEachIntervalAfter( ms_delay, [&]() {
      ret = true;
    } );

    return ret;
  } );
}

StateStatusReturnValue SimulationStateMachine::KeyPress(
    const uint16_t key_code,
    const std::function<bool()> should_key_up ) {
  const auto botcore = bot_->GetBotCore();
  const auto current_state = GetCurrentState<PressStates>();

  switch ( current_state ) {
    case PressStates::Down: {
#if 0
      logging::Log( TEXT( "KeyDown. - " ) +
                    std::to_wstring( static_cast<int>( current_state ) ) +
                    TEXT( "\n" ) );
#endif

      simulation::SendVirtualKeyDown( botcore->GetTargetWindow(), key_code );

      SetNextState( PressStates::kWaitABit );

      return StateStatusReturnValue::kInProgress;
    } break;

    case PressStates::kWaitABit: {
      // wait_timer_.DoEachIntervalAfter(
      //     ms_delay, [&]() { SetNextState( PressStates::Up ); } );

      /*
      logging::Log( TEXT( "kWaitABit. - " ) +
                    std::to_wstring( static_cast<int>( current_state ) ) +
                    TEXT( "\n" ) );
      */

      if ( !should_key_up() )
        return StateStatusReturnValue::kInProgress;
      else {
        SetNextState( PressStates::Up );
        return StateStatusReturnValue::kInProgress;
      }
    } break;

    case PressStates::Up: {
#if 0
      logging::Log( TEXT( "KeyUp. - " ) +
                    std::to_wstring( static_cast<int>( current_state ) ) +
                    TEXT( "\n" ) );
#endif

      // Changed so that it pressed it again instead (THIS WORKED, BUT THIS
      // CAUSED THE CHARACTER TO HOLD 'S')
      // simulation::SendVirtualKeyDown(botcore->GetTargetWindow(), key_code,
      // 50);

      simulation::SendVirtualKeyUp( botcore->GetTargetWindow(), key_code );

      Reset();

      return StateStatusReturnValue::kSucceeded;
    } break;

    default:
      break;
  }

  assert( false && ( std::string( "should never fucking happen" ) +
                     std::to_string( ( int )current_state ) )
                       .c_str() );

  return StateStatusReturnValue::kFailed;
}

StateStatusReturnValue SimulationStateMachine::SimulateMouseclick(
    const MouseClickButtons btn ) {
  const auto botcore = bot_->GetBotCore();
  const auto current_state = GetCurrentState<PressStates>();

  switch ( current_state ) {
    case PressStates::Down: {
      SendMessage( botcore->GetTargetWindow(), WM_LBUTTONDOWN, 0, 0 );

      SetNextState( PressStates::Up );

      return StateStatusReturnValue::kInProgress;
    } break;

    case PressStates::Up: {
      SendMessage( botcore->GetTargetWindow(), WM_LBUTTONUP, 0, 0 );

      // Reset the state for next time
      SetNextState( PressStates::Down );

      Reset();

      return StateStatusReturnValue::kSucceeded;
    } break;

    default:
      break;
  }

  return StateStatusReturnValue::kFailed;
}

void SimulationStateMachine::Reset() {
  // Reset the state for next time
  SetNextState( PressStates::Down );
  wait_timer_.Reset();
}

}  // namespace bot
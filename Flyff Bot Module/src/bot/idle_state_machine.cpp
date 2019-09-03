#include "pch.h"
#include "idle_state_machine.h"

namespace bot {

StateStatusReturnValue IdleStateMachine::Idle() {
  StateStatusReturnValue ret_val = StateStatusReturnValue::kInProgress;

  wait_timer_.DoEachIntervalAfter(
      wait_ms_, [&]() { ret_val = StateStatusReturnValue::kSucceeded; } );

  return ret_val;
}

void IdleStateMachine::SetWaitMs( const uint32_t ms ) {
  wait_ms_ = ms;
}

void IdleStateMachine::NextState( uint32_t state ) {
  next_state_ = state;
}

uint32_t IdleStateMachine::GetNextState() {
  return next_state_;
}

void IdleStateMachine::ResetTimer() {
  wait_timer_.Reset();
}

}  // namespace bot
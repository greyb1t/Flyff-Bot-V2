#pragma once

#include "state_machine.h"
#include "entity.h"
#include "timer_on_interval.h"

namespace bot {

enum class TargetFocusStates {
  AdjustCamera,
  CheckEntityVisibility
};

class Bot;

class FocusTargetStateMachine : public StateMachine {
 public:
  FocusTargetStateMachine( Bot* bot )
      : StateMachine( static_cast<int>( TargetFocusStates::AdjustCamera ) ),
        bot_( bot ),
        prev_entity_screen_pos_{} {}

  StateStatusReturnValue Focus( const Entity& entity );

 private:
  Bot* bot_;
  TimerOnInterval wait_timer_;
  POINT prev_entity_screen_pos_;
};

}  // namespace bot
#pragma once

#include "state_machine.h"
#include "entity.h"
#include "timer_on_interval.h"

namespace bot {

enum class TargetFocusStates {
  AdjustCamera,
  kWaitForCamera,
  CheckEntityVisibility
};

class Bot;

class FocusTargetStateMachine : public
  StateMachine {
  public:
    FocusTargetStateMachine( Bot *bot )
      : StateMachine( static_cast<int>
                      ( TargetFocusStates::AdjustCamera ) ),
        bot_( bot ) { }

    StateStatusReturnValue Focus( const
                                  UniquePtrEntity &entity );

  private:
    Bot *bot_;
    TimerOnInterval wait_timer_;
};

}
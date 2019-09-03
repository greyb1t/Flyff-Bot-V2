#pragma once

#include "state_machine.h"
#include "entity.h"
#include "timer_on_interval.h"

namespace bot {

enum class IdleStateStates { kIdleStateStatesIdle, kIdleStateStatesReady };

class Bot;

class IdleStateMachine : public StateMachine {
 public:
  IdleStateMachine( Bot* bot )
      : StateMachine(
            static_cast<int>( IdleStateStates::kIdleStateStatesIdle ) ),
        bot_( bot ),
        next_state_( 0 ) {}

  StateStatusReturnValue Idle();

  void SetWaitMs( const uint32_t ms );
  void NextState( uint32_t state );
  uint32_t GetNextState();

  void ResetTimer();

 private:
  Bot* bot_;
  TimerOnInterval wait_timer_;
  uint32_t wait_ms_;
  uint32_t next_state_;
};

}  // namespace bot
#pragma once

#include "state_machine.h"
#include "entity.h"
#include "timer_on_interval.h"

namespace bot {

enum class AttackTargetStates { LureTarget2 };

enum class PressStates { Down, kWaitABit, Up };

enum class MouseClickButtons { kRightMouseButton, kLeftMouseButton };

class Bot;

class SimulationStateMachine : public StateMachine {
 public:
  SimulationStateMachine( Bot* bot )
      : StateMachine( static_cast<int>( PressStates::Down ) ), bot_( bot ) {
    Reset();
  }

  StateStatusReturnValue KeyPress( const uint16_t key_code,
                                   const uint32_t ms_delay = 50 );
  StateStatusReturnValue KeyPress(
      const uint16_t key_code,
      const std::function<bool()> key_up_condition );

  StateStatusReturnValue SimulateMouseclick( const MouseClickButtons btn );

  void Reset();

 private:
  Bot* bot_;
  TimerOnInterval wait_timer_;
};

}  // namespace bot
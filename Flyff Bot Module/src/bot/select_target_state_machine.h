#pragma once

#include "state_machine.h"
#include "entity.h"

#include "simluation_state_machine.h"

namespace bot {

enum class TargetSelectionStates { HoverCursor, Click, ResetCursor };

class Bot;

class SelectTargetStateMachine : public StateMachine {
 public:
  SelectTargetStateMachine( Bot* bot )
      : StateMachine( static_cast<int>( TargetSelectionStates::HoverCursor ) ),
        simulation_machine_( bot ),
        bot_( bot ) {}

  StateStatusReturnValue Select( const UniquePtrEntity& entity );

 private:
  Bot* bot_;
  POINT old_cursor_pos_;
  SimulationStateMachine simulation_machine_;
};

}  // namespace bot
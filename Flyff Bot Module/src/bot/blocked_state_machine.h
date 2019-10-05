#pragma once

#include "state_machine.h"
#include "entity.h"
#include "logging.h"

namespace bot {

enum class BlockedStates { kWalkBackwards, kWalkLeft };

class SimulationStateMachine;
class WalkToPositionStateMachine;
class Bot;
class LocalPlayer;

class BlockedStateMachine : public StateMachine {
 public:
  BlockedStateMachine( Bot* bot )
      : StateMachine( static_cast<int>( BlockedStates::kWalkBackwards ) ),
        bot_( bot ) {}

  StateStatusReturnValue Update(
      const LocalPlayer& local_player,
      SimulationStateMachine& simulation,
      WalkToPositionStateMachine& walk_to_pos_machine );

 private:
  Bot* bot_;
};

}  // namespace bot
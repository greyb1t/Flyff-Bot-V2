#pragma once

#include "bot.h"
#include "simluation_state_machine.h"

namespace bot {

enum class PassiveBotStates { kCheckHealing };

class BotPassive : public Bot {
 public:
  BotPassive( BotCore* botcore )
      : Bot( botcore,
             static_cast<int>( PassiveBotStates::kCheckHealing ),
             TEXT( "Passive bot" ) ),
        simulation_machine1_( this ),
        simulation_machine2_( this ) {}

  void UpdateInternal() override;

  virtual void OnStop() override;

 private:
  SimulationStateMachine simulation_machine1_;
  SimulationStateMachine simulation_machine2_;
};

}  // namespace bot
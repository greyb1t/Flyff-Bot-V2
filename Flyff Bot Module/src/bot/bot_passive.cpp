#include "pch.h"
#include "bot_passive.h"
#include "botcore.h"
#include "options/auto_healing_pills_option.h"

namespace bot {

void BotPassive::UpdateInternal() {
  const auto current_state = GetCurrentState<PassiveBotStates>();

  const auto botcore = GetBotCore();
  auto& bot_options = botcore->GetBotOptions();
  const auto& local_player = GetLocalPlayer();

  if ( local_player->IsDeletedOrInvalidMemory() )
    return;

  switch ( current_state ) {
    case PassiveBotStates::kCheckHealing: {
      const auto& auto_healing = bot_options.GetOption<AutoHealingOption>();

      if ( auto_healing.IsEnabled() ) {
        const auto current_health = local_player->GetHealth();

        if ( current_health < auto_healing.GetHealthLimit() )
          simulation_machine1_.KeyPress( auto_healing.GetHealCode() );
      }

      const auto& auto_healing_pills =
          bot_options.GetOption<AutoHealingPillsOption>();

      if ( auto_healing_pills.IsEnabled() ) {
        const auto current_health = local_player->GetHealth();

        if ( current_health < auto_healing_pills.GetHealthLimit() )
          simulation_machine2_.KeyPress( auto_healing_pills.GetHealCode() );
      }
    } break;

    default:
      break;
  }
}

void BotPassive::OnStop() {
  // SetIsStopped( true );
}

}  // namespace bot
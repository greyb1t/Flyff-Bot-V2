#pragma once

#include "whitelisted_names_option.h"

#include "../../res/resource.h"

#include "../entity_list.h"

namespace bot {

class AvoidEngagedMonsterOption : public WhitelistedNamesOption {
 public:
  AvoidEngagedMonsterOption( const std::string& name,
                             const uint32_t control_identifier )
      : WhitelistedNamesOption( name, control_identifier ) {}

  void RefreshControls( bot::BotOptions* bot_options ) override;

  void EnableOrDisableControls( bool enable,
                                bot::BotOptions* bot_options ) override;

  bool TryApplyOption() override;

  virtual bool IsEntityAllowed( const bot::Entity& entity ) const;

  /*
  bool TryApplyOption() override {
    const auto checkbox_avoid_engaged_entities =
        GWH( CHECK_DO_NOT_KILL_OTHER_PLAYERS_MONSTERS );

    if ( gwingui::checkbox::IsChecked( checkbox_avoid_engaged_entities ) )
      SetStatus( true );
    else
      SetStatus( false );

    return true;
  }

  virtual bool IsEntityAllowed( const bot::Entity& entity ) const {
    if ( IsEnabled() ) {
      if ( entity.IsEngaged() ) {
        return false;
      }

      bot::EntityList entity_list( entity.GetFlyffClient() );
      auto& entities = entity_list.GetMoverEntities();

      std::vector<bot::Entity> players;

      for ( const auto& entity : entities ) {
        if ( entity->IsPlayer() ) {
          players.push_back( *entity );
        }
      }

      for ( const auto player : players ) {
        bool is_player_whitelisted = ValueExists( player.GetName() );

        if ( !is_player_whitelisted ) {
          // If the player is within 15 m of the entity
          if ( entity.DistanceTo( player ) < 15.f ) {
            return false;
          }
        }
      }
    }

    return true;
  }
  */
};

}  // namespace bot
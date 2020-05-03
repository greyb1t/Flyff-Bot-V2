#include "pch.h"
#include "avoid_engaged_monsters.h"
#include "../../res/resource.h"

#include "gwinguiv2/controls/listbox.h"

#include "../utils/string_utls.h"

#include "whitelisted_player_names_option.h"
#include "../bot_options.h"
#include "../servers/flyff_client.h"

void bot::AvoidEngagedMonsterOption::RefreshControls(
    bot::BotOptions* bot_options ) {
  Option::RefreshControls( bot_options );

  for ( auto& value : values_ ) {
    const auto listbox_handle = GWH( LISTBOX_WHITELIST_PLAYER_NAMES );
    gwingui::listbox::AddString( listbox_handle,
                                 stringutils::AnsiToWide( value ) );
  }
}

void bot::AvoidEngagedMonsterOption::EnableOrDisableControls(
    bool enable,
    bot::BotOptions* bot_options ) {
  const auto whitelisted_player_names_option =
      bot_options->GetOption<WhitelistedPlayerNamesOption>();

  // Since we are using the same controls for both WhitelistedPlayersOption and this option
  // Make sure we do not disable the controls if the only option is enabled
  if ( gwingui::checkbox::IsChecked(
           GWH( whitelisted_player_names_option.GetControlIdentifier() ) ) ) {
    enable = true;
  }

  gwingui::control::EnableOrDisable( GWH( LISTBOX_WHITELIST_PLAYER_NAMES ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_WHITELISTED_PLAYER_NAME ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_ADD_WHITELISTED_PLAYER_NAME ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_WHITELIST_PLAYER_NAME_REMOVE ),
                                     enable );
  gwingui::control::EnableOrDisable(
      GWH( BUTTON_WHITELIST_FILL_SELECTED_PLAYER_NAME ), enable );
}

bool bot::AvoidEngagedMonsterOption::TryApplyOption() {
  Clear();

  const auto checkbox_avoid_engaged_entities =
      GWH( CHECK_DO_NOT_KILL_OTHER_PLAYERS_MONSTERS );

  if ( gwingui::checkbox::IsChecked( checkbox_avoid_engaged_entities ) ) {
    SetStatus( true );

    const auto listbox_whitelist_playernames =
        GWH( LISTBOX_WHITELIST_PLAYER_NAMES );

    for ( int i = 0, count = gwingui::listbox::GetCount(
                         listbox_whitelist_playernames );
          i < count; ++i ) {
      const std::wstring name =
          gwingui::listbox::GetText( listbox_whitelist_playernames, i );

      AddValue( stringutils::WideToAnsi( name ) );
    }
  } else
    SetStatus( false );

  return true;
}

bool bot::AvoidEngagedMonsterOption::IsEntityAllowed(
    const bot::Entity& entity ) const {
  if ( IsEnabled() ) {
    if ( entity.IsEngaged() ) {
      return false;
    }

    auto flyff_client = entity.GetFlyffClient();

    bot::EntityList entity_list( flyff_client );
    auto& entities = entity_list.GetMoverEntities();

    std::vector<bot::Entity> players;

    for ( const auto& entity : entities ) {
      if ( entity->IsPlayer() ) {
        players.push_back( *entity );
      }
    }

    const auto& local_player = flyff_client->CreateLocalPlayer();

    for ( const auto player : players ) {
      if ( player.GetPointerAddress() == local_player->GetPointerAddress() ) {
        continue;
      }

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
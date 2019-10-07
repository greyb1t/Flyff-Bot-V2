#include "pch.h"
#include "killing_bot.h"
#include "logging.h"
#include "../utils/string_utls.h"
#include "entity_filter.h"
#include "botcore.h"

bot::KillingBot::KillingBot( BotCore* botcore,
                             int origin_state,
                             const std::wstring& bot_name )
    : Bot( botcore, origin_state, bot_name ),
      monster_kill_count_( 0 ),
      entities_not_found_counter_( 0 ) {}

std::unique_ptr<bot::Entity> bot::KillingBot::FindNearestMonster(
    std::vector<std::unique_ptr<Entity>>& entities,
    const LocalPlayer& local_player,
    std::vector<const EntityFilter*>& entity_filters ) {
  SortEntitiesByDistanceToEntity( local_player, entities );

  // Go through the entities in close- to far order and filter away non-monsters
  for ( auto& entity : entities ) {
    if ( entity->IsDeletedOrInvalidMemory() ) {
      continue;
    }

    if ( entity->DistanceTo( local_player ) != 0 ) {
      if ( entity->IsMonster() && entity->IsAlive() ) {
        if ( !IsEntityBlacklisted( *entity ) ) {
          bool let_through = true;

          for ( const auto filter : entity_filters ) {
            if ( !filter->IsEntityAllowed( *entity ) ) {
              let_through = false;
              break;
            }
          }

          if ( !let_through )
            continue;

          // TODO: Add feature to prioritize the aggro monster above ALL other
          // monsters, meaning write the code to bypass the filters

          return botcore_->GetFlyffClient()->CreateEntity(
              entity->GetPointerAddress() );
        }
      }
    }
  }

  return nullptr;
}

bool bot::KillingBot::IsEntityBlacklisted( const Entity& entity ) {
  for ( auto& blacklisted_entity : blacklisted_entities_temporary_ ) {
    // Check if their pointer addresses are the same (the address created when
    // they called new Entity)
    if ( blacklisted_entity.GetPointerAddress() == entity.GetPointerAddress() )
      return true;
  }

  return false;
}

void bot::KillingBot::RestoreBlockedBoundBoxes() {
  logging::Log( TEXT( "BB Count: " ) +
                std::to_wstring( saved_selection_blocked_entities_.size() ) +
                TEXT( "\n" ) );

  // Restore the bound boxes of the entities that previously blocked the bot
  for ( auto& e : saved_selection_blocked_entities_ ) {
    if ( !e->IsDeletedOrInvalidMemory() ) {
      logging::Log( TEXT( "Restored BB: " ) +
                    std::to_wstring( e->GetPointerAddress() ) +
                    TEXT( ", Name: " ) +
                    stringutils::AnsiToWide( e->GetName() ) + TEXT( "\n" ) );
      e->RestoreBoundBox();
    } else
      logging::Log( TEXT( "COULD NOT RESTORE BOUND BOX, DELETED\n" ) );
  }

  saved_selection_blocked_entities_.clear();
}

void bot::KillingBot::RestoreSavedBoundBoxes() {
  logging::Log( TEXT( "Saved BB Count: " ) +
                std::to_wstring( saved_bound_box_changed_entities_.size() ) +
                TEXT( "\n" ) );

  // Restore the bound boxes of the entities that previously blocked the bot
  for ( auto& e : saved_bound_box_changed_entities_ ) {
    if ( !e->IsDeletedOrInvalidMemory() ) {
      logging::Log( TEXT( "Restored Saved BB: " ) +
                    std::to_wstring( e->GetPointerAddress() ) +
                    TEXT( ", Name: " ) +
                    stringutils::AnsiToWide( e->GetName() ) + TEXT( "\n" ) );
      e->RestoreBoundBox();
    } else
      logging::Log( TEXT( "COULD NOT RESTORE SAVED BOUND BOX, DELETED\n" ) );
  }

  saved_bound_box_changed_entities_.clear();
}
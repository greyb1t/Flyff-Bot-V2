#include "pch.h"
#include "bot.h"
#include "botcore.h"
#include "math.h"
#include "bot_ai_one_vs_one.h"
#include "do_once.h"
#include "../utils/simulation.h"

#include "gwinguiv2/drawing.h"

#include "options/whitelisted_player_names_option.h"

#include "../utils/string_utls.h"

namespace bot {

Bot::Bot( BotCore* botcore, int origin_state )
    : StateMachine( origin_state ),
      botcore_( botcore ),
      is_state_stopped_( false ),
      current_target_selection_state_(
          static_cast<int>( TargetSelectionStates::HoverCursor ) ),
      // average_y_pos_( 0.f ),
      entities_not_found_counter_( 0 ) {
  local_player_ = botcore->GetFlyffClient()->CreateLocalPlayer();
}

void Bot::SortEntitiesByDistanceToEntity(
    const Entity* entity,
    std::vector<UniquePtrEntity>& entities ) {
  std::sort(
      entities.begin(), entities.end(),
      [=]( const UniquePtrEntity& e1, const UniquePtrEntity& e2 ) -> bool {
        if ( e1->IsDeletedOrInvalidMemory() || e2->IsDeletedOrInvalidMemory() )
          return false;

        return e1->DistanceTo( entity ) < e2->DistanceTo( entity );
      } );
}

bool Bot::FindNearestMonster(
    std::vector<UniquePtrEntity>& entities,
    const LocalPlayer* local_player,
    UniquePtrEntity& entity_out,
    std::vector<const EntityFilter*>& entity_filters ) {
  SortEntitiesByDistanceToEntity( local_player, entities );

  // Go through the entities in close- to far order and filter away non-monsters
  for ( auto& entity : entities ) {
    if ( entity->IsDeletedOrInvalidMemory() ) {
      continue;
    }

    if ( entity->DistanceTo( local_player ) != 0 ) {
      if ( entity->IsMonster() && entity->IsAlive() ) {
        if ( !IsEntityBlacklisted( entity ) ) {
          bool let_through = true;

          for ( const auto filter : entity_filters ) {
            if ( !filter->IsEntityAllowed( entity ) ) {
              let_through = false;
              break;
            }
          }

          if ( !let_through )
            continue;

          // TODO: Add feature to prioritize the aggro monster above ALL other
          // monsters, meaning write the code to bypass the filters

          entity_out = botcore_->GetFlyffClient()->CreateEntity(
              entity->GetPointerAddress() );

          return true;
        }
      }
    }
  }

  return false;
}

void Bot::AdjustCameraTowardsEntity( const LocalPlayer* local_player,
                                     const UniquePtrEntity& entity ) {
  const D3DXVECTOR3 entity_pos = entity->GetPosition();
  const D3DXVECTOR3 local_player_pos = local_player->GetPosition();
  const D3DXVECTOR3 player_entity_delta = local_player_pos - entity_pos;

  const float x_diff = player_entity_delta.x;
  const float z_diff = player_entity_delta.z;
  const float y_diff = player_entity_delta.y;

  const float angle_radians_x = atan2( x_diff, z_diff );
  const float angle_degrees_x = D3DXToDegree( angle_radians_x );

  local_player->SetCameraRotX( angle_degrees_x );

  // Some magic calculation based on the scroll distance that is done by flyff
  const float scroll_distance = local_player->GetScrollDistance();
  const float scroll_distance_degree_offset = scroll_distance * 4;

  float angle_radians_y =
      atan2( y_diff, sqrt( x_diff * x_diff + z_diff * z_diff ) );

  float angle_degrees_y = D3DXToDegree( angle_radians_y );

  local_player->SetCameraRotY( angle_degrees_y -
                               scroll_distance_degree_offset );
}

bool Bot::IsEntityBlacklisted( const UniquePtrEntity& entity ) {
  for ( auto& blacklisted_entity : blacklisted_entities_temporary_ ) {
    // Check if their pointer addresses are the same (the address created when
    // they called new Entity)
    if ( blacklisted_entity.GetPointerAddress() == entity->GetPointerAddress() )
      return true;
  }

  /*
    for (auto &blacklisted_entity : blacklisted_entities_permanent_) {
    if (blacklisted_entity.GetPointerAddress() == entity->GetPointerAddress()) {
      return true;
    }
    }
  */

  return false;
}

void Bot::DeSelectEntity() {
  simulation::SendVirtualKeypress( botcore_->GetTargetWindow(), VK_ESCAPE, 50 );
}

bool Bot::IsEntityValid( const UniquePtrEntity& entity ) {
  return !entity->IsDeletedOrInvalidMemory() && entity->IsMonster() &&
         entity->IsAlive();
}

void Bot::LogOnce( DoOnce& do_once, const std::wstring& text ) {
  do_once.Do( [&]() { logging::Log( text ); } );
}

// bool Bot::IsEntityWhitelisted( const UniquePtrEntity& entity ) {
//  auto& bot_options = botcore_->GetBotOptions();
//  const auto whitelisted_names = bot_options.GetWhitelistedNamesOption();
//
//  if ( whitelisted_names->ValueExists( entity->GetName() ) )
//    return true;
//
//  return false;
//}

bool Bot::IsNonWhitelistedPlayerFound(
    const std::vector<UniquePtrEntity>& entities,
    const UniquePtrLocalPlayer& local_player,
    UniquePtrEntity* entity_found ) {
  auto& bot_options = botcore_->GetBotOptions();
  const auto whitelisted_player_names =
      bot_options.GetWhitelistedPlayerNamesOption();

  for ( const auto& entity : entities ) {
    if ( entity->IsDeletedOrInvalidMemory() )
      continue;

    // If a player is found and it is not me
    if ( entity->IsPlayer() &&
         entity->GetPointerAddress() != local_player->GetPointerAddress() ) {
      if ( !whitelisted_player_names->ValueExists( entity->GetName() ) ) {
        *entity_found = entity->GetFlyffClient()->CreateEntity(
            entity->GetPointerAddress() );
        return true;
      }
    }
  }

  return false;
}

// bool Bot::IsEntityAboveAverageYPosition( const UniquePtrEntity& entity ) {
//   return entity->GetPosition().y >= average_y_pos_;
// }

void Bot::RestoreBlockedBoundBoxes() {
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

void Bot::RestoreSavedBoundBoxes() {
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

bool Bot::ClickEntity( const UniquePtrEntity& entity ) {
  POINT entity_screen_pos;

  if ( GetEntityScreenPosition( entity, entity_screen_pos ) ) {
    SetCursorPos( entity_screen_pos.x, entity_screen_pos.y );

    mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
    mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
  }

  return true;
}

bool Bot::GetEntityScreenPosition( const UniquePtrEntity& entity,
                                   POINT& entity_screen_pos ) {
  BOUND_BOX bound_box = entity->GetBoundBox();

  D3DXVECTOR3 bound_box_center_pos = math::CalculateBoxCenter( &bound_box );

  D3DXVECTOR3 pos_screen;

  if ( !math::FlyffWorldToScreen( botcore_, entity, bound_box_center_pos,
                                  &pos_screen ) )
    return false;

  int titlebar_height = GetSystemMetrics( SM_CYFRAME ) +
                        GetSystemMetrics( SM_CYCAPTION ) +
                        GetSystemMetrics( SM_CXPADDEDBORDER );

  int window_border_width =
      GetSystemMetrics( SM_CXBORDER ) + GetSystemMetrics( SM_CXSIZEFRAME );

  RECT neuz_window_rect;
  GetWindowRect( botcore_->GetTargetWindow(), &neuz_window_rect );

  POINT adjusted_point;
  adjusted_point.x = static_cast<LONG>( pos_screen.x + neuz_window_rect.left +
                                        window_border_width );
  adjusted_point.y = static_cast<LONG>( pos_screen.y + neuz_window_rect.top +
                                        titlebar_height );

  entity_screen_pos.x = adjusted_point.x;
  entity_screen_pos.y = adjusted_point.y;

  // Shrink the rect to avoid clicking the edges of something that is a window
  // in the window
  // RECT_SHRINK( neuz_window_rect, 100 );
  neuz_window_rect = gwingui::drawing::GrowRect( neuz_window_rect, -100, -100 );

  if ( PtInRect( &neuz_window_rect, entity_screen_pos ) )
    return true;

  return false;
}

void Bot::OnStateChanging() {
  execute_once_map_.clear();
}

BotCore* Bot::GetBotCore() {
  return botcore_;
}

UniquePtrLocalPlayer& Bot::GetLocalPlayer() {
  return local_player_;
}

bool Bot::IsStateStopped() {
  return is_state_stopped_;
}

// float Bot::GetAverageY() {
//   return average_y_pos_;
// }

void Bot::SetIsStopped( bool value ) {
  is_state_stopped_ = value;
}

}  // namespace bot
#include "pch.h"
#include "bot.h"
#include "botcore.h"
#include "math.h"
#include "bot_ai_one_vs_one.h"
#include "../utils/simulation.h"

#include "gwinguiv2/drawing.h"

#include "options/whitelisted_player_names_option.h"

#include "../utils/string_utls.h"

namespace bot {

Bot::Bot( BotCore* botcore, int origin_state, const std::wstring& bot_name )
    : StateMachine( origin_state ),
      botcore_( botcore ),
      bot_name_( bot_name ),
      internal_bot_state_machine_(
          static_cast<int>( BaseBotStates::Running ) ) {
  local_player_ = botcore->GetFlyffClient()->CreateLocalPlayer();

  bot_duration_stopwatch_.Start();
}

Bot::~Bot() {
  bot_duration_stopwatch_.Stop();

  LogQueue().Notify();
}

void Bot::Update() {
  if ( !botcore_->GetStarted() )
    internal_bot_state_machine_.SetNextState( BaseBotStates::Stopped );

  const auto current_state =
      internal_bot_state_machine_.GetCurrentState<BaseBotStates>();

  switch ( current_state ) {
    case BaseBotStates::Running: {
      UpdateInternal();
    } break;
    case BaseBotStates::Stopped: {
      OnStop();
    } break;
    default:
      break;
  }
}

void Bot::SortEntitiesByDistanceToEntity(
    const Entity& entity,
    std::vector<std::unique_ptr<Entity>>& entities ) {
  std::sort(
      entities.begin(), entities.end(),
      [ = ]( const std::unique_ptr<Entity>& e1,
             const std::unique_ptr<Entity>& e2 ) -> bool {
        if ( e1->IsDeletedOrInvalidMemory() || e2->IsDeletedOrInvalidMemory() )
          return false;

        return e1->DistanceTo( entity ) < e2->DistanceTo( entity );
      } );
}

void Bot::AdjustCameraTowardsEntity( const LocalPlayer* local_player,
                                     const Entity& entity ) {
  const D3DXVECTOR3 entity_pos = entity.GetPosition();
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

bool Bot::IsEntityValid( const Entity& entity ) {
  return !entity.IsDeletedOrInvalidMemory() && entity.IsMonster() &&
         entity.IsAlive();
}

std::unique_ptr<Entity> Bot::IsNonWhitelistedPlayerFound(
    const WhitelistedPlayerNamesOption& whitelisted_names_option,
    const std::vector<std::unique_ptr<Entity>>& entities,
    const LocalPlayer& local_player ) {
  // auto& bot_options = botcore_->GetBotOptions();
  // const auto whitelisted_player_names =
  //     bot_options.GetWhitelistedPlayerNamesOption();

  for ( const auto& entity : entities ) {
    if ( entity->IsDeletedOrInvalidMemory() )
      continue;

    // If a player is found and it is not me
    if ( entity->IsPlayer() &&
         entity->GetPointerAddress() != local_player.GetPointerAddress() ) {
      if ( !whitelisted_names_option.ValueExists( entity->GetName() ) ) {
        return entity->GetFlyffClient()->CreateEntity(
            entity->GetPointerAddress() );
      }
    }
  }

  return nullptr;
}

bool Bot::GetEntityScreenPosition( const Entity& entity,
                                   POINT& entity_screen_pos ) {
  const BOUND_BOX bound_box = entity.GetBoundBox();
  const D3DXVECTOR3 bound_box_center_pos =
      math::CalculateBoxCenter( &bound_box );
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
  neuz_window_rect = gwingui::drawing::GrowRect( neuz_window_rect, -100, -100 );
  // neuz_window_rect = gwingui::drawing::GrowRect( neuz_window_rect, -200, -200 );

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

std::unique_ptr<LocalPlayer>& Bot::GetLocalPlayer() {
  return local_player_;
}

void Bot::SetInternalState( BaseBotStates state ) {
  internal_bot_state_machine_.SetNextState( state );
}

BaseBotStates Bot::GetInternalState() const {
  return internal_bot_state_machine_.GetCurrentState<BaseBotStates>();
}

}  // namespace bot
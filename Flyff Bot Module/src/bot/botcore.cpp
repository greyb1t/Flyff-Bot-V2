#include "pch.h"
#include "botcore.h"
#include "entity_list.h"
#include "math.h"
#include "bot_initializer.h"
#include "bot_ai_one_vs_one.h"
#include "bot_passive.h"
#include "..\window\waiting_window.h"
#include "entity_replicate_box.h"
#include "../utils/string_utls.h"
#include "options/average_y_pos_option.h"
#include "options/level_area_option.h"
#include "options/bot_mode_option.h"
#include "options/whitelisted_names_option.h"
#include "options/remove_all_obstacles_option.h"
#include "..\servers\flyff_client_mazey.h"
#include "..\servers\flyff_client_ignite.h"
#include "..\servers\flyff_client_dragon_crusade.h"

#include "../utils/trace.h"

#include "gwinmem/process_memory_internal.h"

#include "gwinguiv2\controls\richedit.h"
#include "gwinguiv2\controls\listbox.h"
#include "gwinguiv2/message_box.h"

std::mutex g_hooks_mutex;

namespace bot {

BotCore::BotCore()
    : client_( nullptr ),
      started_( false ),
      frame_counter_( 0 ),
      last_frame_counter_( 0 ),
      simulated_cursor_pos_{ -1, -1 },
      last_cheats_update_ms_( 0 ),
      ms_elapsed_( 0 ),
      fps_( 0 ),
      fps_frame_counter_( 0 ),
      last_time_( timeGetTime() ),
      is_stopwatch_started_( false ),
      has_initialized_d3d_( false ),
      has_initialized_matrix_addresses_( false ),
      bad_boy_present_( false ) {}

HRESULT BotCore::EndSceneHooked( LPDIRECT3DDEVICE9 device ) {
  const auto endscene_original_hook =
      Initializer().GetBotCore()->GetFlyffClient()->GetHookManager()->GetHook(
          HookType::EndScene );

  const auto endscene_original_function =
      reinterpret_cast<tEndScene>( endscene_original_hook.original_function );

  if ( !g_hooks_mutex.try_lock() ) {
    return endscene_original_function( device );
  }

  std::lock_guard<std::mutex>( g_hooks_mutex, std::adopt_lock );

  auto botcore = Initializer().GetBotCore();

  if ( !device ) {
    return endscene_original_function( device );
  }

  if ( !botcore->has_initialized_d3d_ ) {
    botcore->drawing_.SetDevice( device );
    botcore->drawing_.CreateFontToMap( TEXT( "Arial" ), NORMAL_TEXT, 14 );
    botcore->drawing_.CreateFontToMap( TEXT( "Arial" ), NORMAL_TEXT_BIG, 18 );
    botcore->drawing_.CreateFontToMap( TEXT( "Arial" ), BIG_TEXT, 28 );
    D3DDEVICE_CREATION_PARAMETERS dcp;
    device->GetCreationParameters( &dcp );
    botcore->has_initialized_d3d_ = true;
  }

  // Calculate the fps
  botcore->ms_elapsed_ += ( timeGetTime() - botcore->last_time_ );
  ++botcore->fps_frame_counter_;

  if ( botcore->ms_elapsed_ >= 1000 ) {
    botcore->fps_ = botcore->fps_frame_counter_;

    botcore->fps_frame_counter_ = 0;
    botcore->ms_elapsed_ = 0;
  }

  // Sleep(max(1000 / 50 - (int)(timeGetTime() - botcore->last_time_), 0));

  //// Limit the fps to 30
  // if ( botcore->fps_frame_counter_ >= 30 && botcore->ms_elapsed_ < 1000 ) {
  //  // Sleep((1000 - botcore->ms_elapsed_) / 10);
  //  gWin::Stopwatch sw;
  //  sw.Start();

  //  while (botcore->ms_elapsed_ < 1000) {
  //    Sleep(1);
  //    sw.Stop();
  //    botcore->ms_elapsed_ += sw.GetElapsedMilliseconds();
  //    sw.Restart();
  //  }
  //}

  // if the fps_frame_counter_ is >= 30
  // and ms_elapsed < 1000
  // then stop rendering anything until ms_elapsed >= 1000

  botcore->last_time_ = timeGetTime();

  const auto projection_matrix_addr = botcore->GetFlyffClient()->GetClientVar(
      MemoryContants::kProjectionMatrixAddress );
  const auto view_matrix_addr = botcore->GetFlyffClient()->GetClientVar(
      MemoryContants::kViewMatrixAddress );

  if ( projection_matrix_addr && view_matrix_addr )
    botcore->Render( device );

  if ( ( botcore->frame_counter_ - botcore->last_frame_counter_ ) >
       botcore->skipped_frames_ ) {
    botcore->skipped_frames_ = 0;

    // As an anti-debugging measure, do not call update for the bot is the
    // debugger is attached
    if ( !botcore->bad_boy_present_ ) {
      botcore->Update();
    }
  }

  ++botcore->frame_counter_;

  return endscene_original_function( device );
}

D3DXVECTOR3* BotCore::D3DXVec3ProjectHooked( D3DXVECTOR3* pOut,
                                             const D3DXVECTOR3* pV,
                                             const D3DVIEWPORT9* pViewport,
                                             const D3DXMATRIX* pProjection,
                                             const D3DXMATRIX* pView,
                                             const D3DXMATRIX* pWorld ) {
  auto botcore = Initializer().GetBotCore();

  const auto d3dvec3project_hook =
      Initializer().GetBotCore()->GetFlyffClient()->GetHookManager()->GetHook(
          HookType::D3DVec3Project );

  const auto d3dvec3project_hook_original_function =
      reinterpret_cast<tD3DXVec3Project>(
          d3dvec3project_hook.original_function );

  if ( !botcore->has_initialized_matrix_addresses_ ) {
    const auto& client = botcore->GetFlyffClient();

    client->SetServerVar( MemoryContants::kCameraAddress,
                          reinterpret_cast<uint64_t>( pView ) - 0x10 );

    botcore->SetViewport( *pViewport );

    client->SetServerVar( MemoryContants::kProjectionMatrixAddress,
                          reinterpret_cast<uint64_t>( pProjection ) );

    client->SetServerVar( MemoryContants::kViewMatrixAddress,
                          reinterpret_cast<uint32_t>( pView ) );

    GWIN_TRACE( "kCameraAddress: 0x%08x\n",
                client->GetClientVar( MemoryContants::kCameraAddress ) );

    GWIN_TRACE(
        "kProjectionMatrixAddress: 0x%08x\n",
        client->GetClientVar( MemoryContants::kProjectionMatrixAddress ) );

    GWIN_TRACE( "kViewMatrixAddress: 0x%08x\n",
                client->GetClientVar( MemoryContants::kViewMatrixAddress ) );

    botcore->has_initialized_matrix_addresses_ = true;
  }

  return d3dvec3project_hook_original_function( pOut, pV, pViewport,
                                                pProjection, pView, pWorld );
}

BOOL BotCore::GetCursorPosHooked( LPPOINT lpPoint ) {
  const auto botcore = Initializer().GetBotCore();

  const auto getcursorpos_hook =
      botcore->GetFlyffClient()->GetHookManager()->GetHook(
          HookType::GetCursorPos );

  const auto getcursorpos_hook_original_function =
      reinterpret_cast<tGetCursorPos>( getcursorpos_hook.original_function );

  if ( !g_hooks_mutex.try_lock() ) {
    return getcursorpos_hook_original_function( lpPoint );
  }

  std::lock_guard<std::mutex>( g_hooks_mutex, std::adopt_lock );

  const BOOL ret = getcursorpos_hook_original_function( lpPoint );

  if ( botcore->GetStarted() && botcore->simulated_cursor_pos_.x != -1 &&
       botcore->simulated_cursor_pos_.y != -1 ) {
    lpPoint->x = botcore->simulated_cursor_pos_.x;
    lpPoint->y = botcore->simulated_cursor_pos_.y;
  }

  return ret;
}

SHORT BotCore::GetKeyStateHooked( int nVirtKey ) {
  const auto botcore = Initializer().GetBotCore();

  const auto getkeystate_hook =
      botcore->GetFlyffClient()->GetHookManager()->GetHook(
          HookType::GetKeyState );

  const auto getkeystate_hook_original_function =
      reinterpret_cast<tGetKeyState>( getkeystate_hook.original_function );

  if ( !g_hooks_mutex.try_lock() ) {
    return getkeystate_hook_original_function( nVirtKey );
  }

  std::lock_guard<std::mutex>( g_hooks_mutex, std::adopt_lock );

  const auto module_name =
      Initializer().GetBotCore()->GetFlyffClient()->GetModuleName();

  const auto module = gwinmem::CurrentProcess().GetModule( module_name );

  const auto ret_addr = reinterpret_cast<uint32_t>( _ReturnAddress() );

  // Is the GetKeyState called from inside the Ignite.exe module
  if ( ret_addr >= module.base && ret_addr <= ( module.base + module.size ) ) {
    // Always return the status that the key is down,
    // this bypasses Ignite Flyff's protection against mouse click simulator
    // with window messages

    // Spoof the value to bypass the check
    return MAKEWORD( 0, 1 );
  } else {
    // Call the original for functionality to stay the same for other stuff e.g.
    // ScrollBar in my GUI
    return getkeystate_hook_original_function( nVirtKey );
  }

  return MAKEWORD( 0, 1 );
}

#if 0
int __fastcall BotCore::SendMeleeAttackHooked( void* this_,
                                               void* unknown,
                                               uint32_t attack_message,
                                               uint32_t obj_id,
                                               __int32 nparam2,
                                               __int32 nparam3,
                                               float f_val ) {
  auto botcore = Initializer().GetBotCore();
  auto send_melee_attack = botcore->send_melee_attack_original_;
  std::wcout << TEXT( "attack_message: " ) + std::to_wstring( attack_message ) +
                    TEXT( ", obj_id: " ) + std::to_wstring( obj_id ) +
                    TEXT( ", nparam2: " ) + std::to_wstring( nparam2 ) +
                    TEXT( ", nparam3: " ) + std::to_wstring( nparam3 ) +
                    TEXT( ", f_val: " ) + std::to_wstring( f_val ) +
                    TEXT( "\n" );

  if ( botcore->blade_damage_hack_ )
    attack_message = 33;

  return send_melee_attack( this_, attack_message, obj_id, nparam2, nparam3,
                            f_val );
}

signed int __fastcall BotCore::DoAttackMeleeHooked( void* this_,
                                                    void* unknown,
                                                    int target,
                                                    int attack_message,
                                                    int item_id ) {
  auto botcore = Initializer().GetBotCore();
  auto do_attack_melee = botcore->do_attack_melee_original_;

  if ( botcore->blade_damage_hack_ ) {
    // std::wcout << TEXT( "this_: " ) + std::to_wstring( ( uint32_t )this_ ) +
    //                  TEXT( ", unknown: " ) +
    //                  std::to_wstring( ( uint32_t )unknown ) +
    //                  TEXT( ", target: " ) + std::to_wstring( target ) +
    //                  TEXT( ", attack_message: " ) +
    //                  std::to_wstring( attack_message ) +
    //                  TEXT( ", item_id: " ) + std::to_wstring( item_id ) +
    //                  TEXT( "\n" );
  }

  /*
  if ( botcore->blade_damage_hack_ )
    attack_message = 33;
  */

  return do_attack_melee( this_, target, attack_message, item_id );
}

int __fastcall BotCore::GetCmdParamHooked( void* this_,
                                           void* unknown,
                                           int index ) {
  auto botcore = Initializer().GetBotCore();
  auto get_cmd_param_original = botcore->get_cmd_param_original_;

  auto value = get_cmd_param_original( this_, index );

  if ( index == 2 && botcore->no_cast_delay_ ) {
    printf( "returning 2 as cast queue\n" );
    return 2;
  }

  return value;
}
#endif

LRESULT CALLBACK BotCore::KeyboardCallback( int code,
                                            WPARAM wparam,
                                            LPARAM lparam ) {
  PKBDLLHOOKSTRUCT kb_hook = ( PKBDLLHOOKSTRUCT )lparam;
  auto botcore = Initializer().GetBotCore();

  if ( code == HC_ACTION ) {
    if ( wparam == WM_KEYUP ) {
      if ( GetForegroundWindow() == botcore->GetTargetWindow() ) {
        if ( kb_hook->vkCode == VK_END ) {
          Beep( 1000, 500 );
          auto botcore = Initializer().GetBotCore();
          botcore->SetStarted( false );
          botcore->ShowBotHasStoppedWindow();
        } else if ( kb_hook->vkCode == VK_HOME ) {
          Beep( 200, 500 );
          const auto botcore = Initializer().GetBotCore();
          botcore->ToggleBot();
        }
      }
    }
  }

  return CallNextHookEx( botcore->kb_hook_handle_, code, wparam, lparam );
}

void BotCore::ShowBotHasStoppedWindow() {
  if ( !GetStarted() ) {
    if ( GetActiveBots().size() ) {
      // disable start/bot bot button
      // enable start/bot bot button
      const auto toggle_bot_button_handle = GWH( BUTTON_TOGGLEBOT );
      gwingui::control::EnableOrDisable( toggle_bot_button_handle, false );

      waiting_window::DisplayWaitingWindowAsync(
          TEXT( "Waiting for the bot to finish its tasks..." ), [=]() {
            // Wait until the state is set to stopped
            for ( const auto& bot : GetActiveBots() ) {
              while ( bot->GetInternalState() != BaseBotStates::Stopped )
                Sleep( 5 );
            }

            SetActiveBots( {} );

            // enable start/bot bot button
            const auto toggle_bot_button_handle = GWH( BUTTON_TOGGLEBOT );
            gwingui::control::EnableOrDisable( toggle_bot_button_handle, true );
          } );
    }
  }
}

void BotCore::DrawEntity( const Entity& local_player,
                          const Entity& entity,
                          const D3DCOLOR box_color ) {
  BOUND_BOX local_player_bound_box = local_player.GetBoundBox();
  D3DXMATRIX local_player_mat_world = local_player.GetWorldMatrix();
  D3DXVECTOR3 local_player_bound_box_center =
      math::CalculateBoxCenter( &local_player_bound_box );
  D3DXVECTOR3 local_player_sceen_pos;

  math::FlyffWorldToScreen( this, local_player, local_player_bound_box_center,
                            &local_player_sceen_pos );
  BOUND_BOX bound_box = entity.GetBoundBox();
  bool inside_view = true;
  BOUND_BOX bound_box_screen;

  for ( int i = 0; i < BOUND_BOX::kBoxCorners; ++i ) {
    D3DXVECTOR3 screen_pos;

    if ( !math::FlyffWorldToScreen( this, entity, bound_box.pos[ i ],
                                    &screen_pos ) ) {
      inside_view = false;
      break;
    }

    bound_box_screen.pos[ i ] = screen_pos;
  }

  // don't draw an entity outside of the screen
  if ( !inside_view )
    return;

  for ( int i = 0; i < BOUND_BOX::kBoxCorners; ++i ) {
    const float kCornerSize = 5.0f;
    // drawing_.DrawRectangle(
    //  bound_box_screen.pos[i].x - (kCornerSize / 2),
    //  bound_box_screen.pos[i].y - (kCornerSize / 2),
    //  kCornerSize,
    //  kCornerSize,
    //  D3DCOLOR_RGBA(255, 255, 255, 255));
    /*
      drawing_.DrawTextC(
      TEXT("X: ") + std::to_wstring(bound_box.pos[i].x) + TEXT(", Y: ") +
      std::to_wstring(bound_box.pos[i].y), bound_box_screen.pos[i].x,
      bound_box_screen.pos[i].y,
      D3DCOLOR_RGBA(255, 255, 255, 255),
      NORMAL_TEXT);
    */
    // drawing_.DrawTextC(
    //  TEXT("ObjType: ") + std::to_wstring(entity->GetObjectType()),
    //  bound_box_screen.pos[i].x,
    //  bound_box_screen.pos[i].y,
    //  D3DCOLOR_RGBA(255, 255, 255, 255),
    //  NORMAL_TEXT);

    // drawing_.DrawTextC( std::to_wstring( i ), bound_box_screen.pos[ i ].x,
    //                    bound_box_screen.pos[ i ].y,
    //                    D3DCOLOR_RGBA( 255, 255, 255, 255 ), NORMAL_TEXT );
  }

  // drawing_.DrawTextC(
  //    TEXT( "Name: " ) + entity->GetName() + TEXT( ", Visibility: " ) +
  //        std::to_wstring( ( bool )( entity->GetType() & OBJ_FLAG_DELETE ) ),
  //    bound_box_screen.pos[ 0 ].x, bound_box_screen.pos[ 0 ].y,
  //    D3DCOLOR_RGBA( 255, 255, 255, 255 ), NORMAL_TEXT );

  drawing_.DrawTextC(
      TEXT( "Name: " ) + stringutils::AnsiToWide( entity.GetName() ),
      static_cast<int>( bound_box_screen.pos[ 0 ].x ),
      static_cast<int>( bound_box_screen.pos[ 0 ].y ),
      D3DCOLOR_RGBA( 255, 255, 255, 255 ), NORMAL_TEXT );

  union ARGB {
    std::uint32_t color;
    struct {
      std::uint8_t b, g, r, a;
    };
  };
  D3DCOLOR box_color_copy = box_color;

  if ( entity.GetName() == "Mocomochi" ) {
    drawing_.DrawBoundBoxOutline( &bound_box_screen,
                                  D3DCOLOR_RGBA( 255, 0, 0, 255 ) );
    drawing_.DrawBoundBoxFilled( &bound_box_screen,
                                 D3DCOLOR_RGBA( 255, 0, 0, 30 ) );
  } else {
    drawing_.DrawBoundBoxOutline( &bound_box_screen, box_color_copy );
    reinterpret_cast<ARGB*>( &box_color_copy )->a = 30;
    drawing_.DrawBoundBoxFilled( &bound_box_screen, box_color_copy );
  }

  D3DXVECTOR3 bound_box_center_pos = math::CalculateBoxCenter( &bound_box );
  D3DXVECTOR3 bound_box_center_pos_screen;

  if ( math::FlyffWorldToScreen( this, entity, bound_box_center_pos,
                                 &bound_box_center_pos_screen ) ) {
    // Draw a line from the character to the entity
    if ( entity.GetName() == "Mocomochi" ) {
      drawing_.DrawLine( local_player_sceen_pos.x, local_player_sceen_pos.y,
                         bound_box_center_pos_screen.x,
                         bound_box_center_pos_screen.y,
                         D3DCOLOR_ARGB( 100, 255, 0, 0 ) );
    } else {
      drawing_.DrawLine(
          local_player_sceen_pos.x, local_player_sceen_pos.y,
          bound_box_center_pos_screen.x, bound_box_center_pos_screen.y,
          D3DCOLOR_ARGB( 100, reinterpret_cast<ARGB*>( &box_color_copy )->r,
                         reinterpret_cast<ARGB*>( &box_color_copy )->g,
                         reinterpret_cast<ARGB*>( &box_color_copy )->b ) );
    }

    const float rect_size = 5.f;
    // Draw the middle rectangle dot
    drawing_.DrawRectangle( bound_box_center_pos_screen.x - rect_size / 2,
                            bound_box_center_pos_screen.y - rect_size / 2,
                            rect_size, rect_size,
                            D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
  }
}

void BotCore::Render( LPDIRECT3DDEVICE9 pDevice ) {
  Stopwatch stopwatch;
  stopwatch.Start();
  const auto local_player_entity = client_->CreateLocalPlayer();

  if ( local_player_entity->IsDeletedOrInvalidMemory() )
    return;

  drawing_.SetRenderStates( true );

  EntityList entity_list( client_.get() );
  auto& mover_entities = entity_list.GetMoverEntities();

  // add to the entities
  for ( const auto& entity : extra_entities_ )
    DrawEntity( *local_player_entity, *entity,
                D3DCOLOR_RGBA( 255, 255, 255, 255 ) );

  for ( auto& entity : mover_entities ) {
    if ( entity->IsDeletedOrInvalidMemory() )
      continue;

    if ( entity->GetPointerAddress() ==
         local_player_entity->GetPointerAddress() )
      continue;

    auto string_contains_any_of =
        []( const std::string& s,
            const std::initializer_list<char>& characters ) -> bool {
      for ( auto c : characters ) {
        const auto result = s.find( c );
        if ( result != std::string::npos ) {
          return true;
        }
      }

      return false;
    };

    if ( entity->IsPlayer() ) {
      DrawEntity( *local_player_entity, *entity,
                  D3DCOLOR_RGBA( 0, 162, 232, 255 ) );
    } else {
      DrawEntity( *local_player_entity, *entity,
                  D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
    }
  }

  /*
  auto& common_entities = entity_list.GetCommonCtrlEntities();

  for ( auto& entity : common_entities )
    DrawEntity( local_player_entity, entity, D3DCOLOR_RGBA( 255, 0, 0, 255 ) );
  */

  if ( GetStarted() ) {
    auto& object_entities = entity_list.GetObjectEntities();

    for ( auto& entity : object_entities ) {
      // is the option enabled?
      if ( bot_options_.GetOption<RemoveAllObstacleOption>().IsEnabled() ) {
        if ( entity->IsDeletedOrInvalidMemory() ) {
          continue;
        }

        entity->SetFlags( entity->GetFlags() | OBJ_FLAG_DELETE );
      }
    }
  }

  const auto& avg_y_pos = bot_options_.GetOption<AverageYPosOption>();

  if ( GetStarted() ) {
    if ( avg_y_pos.IsEnabled() ) {
      // TODO: Consider only creating it once, then just changing the position.
      // This is a waste. But fuck it, can't bother.
      const std::unique_ptr<Entity> plane_entity =
          std::make_unique<EntityReplicateBox>( client_.get(),
                                                *local_player_entity );
      const float size = 3.f;
      float plane_y = avg_y_pos.GetYPos();
      auto plane_position = plane_entity->GetPosition();
      plane_position.y = plane_y;
      plane_entity->SetPosition( plane_position );
      BOUND_BOX plane_bound_box;
      plane_bound_box.pos[ 0 ] = { -size, 0.01f, -size };
      plane_bound_box.pos[ 1 ] = { size, 0.01f, -size };
      plane_bound_box.pos[ 2 ] = { size, 0.01f, size };
      plane_bound_box.pos[ 3 ] = { -size, 0.01f, size };
      plane_bound_box.pos[ 4 ] = { -size, -0.01f, -size };
      plane_bound_box.pos[ 5 ] = { size, -0.01f, -size };
      plane_bound_box.pos[ 6 ] = { size, -0.01f, size };
      plane_bound_box.pos[ 7 ] = { -size, -0.01f, size };
      plane_entity->SetBoundBox( plane_bound_box );
      // Draw the flat plane
      DrawEntity( *local_player_entity, *plane_entity,
                  D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
    }
  }

  const auto& level_area = bot_options_.GetOption<LevelAreaOption>();

  if ( level_area.IsEnabled() ) {
    const auto bot_start_position = level_area.GetOriginPosition();

    const auto length1 = level_area.GetLength1();
    const auto length2 = level_area.GetLength2();

    for ( int i = 0; i < 4; ++i ) {
      const std::unique_ptr<Entity> corner_entity =
          std::make_unique<EntityReplicateBox>( client_.get(),
                                                *local_player_entity );

      corner_entity->SetWorldMatrix( local_player_entity->GetWorldMatrix() );

      // Spread the corners out from the origin point (local player position)
      auto corner_pos = level_area.GetCornerPosition( bot_start_position, i );
      corner_pos.y = local_player_entity->GetPosition().y;

      corner_entity->SetPosition( corner_pos );

      const float size_x = 0.1f;
      const float size_z = 0.1f;
      const float size_y = 10.f;

      BOUND_BOX corner_bound_box;
      corner_bound_box.pos[ 0 ] = { -size_x, size_y, -size_z };
      corner_bound_box.pos[ 1 ] = { size_x, size_y, -size_z };
      corner_bound_box.pos[ 2 ] = { size_x, size_y, size_z };
      corner_bound_box.pos[ 3 ] = { -size_x, size_y, size_z };

      corner_bound_box.pos[ 4 ] = { -size_x, -size_y, -size_z };
      corner_bound_box.pos[ 5 ] = { size_x, -size_y, -size_z };
      corner_bound_box.pos[ 6 ] = { size_x, -size_y, size_z };
      corner_bound_box.pos[ 7 ] = { -size_x, -size_y, size_z };

      corner_entity->SetBoundBox( corner_bound_box );

      // Draw the flat plane
      DrawEntity( *local_player_entity, *corner_entity,
                  D3DCOLOR_RGBA( 255, 242, 0, 255 ) );
    }
  }

  int total_entities = mover_entities.size();
  stopwatch.Stop();

  const auto viewport = GetViewport();
  const int x = viewport.Width / 2;
  const int y = viewport.Height;
  const std::wstring s1 =
      TEXT( "Render Time: " ) +
      std::to_wstring( stopwatch.GetElapsedMilliseconds() ) +
      TEXT( " ms - Frame Count: " ) + std::to_wstring( frame_counter_ ) +
      TEXT( " - FPS: " ) + std::to_wstring( fps_ );

  drawing_.GetTextWidth( s1, NORMAL_TEXT );

  drawing_.DrawTextC( s1, x - ( drawing_.GetTextWidth( s1, NORMAL_TEXT ) / 2 ),
                      y - 30, D3DCOLOR_RGBA( 0, 0, 0, 255 ), NORMAL_TEXT );

  std::wstring s2 = TEXT( "Total Entities: " ) +
                    std::to_wstring( total_entities ) + TEXT( " Types: " );

  for ( int i = 0; i < ( int )ObjectType::kMaxObjectTypes; ++i ) {
    s2 += std::to_wstring( i ) + TEXT( "=" ) +
          std::to_wstring(
              entity_list.GetEntitiesOfType( ( ObjectType )i ).size() ) +
          TEXT( ", " );
  }

  drawing_.DrawTextC( s2, x - ( drawing_.GetTextWidth( s2, NORMAL_TEXT ) / 2 ),
                      y - 45, D3DCOLOR_RGBA( 0, 0, 0, 255 ), NORMAL_TEXT );
}

void BotCore::Update() {
  for ( const auto& bot : active_bots_ ) {
    bot->Update();
  }

  const auto diff =
      static_cast<int64_t>( GetTickCount64() - last_cheats_update_ms_ );

  // update the values every second
  if ( std::abs( diff ) > 500 ) {
    UpdateCheatsValues();
    last_cheats_update_ms_ = GetTickCount64();
  }
}

typedef enum _PROCESSINFOCLASS {
  ProcessBasicInformation,  // q: PROCESS_BASIC_INFORMATION,
                            // PROCESS_EXTENDED_BASIC_INFORMATION
  ProcessQuotaLimits,  // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
  ProcessIoCounters,  // q: IO_COUNTERS
  ProcessVmCounters,  // q: VM_COUNTERS, VM_COUNTERS_EX, VM_COUNTERS_EX2
  ProcessTimes,  // q: KERNEL_USER_TIMES
  ProcessBasePriority,  // s: KPRIORITY
  ProcessRaisePriority,  // s: ULONG
  ProcessDebugPort,  // q: HANDLE
  ProcessExceptionPort,  // s: PROCESS_EXCEPTION_PORT
  ProcessAccessToken,  // s: PROCESS_ACCESS_TOKEN
  ProcessLdtInformation,  // qs: PROCESS_LDT_INFORMATION // 10
  ProcessLdtSize,  // s: PROCESS_LDT_SIZE
  ProcessDefaultHardErrorMode,  // qs: ULONG
  ProcessIoPortHandlers,  // (kernel-mode only)
  ProcessPooledUsageAndLimits,  // q: POOLED_USAGE_AND_LIMITS
  ProcessWorkingSetWatch,  // q: PROCESS_WS_WATCH_INFORMATION[]; s: void
  ProcessUserModeIOPL,
  ProcessEnableAlignmentFaultFixup,  // s: BOOLEAN
  ProcessPriorityClass,  // qs: PROCESS_PRIORITY_CLASS
  ProcessWx86Information,
  ProcessHandleCount,  // q: ULONG, PROCESS_HANDLE_INFORMATION // 20
  ProcessAffinityMask,  // s: KAFFINITY
  ProcessPriorityBoost,  // qs: ULONG
  ProcessDeviceMap,  // qs: PROCESS_DEVICEMAP_INFORMATION,
                     // PROCESS_DEVICEMAP_INFORMATION_EX
  ProcessSessionInformation,  // q: PROCESS_SESSION_INFORMATION
  ProcessForegroundInformation,  // s: PROCESS_FOREGROUND_BACKGROUND
  ProcessWow64Information,  // q: ULONG_PTR
  ProcessImageFileName,  // q: UNICODE_STRING
  ProcessLUIDDeviceMapsEnabled,  // q: ULONG
  ProcessBreakOnTermination,  // qs: ULONG
  ProcessDebugObjectHandle,  // q: HANDLE // 30
  ProcessDebugFlags,  // qs: ULONG
  ProcessHandleTracing,  // q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables,
                         // otherwise enables
  ProcessIoPriority,  // qs: IO_PRIORITY_HINT
  ProcessExecuteFlags,  // qs: ULONG
  ProcessResourceManagement,  // ProcessTlsInformation //
                              // PROCESS_TLS_INFORMATION
  ProcessCookie,  // q: ULONG
  ProcessImageInformation,  // q: SECTION_IMAGE_INFORMATION
  ProcessCycleTime,  // q: PROCESS_CYCLE_TIME_INFORMATION // since VISTA
  ProcessPagePriority,  // q: PAGE_PRIORITY_INFORMATION
  ProcessInstrumentationCallback,  // qs:
                                   // PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION
                                   // // 40
  ProcessThreadStackAllocation,  // s: PROCESS_STACK_ALLOCATION_INFORMATION,
                                 // PROCESS_STACK_ALLOCATION_INFORMATION_EX
  ProcessWorkingSetWatchEx,  // q: PROCESS_WS_WATCH_INFORMATION_EX[]
  ProcessImageFileNameWin32,  // q: UNICODE_STRING
  ProcessImageFileMapping,  // q: HANDLE (input)
  ProcessAffinityUpdateMode,  // qs: PROCESS_AFFINITY_UPDATE_MODE
  ProcessMemoryAllocationMode,  // qs: PROCESS_MEMORY_ALLOCATION_MODE
  ProcessGroupInformation,  // q: USHORT[]
  ProcessTokenVirtualizationEnabled,  // s: ULONG
  ProcessConsoleHostProcess,  // q: ULONG_PTR // ProcessOwnerInformation
  ProcessWindowInformation,  // q: PROCESS_WINDOW_INFORMATION // 50
  ProcessHandleInformation,  // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since
                             // WIN8
  ProcessMitigationPolicy,  // s: PROCESS_MITIGATION_POLICY_INFORMATION
  ProcessDynamicFunctionTableInformation,
  ProcessHandleCheckingMode,  // qs: ULONG; s: 0 disables, otherwise enables
  ProcessKeepAliveCount,  // q: PROCESS_KEEPALIVE_COUNT_INFORMATION
  ProcessRevokeFileHandles,  // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
  ProcessWorkingSetControl,  // s: PROCESS_WORKING_SET_CONTROL
  ProcessHandleTable,  // q: ULONG[] // since WINBLUE
  ProcessCheckStackExtentsMode,
  ProcessCommandLineInformation,  // q: UNICODE_STRING // 60
  ProcessProtectionInformation,  // q: PS_PROTECTION
  ProcessMemoryExhaustion,  // PROCESS_MEMORY_EXHAUSTION_INFO // since THRESHOLD
  ProcessFaultInformation,  // PROCESS_FAULT_INFORMATION
  ProcessTelemetryIdInformation,  // PROCESS_TELEMETRY_ID_INFORMATION
  ProcessCommitReleaseInformation,  // PROCESS_COMMIT_RELEASE_INFORMATION
  ProcessDefaultCpuSetsInformation,
  ProcessAllowedCpuSetsInformation,
  ProcessSubsystemProcess,
  ProcessJobMemoryInformation,  // PROCESS_JOB_MEMORY_INFO
  ProcessInPrivate,  // since THRESHOLD2 // 70
  ProcessRaiseUMExceptionOnInvalidHandleClose,  // qs: ULONG; s: 0 disables,
                                                // otherwise enables
  ProcessIumChallengeResponse,
  ProcessChildProcessInformation,  // PROCESS_CHILD_PROCESS_INFORMATION
  ProcessHighGraphicsPriorityInformation,
  ProcessSubsystemInformation,  // q: SUBSYSTEM_INFORMATION_TYPE // since
                                // REDSTONE2
  ProcessEnergyValues,  // PROCESS_ENERGY_VALUES, PROCESS_EXTENDED_ENERGY_VALUES
  ProcessActivityThrottleState,  // PROCESS_ACTIVITY_THROTTLE_STATE
  ProcessActivityThrottlePolicy,  // PROCESS_ACTIVITY_THROTTLE_POLICY
  ProcessWin32kSyscallFilterInformation,
  ProcessDisableSystemAllowedCpuSets,  // 80
  ProcessWakeInformation,  // PROCESS_WAKE_INFORMATION
  ProcessEnergyTrackingState,  // PROCESS_ENERGY_TRACKING_STATE
  ProcessManageWritesToExecutableMemory,  // MANAGE_WRITES_TO_EXECUTABLE_MEMORY
                                          // // since REDSTONE3
  ProcessCaptureTrustletLiveDump,
  ProcessTelemetryCoverage,
  ProcessEnclaveInformation,
  ProcessEnableReadWriteVmLogging,  // PROCESS_READWRITEVM_LOGGING_INFORMATION
  ProcessUptimeInformation,  // PROCESS_UPTIME_INFORMATION
  ProcessImageSection,  // q: HANDLE
  ProcessDebugAuthInformation,  // since REDSTONE4 // 90
  ProcessSystemResourceManagement,  // PROCESS_SYSTEM_RESOURCE_MANAGEMENT
  ProcessSequenceNumber,  // q: ULONGLONG
  ProcessLoaderDetour,  // since REDSTONE5
  ProcessSecurityDomainInformation,  // PROCESS_SECURITY_DOMAIN_INFORMATION
  ProcessCombineSecurityDomainsInformation,  // PROCESS_COMBINE_SECURITY_DOMAINS_INFORMATION
  ProcessEnableLogging,  // PROCESS_LOGGING_INFORMATION
  ProcessLeapSecondInformation,  // PROCESS_LEAP_SECOND_INFORMATION
  ProcessFiberShadowStackAllocation,  // PROCESS_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION
                                      // // since 19H1
  ProcessFreeFiberShadowStackAllocation,  // PROCESS_FREE_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION
  MaxProcessInfoClass
} PROCESSINFOCLASS;

using tNtSetInformationProcess =
    NTSTATUS( NTAPI* )( IN HANDLE ProcessHandle,
                        IN PROCESSINFOCLASS ProcessInformationClass,
                        IN PVOID ProcessInformation,
                        IN ULONG ProcessInformationLength );

using tNtQueryInformationProcess =
    NTSTATUS( NTAPI* )( IN HANDLE ProcessHandle,
                        IN PROCESSINFOCLASS ProcessInformationClass,
                        OUT PVOID ProcessInformation,
                        IN ULONG ProcessInformationLength,
                        OUT PULONG ReturnLength );

const auto ntdll = LoadLibrary( TEXT( "ntdll.dll" ) );

const auto NtQueryInformationProcess =
    reinterpret_cast<tNtQueryInformationProcess>(
        GetProcAddress( ntdll, "NtQueryInformationProcess" ) );

const auto NtSetInformationProcess = reinterpret_cast<tNtSetInformationProcess>(
    GetProcAddress( ntdll, "NtSetInformationProcess" ) );

NTSTATUS SetDebugFlags( const bool debugging ) {
  ULONG no_debug_inherit = !debugging;

  return NtSetInformationProcess(
      reinterpret_cast<HANDLE>( -1 ), PROCESSINFOCLASS::ProcessDebugFlags,
      &no_debug_inherit, sizeof( no_debug_inherit ) );
}

bool HasDebugFlags() {
  ULONG no_debug_inherit = 0;
  ULONG return_length = 0;

  // Check debugging, it SHOULD be true because we set it earlier
  const NTSTATUS status = NtQueryInformationProcess(
      reinterpret_cast<HANDLE>( -1 ), PROCESSINFOCLASS::ProcessDebugFlags,
      &no_debug_inherit, sizeof( no_debug_inherit ), &return_length );

  // TODO: Figure out what to do if fails
  if ( status != 0 )
    return false;

  if ( return_length != sizeof( no_debug_inherit ) )
    return true;

  return no_debug_inherit == FALSE;
}

bool IsAntiAntiDebugFlagsDetected() {
  // Explicitly enable the debug flags to say that we are being debugged
  SetDebugFlags( true );

  // Is the method returning debug flags that are saying we're debugging?
  // If not, then we know it has been hooked and returning that we are not under
  // a debugger.
  const auto has_debug_flags = HasDebugFlags();

  // Reset the debugger flag for the next call
  SetDebugFlags( false );

  return !has_debug_flags;
}

bool IsDebuggerAttached() {
  // Is a debugger attached?
  if ( HasDebugFlags() )
    return true;

  // If we did not find a debugger attached above, check if an anti anti
  // debugging is present
  if ( IsAntiAntiDebugFlagsDetected() )
    return true;

  return false;
}

void BotCore::UpdateCheatsValues() {
  auto local_player = client_->CreateLocalPlayer();

  if ( local_player->IsDeletedOrInvalidMemory() )
    return;

  auto speed_multiplier = bot_options_.GetSpeedMultiplier();

  if ( speed_multiplier != 0 )
    local_player->SetSpeedMultiplier( speed_multiplier );

  auto botcore = Initializer().GetBotCore();
  botcore->SetIsDebuggerAttached( IsDebuggerAttached() );
}

void BotCore::ToggleBot() {
  auto local_player = client_->CreateLocalPlayer();

  if ( local_player->IsDeletedOrInvalidMemory() ) {
    gwingui::messagebox::Error(
        TEXT( "Unable to toggle the bot, local player is invalid." ) );
    return;
  }

  if ( bot_options_.HasOptionsBeenChanged() ) {
    // If the previous bot status was off, then we are trying to turn it on.
    if ( !GetStarted() ) {
      if ( !bot_options_.TryApplyOptions( local_player->GetName() ) ) {
        gwingui::messagebox::Error( TEXT(
            "Unable to start the bot, the options could not be applied." ) );
        return;
      }
    }
  }

  SetStarted( !GetStarted() );

  if ( GetStarted() ) {
    const auto level_area_option = bot_options_.GetOption<LevelAreaOption>();

    if ( level_area_option.IsEnabled() ) {
      // Check if the user starts the bot outside the level area and warn them
      if ( !level_area_option.IsEntityAllowed( *local_player ) ) {
        gwingui::messagebox::Warning(
            TEXT( "You started the bot while being outside the level area you "
                  "have set for the bot. This might cause the bot to not find "
                  "monsters." ) );
      }
    }

    switch ( bot_options_.GetOption<BotModeOption>().GetBotMode() ) {
      case BotMode::kBotModeOneVsOne: {
        std::vector<std::unique_ptr<Bot>> bots;

        bots.push_back( std::make_unique<BotAIOneVsOne>( this ) );
        bots.push_back( std::make_unique<BotPassive>( this ) );

        SetActiveBots( std::move( bots ) );
      } break;

      default:
        break;
    }
  }

  ShowBotHasStoppedWindow();
}

void BotCore::OnBotStatusChange() {
  if ( GetStarted() ) {
    gwingui::control::SetText( GWH( BUTTON_TOGGLEBOT ), TEXT( "Stop bot" ) );
  } else {
    gwingui::control::SetText( GWH( BUTTON_TOGGLEBOT ), TEXT( "Start bot" ) );
  }
}

void BotCore::SkipUpdateForFrames( int64_t frames ) {
  last_frame_counter_ = frame_counter_;
  skipped_frames_ = frames;
}

void BotCore::SetStarted( bool started ) {
  started_ = started;
  OnBotStatusChange();
}

void BotCore::SetIsDebuggerAttached( bool value ) {
  bad_boy_present_ = value;
}

bool BotCore::GetStarted() const {
  return started_;
}

int64_t BotCore::GetFrameCount() const {
  return frame_counter_;
}

BotOptions& BotCore::GetBotOptions() {
  return bot_options_;
}

FlyffClient* BotCore::GetFlyffClient() {
  return client_.get();
}

D3DVIEWPORT9 BotCore::GetViewport() const {
  return viewport_;
}

void BotCore::SetViewport( D3DVIEWPORT9 viewport ) {
  viewport_ = viewport;
}

void BotCore::AddEntityToDraw( std::unique_ptr<Entity>& entity ) {
  extra_entities_.push_back( std::move( entity ) );
}

std::vector<std::unique_ptr<Bot>>& BotCore::GetActiveBots() {
  return active_bots_;
}

LPDIRECT3DDEVICE9 BotCore::GetDevice() {
  return drawing_.GetDevice();
}

HWND BotCore::GetTargetWindow() {
  return target_window_handle_;
}

void BotCore::SetActiveBots( std::vector<std::unique_ptr<Bot>>&& bots ) {
  active_bots_ = std::move( bots );
}

void BotCore::SetSimulatedCursorPos( POINT& pos ) {
  simulated_cursor_pos_ = pos;
}

}  // namespace bot
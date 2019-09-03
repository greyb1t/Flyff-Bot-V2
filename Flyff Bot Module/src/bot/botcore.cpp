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
#include "..\servers\flyff_client_mazey.h"
#include "..\servers\flyff_client_ignite.h"
#include "..\servers\flyff_client_dragon_crusade.h"

#include "../utils/trace.h"

#include "gwinmem/process_memory_internal.h"

#include "gwinguiv2\controls\richedit.h"
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

    /*
    // Unhook itself to avoid being detected by the integrity checks
    // This function being hooked lays inside the ignite flyff module
    // TODO: Avoid being detected by integrity checks by not using detours
    // anymore. Start using VEH hooking...
    DetourTransactionBegin();

    DetourUpdateThread( GetCurrentThread() );

    DetourDetach( reinterpret_cast<PVOID*>(
                      &Initializer().GetBotCore()->d3dxvec3proj_original_ ),
                  BotCore::D3DXVec3ProjectHooked );

    if ( DetourTransactionCommit() != NO_ERROR ) {
      gwingui::messagebox::Error(
          TEXT( "Error while trying to detach all hooks." ) );
    }
    */
  }

  // LeaveCriticalSection( &g_d3dvec3project_critical_section_ );

  // return botcore->d3dxvec3proj_original_( pOut, pV, pViewport, pProjection,
  //                                         pView, pWorld );

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
    // return botcore->get_cursor_pos_original_( lpPoint );
    return getcursorpos_hook_original_function( lpPoint );
  }

  std::lock_guard<std::mutex>( g_hooks_mutex, std::adopt_lock );

  // BOOL ret = botcore->get_cursor_pos_original_( lpPoint );
  BOOL ret = getcursorpos_hook_original_function( lpPoint );

  if ( botcore->GetStarted() && botcore->simulated_cursor_pos_.x != -1 &&
       botcore->simulated_cursor_pos_.y != -1 ) {
    lpPoint->x = botcore->simulated_cursor_pos_.x;
    lpPoint->y = botcore->simulated_cursor_pos_.y;
  }

  // LeaveCriticalSection( &g_getcursorpos_critical_section_ );

  return ret;
}

std::wstring GetRandomString( uint32_t length ) {
  std::wstring s = TEXT( "" );

  std::wstring ascii = TEXT( "abcdef1234567890" );

  for ( uint32_t i = 0; i < length; ++i ) {
    uint32_t random_value = rand() % ( ascii.size() - 1 );
    s += ascii[ random_value ];
  }

  return s;
}

SHORT BotCore::GetKeyStateHooked( int nVirtKey ) {
  const auto botcore = Initializer().GetBotCore();

  const auto getkeystate_hook =
      botcore->GetFlyffClient()->GetHookManager()->GetHook(
          HookType::GetKeyState );

  const auto getkeystate_hook_original_function =
      reinterpret_cast<tGetKeyState>( getkeystate_hook.original_function );

  if ( !g_hooks_mutex.try_lock() ) {
    // return botcore->get_key_state_original_( nVirtKey );
    return getkeystate_hook_original_function( nVirtKey );
  }

  std::lock_guard<std::mutex>( g_hooks_mutex, std::adopt_lock );

  // Always return the status that the key is down,
  // this bypasses Ignite Flyff's protection against mouse click simulator with
  // window messages
  // auto get_key_state_original =
  //     Initializer().GetBotCore()->get_key_state_original_;

  const auto module_name =
      Initializer().GetBotCore()->GetFlyffClient()->GetModuleName();

  const auto module = gwinmem::CurrentProcess().GetModule( module_name );

  const auto ret_addr = reinterpret_cast<uint32_t>( _ReturnAddress() );

  // Is the GetKeyState called from inside the Ignite.exe module
  if ( ret_addr >= module.base && ret_addr <= ( module.base + module.size ) ) {
    // Spoof the value to bypass the check
    return MAKEWORD( 0, 1 );
  } else {
    // Call the original for functionality to stay the same for other stuff e.g.
    // ScrollBar in my GUI
    // return get_key_state_original( nVirtKey );
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
    if ( GetBot() ) {
      waiting_window::DisplayWaitingWindowAsync(
          TEXT( "Waiting for the bot to finish its tasks..." ), [=]() {
            // Wait until the state is set to stopped
            while ( !GetBot()->IsStateStopped() )
              Sleep( 5 );

            SetActiveBot( nullptr );
          } );
    }
  }
}

void BotCore::DrawEntity( const UniquePtrEntity& local_player,
                          const UniquePtrEntity& entity,
                          const D3DCOLOR box_color ) {
  BOUND_BOX local_player_bound_box = local_player->GetBoundBox();
  D3DXMATRIX local_player_mat_world = local_player->GetWorldMatrix();
  D3DXVECTOR3 local_player_bound_box_center =
      math::CalculateBoxCenter( &local_player_bound_box );
  D3DXVECTOR3 local_player_sceen_pos;
  math::FlyffWorldToScreen( this, local_player, local_player_bound_box_center,
                            &local_player_sceen_pos );
  BOUND_BOX bound_box = entity->GetBoundBox();
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
      TEXT( "Name: " ) + stringutils::AnsiToWide( entity->GetName() ),
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

  if ( entity->GetName() == "Mocomochi" ) {
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
    if ( entity->GetName() == "Mocomochi" ) {
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

// #pragma optimize( "", off )

void BotCore::Render( LPDIRECT3DDEVICE9 pDevice ) {
  Stopwatch stopwatch;
  stopwatch.Start();
  const auto local_player = client_->CreateLocalPlayer();
  const UniquePtrEntity local_player_entity = client_->CreateLocalPlayer();

  if ( local_player->IsDeletedOrInvalidMemory() ||
       local_player_entity->IsDeletedOrInvalidMemory() )
    return;

  drawing_.SetRenderStates( true );

  EntityList entity_list( client_.get() );
  auto& mover_entities = entity_list.GetMoverEntities();

  // add to the entities
  for ( const auto& entity : extra_entities_ )
    DrawEntity( local_player_entity, entity,
                D3DCOLOR_RGBA( 255, 255, 255, 255 ) );

  for ( auto& entity : mover_entities ) {
    if ( entity->IsDeletedOrInvalidMemory() )
      continue;

    if ( entity->GetPointerAddress() == local_player->GetPointerAddress() )
      continue;

    // if (!entity->IsMonster() && !entity->IsItem() && !entity->IsNpc() &&
    // !entity->IsPet() && !entity->IsPlayer()) {
    //  if (GetStarted()) {
    //    // is the option enabled?
    //    if (bot_options_.GetRemoveAllObstaclesOption()->IsEnabled()) {
    //      // Removes all the objects high high up in the sky!
    //      auto pos = entity->GetPosition();
    //      pos.y = 1000.f;
    //      entity->SetPosition(pos);
    //    }
    //  }
    //  continue;
    //}
    // if (!entity->IsMonster() || !entity->IsAlive())
    //   continue;

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
      DrawEntity( local_player_entity, entity,
                  D3DCOLOR_RGBA( 0, 162, 232, 255 ) );
    } else {
      DrawEntity( local_player_entity, entity,
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
      if ( bot_options_.GetRemoveAllObstaclesOption()->IsEnabled() ) {
        // Removes all the objects high high up in the sky!
        uint32_t address_ptr = 0;
        int ptr = 0;
        bool ptr_read_ret = false;
        bool th = false;

        if ( entity->IsDeletedOrInvalidMemory() ) {
          address_ptr = entity->GetPointerAddress();
          // ptr_read_ret =
          ptr = gwinmem::CurrentProcess().Read<int>( address_ptr );
          th = true;
        }

        // One time, the crash was caused by the __vfptr being the y pos value
        // DOES IT HAVE THE POSISON????
        auto pos = entity->GetPosition();

        // If the position is invalid, then the entity is invalid...
        if ( pos.x == 0.f && pos.y == 0.f && pos.z == 0.f )
          continue;

        /*
        if ( ( entity->GetFlags() & OBJ_FLAG_DELETE ) != OBJ_FLAG_DELETE ) {
          entity->SetFlags(entity->GetFlags() | OBJ_FLAG_DELETE);
        }
        */

        /*
        // Has the object been moved previously?
        if ( entity->GetWorldMatrix() != D3DMATRIX{} ) {
          entity->SetWorldMatrix( {} );
        }
        */

        // Has the object been moved previously?
        if ( pos.y != 1000.f ) {
          pos.y = 1000.f;
          entity->SetPosition( pos );
        }

        if ( ( entity->GetFlags() & OBJ_FLAG_DELETE ) != OBJ_FLAG_DELETE ) {
          entity->SetFlags( entity->GetFlags() | OBJ_FLAG_DELETE );
        }
      }
    }
  }

  const auto current_active_bot = GetBot();

  const auto avg_y_pos =
      bot_options_.GetAverageYPositionTargetSelectionOption();

  if ( current_active_bot ) {
    if ( avg_y_pos->IsEnabled() ) {
      // TODO: Consider only creating it once, then just changing the position.
      // This is a waste. But fuck it, can't bother.
      const UniquePtrEntity plane_entity =
          std::make_unique<EntityReplicateBox>( client_.get(), local_player );
      const float size = 3.f;
      float plane_y = avg_y_pos->GetYPos();
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
      DrawEntity( local_player_entity, plane_entity,
                  D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
    }
  }

  const auto level_area = bot_options_.GetLevelAreaOption();

  if ( level_area->IsEnabled() ) {
    const auto bot_start_position = level_area->GetOriginPosition();

    const auto length1 = level_area->GetLength1();
    const auto length2 = level_area->GetLength2();

    for ( int i = 0; i < 4; ++i ) {
      const UniquePtrEntity corner_entity =
          std::make_unique<EntityReplicateBox>( client_.get(), local_player );

      corner_entity->SetWorldMatrix( local_player->GetWorldMatrix() );

      // Spread the corners out from the origin point (local player position)
      auto corner_pos = level_area->GetCornerPosition( bot_start_position, i );
      corner_pos.y = local_player->GetPosition().y;

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

      // TODO: Make DrawEntity() take a pointer to a Entity instead of a
      // unique ptr, same with FlyffWorldToScreen()

      // TODO: Remove the opacity on the corners

      // Draw the flat plane
      DrawEntity( local_player_entity, corner_entity,
                  D3DCOLOR_RGBA( 255, 242, 0, 255 ) );
    }
  }

  int total_entities = mover_entities.size();
  stopwatch.Stop();
  /*
    if (GetStarted()) {
    const auto viewport = GetViewport();
    const int x = viewport.Width / 2;
    const int y = viewport.Height - 50;

    drawing_.DrawTextC(
      TEXT("PRESS END TO STOP THE BOT!"),
      x,
      y,
      D3DCOLOR_RGBA(255, 255, 255, 255),
      BIG_TEXT);
    }
  */

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

// #pragma optimize( "", on )

void BotCore::Update() {
  if ( bot_.get() )
    bot_->Update();

  if ( passive_bot_.get() )
    passive_bot_->Update();

  const auto diff =
      static_cast<int64_t>( GetTickCount64() - last_cheats_update_ms_ );

  // update the values every second
  if ( std::abs( diff ) > 500 ) {
    UpdateCheatsValues();
    last_cheats_update_ms_ = GetTickCount64();
  }
}

void BotCore::UpdateCheatsValues() {
  auto local_player = client_->CreateLocalPlayer();

  if ( local_player->IsDeletedOrInvalidMemory() )
    return;

  auto speed_multiplier = bot_options_.GetSpeedMultiplier();

  if ( speed_multiplier != 0 )
    local_player->SetSpeedMultiplier( speed_multiplier );
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
      if ( !bot_options_.ApplyOptions( local_player->GetName() ) ) {
        gwingui::messagebox::Error( TEXT(
            "Unable to start the bot, the options could not be applied." ) );
        return;
      }
    }
  }

  SetStarted( !GetStarted() );

  if ( GetStarted() ) {
    switch ( bot_options_.GetBotModeOption()->GetBotMode() ) {
      case BotMode::kBotModeOneVsOne: {
        SetActiveBot( new BotAIOneVsOne( this ) );
      } break;

      default:
        break;
    }
  }

  ShowBotHasStoppedWindow();
}

void BotCore::OnBotStatusChange() {
  if ( GetStarted() )
    gwingui::control::SetText( GWH( BUTTON_TOGGLEBOT ), TEXT( "Stop bot" ) );
  else
    gwingui::control::SetText( GWH( BUTTON_TOGGLEBOT ), TEXT( "Start bot" ) );
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

void BotCore::AddEntityToDraw( UniquePtrEntity& entity ) {
  extra_entities_.push_back( std::move( entity ) );
}

Bot* BotCore::GetBot() {
  return bot_.get();
}

Bot* BotCore::GetPassiveBot() {
  return passive_bot_.get();
}

LPDIRECT3DDEVICE9 BotCore::GetDevice() {
  return drawing_.GetDevice();
}

HWND BotCore::GetTargetWindow() {
  return target_window_handle_;
}

void BotCore::SetActiveBot( Bot* bot ) {
  bot_.reset( bot );

  if ( bot )
    passive_bot_.reset( new BotPassive( this ) );
  else
    passive_bot_.reset( 0 );
}

void BotCore::SetSimulatedCursorPos( POINT& pos ) {
  simulated_cursor_pos_ = pos;
}

}  // namespace bot
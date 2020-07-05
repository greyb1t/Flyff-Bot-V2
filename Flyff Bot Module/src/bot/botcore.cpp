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
      last_gui_entity_population_ms_( 0 ),
      ms_elapsed_( 0 ),
      fps_( 0 ),
      fps_frame_counter_( 0 ),
      last_time_( timeGetTime() ),
      is_stopwatch_started_( false ),
      has_initialized_d3d_( false ),
      has_initialized_matrix_addresses_( false ),
      bad_boy_present_( false ),
      endscene_ret_addr_( 0 ) {}

HRESULT BotCore::EndSceneHooked( LPDIRECT3DDEVICE9 device ) {
  const auto endscene_original_hook =
      Initializer().GetBotCore()->GetFlyffClient()->GetHookManager()->GetHook(
          HookType::EndScene );

  const auto endscene_original_function =
      reinterpret_cast<tEndScene>( endscene_original_hook.original_function );

  const auto endscene_ret_addr = Initializer().GetBotCore()->endscene_ret_addr_;

  // If we have saved the return address
  if ( endscene_ret_addr != 0 ) {
    // If the return address is not the same as the one we saved on the first call
    // do not render to ensure we only do it once each frame
    if ( reinterpret_cast<uintptr_t>( _ReturnAddress() ) !=
         endscene_ret_addr ) {
      return endscene_original_function( device );
    }
  }

  if ( !g_hooks_mutex.try_lock() ) {
    return endscene_original_function( device );
  }

  std::lock_guard<std::mutex>( g_hooks_mutex, std::adopt_lock );

  auto botcore = Initializer().GetBotCore();

  if ( !device ) {
    return endscene_original_function( device );
  }

  if ( !botcore->has_initialized_d3d_ ) {
    // Flyff calls EndScene multiple times each frame
    // Therefore we save the return address of the first call and
    // only draw on that to increase performance and avoid issues
    // with rendering on inventory and weird crap like that
    // NOTE: This can causes issues with rendering in case it saves the incorrect return address
    // but that has low chance of occuring, and one can only restart it to fix

    const auto ret_addr = reinterpret_cast<uintptr_t>( _ReturnAddress() );

    GWIN_TRACE( "Saved Return Address: %d\n", ret_addr );

    botcore->endscene_ret_addr_ = ret_addr;

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
          TEXT( "Waiting for the bot to finish its tasks..." ), [ = ]() {
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

//void BotCore::DrawEntityBBox( const Entity& local_player,
//                              const Entity& entity,
//                              BBOX& bbox,
//                              const D3DCOLOR box_color ) {
//  //BOUND_BOX bound_box = entity.GetBoundBox();
//
//  D3DXVECTOR3 screen_pos = {};
//
//  bbox.Extent[ 0 ] /= 2;
//  bbox.Extent[ 1 ] /= 2;
//  bbox.Extent[ 2 ] /= 2;
//
//  BOUND_BOX new_bb;
//
//  new_bb.pos[ 0 ].x = bbox.Extent[ 0 ];
//  new_bb.pos[ 0 ].y = bbox.Extent[ 1 ];
//  new_bb.pos[ 0 ].z = bbox.Extent[ 2 ];
//
//  new_bb.pos[ 4 ].x = bbox.Extent[ 0 ];
//  new_bb.pos[ 4 ].y = -bbox.Extent[ 1 ];
//  new_bb.pos[ 4 ].z = bbox.Extent[ 2 ];
//
//  new_bb.pos[ 5 ].x = bbox.Extent[ 0 ];
//  new_bb.pos[ 5 ].y = -bbox.Extent[ 1 ];
//  new_bb.pos[ 5 ].z = -bbox.Extent[ 2 ];
//
//  new_bb.pos[ 1 ].x = bbox.Extent[ 0 ];
//  new_bb.pos[ 1 ].y = bbox.Extent[ 1 ];
//  new_bb.pos[ 1 ].z = -bbox.Extent[ 2 ];
//
//  new_bb.pos[ 2 ].x = -bbox.Extent[ 0 ];
//  new_bb.pos[ 2 ].y = bbox.Extent[ 1 ];
//  new_bb.pos[ 2 ].z = -bbox.Extent[ 2 ];
//
//  new_bb.pos[ 6 ].x = -bbox.Extent[ 0 ];
//  new_bb.pos[ 6 ].y = -bbox.Extent[ 1 ];
//  new_bb.pos[ 6 ].z = -bbox.Extent[ 2 ];
//
//  new_bb.pos[ 7 ].x = -bbox.Extent[ 0 ];
//  new_bb.pos[ 7 ].y = -bbox.Extent[ 1 ];
//  new_bb.pos[ 7 ].z = bbox.Extent[ 2 ];
//
//  new_bb.pos[ 3 ].x = -bbox.Extent[ 0 ];
//  new_bb.pos[ 3 ].y = bbox.Extent[ 1 ];
//  new_bb.pos[ 3 ].z = bbox.Extent[ 2 ];
//
//  /*
//    	Extent[0] = vExtent.x
//      Extent[1] = vExtent.y
//      Extent[2] = vExtent.z
//  */
//
//  /*
//    Front face
//    0, 4, 5, 1
//  */
//
//  //const bool inside_view =
//  //    math::FlyffWorldToScreen( this, entity, loly, &screen_pos );
//
//  bool inside_view = true;
//  BOUND_BOX bound_box_screen;
//
//  for ( int i = 0; i < BOUND_BOX::kBoxCorners; ++i ) {
//    D3DXVECTOR3 screen_pos;
//
//    if ( !math::FlyffWorldToScreen( this, entity, new_bb.pos[ i ],
//                                    &screen_pos ) ) {
//      inside_view = false;
//      break;
//    }
//
//    bound_box_screen.pos[ i ] = screen_pos;
//  }
//
//  // don't draw an entity outside of the screen
//  //if ( !inside_view )
//  //  return;
//
//  union ARGB {
//    std::uint32_t color;
//    struct {
//      std::uint8_t b, g, r, a;
//    };
//  };
//
//  D3DCOLOR box_color_copy = box_color;
//
//  /*
//  const float kCornerSize = 5.0f;
//  drawing_.DrawRectangle( screen_pos.x - ( kCornerSize / 2 ),
//                          screen_pos.y - ( kCornerSize / 2 ), kCornerSize,
//                          kCornerSize, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
//  */
//
//  drawing_.DrawBoundBoxOutline( &bound_box_screen, box_color_copy );
//  reinterpret_cast<ARGB*>( &box_color_copy )->a = 30;
//  drawing_.DrawBoundBoxFilled( &bound_box_screen, box_color_copy );
//
//  /*
//  D3DXVECTOR3 bound_box_center_pos = math::CalculateBoxCenter( &bound_box );
//  D3DXVECTOR3 bound_box_center_pos_screen;
//
//  if ( math::FlyffWorldToScreen( this, entity, bound_box_center_pos,
//                                 &bound_box_center_pos_screen ) ) {
//    const float rect_size = 5.f;
//    // Draw the middle rectangle dot
//    drawing_.DrawRectangle( bound_box_center_pos_screen.x - rect_size / 2,
//                            bound_box_center_pos_screen.y - rect_size / 2,
//                            rect_size, rect_size,
//                            D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
//  }
//  */
//}

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

void PopulateWhitelistedMonstersVicinityListbox( EntityList& entitylist ) {
  const auto& mover_entities = entitylist.GetMoverEntities();

  const auto vicinity_listbox_handle = GWH( LISTBOX_WHITELIST_NAMES_VICINITY );

  std::unordered_set<std::wstring> names_in_vicinity;

  for ( const auto& entity : mover_entities ) {
    if ( entity->IsMonster() ) {
      const auto entity_name = stringutils::AnsiToWide( entity->GetName() );

      names_in_vicinity.insert( entity_name );
    }
  }

  // We use async to exeucte the ui interaction code otherwise if we interact
  // with the ui from the game thread, it freezes for some reason
  std::async( [ = ]() {
    for ( const auto& name : names_in_vicinity ) {
      const bool exists_in_listbox =
          gwingui::listbox::FindString( vicinity_listbox_handle, 0, name ) !=
          LB_ERR;

      if ( !exists_in_listbox )
        gwingui::listbox::AddString( vicinity_listbox_handle, name );
    }
  } );
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

    /*
    if ( entity->IsPlayer() &&
         ( entity->GetName() == "Kypo" || entity->GetName() == "Sebi" ) ) {
      DrawEntity( *local_player_entity, *entity,
                  D3DCOLOR_RGBA( 0, 162, 232, 255 ) );
    }
    */

    if ( std::find( names_of_entities_to_draw.begin(),
                    names_of_entities_to_draw.end(),
                    entity->GetName() ) != names_of_entities_to_draw.end() ) {
      //if ( entity->IsPlayer() ) {
      //  DrawEntity( *local_player_entity, *entity,
      //              D3DCOLOR_RGBA( 0, 162, 232, 255 ) );
      //} else {
      DrawEntity( *local_player_entity, *entity,
                  D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
      //}
    } else if ( GetStarted() && entity->IsPlayer() ) {
      DrawEntity( *local_player_entity, *entity,
                  D3DCOLOR_RGBA( 0, 162, 232, 255 ) );
    }
  }

  // Every 300 ms do pop
  if ( ( timeGetTime() - last_gui_entity_population_ms_ ) > 300 ) {
    PopulateWhitelistedMonstersVicinityListbox( entity_list );

    last_gui_entity_population_ms_ = timeGetTime();
  }

  /*
  auto& common_entities = entity_list.GetCommonCtrlEntities();

  for ( auto& entity : common_entities )
    DrawEntity( local_player_entity, entity, D3DCOLOR_RGBA( 255, 0, 0, 255 ) );
  */

  //if ( GetStarted() ) {
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
  //}

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

POINT BotCore::GetSimulatedCursorPos() const {
  return simulated_cursor_pos_;
}

}  // namespace bot
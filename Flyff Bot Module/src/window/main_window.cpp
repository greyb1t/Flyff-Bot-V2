#include "pch.h"
#include "main_window.h"
#include "waiting_window.h"
#include "..\bot\bot_initializer.h"
#include "..\bot\entity_list.h"
#include "../utils/string_utls.h"
#include "../build_time.h"
#include "../utils/trace.h"

#include "gwinguiv2/controls/richedit.h"
#include "gwinguiv2/message_box.h"

#include "gwinmem/process_memory_internal.h"
#include "../bot/logging.h"

namespace bot {

void MainWindow::Initialize( const HWND window_handle ) {
  // std::wstring title = TEXT( "RIMJOB - RIghteously Mighty JOllygood Bot" );
  std::wstring title = TEXT( "RIMJOB" );
  gwingui::control::SetText( GetHandle(), title );

  const auto richedit_log_handle = GWH( RICHEDIT_LOG );
  gwingui::richedit::EnableTextChangedNotification( richedit_log_handle );

  const auto main_tabcontrol_handle = GWH( TAB_MAIN );

  AddTabWindow( main_tabcontrol_handle, DIALOG_TAB1, TEXT( "Bot Settings" ),
                new TabWindow1 );

  AddTabWindow( main_tabcontrol_handle, DIALOG_TAB2, TEXT( "Cheats" ),
                new TabWindow2 );

  AddTabWindow( main_tabcontrol_handle, DIALOG_TAB3, TEXT( "Events" ),
                new TabWindow3 );

  // Should no longer be needed, we'll see tho.
  // gWin::Tabcontrol::SetSelected( TAB_MAIN, 0 );

  std::string compilation_string( MAX_PATH, 0 );

  snprintf( &compilation_string[ 0 ], compilation_string.size(), "%s - %s",
            build_time::BuildDate, build_time::BuildTime );

  const auto compliation_date_handle = GWH( STATIC_COMPILATION_DATE );

  gwingui::control::SetText( compliation_date_handle,
                             stringutils::AnsiToWide( compilation_string ) );

  const auto richedit_feature_desc_handle = GWH( RICHEDIT_FEATURE_DESCRIPTION );
  gwingui::richedit::SetTextItalic( richedit_feature_desc_handle, true );

  // TODO: Replace SetTimer/KillTimer solution with another because the game
  // crashes when you inject the dll the second time to the game
  // NOTE: A better solution would be to just use GetTickCount in the Update()
  // loop
  /*
  if ( !SetTimer( window_handle, kBadboyTimerId, 1000, NULL ) ) {
    gwingui::messagebox::Error( TEXT( "Failed to initialize time." ) );
  } else {
    gwingui::messagebox::Error( std::to_wstring( ( uint32_t )window_handle ) );
  }
  */
}

void MainWindow::OnButtonClick( const HWND button_handle,
                                const uint32_t button_id ) {
  switch ( button_id ) {
    case BUTTON_APPLY_SETTINGS: {
      auto botcore = Initializer().GetBotCore();
      auto& bot_options = botcore->GetBotOptions();
      auto client = botcore->GetFlyffClient();
      auto local_player = client->CreateLocalPlayer();

      if ( local_player->IsDeletedOrInvalidMemory() ) {
        gwingui::messagebox::Error(
            TEXT( "Unable to apply the options, local player is invalid." ) );
        return;
      }

      bot_options.TryApplyOptions( local_player->GetName() );
    } break;

    case BUTTON_HIDE_WINDOW: {
      const auto botcore = Initializer().GetBotCore();
      const auto target_window = botcore->GetTargetWindow();

      if ( IsWindowVisible( target_window ) )
        ShowWindow( target_window, SW_HIDE );
      else
        SetWindowPos( target_window, 0, 0, 0, 0, 0,
                      SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );

      return;

      auto client = botcore->GetFlyffClient();
      auto local_player = client->CreateLocalPlayer();

      if ( local_player->IsDeletedOrInvalidMemory() ) {
        gwingui::messagebox::Error(
            TEXT( "Unable to view the stats, local player is invalid." ) );
        return;
      }

      /*
      bool new_val = !botcore->GetBladeDamageHack();

      if ( new_val )
        logging::Log( TEXT( "BLADE DAMAGE HACK - ON\n" ) );
      else
        logging::Log( TEXT( "BLADE DAMAGE HACK - OFF\n" ) );

      LogQueue().Notify();

      botcore->SetBladeDamageHack( new_val );

      return;
      */

      struct BaseStats {
        int32_t STR;
        int32_t STA;
        int32_t DEX;
        int32_t INT;
      };

      const uint32_t stats_offset = 0x6e4;

      auto selected_entity = local_player->GetSelectedEntity();

      /*
        auto test1 = selected_entity->GetFatigue();
        auto test2 = selected_entity->GetHealth();
        auto test3 = selected_entity->GetLevel();
        auto test4 = selected_entity->GetMana();
        auto test5 = selected_entity->GetMotion();
        auto test6 = selected_entity->GetName();
        auto test7 = selected_entity->GetSpeed();
        auto test8 = selected_entity->GetSpeedMultiplier();
        auto test9 = selected_entity->GetType();
      */

      /*
        const uint32_t visibility_offset = 0x19ec;
        gWin::ProcessInternal().Write(selected_entity->GetPointerAddress() +
        visibility_offset, 1024);

        logging::Log(TEXT("Make invisible nice!\n"));

        static UniquePtrEntity last_selected_entity;

        if (last_selected_entity) {
        logging::Log(TEXT("Made visible again\n"));
        gWin::ProcessInternal().Write(last_selected_entity->GetPointerAddress()
        + visibility_offset, 0);
        }

        last_selected_entity = std::move(selected_entity);

      */
      // TODO: Add a important log
      // TODO: Try changing the visibility of all object to shoot through them
      // Otherwise, try delete them like FlyFF Does SAFFE_DELETE

      // const uint32_t visiblity_offset = 0x19ec;
      //
      // gWin::ProcessInternal().Write(selected_entity->GetPointerAddress() +
      // visiblity_offset, 1024);

      // Ignite Flyff only registers and responds to keypresses when the window
      // is in focus

      // SetForegroundWindow(botcore->GetTargetWindow());

      BaseStats stats = gwinmem::CurrentProcess().Read<BaseStats>(
          selected_entity->GetPointerAddress() + stats_offset );

      const auto health_offset =
          client->GetClientVar( MemoryContants::kHealthOffset );

      int health =
          *( int* )( selected_entity->GetPointerAddress() + health_offset );
      int health2;
      memcpy( &health2,
              ( void* )( selected_entity->GetPointerAddress() + health_offset ),
              sizeof( health2 ) );

      gwingui::messagebox::Error(
          TEXT( "The selected user's base stats:\n" ) TEXT( "PtrAddress: " ) +
          std::to_wstring( selected_entity->GetPointerAddress() ) +
          TEXT( "\n" ) + TEXT( "STR: " ) + std::to_wstring( stats.STR ) +
          TEXT( "\n" ) + TEXT( "STA: " ) + std::to_wstring( stats.STA ) +
          TEXT( "\n" ) + TEXT( "DEX: " ) + std::to_wstring( stats.DEX ) +
          TEXT( "\n" ) + TEXT( "INT: " ) + std::to_wstring( stats.INT ) +
          TEXT( "\n" ) + TEXT( "HP: " ) +
          std::to_wstring( selected_entity->GetHealth() ) + TEXT( "\n" ) +
          TEXT( "HP (Cast): " ) + std::to_wstring( health ) + TEXT( "\n" ) +
          TEXT( "HP (Cast2): " ) + std::to_wstring( health2 ) + TEXT( "\n" ) +
          TEXT( "MP: " ) + std::to_wstring( selected_entity->GetMana() ) +
          TEXT( "\n" ) + TEXT( "FP: " ) +
          std::to_wstring( selected_entity->GetFatigue() ) + TEXT( "\n" ) );

      // selected_entity.GetPointerAddress();
      // int32_t shit = 70;
      // gWin::ProcessInternal().Write(selected_entity.GetPointerAddress() +
      // 0x760, shit);
    } break;

    case BUTTON_TOGGLEBOT: {
      const auto botcore = Initializer().GetBotCore();

      botcore->ToggleBot();
    } break;

    case BTN_TEST: {
      //       const auto botcore = Initializer().GetBotCore();
      //       const auto client = botcore->GetFlyffClient();
      //       const auto local_player = client->CreateLocalPlayer();
      //
      //       const auto selected_entity = local_player->GetSelectedEntity();
      //
      //       gwingui::messagebox::Warning(
      //           std::to_wstring( selected_entity->GetMotion() ) );

      static bool lol = []() -> bool {
        __try {
          *( int* )0 = 0;
        } __except ( 1 ) {
          MessageBox( 0, TEXT( "worked" ), TEXT( "wporked" ), 0 );
        }

        return true;
      }();

      try {
        throw std::exception( "hello, std::exception it worked" );
      } catch ( std::exception& e ) {
        MessageBoxA( 0, e.what(), e.what(), 0 );
      }

      const auto botcore = Initializer().GetBotCore();
      const auto client = botcore->GetFlyffClient();

      EntityList entity_list( client );
      auto& mover_entities = entity_list.GetMoverEntities();

      bool gm_found = false;
      std::string name;
      int32_t auth = 0;

      for ( const auto& entity : mover_entities ) {
        auth = gwinmem::CurrentProcess().Read<int32_t>(
            entity->GetPointerAddress() + 0x760 );

        if ( entity->GetName() == "Hope" || ( auth > 70 && auth <= 90 ) ) {
          gm_found = true;
          name = entity->GetName();
          break;
        }
      }

      if ( gm_found )
        gwingui::messagebox::Error( TEXT( "A GM WAS FOUND Auth: " ) +
                                    std::to_wstring( auth ) +
                                    stringutils::AnsiToWide( name ) );
      else
        gwingui::messagebox::Error(
            TEXT( "NOTHING WAS FOUND, GM NOT VISIBLE FUCK SHIT DAMN" ) );

      const auto address_window_handle = GWH( DIALOG_ADDRESSES );
      ShowWindow( address_window_handle, SW_SHOW );

      /*
        auto botcore = Initializer().GetBotCore();
        auto client = botcore->GetFlyffClient();
        auto entitylist = bot::entitylist::ReadEntityList(client);
        auto local_player = client->CreateLocalPlayer();

        auto selected_entity = local_player->GetSelectedEntity();

        if (selected_entity.IsMonster()) {
        selected_entity.SetPosition(local_player->GetPosition());
        }
      */

      // for (auto &entity : entitylist) {
      //  if (entity->IsMonster()) {
      //    if (entity->GetPosition() != local_player->GetPosition()) {
      //      auto entity_pos = entity->GetPosition();
      //      entity->SetPosition(local_player_position);
      //      break;
      //    }
      //  }
      //}

      // TODO: Test the idea I had with teleporting monster to me and back again
      // see if it crashes
    } break;

    default: {
      break;
    }
  }
}

INT_PTR CALLBACK MainWindow::DialogProc( HWND window_handle,
                                         UINT message,
                                         WPARAM wparam,
                                         LPARAM lparam ) {
  switch ( message ) {
      /*
      case WM_TIMER: {
        if ( wparam == 0x65 ) {
          // A timer event that goes off every 1000 ms, created by SetTimer
          auto botcore = Initializer().GetBotCore();
          botcore->SetIsDebuggerAttached( IsDebuggerAttached() );
        }
      } break;
        */

    case WM_CLOSE: {
      // TODO: Replace SetTimer/KillTimer solution with another because the game
      // crashes when you inject the dll the second time to the game
      /*
      if ( !KillTimer( window_handle, kBadboyTimerId ) ) {
        gwingui::messagebox::Error( TEXT( "killing badboy timer error." ) );
      } else {
        gwingui::messagebox::Error(
            std::to_wstring( ( uint32_t )window_handle ) );
      }
      */

      auto botcore = Initializer().GetBotCore();

      if ( botcore->GetStarted() ) {
        gwingui::messagebox::Warning(
            TEXT( "Please stop the bot before closing the bot." ) );
        return 1;
      }

      // WM_QUIT makes GetMessage return 0 and exit the message loop
      SendMessage( window_handle, WM_QUIT, 0, 0 );

      const auto local_player = botcore->GetFlyffClient()->CreateLocalPlayer();

      if ( local_player ) {
        if ( !local_player->IsDeletedOrInvalidMemory() ) {
          const auto name = local_player->GetName();

          if ( name != "" )
            botcore->GetBotOptions().SaveOptions( name );
        }
      }

      DestroyWindow( window_handle );
    } break;

    case WM_DESTROY: {
      PostQuitMessage( 0 );
    } break;

    default:
      break;
  }

  return FALSE;
}

void MainWindow::OnRichEditTextChanged( uint32_t ctrlId, CHANGENOTIFY* pCn ) {
  if ( ctrlId == RICHEDIT_LOG || ctrlId == RICHEDIT_LOG_IMPORTANT )
    gwingui::richedit::ScrollToBottom( GWH( ctrlId ) );
}

}  // namespace bot
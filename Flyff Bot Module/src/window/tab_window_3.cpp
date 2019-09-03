#include "pch.h"
#include "tab_window_3.h"
#include "../bot/bot_initializer.h"
#include "../bot/monster_definitions.h"
#include "../utils/string_utls.h"

#include "gwinguiv2/controls/editcontrol.h"
#include "gwinguiv2/message_box.h"

#include "gwinmem/process_memory_internal.h"

namespace bot {

void TabWindow3::OnButtonClick( const HWND button_handle,
                                const uint32_t button_id ) {
  switch ( button_id ) {
    case BUTTON_GET_PLAYER_DISGUISE_NAME: {
      auto func = [&]() {
        while ( is_disguised_thread_enabled_ ) {
          const auto botcore = Initializer().GetBotCore();
          const auto client = botcore->GetFlyffClient();

          static std::wstring last_monster_name = TEXT( "" );

          const auto local_player = client->CreateLocalPlayer();

          if ( local_player->IsDeletedOrInvalidMemory() ) {
            gwingui::messagebox::Error(
                TEXT( "Unable to use disguise finder, local player is invalid. "
                      "Stopping the thread." ) );
            return;
          }

          const auto selected_entity = local_player->GetSelectedEntity();

          // Disguise offset for Ignite Flyff only...
          const auto disguise_index_offset = 0x178;

          int disguise_index = gwinmem::CurrentProcess().Read<int>(
              selected_entity->GetPointerAddress() + disguise_index_offset );

          const std::wstring& monster_name = g_monster_names[ disguise_index ];

          if ( last_monster_name != monster_name ) {
            const auto editcontrol_player_disguise_name =
                GWH( EDIT_PLAYER_DISGUISE_NAME );

            gwingui::editcontrol::SetText( editcontrol_player_disguise_name,
                                           monster_name );

            const std::string monster_name_ansi =
                stringutils::WideToAnsi( monster_name );

            const size_t monster_name_length = monster_name.size() + 1;
            HGLOBAL hMem = GlobalAlloc( GMEM_MOVEABLE, monster_name_length );
            memcpy( GlobalLock( hMem ), monster_name_ansi.c_str(),
                    monster_name_length );
            GlobalUnlock( hMem );

            if ( !OpenClipboard( 0 ) )
              gwingui::messagebox::Error( TEXT( "OpenClipboard failed" ) );

            if ( !EmptyClipboard() )
              gwingui::messagebox::Error( TEXT( "EmptyClipboard failed" ) );

            if ( !SetClipboardData( CF_TEXT, hMem ) )
              gwingui::messagebox::Error( TEXT( "SetClipboardData failed" ) );

            CloseClipboard();

            last_monster_name = monster_name;
          }

          Sleep( 10 );
        }
      };

      const auto button_get_player_disguise_name =
          GWH( BUTTON_GET_PLAYER_DISGUISE_NAME );

      if ( !is_disguised_thread_enabled_ ) {
        is_disguised_thread_enabled_ = true;
        disguised_thread_ = std::thread( func );

        gwingui::control::SetText( button_get_player_disguise_name,
                                   TEXT( "Stop Get Name Thread" ) );
      } else {
        is_disguised_thread_enabled_ = false;
        disguised_thread_.join();

        gwingui::control::SetText( button_get_player_disguise_name,
                                   TEXT( "Start Get Name Thread" ) );
      }
    } break;

    default:
      break;
  }
}

}  // namespace bot
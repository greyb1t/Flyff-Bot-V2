#include "pch.h"
#include "tab_window_2.h"
#include "../bot/bot_initializer.h"

#include "gwinguiv2/message_box.h"
#include "gwinguiv2/controls/trackbar.h"
#include "gwinguiv2/controls/listbox.h"
#include "gwinguiv2/controls/editcontrol.h"

#include "../utils/string_utls.h"

namespace bot {

void TabWindow2::Initialize( const HWND window_handle ) {
  const auto slider_speed_handle = GWH( SLIDER_SPEED_MULTIPLIER );
  gwingui::trackbar::SetRange( slider_speed_handle, 10, 100 );
}

void TabWindow2::OnTrackbarSliderChanging( const HWND trackbar_handle,
                                           int32_t position ) {
  auto slider_id = gwingui::control::GetWindowId( trackbar_handle );

  switch ( slider_id ) {
    case SLIDER_SPEED_MULTIPLIER: {
      auto botcore = Initializer().GetBotCore();
      auto& bot_options = botcore->GetBotOptions();

      LocalPlayer local_player( botcore->GetFlyffClient() );

      const float multiplier = static_cast<float>( position ) / 10;

      local_player.SetSpeedMultiplier( multiplier );
      bot_options.SetSpeedMultiplier( multiplier );

      const auto static_speed_multiplier_handle =
          GWH( STATIC_SPEED_MULTIPLIER );

      gwingui::control::SetText( static_speed_multiplier_handle,
                                 std::to_wstring( multiplier ) );
    } break;

    default:
      break;
  }
}

void TabWindow2::OnButtonClick( const HWND button_handle,
                                const uint32_t button_id ) {
  switch ( button_id ) {
    // Default no collision is now obselete and has been replaced with a better
    // solution that is undetected on all servers with no effort
#if 0
    case CHECK_NO_COLLISION: {
      const auto botcore = Initializer().GetBotCore();
      auto& bot_options = botcore->GetBotOptions();
      const auto client = botcore->GetFlyffClient();
      const auto local_player = client->CreateLocalPlayer();

      if ( local_player->IsDeletedOrInvalidMemory() ) {
        gwingui::messagebox::Error(
            TEXT( "Unable to apply no collision, local player is invalid." ) );
        return;
      }

      const auto checkbox_handle = GWH( button_id );

      bool value = !gwingui::checkbox::IsChecked( checkbox_handle );
      bot_options.SetNoCollision( value );
    } break;
#endif

    case BUTTON_ADD_DRAW_NAME: {
      const auto text = gwingui::editcontrol::GetText( GWH( EDIT_DRAW_NAME ) );
      gwingui::listbox::AddString( GWH( LISTBOX_DRAW_NAMES ), text );
      // also update in botcore vector

      const auto botcore = Initializer().GetBotCore();

      botcore->names_of_entities_to_draw.push_back(
          stringutils::WideToAnsi( text ) );
    } break;

    case BUTTON_DRAW_FILL_NAME: {
      const auto botcore = Initializer().GetBotCore();
      const auto& client = botcore->GetFlyffClient();
      const auto local_player = client->CreateLocalPlayer();

      if ( local_player->IsDeletedOrInvalidMemory() ) {
        gwingui::messagebox::Error( TEXT(
            "Unable to fill the selected name, local player is invalid." ) );
        return;
      }

      const auto selected_entity = local_player->GetSelectedEntity();

      if ( !local_player->IsEntitySelected() ) {
        gwingui::messagebox::Error( TEXT( "Please select an entity." ) );
        return;
      }

      gwingui::editcontrol::SetText(
          GWH( EDIT_DRAW_NAME ),
          stringutils::AnsiToWide( selected_entity->GetName() ) );
    } break;

    case BUTTON_REMOVE_DRAW_NAME: {
      const auto index =
          gwingui::listbox::GetSelectedIndex( GWH( LISTBOX_DRAW_NAMES ) );

      if ( index == -1 ) {
        return;
      }

      const auto text =
          gwingui::listbox::GetText( GWH( LISTBOX_DRAW_NAMES ), index );

      gwingui::listbox::DeleteItem( GWH( LISTBOX_DRAW_NAMES ), index );

      const auto botcore = Initializer().GetBotCore();

      const auto to_remove =
          std::find( botcore->names_of_entities_to_draw.begin(),
                     botcore->names_of_entities_to_draw.end(),
                     stringutils::WideToAnsi( text ) );

      if ( to_remove == botcore->names_of_entities_to_draw.end() ) {
        gwingui::messagebox::Error(
            TEXT( "Did not exist in the internal cache?" ) );
        return;
      }

      botcore->names_of_entities_to_draw.erase( to_remove );
    } break;

    default:
      break;
  }
}

}  // namespace bot
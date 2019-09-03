#include "pch.h"
#include "tab_window_2.h"
#include "../bot/bot_initializer.h"

#include "gwinguiv2/message_box.h"
#include "gwinguiv2/controls/trackbar.h"

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

    default:
      break;
  }
}

}  // namespace bot
#include "pch.h"
#include "waiting_window.h"
#include "..\res\resource.h"

#include "gwinguiv2/controls/editcontrol.h"
#include "gwinguiv2/message_box.h"
#include "gwinguiv2/controls/control.h"

namespace bot {}

void bot::waiting_window::DisplayWaitingWindowAsync(
    const std::wstring& text,
    std::function<void()> func_to_wait_for ) {
  const auto waiting_window_handle = GWH( DIALOG_WAITING );

  auto wait = [=]() {
    // Display a child window saying fuck you, you have to wait
    const auto static_waiting_handle = GWH( STATIC_WAITING_TEXT );
    gwingui::editcontrol::SetText( static_waiting_handle, text );
    ShowWindow( waiting_window_handle, SW_SHOW );

    func_to_wait_for();

    // Hide the child window
    ShowWindow( waiting_window_handle, SW_HIDE );
  };

  // If the bot already is in waiting more, don't wait again.
  if ( !IsWindowVisible( waiting_window_handle ) ) {
    std::thread stop_thread( wait );
    stop_thread.detach();
  }
}

void bot::waiting_window::DisplayWaitingWindow(
    const std::wstring& text,
    std::function<void()> func_to_wait_for ) {
  const auto static_waiting_handle = GWH( STATIC_WAITING_TEXT );

  // Display a child window saying fuck you, you have to wait
  gwingui::editcontrol::SetText( static_waiting_handle, text );

  const auto waiting_window_handle = GWH( DIALOG_WAITING );

  ShowWindow( waiting_window_handle, SW_SHOW );

  func_to_wait_for();

  // Hide the child window
  ShowWindow( waiting_window_handle, SW_HIDE );
}

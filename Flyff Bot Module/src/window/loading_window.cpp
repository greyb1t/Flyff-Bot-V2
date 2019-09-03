#include "pch.h"
#include "loading_window.h"

#include "gwinguiv2/message_box.h"
#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/progressbar.h"

#include "../res/resource.h"

void bot::LoadingWindow::Initialize( const HWND window_handle ) {
  gwingui::control::SetText( window_handle, TEXT( "Loading..." ) );

  const auto progress_load_adresses_handle = GWH( PROGRESS_LOAD_ADDRESSES );

  gwingui::progressbar::SetRange( progress_load_adresses_handle, 0, 100 );

  gwingui::control::AddExtendedStyles( window_handle, WS_EX_TOPMOST );
  gwingui::control::UpdateControl( window_handle );
}

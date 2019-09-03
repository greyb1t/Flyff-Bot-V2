#pragma once

#include "gwinguiv2/window.h"

namespace bot {

class AddressesOffsetsWindow : public gwingui::Window {
 public:
  virtual void Initialize( const HWND window_handle ) override {
    gwingui::control::SetText( window_handle,
                               TEXT( "Addresses and offsets boi" ) );
  }

  INT_PTR CALLBACK DialogProc( HWND window_handle,
                               UINT message,
                               WPARAM wparam,
                               LPARAM lPlparamaram ) override {
    switch ( message ) {
      case WM_CLOSE: {
        ShowWindow( GetHandle(), SW_HIDE );

        // Since we are in a dialog window, we have to set the return value this
        // way
        gwingui::control::SetReturnValue( window_handle, 0 );

        // We processed the message
        return TRUE;
      } break;

      default:
        break;
    }

    // We did not process the message
    return FALSE;
  }
};

}  // namespace bot
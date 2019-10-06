#pragma once

#include "gwinguiv2/window.h"
#include "gwinguiv2/message_box.h"
#include "gwinguiv2/controls/control.h"

#include <string>

using namespace gwingui;

class ProcessSelectorWindow : public Window {
 public:
  virtual void Initialize( const HWND window_handle ) override;

  virtual void OnButtonClick( const HWND button_handle,
                              const uint32_t button_id ) override;
  virtual void OnListViewDoubleClick( NMITEMACTIVATE* item ) override;

  virtual INT_PTR CALLBACK DialogProc( HWND window_handle,
                                       UINT message,
                                       WPARAM wparam,
                                       LPARAM lparam ) {
    switch ( message ) {
#if 0
      case WM_SIZING: {
        const auto new_rect = reinterpret_cast<RECT*>( lparam );

        HWND handle = GetTopWindow( window_handle );

        RECT old_rect;
        GetWindowRect( window_handle, &old_rect );

        // For each control in this window
        while ( handle ) {
          auto size = gwingui::control::GetControlSize( handle );

          const auto delta_right = new_rect->right - old_rect.right;
          const auto delta_left = new_rect->left - old_rect.left;

          if ( delta_right ) {
            if ( delta_right > 0 ) {
              size.cx += delta_right;
            } else if ( delta_right < 0 ) {
              size.cx += delta_right;
            }
          } else if ( delta_left ) {
            if ( delta_left > 0 ) {
              size.cx -= delta_left;
            } else if ( delta_left < 0 ) {
              size.cx -= delta_left;
            }
          }

          gwingui::control::SetControlSize( handle, size );

          handle = GetNextWindow( handle, GW_HWNDNEXT );
        }

        return TRUE;
      } break;
#endif

      default:
        break;
    }

    return FALSE;
  }
};
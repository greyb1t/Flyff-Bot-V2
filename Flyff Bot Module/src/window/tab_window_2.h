#pragma once

#include "gwinguiv2/window.h"

namespace bot {

class TabWindow2 : public gwingui::Window {
 public:
  void Initialize( const HWND window_handle ) override;

  void OnTrackbarSliderChanging( const HWND trackbar_handle,
                                 int32_t position ) override;

  void OnButtonClick( const HWND button_handle,
                      const uint32_t button_id ) override;
};

}  // namespace bot
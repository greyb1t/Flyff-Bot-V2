#pragma once

#include "gwinguiv2/window.h"

namespace bot {

class LoadingWindow : public gwingui::Window {
 public:
  void Initialize( const HWND window_handle ) override;
};

}  // namespace bot
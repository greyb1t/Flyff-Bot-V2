#pragma once

#include "gwinguiv2/window.h"

namespace bot {

class TabWindow3 : public gwingui::Window {
 public:
  TabWindow3() : is_disguised_thread_enabled_( false ) {}

  void OnButtonClick( const HWND button_handle,
                      const uint32_t button_id ) override;

 private:
  std::thread disguised_thread_;
  bool is_disguised_thread_enabled_;
};

}  // namespace bot
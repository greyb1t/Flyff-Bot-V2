#pragma once

#include "..\window\tab_window_1.h"
#include "..\window\tab_window_2.h"
#include "..\window\tab_window_3.h"

namespace bot {

class Bot;

class MainWindow : public gwingui::Window {
 public:
  void Initialize( const HWND window_handle ) override;

  void OnButtonClick( const HWND button_handle,
                      const uint32_t button_id ) override;

  INT_PTR CALLBACK DialogProc( HWND window_handle,
                               UINT message,
                               WPARAM wparam,
                               LPARAM lparam ) override;

  void OnRichEditTextChanged( uint32_t ctrlId, CHANGENOTIFY* pCn ) override;

 private:
  std::unique_ptr<Bot> bot_mode_;
};

}  // namespace bot
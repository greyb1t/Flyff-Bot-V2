#pragma once

#include "gwinguiv2/window.h"

namespace bot {

class WaitingWindow : public gwingui::Window {
 public:
};

namespace waiting_window {

void DisplayWaitingWindowAsync( const std::wstring& text,
                                std::function<void()> func_to_wait_for );

void DisplayWaitingWindow( const std::wstring& text,
                           std::function<void()> func_to_wait_for );

}  // namespace waiting_window

}  // namespace bot
#pragma once

// class BotCore;

namespace simulation {

// void SendMouseClick(HWND window_handle, int x, int y);

// Will only work if the target window is in focus
// void SendKeypress(HWND window_handle, WORD key_code);

// Sends a message to the target window with a left click request, the delay_ms
// is sleeping between requests
void SendVirtualMouseClick( const HWND window_handle, const int delay_ms );

// Sends a message to the target window with a keypress request, the delay_ms is
// sleeping between requests
void SendVirtualKeypress( const HWND window_handle,
                          const uint16_t key_code,
                          const uint32_t delay_ms );
void SendVirtualKeyDown( const HWND window_handle, const uint16_t key_code );
void SendVirtualKeyUp( const HWND window_handle, const uint16_t key_code );

// void SendKeypress(HWND window_handle, WORD key_code);
// void SendMouseClick(HWND window_handle);
//
// void SimulateMouseClick(int x, int y);
// void SimulateKeypress(WORD key_code);
//
// void SimulateDownHardwareKeyPress(WORD key_code);
//
// void SimulateUpHardwareKeyPress(WORD key_code);
//
// void SimulateSingleHardwareKeyPress(WORD key_code, int delay);

}  // namespace simulation
#include "pch.h"
#include "simulation.h"

// #include "..\bot\botcore.h"

namespace simulation {

/*
  void SendMouseClick(HWND window_handle, int x, int y) {

  if (GetForegroundWindow() != window_handle)
    SetForegroundWindow(window_handle);

  mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x, y, 0, 0);
  mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
  mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
  }
*/

/*
  void SendKeypress(HWND window_handle, WORD key_code) {

  if (GetForegroundWindow() != window_handle)
    SetForegroundWindow(window_handle);

  INPUT input = { 0 };

  input.type = INPUT_KEYBOARD;

  // Hardware scancode instead of a virtual keycode, that makes it work.
  input.ki.dwFlags = 0;
  input.ki.wVk = key_code;

  SendInput(1, &input, sizeof(INPUT));

  Sleep(50);

  input.ki.dwFlags = KEYEVENTF_KEYUP;

  SendInput(1, &input, sizeof(INPUT));
  }
*/

void SendMouseDown( const HWND window_handle, const POINT& p ) {
  SendMessage( window_handle, WM_LBUTTONDOWN, 0, MAKELPARAM( p.x, p.y ) );
}

void SendMouseUp( const HWND window_handle, const POINT& p ) {
  SendMessage( window_handle, WM_LBUTTONUP, 0, MAKELPARAM( p.x, p.y ) );
}

void SendVirtualMouseClick( const HWND window_handle, const int delay_ms ) {
  SendMessage( window_handle, WM_LBUTTONDOWN, 0, 0 );
  Sleep( delay_ms );
  SendMessage( window_handle, WM_LBUTTONUP, 0, 0 );
}

void SendVirtualKeypress( const HWND window_handle,
                          const uint16_t key_code,
                          const uint32_t delay_ms ) {
  SendMessage( window_handle, WM_KEYDOWN, key_code, 0 );
}

void SendVirtualKeyDown( const HWND window_handle, const uint16_t key_code ) {
  LPARAM repeatCount = 1;
  LPARAM scanCode = MapVirtualKey( key_code, MAPVK_VK_TO_VSC );
  LPARAM extended = 0;  // fExtended (spy++)
  LPARAM context = 0;  // fAltDown (spy++)
  LPARAM previousState = 0;  // fRepeat (spy++)
  LPARAM transition = 0;  // fUp (spy++)

  LPARAM lParam = repeatCount | ( scanCode << 16 ) | ( extended << 24 ) |
                  ( context << 29 ) | ( previousState << 30 ) |
                  ( transition << 31 );

  PostMessage( window_handle, WM_KEYDOWN, key_code, lParam );
}

void SendVirtualKeyUp( const HWND window_handle, const uint16_t key_code ) {
  LPARAM repeatCount = 1;
  LPARAM scanCode = MapVirtualKey( key_code, MAPVK_VK_TO_VSC );
  LPARAM extended = 0;  // fExtended (spy++)
  LPARAM context = 0;  // fAltDown (spy++)
  LPARAM previousState = 1;  // fRepeat (spy++)
  LPARAM transition = 1;  // fUp (spy++)

  LPARAM lParam = repeatCount | ( scanCode << 16 ) | ( extended << 24 ) |
                  ( context << 29 ) | ( previousState << 30 ) |
                  ( transition << 31 );

  PostMessage( window_handle, WM_KEYUP, key_code, lParam );
}

// void SendKeypress(HWND window_handle, WORD key_code) {
//  SendMessage(window_handle, WM_KEYDOWN, key_code, 0);
//  Sleep(50);
//  SendMessage(window_handle, WM_KEYUP, key_code, 0);
//}
//
// void SendMouseClick(HWND window_handle) {
//  SendMessage(window_handle, WM_LBUTTONDOWN, 0, 0);
//  Sleep(50);
//  SendMessage(window_handle, WM_LBUTTONUP, 0, 0);
//}
//
// void SimulateMouseClick(int x, int y) {
//  mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x, y, 0, 0);
//  mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
//  mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
//}
//
// void SimulateKeypress(WORD key_code) {
//
//}
//
// void SimulateDownHardwareKeyPress(WORD key_code) {
//  INPUT input = { 0 };
//
//  input.type = INPUT_KEYBOARD;
//
//  // Hardware scancode instead of a virtual keycode, that makes it work.
//  input.ki.dwFlags = KEYEVENTF_SCANCODE;
//  input.ki.wScan = MapVirtualKey(key_code, MAPVK_VK_TO_VSC);
//
//  SendInput(1, &input, sizeof(INPUT));
//}
//
// void SimulateUpHardwareKeyPress(WORD key_code) {
//  INPUT input = { 0 };
//
//  input.type = INPUT_KEYBOARD;
//
//  // Hardware scancode instead of a virtual keycode, that makes it work.
//  input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
//  input.ki.wScan = MapVirtualKey(key_code, MAPVK_VK_TO_VSC);
//
//  SendInput(1, &input, sizeof(INPUT));
//}
//
// void SimulateSingleHardwareKeyPress(WORD key_code, int delay) {
//  INPUT input = { 0 };
//
//  input.type = INPUT_KEYBOARD;
//
//  // Hardware scancode instead of a virtual keycode, that makes it work.
//  input.ki.dwFlags = KEYEVENTF_SCANCODE;
//  input.ki.wScan = MapVirtualKey(key_code, MAPVK_VK_TO_VSC);
//
//  SendInput(1, &input, sizeof(INPUT));
//
//  Sleep(delay);
//
//  input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
//
//  SendInput(1, &input, sizeof(INPUT));
//}

}  // namespace simulation
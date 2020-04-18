#pragma once

#include "botcore.h"

#define ENABLE_PROTECTION 0

constexpr auto kBadboyTimerId = ( 0x65 );

namespace bot {

class BotInitializer {
 public:
  BotInitializer();
  ~BotInitializer() = default;

  int Load( HINSTANCE instance_handle, const uint32_t reserved_value );
  bool Unload( const HWND mainwindow_handle, const uintptr_t dllbase );

  void PostGuiCreation( const HWND loading_window_handle );

  // Hooks CopyRect and inside the hook it checks if the current thread
  // id is the same as the main window thread, if it is, it unregisters
  // exception handlers that was previously registered by the main window thread
  // if the thread id is not the same as the window thread id, then it tries
  // untill it is.
  bool UnregisterExceptionHandler( uint32_t thread_id_of_ex_handlers );

  static BOOL WINAPI CopyRectHooked( LPRECT destination, const RECT* source );
  static LRESULT WINAPI DefWindowProcAHooked( HWND window_handle,
                                              UINT message,
                                              WPARAM wparam,
                                              LPARAM lparam );
  static BOOL WINAPI PeekMessageAHooked( LPMSG message,
                                  HWND window_handle,
                                  UINT msg_filter_min,
                                  UINT msg_filter_max,
                                  UINT remove_msg );

  static BotInitializer& Instance();

  HINSTANCE GetInstance();
  BotCore* GetBotCore();

 private:
  HINSTANCE instance_handle_;
  BotCore botcore_;

  LPTOP_LEVEL_EXCEPTION_FILTER prev_filter_;

  std::atomic_bool has_removed_exception_handler_;

  uint32_t thread_id_of_ex_handlers_;

  decltype( CopyRect )* copy_rect_original_;
  decltype( DefWindowProcA )* def_window_proc_a_original_;
  decltype( PeekMessageA )* peek_message_a_original_;
};

inline BotInitializer& Initializer() {
  return BotInitializer::Instance();
}

}  // namespace bot
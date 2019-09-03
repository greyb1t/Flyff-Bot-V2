#pragma once

#include "botcore.h"

namespace bot {

class BotInitializer {
 public:
  BotInitializer();
  ~BotInitializer() = default;

  int Load( HINSTANCE instance_handle, const uint32_t reserved_value );
  bool Unload();

  void PostGuiCreation( const HWND loading_window_handle );

  bool InitializeHooks( LPDIRECT3DDEVICE9 device );

  // Hooks CopyRect and inside the hook it checks if the current thread
  // id is the same as the main window thread, if it is, it unregisters
  // exception handlers that was previously registered by the main window thread
  // if the thread id is not the same as the window thread id, then it tries
  // untill it is.
  bool UnregisterExceptionHandler( uint32_t thread_id_of_ex_handlers );

  static BOOL WINAPI CopyRectHooked( LPRECT destination, const RECT* source );

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
};

inline BotInitializer& Initializer() {
  return BotInitializer::Instance();
}

}  // namespace bot
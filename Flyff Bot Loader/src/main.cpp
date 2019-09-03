#include <Windows.h>

#include "process_selector_window.h"
#include "res/resource.h"

#include "gwinguiv2\gui.h"

#pragma comment( lib, "gwingui_v2.lib" )
#pragma comment( lib, "gwinmem.lib" )

int CALLBACK WinMain( HINSTANCE instance_handle,
                      HINSTANCE prev_instance_handle,
                      LPSTR cmd_line,
                      int cmd_show ) {
  gwingui::Gui gui( instance_handle );

  gui.AddWindow( DIALOG_MAIN, nullptr, new ProcessSelectorWindow );

  gui.Run();

  return 0;
}
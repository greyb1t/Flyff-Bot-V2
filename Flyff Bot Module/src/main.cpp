#include "pch.h"
#include "bot\bot_initializer.h"

#include "gwinmem/process_memory_internal.h"
#include "gwinguiv2/message_box.h"

#pragma comment( lib, "gwinmem.lib" )
#pragma comment( lib, "gwingui_v2.lib" )
#pragma comment( lib, "detours.lib" )

// Link to statically linked d3d9 libs
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9dt.lib" )

// Fixes the error unresolved external symbol (sock stuff) when using d3d9
#pragma comment( lib, "ws2_32.lib" )

// Fixes the error unresolved external symbol _sprintf when using d3d9
#pragma comment( lib, "legacy_stdio_definitions.lib" )

// Required for the mini crash dumps
#pragma comment( lib, "Dbghelp.lib" )

#pragma comment( lib, "urlmon.lib" )

uint32_t g_reserved = 0;

DWORD WINAPI WrappedDllMain( const HINSTANCE instance_handle ) {
  try {
    return bot::Initializer().Load( instance_handle, g_reserved );
  } catch ( ... ) {
    gwingui::messagebox::Error( TEXT( "Unhandled exception occured." ) );
    return -1;
  }
}

BOOL WINAPI DllMain( HINSTANCE instance_handle,
                     DWORD reason,
                     LPVOID reserved ) {
  switch ( reason ) {
    case DLL_PROCESS_ATTACH: {
      DisableThreadLibraryCalls( instance_handle );

      g_reserved = reinterpret_cast<uint32_t>( reserved );

      CreateThread( 0, 0,
                    reinterpret_cast<LPTHREAD_START_ROUTINE>( WrappedDllMain ),
                    instance_handle, 0, 0 );
    } break;

    case DLL_PROCESS_DETACH: {
    } break;

    default:
      break;
  }

  return TRUE;
}
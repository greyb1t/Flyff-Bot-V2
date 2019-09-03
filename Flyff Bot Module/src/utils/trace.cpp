#include "pch.h"
#include "trace.h"

#include <Windows.h>
#include <cstdarg>

void __Trace2( const char* format, ... ) {
  char buf[ 1000 ];

  va_list argptr;
  va_start( argptr, format );
  vsprintf_s( buf, format, argptr );
  va_end( argptr );

  OutputDebugStringA( buf );
}
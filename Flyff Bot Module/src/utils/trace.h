#pragma once

void __Trace2( const char* format, ... );

#define ENABLE_DEBUGGING 1
#define TO_CONSOLE 1

#if ENABLE_DEBUGGING
#if TO_CONSOLE
#define GWIN_TRACE printf
#else
#define GWIN_TRACE __Trace2
#endif
#else
#define GWIN_TRACE( s, ... )
#endif
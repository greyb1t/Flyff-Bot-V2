#pragma once

namespace crash_handler {

	LONG WINAPI MainExceptionHandlerBp( EXCEPTION_POINTERS* ex );
LONG WINAPI MainExceptionHandler( EXCEPTION_POINTERS* ex );

}
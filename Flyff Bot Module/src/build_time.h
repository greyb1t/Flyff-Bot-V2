#pragma once

/*
  We store these variable in a separate source file because we need the __TIME__
  to update each compilation.

  It has been added into the Pre-Build Event with the command:
  del
  "$(ProjectDir)intermediates\$(PlatformName)\$(Configuration)\build_time.obj"

  It deletes this source's obj file to force a recompilation of the file.
*/

#include <string>

namespace build_time {

extern const char BuildDate[];
extern const char BuildTime[];

}  // namespace build_time
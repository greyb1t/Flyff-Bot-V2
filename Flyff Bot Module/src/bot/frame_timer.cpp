#include "pch.h"
#include "frame_timer.h"
#include "botcore.h"

namespace bot {

FrameTimer::FrameTimer( BotCore* botcore, bool execute_immediately )
    : botcore_( botcore ) {
  if ( execute_immediately )
    last_frames_value_ = 0;
  else
    last_frames_value_ = botcore->GetFrameCount();
}

void FrameTimer::EachFrames( int64_t frame_interval,
                             std::function<void()> callback ) {
  const auto current_frame_count = botcore_->GetFrameCount();

  if ( abs( last_frames_value_ - current_frame_count ) > frame_interval ) {
    callback();

    last_frames_value_ = current_frame_count;
  }
}

}  // namespace bot
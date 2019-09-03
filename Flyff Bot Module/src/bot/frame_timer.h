#pragma once

namespace bot {

class BotCore;

class FrameTimer {
 public:
  FrameTimer( BotCore* botcore, bool execute_immediately );

  void EachFrames( int64_t frame_interval, std::function<void()> callback );

 private:
  int64_t last_frames_value_;
  BotCore* botcore_;
};

}  // namespace bot
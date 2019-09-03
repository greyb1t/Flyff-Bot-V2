#pragma once

#include <stdint.h>
#include <functional>
#include <Windows.h>

class TimerOnInterval {
 public:
  TimerOnInterval() {
    Reset();
  }

  void DoEachIntervalAfter(
      uint32_t ms_interval,
      std::function<void()> time_reached_callback,
      std::function<void()> time_not_reached_callback = 0 ) {
    if ( start_time_ms_ == 0 )
      start_time_ms_ = GetTickCount64();
    else {
      const int32_t ms_difference =
          static_cast<int32_t>( GetTickCount64() - start_time_ms_ );

      if ( std::abs( ms_difference ) >= static_cast<int32_t>( ms_interval ) ) {
        time_reached_callback();
        start_time_ms_ = 0;
      } else {
        if ( time_not_reached_callback )
          time_not_reached_callback();
      }
    }
  }

  void DoEachIntervalBefore(
      uint32_t ms_interval,
      std::function<void()> time_reached_callback,
      std::function<void()> time_not_reached_callback = 0 ) {
    const int32_t ms_difference =
        static_cast<int32_t>( GetTickCount64() - start_time_ms_ );

    if ( std::abs( ms_difference ) >= static_cast<int32_t>( ms_interval ) ) {
      time_reached_callback();
      start_time_ms_ = 0;
    } else {
      if ( time_not_reached_callback )
        time_not_reached_callback();
    }
  }

  void Reset() {
    start_time_ms_ = 0;
  }

  void SetStartTime( uint64_t time ) {
    start_time_ms_ = time;
  }

 private:
  uint64_t start_time_ms_;
};
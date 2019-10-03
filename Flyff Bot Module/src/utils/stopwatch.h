#pragma once

#include <Windows.h>
#include <string>

class Stopwatch {
 public:
  Stopwatch() : is_running_( false ) {
    QueryPerformanceFrequency( &frequency_ );
  }

  void Start() {
    QueryPerformanceCounter( &started_count_ );
    is_running_ = true;
  }

  void Stop() {
    QueryPerformanceCounter( &stopped_count_ );
    is_running_ = false;
  }

  double GetElapsedMilliseconds() const {
    LONGLONG count_to_use;

    // if it is running get the current count and not the stopped count
    if ( is_running_ ) {
      LARGE_INTEGER current_count;
      QueryPerformanceCounter( &current_count );
      count_to_use = current_count.QuadPart;
    } else {
      count_to_use = stopped_count_.QuadPart;
    }

    return ( count_to_use - started_count_.QuadPart ) * 1000.0 /
           frequency_.QuadPart;
  }

  double GetElapsedSeconds() const {
    return GetElapsedMilliseconds() / 1000;
  }

  double GetElapsedMinutes() const {
    return GetElapsedSeconds() / 60;
  }

  double GetElapsedHours() const {
    return GetElapsedMinutes() / 60;
  }

  std::wstring GetElapsedString() const {
    auto hours = GetElapsedHours();
    auto minutes = GetElapsedMinutes();
    auto seconds = GetElapsedSeconds();
    auto ms = GetElapsedMilliseconds();

    ms = std::floor( ms ) - ( std::floor( seconds ) * 1000 );
    seconds = std::floor( seconds ) - ( std::floor( minutes ) * 60 );
    minutes = std::floor( minutes ) - ( std::floor( hours ) * 60 );
    hours = std::floor( hours );

    std::wstring s =
        std::to_wstring( static_cast<int>( hours ) ) + TEXT( ":" ) +
        std::to_wstring( static_cast<int>( minutes ) ) + TEXT( ":" ) +
        std::to_wstring( static_cast<int>( seconds ) ) + TEXT( ":" ) +
        std::to_wstring( static_cast<int>( ms ) );

    return s;
  }

 private:
  LARGE_INTEGER frequency_;

  LARGE_INTEGER started_count_;
  LARGE_INTEGER stopped_count_;

  bool is_running_;
};

//class Stopwatch {
// public:
//  Stopwatch() : m_timeBegin{ 0 }, m_timeEnd{ 0 }, m_isRunning( false ) {
//    LARGE_INTEGER frequency;
//
//    if ( !QueryPerformanceFrequency( &frequency ) ) {
//      throw std::runtime_error(
//          "QueryPerformanceFrequency failed with error code: " +
//          std::to_string( GetLastError() ) );
//    }
//
//    frequency_ = ( double )frequency.QuadPart;
//  }
//
//  void Start() {
//    if ( !m_isRunning ) {
//      QueryPerformanceCounter( &m_timeBegin );
//      m_isRunning = true;
//    }
//  }
//
//  void Stop() {
//    if ( m_isRunning ) {
//      QueryPerformanceCounter( &m_timeEnd );
//      m_elapsedTime =
//          ( m_timeEnd.QuadPart - m_timeBegin.QuadPart ) * 1000.0 / frequency_;
//      m_isRunning = false;
//    }
//  }
//
//  void Restart() {
//    Reset();
//    Start();
//  }
//
//  void Reset() {
//    m_isRunning = false;
//    m_timeBegin.QuadPart = 0;
//    m_timeEnd.QuadPart = 0;
//    m_elapsedTime = 0;
//  }
//
//  double GetElapsedMilliseconds() {
//    if ( m_isRunning ) {
//      QueryPerformanceCounter( &m_timeEnd );
//      return ( m_timeEnd.QuadPart - m_timeBegin.QuadPart ) * 1000.0 /
//             frequency_;
//    }
//
//    return m_elapsedTime;
//  }
//
// private:
//  LARGE_INTEGER m_timeBegin;
//  LARGE_INTEGER m_timeEnd;
//  double frequency_;
//
//  double m_elapsedTime;
//  bool m_isRunning;
//};
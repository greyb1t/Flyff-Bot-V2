#pragma once

enum class LoggingType { kLoggingNormal, kLoggingImportant };

struct LoggingMessage {
  LoggingType type;
  std::wstring message;
};

class LoggerPump {
 public:
  LoggerPump();

  void AddMessagePumpQueue( const std::wstring& message );

  void AddMessageImportantPumpQueue( const std::wstring& message );

  // Notify the thread to "parse" all the pending messages
  void Notify();

  void Exit();

 private:
  void LogPumpThread();

  std::thread pump_thread_;
  std::mutex mutex_;
  std::queue<LoggingMessage> message_queue_;
  std::condition_variable cv_;
};

extern LoggerPump g_pump;

inline LoggerPump& LogQueue() {
  return g_pump;
}

namespace logging {

inline void Log( const std::wstring& s ) {
  LogQueue().AddMessagePumpQueue( s );

  /*
    std::thread thread([=]() {
    const auto t = time(0);
    struct tm time_now;
    localtime_s(&time_now, &t);

    gWin::Richedit::AppendText(RICHEDIT_LOG,
      TEXT("[") + std::to_wstring(time_now.tm_hour) + TEXT(":") +
      std::to_wstring(time_now.tm_min) +
      TEXT(":") + std::to_wstring(time_now.tm_sec) + TEXT("] ") + s);
    });

    thread.detach();
  */
}

inline void LogImportant( const std::wstring& s ) {
  LogQueue().AddMessageImportantPumpQueue( s );
  /*
    std::thread thread([=]() {
    const auto t = time(0);
    struct tm time_now;
    localtime_s(&time_now, &t);

    gWin::Richedit::AppendText(RICHEDIT_LOG_IMPORTANT,
      TEXT("[") + std::to_wstring(time_now.tm_hour) + TEXT(":") +
      std::to_wstring(time_now.tm_min) +
      TEXT(":") + std::to_wstring(time_now.tm_sec) + TEXT("] ") + s);
    });

    thread.detach();
  */
}

}  // namespace logging
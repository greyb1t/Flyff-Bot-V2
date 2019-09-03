#include "pch.h"
#include "logging.h"
#include "../res/resource.h"

#include "gwinguiv2/controls/richedit.h"
#include "gwinguiv2/controls/control.h"

LoggerPump::LoggerPump() {
  pump_thread_ = std::thread( &LoggerPump::LogPumpThread, this );
}

void LoggerPump::LogPumpThread() {
  while ( true ) {
    std::unique_lock<std::mutex> lk( mutex_ );

    while ( message_queue_.empty() )
      cv_.wait( lk );

    if ( message_queue_.empty() )
      continue;

    auto message = message_queue_.front();
    message_queue_.pop();

    const auto t = time( 0 );
    struct tm time_now;
    localtime_s( &time_now, &t );

    if ( message.message == TEXT( "exit" ) )
      return;

    switch ( message.type ) {
      case LoggingType::kLoggingNormal: {
        const auto richedit_log_handle = GWH( RICHEDIT_LOG );

        gwingui::richedit::AppendText(
            richedit_log_handle,
            TEXT( "[" ) + std::to_wstring( time_now.tm_hour ) + TEXT( ":" ) +
                std::to_wstring( time_now.tm_min ) + TEXT( ":" ) +
                std::to_wstring( time_now.tm_sec ) + TEXT( "] " ) +
                message.message );

        // Set the cursor to the last character in the log
        const auto buf_size =
            gwingui::richedit::GetTextBufferSize( richedit_log_handle );
        gwingui::richedit::SetSelection( richedit_log_handle, buf_size,
                                         buf_size );

        gwingui::richedit::ScrollToBottom( richedit_log_handle );
      } break;

      case LoggingType::kLoggingImportant: {
        const auto richedit_imp_log_handle = GWH( RICHEDIT_LOG_IMPORTANT );

        gwingui::richedit::AppendText(
            richedit_imp_log_handle,
            TEXT( "[" ) + std::to_wstring( time_now.tm_hour ) + TEXT( ":" ) +
                std::to_wstring( time_now.tm_min ) + TEXT( ":" ) +
                std::to_wstring( time_now.tm_sec ) + TEXT( "] " ) +
                message.message );

        // Set the cursor to the last character in the log
        const auto buf_size =
            gwingui::richedit::GetTextBufferSize( richedit_imp_log_handle );
        gwingui::richedit::SetSelection( richedit_imp_log_handle, buf_size,
                                         buf_size );

        gwingui::richedit::ScrollToBottom( richedit_imp_log_handle );
      } break;

      default:
        break;
    }
  }
}

void LoggerPump::AddMessagePumpQueue( const std::wstring& message ) {
  std::unique_lock<std::mutex> lk( mutex_ );
  message_queue_.push( { LoggingType::kLoggingNormal, message } );
}

void LoggerPump::AddMessageImportantPumpQueue( const std::wstring& message ) {
  std::unique_lock<std::mutex> lk( mutex_ );
  message_queue_.push( { LoggingType::kLoggingImportant, message } );
}

void LoggerPump::Notify() {
  cv_.notify_one();
}

void LoggerPump::Exit() {
  AddMessagePumpQueue( TEXT( "exit" ) );
  Notify();
  pump_thread_.join();
}
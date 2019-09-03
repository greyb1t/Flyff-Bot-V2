#include "pch.h"
#include "thread_pool.h"

ThreadPool2::ThreadPool2( int total_threads ) : stop_( false ) {
  for ( int i = 0; i < total_threads; ++i ) {
    detail::ThreadPoolThreadObject thread;

    thread.thread = std::thread( &ThreadPool2::ThreadLoop, this );
    thread.is_busy = false;

    thread_pool_map_.Add( thread.GetThreadId(), thread );
  }
}

ThreadPool2::~ThreadPool2() {
  {
    std::unique_lock<std::mutex> lock( queue_mutex_ );
    SignalTasksEnd();
  }
  queue_condition_.notify_all();

  // thread_pool_map_.JoinAll();
  thread_pool_map_.ForEachItem(
      []( std::pair<const int, detail::ThreadPoolThreadObject>& item ) {
        item.second.thread.join();
      } );
}

void ThreadPool2::PushTask( detail::TaskFunc task_func ) {
  task_queue_.Push( task_func );
  queue_condition_.notify_one();
}

void ThreadPool2::SignalTasksEnd() {
  stop_ = true;
}

void ThreadPool2::WaitTasks() {
#define WAIT return false
#define CONTINUE return true

  std::unique_lock<std::mutex> lock( wait_tasks_mutex_ );
  condition_wait_tasks_.wait( lock, [this]() -> bool {
    // thread_pool_map_.ForEachItem([](std::pair<const int,
    //  detail::ThreadPoolThreadObject> &item) {
    //  if (item.second.is_busy)
    //    WAIT;
    //});
    //
    // if (!task_queue_.IsEmpty())
    //  WAIT;
    //
    // CONTINUE;

    return !thread_pool_map_.IsAnyBusy() && task_queue_.IsEmpty();
  } );
}

void ThreadPool2::ThreadLoop() {
  while ( true ) {
    const int current_thread_id =
        detail::ThreadPoolThreadObject::GetCurrentThreadId();

    {
      std::unique_lock<std::mutex> lock( queue_mutex_ );

      queue_condition_.wait(
          lock, [this]() -> bool { return !task_queue_.IsEmpty() || stop_; } );

      if ( stop_ )
        return;
    }

    detail::TaskFunc task_function = task_queue_.Front();
    task_queue_.Pop();

    thread_pool_map_.SetIsBusy( current_thread_id, true );

    if ( task_function )
      task_function( &stop_ );

    thread_pool_map_.SetIsBusy( current_thread_id, false );

    condition_wait_tasks_.notify_one();
  }
}
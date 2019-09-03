#pragma once

namespace detail {

using TaskFunc = std::function<void( const bool* )>;

struct ThreadPoolThreadObject {
  bool is_busy;
  std::thread thread;

  int GetThreadId() const {
    std::wstringstream ss;
    ss << thread.get_id();
    return std::stoi( ss.str() );
  }

  static int GetCurrentThreadId() {
    std::wstringstream ss;
    ss << std::this_thread::get_id();
    return std::stoi( ss.str() );
  }
};

class ThreadMap {
 public:
  void Add( int thread_id, ThreadPoolThreadObject& object ) {
    std::unique_lock<std::mutex> lock( mutex_ );
    map_[ thread_id ] = std::move( object );
  }

  void SetIsBusy( int thread_id, bool is_busy ) {
    std::unique_lock<std::mutex> lock( mutex_ );
    map_[ thread_id ].is_busy = is_busy;
  }

  void SetThread( int thread_id, std::thread thread ) {
    std::unique_lock<std::mutex> lock( mutex_ );
    map_[ thread_id ].thread = std::move( thread );
  }

  ThreadPoolThreadObject& Get( int thread_id ) {
    std::unique_lock<std::mutex> lock( mutex_ );
    return map_[ thread_id ];
  }

  bool IsAnyBusy() {
    std::unique_lock<std::mutex> lock( mutex_ );

    for ( auto& thread : map_ ) {
      if ( thread.second.is_busy )
        return true;
    }

    return false;
  }

  void ForEachItem(
      std::function<void( std::pair<const int, ThreadPoolThreadObject>& )>
          callback ) {
    std::unique_lock<std::mutex> lock( mutex_ );

    for ( auto& item : map_ )
      callback( item );
  }

  void JoinAll() {
    std::unique_lock<std::mutex> lock( mutex_ );

    for ( auto& thread : map_ )
      thread.second.thread.join();
  }

  void GetItem( int thread_id,
                std::function<void( ThreadPoolThreadObject& )> callback ) {
    std::unique_lock<std::mutex> lock( mutex_ );
    callback( map_[ thread_id ] );
  }

  void SetItem( int thread_id,
                std::function<void( ThreadPoolThreadObject& )> callback ) {
    std::unique_lock<std::mutex> lock( mutex_ );
    callback( map_[ thread_id ] );
  }

 private:
  std::mutex mutex_;
  std::map<int, detail::ThreadPoolThreadObject> map_;
};

class ThreadPoolQueue {
 public:
  void Push( TaskFunc task ) {
    std::unique_lock<std::mutex> lock( mutex_ );
    queue_.push( task );
  }

  bool IsEmpty() {
    std::unique_lock<std::mutex> lock( mutex_ );
    return queue_.empty();
  }

  TaskFunc Front() {
    std::unique_lock<std::mutex> lock( mutex_ );

    if ( queue_.empty() )
      return 0;

    return queue_.front();
  }

  void Pop() {
    std::unique_lock<std::mutex> lock( mutex_ );

    if ( queue_.empty() )
      return;

    return queue_.pop();
  }

 private:
  std::mutex mutex_;
  std::queue<TaskFunc> queue_;
};

}  // namespace detail

class ThreadPool2 {
 public:
  explicit ThreadPool2( int total_threads );
  ~ThreadPool2();

  void PushTask( detail::TaskFunc task_func );

  void SignalTasksEnd();

  void WaitTasks();

 private:
  void ThreadLoop();

  bool stop_;

  detail::ThreadMap thread_pool_map_;
  detail::ThreadPoolQueue task_queue_;

  std::mutex queue_mutex_;
  std::condition_variable queue_condition_;

  std::mutex wait_tasks_mutex_;
  std::condition_variable condition_wait_tasks_;
};
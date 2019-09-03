#pragma once

#include <functional>

class DoOnce {
  public:
    DoOnce()
      : has_done_( false ) {

    }

    void Do( std::function<void()> callback ) {
      if ( !has_done_ ) {
        callback();
        has_done_ = true;
      }
    }

    void Reset() {
      has_done_ = false;
    }

    bool HasDone() const {
      return has_done_;
    }

  private:
    bool has_done_;
};
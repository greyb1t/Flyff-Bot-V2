#pragma once

enum class StateStatusReturnValue { kInProgress, kSucceeded, kFailed };

class StateMachine {
 public:
  StateMachine( int origin_state ) : current_state_( origin_state ) {}

  template <typename T>
  void SetNextState( T next_state ) {
    OnStateChanging();
    current_state_ = static_cast<int>( next_state );
  }

  template <typename T>
  T GetCurrentState() const {
    return static_cast<T>( current_state_ );
  }

  virtual void OnStateChanging() {}

 protected:
  int current_state_;
};
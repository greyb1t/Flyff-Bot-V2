#include "pch.h"
#include "key_sequence.h"
#include "simluation_state_machine.h"
#include "options/key_sequence_option.h"
#include "logging.h"

namespace bot {

KeySequence::KeySequence() {
  Reset();
}

KeySequenceReturnValues KeySequence::ExecuteSequence(
    const KeySequenceOption& key_sequence_options,
    SimulationStateMachine& simulation_machine ) {
  if ( key_sequence_options.Count() <= 0 )
    return KeySequenceReturnValues::kError;

  // is it that last key?
  if ( key_index_ >= key_sequence_options.Count() ) {
    int32_t ms_diff = static_cast<int32_t>( GetTickCount64() - last_time_ms_ );

    if ( std::abs( ms_diff ) >= next_wait_time_ ) {
      key_index_ = 0;
      return KeySequenceReturnValues::kFinishedSequence;
    }

    return KeySequenceReturnValues::kSequenceInProgress;
  }

  const auto current_key = key_sequence_options.GetAt( key_index_ );

  int32_t ms_diff = static_cast<int32_t>( GetTickCount64() - last_time_ms_ );

  // If the first key in the sequence and the sequence contains more than one
  // key, press immediately
  if ( key_index_ == 0 && key_sequence_options.Count() > 1 )
    ms_diff = next_wait_time_;

  if ( std::abs( ms_diff ) >= next_wait_time_ ) {
    const auto simulation_status =
        simulation_machine.KeyPress( current_key.key_code );

    switch ( simulation_status ) {
      case StateStatusReturnValue::kInProgress:
        break;

      case StateStatusReturnValue::kSucceeded: {
#if 0
        logging::Log( TEXT( "Executed Key: " ) + std::to_wstring( key_index_ ) +
                      TEXT( " - " ) + std::to_wstring( current_key.key_code ) +
                      TEXT( "\n" ) );
#endif

        last_time_ms_ = GetTickCount64();

        next_wait_time_ = current_key.time_ms_after_last_attack;

        ++key_index_;

        return KeySequenceReturnValues::kFinishedSingleKey;
      } break;

      case StateStatusReturnValue::kFailed:
        assert( false && "FUCKING SHIT FUCK OFF BLOW DICK" );
        break;

      default:
        break;
    }
  }

  return KeySequenceReturnValues::kSequenceInProgress;
}

void KeySequence::Reset() {
  begin_time_ms_ = GetTickCount64();
  last_time_ms_ = GetTickCount64();
  key_index_ = 0;
  next_wait_time_ = 0;
}

}  // namespace bot
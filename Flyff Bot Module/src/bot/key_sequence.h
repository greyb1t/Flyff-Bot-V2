#pragma once

namespace bot {

class SimulationStateMachine;
class KeySequenceOption;

enum class KeySequenceReturnValues {
  kSequenceInProgress,
  kFinishedSequence,
  kFinishedSingleKey,
  kError
};

class KeySequence {
 public:
  KeySequence();

  KeySequenceReturnValues ExecuteSequence(
      const KeySequenceOption& attack_sequence_options,
      SimulationStateMachine& simulation_machine );

  void Reset();

 private:
  uint64_t last_time_ms_;
  uint64_t begin_time_ms_;
  uint32_t key_index_;
  int32_t next_wait_time_;
};

}  // namespace bot
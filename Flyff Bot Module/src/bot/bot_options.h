#pragma once

#include "options/character_rebuff.h"

namespace bot {

enum class OptionType {
  kOptionTypeWhitelistedNames,
  kOptionTypeAttackSequence,
  kOptionTypeAutoHealingFood,
  kOptionTypeAutoHealingPills,
  kOptionTypeWhitelistedPlayerNames,
  kOptionTypeAverageYPositionTargetSelection,
  kOptionTypeRemoveAllObstacles,
  kOptionTypeBotMode,
  kOptionTypeLureTargetKeyCode,
  kOptionTypeCharacterRebuffList,
  kOptionTypeUpdateCharacterPos,
  kOptionTypeLevelArea
};

class AverageYPosOption;
class LevelAreaOption;
class AutoHealingPillsOption;
class CharacterRebuffListOption;
class WhitelistedPlayerNamesOption;
class AttackSequenceOption;
class LureTargetKeyOption;
class BotModeOption;
class WhitelistedNamesOption;
class AutoHealingOption;

class BotOptions {
 public:
  BotOptions();

  void SaveOptions( const std::string character_name );
  void LoadOptions( const std::string character_name );

  void SetOptionHasBeenChanged( bool value );
  bool HasOptionsBeenChanged();

  std::shared_ptr<WhitelistedNamesOption> GetWhitelistedNamesOption();
  std::shared_ptr<AttackSequenceOption> GetAttackSequenceOption();
  std::shared_ptr<AutoHealingOption> GetAutoHealingFoodOption();
  std::shared_ptr<AutoHealingPillsOption> GetAutoHealingPillsOption();
  std::shared_ptr<WhitelistedPlayerNamesOption>
  GetWhitelistedPlayerNamesOption();
  std::shared_ptr<CharacterRebuffListOption> GetRebuffSequenceListOption();
  std::shared_ptr<AverageYPosOption> GetAverageYPositionTargetSelectionOption();
  std::shared_ptr<Option> GetRemoveAllObstaclesOption();
  std::shared_ptr<Option> GetUpdateCharPosOption();
  std::shared_ptr<LevelAreaOption> GetLevelAreaOption();

  std::shared_ptr<BotModeOption> GetBotModeOption();
  std::shared_ptr<LureTargetKeyOption> GetLureKeyCodeOption();

  // Checks if any control event was occured by any controls that are required
  // to be applied
  void CheckOptionControlsChanged( uint32_t control_identifier );

  bool ApplyOptions( const std::string& character_name );

  bool ApplyBotMode();
  bool ApplyLureKey();
  bool ApplyAttackSequence();
  bool ApplyWhitelistedNames();
  bool ApplyAutoHealing();
  bool ApplyAutoHealingPills();
  bool ApplyWhitelistedPlayerNames();
  bool ApplyRebuffSequence();
  bool ApplyRebuffSequenceList();
  bool ApplyAverageYPostionTargetSelection();
  bool ApplyRemoveAllObstacles();
  bool ApplyUpdateCharPos();
  bool ApplyLevelArea();

  CharacterRebuffOption ReadCharacterRebuffOptions( uint32_t index );

  bool GetNoCollision();
  void SetNoCollision( bool value );

  float GetSpeedMultiplier();
  void SetSpeedMultiplier( float value );

 private:
  bool has_options_been_changed_;
  std::map<OptionType, std::shared_ptr<Option>> options_;

  bool no_collision_;
  float speed_multiplier_;
};

}  // namespace bot
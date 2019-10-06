#pragma once

#include "options/character_rebuff.h"
#include "options/character_rebuff_list.h"
#include "options/lure_target_key_option.h"
#include "options/update_char_pos_option.h"
#include "options/remove_all_obstacles_option.h"
#include "options/average_y_pos_option.h"
#include "options/whitelisted_names_option.h"
#include "options/whitelisted_player_names_option.h"
#include "options/attack_sequence_option.h"
#include "options/auto_healing_option.h"
#include "options/auto_healing_pills_option.h"
#include "options/level_area_option.h"
#include "options/bot_mode_option.h"

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

class BotOptions {
 public:
  BotOptions();

  void SaveOptions( const std::string& character_name );
  void LoadOptions( const std::string& character_name );

  void SetOptionHasBeenChanged( bool value );
  bool HasOptionsBeenChanged();

  template <typename T>
  T& GetOption();

  template <>
  WhitelistedNamesOption& GetOption<WhitelistedNamesOption>();

  template <>
  WhitelistedPlayerNamesOption& GetOption<WhitelistedPlayerNamesOption>();

  template <>
  AttackSequenceOption& GetOption<AttackSequenceOption>();

  template <>
  AutoHealingOption& GetOption<AutoHealingOption>();

  template <>
  AutoHealingPillsOption& GetOption<AutoHealingPillsOption>();

  template <>
  CharacterRebuffListOption& GetOption<CharacterRebuffListOption>();

  template <>
  AverageYPosOption& GetOption<AverageYPosOption>();

  template <>
  RemoveAllObstacleOption& GetOption<RemoveAllObstacleOption>();

  template <>
  UpdateCharPosOption& GetOption<UpdateCharPosOption>();

  template <>
  LevelAreaOption& GetOption<LevelAreaOption>();

  template <>
  BotModeOption& GetOption<BotModeOption>();

  template <>
  LureTargetKeyOption& GetOption<LureTargetKeyOption>();

  template <typename T>
  T& InternalGetOption( OptionType type ) {
    auto& option = options_[ type ];
    // In order for the static_cast to work, we cannot simply forward declare
    // the class, we need to include it because we use it
    T& option_ref = *( static_cast<T*>( option.get() ) );
    return option_ref;
  }

  // Checks if any control event was occured by any controls that are required
  // to be applied
  void CheckOptionControlsChanged( uint32_t control_identifier );

  bool TryApplyOptions( const std::string& character_name );

  float GetSpeedMultiplier();
  void SetSpeedMultiplier( float value );

 private:
  bool has_options_been_changed_;
  std::map<OptionType, std::unique_ptr<Option>> options_;

  float speed_multiplier_;
};

template <typename T>
T& BotOptions::GetOption() {
  static_assert( false, "Option requires specialization" );
}

template <>
WhitelistedNamesOption& BotOptions::GetOption<WhitelistedNamesOption>() {
  return InternalGetOption<WhitelistedNamesOption>(
      OptionType::kOptionTypeWhitelistedNames );
}

template <>
WhitelistedPlayerNamesOption&
BotOptions::GetOption<WhitelistedPlayerNamesOption>() {
  return InternalGetOption<WhitelistedPlayerNamesOption>(
      OptionType::kOptionTypeWhitelistedPlayerNames );
}

template <>
AttackSequenceOption& BotOptions::GetOption<AttackSequenceOption>() {
  return InternalGetOption<AttackSequenceOption>(
      OptionType::kOptionTypeAttackSequence );
}

template <>
AutoHealingOption& BotOptions::GetOption<AutoHealingOption>() {
  return InternalGetOption<AutoHealingOption>(
      OptionType::kOptionTypeAutoHealingFood );
}

template <>
AutoHealingPillsOption& BotOptions::GetOption<AutoHealingPillsOption>() {
  return InternalGetOption<AutoHealingPillsOption>(
      OptionType::kOptionTypeAutoHealingPills );
}

template <>
CharacterRebuffListOption& BotOptions::GetOption() {
  return InternalGetOption<CharacterRebuffListOption>(
      OptionType::kOptionTypeCharacterRebuffList );
}

template <>
AverageYPosOption& BotOptions::GetOption<AverageYPosOption>() {
  return InternalGetOption<AverageYPosOption>(
      OptionType::kOptionTypeAverageYPositionTargetSelection );
}

template <>
RemoveAllObstacleOption& BotOptions::GetOption<RemoveAllObstacleOption>() {
  return InternalGetOption<RemoveAllObstacleOption>(
      OptionType::kOptionTypeRemoveAllObstacles );
}

template <>
UpdateCharPosOption& BotOptions::GetOption<UpdateCharPosOption>() {
  return InternalGetOption<UpdateCharPosOption>(
      OptionType::kOptionTypeUpdateCharacterPos );
}

template <>
LevelAreaOption& BotOptions::GetOption<LevelAreaOption>() {
  return InternalGetOption<LevelAreaOption>( OptionType::kOptionTypeLevelArea );
}

template <>
BotModeOption& BotOptions::GetOption<BotModeOption>() {
  return InternalGetOption<BotModeOption>( OptionType::kOptionTypeBotMode );
}

template <>
LureTargetKeyOption& BotOptions::GetOption<LureTargetKeyOption>() {
  return InternalGetOption<LureTargetKeyOption>(
      OptionType::kOptionTypeLureTargetKeyCode );
}

}  // namespace bot
#include "pch.h"
#include "bot_options.h"

#include "gwinguiv2/message_box.h"
#include "gwinguiv2/controls/listview.h"
#include "gwinguiv2/controls/listbox.h"
#include "gwinguiv2/controls/editcontrol.h"
#include "gwinguiv2/controls/control.h"

#include "gwinmem/utils/safe_handle.h"

#include "options/average_y_pos_option.h"
#include "options/level_area_option.h"
#include "options/auto_healing_pills_option.h"
#include "options/character_rebuff_list.h"
#include "options/whitelisted_player_names_option.h"
#include "options/attack_sequence_option.h"
#include "options/lure_target_key_option.h"
#include "options/bot_mode_option.h"

#include "../utils/string_utls.h"

using namespace nlohmann;

namespace bot {

BotOptions::BotOptions()
    : has_options_been_changed_( false ),
      speed_multiplier_( 0 ),
      no_collision_( false ) {
  options_[ OptionType::kOptionTypeWhitelistedNames ] =
      std::make_shared<WhitelistedNamesOption>( "whitelisted_names",
                                                CHECK_WHITELIST_NAMES );
  options_[ OptionType::kOptionTypeAttackSequence ] =
      std::make_shared<AttackSequenceOption>( "attack_sequence",
                                              CHECK_ATTACK_SEQUENCE );
  options_[ OptionType::kOptionTypeAutoHealingFood ] =
      std::make_shared<AutoHealingOption>( "auto_healing",
                                           CHECK_AUTO_HEALTH_FOOD );
  options_[ OptionType::kOptionTypeWhitelistedPlayerNames ] =
      std::make_shared<WhitelistedPlayerNamesOption>(
          "whitelisted_player_names", CHECK_WHITELIST_PLAYER_NAMES );
  options_[ OptionType::kOptionTypeAverageYPositionTargetSelection ] =
      std::make_shared<AverageYPosOption>(
          "average_y_position_target_selection", CHECK_AVERAGE_Y_POSITION );
  options_[ OptionType::kOptionTypeRemoveAllObstacles ] =
      std::make_shared<Option>( "remove_all_obstacles",
                                CHECK_REMOVE_ALL_OBSTACLES );
  options_[ OptionType::kOptionTypeBotMode ] =
      std::make_shared<BotModeOption>( "botmode", 0 );
  options_[ OptionType::kOptionTypeLureTargetKeyCode ] =
      std::make_shared<LureTargetKeyOption>( "lure_target_key_code", 0 );
  options_[ OptionType::kOptionTypeCharacterRebuffList ] =
      std::make_shared<CharacterRebuffListOption>( "character_rebuff_list",
                                                   CHECK_REBUFF_SEQUENCES );
  options_[ OptionType::kOptionTypeAutoHealingPills ] =
      std::make_shared<AutoHealingPillsOption>( "auto_healing_pills",
                                                CHECK_AUTO_HEALTH_PILLS );
  options_[ OptionType::kOptionTypeUpdateCharacterPos ] =
      std::make_shared<Option>( "update_char_pos", CHECK_UPDATE_POS_EACH_KILL );

  options_[ OptionType::kOptionTypeLevelArea ] =
      std::make_shared<LevelAreaOption>( "level_area", CHECK_LEVEL_AREA );
}

bool WriteTextFile( const std::wstring& filename_dir,
                    const std::string& text ) {
  gwinmem::SafeHandle file_handle =
      CreateFile( filename_dir.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                  FILE_ATTRIBUTE_NORMAL, NULL );

  const auto bytes_to_write = text.size();
  DWORD bytes_written = 0;

  if ( !WriteFile( file_handle.GetValue(), &text[ 0 ], bytes_to_write,
                   &bytes_written, NULL ) )
    return false;

  if ( bytes_written != bytes_to_write )
    return false;

  return true;
}

void BotOptions::SaveOptions( const std::string character_name ) {
  for ( auto c : character_name ) {
    if ( !isascii( c ) ) {
      gwingui::messagebox::Error(
          TEXT( "Invalid character name, not saving options." ) );
      return;
    }
  }

  json json_parent;

  for ( auto& option : options_ ) {
    auto json_child = option.second->GetJson();

    auto s1 = json_child.dump( 4 );

    json_parent[ option.second->GetName() ] = json_child;

    auto s2 = json_parent.dump( 4 );
    s2 = "";
  }

  const auto greyb1t_directory = optionutils::GetGreyb1tDirectory();

  CreateDirectory( greyb1t_directory.c_str(), 0 );

  const auto options_directory = optionutils::GetOptionsDirectory();

  CreateDirectory( options_directory.c_str(), 0 );

  std::string json_dump = json_parent.dump( 4 );

  bool ret = WriteTextFile( options_directory + TEXT( "\\options-" ) +
                                stringutils::AnsiToWide( character_name ) +
                                TEXT( ".rimjob" ),
                            json_dump );

  if ( !ret )
    gwingui::messagebox::Error( TEXT( "Unable to save the options." ) );
}

std::vector<std::wstring> GetFilesInDirectory( const std::wstring& dir ) {
  WIN32_FIND_DATA find_data;
  HANDLE file_handle =
      FindFirstFile( ( dir + TEXT( "\\*" ) ).c_str(), &find_data );

  std::vector<std::wstring> files;

  do {
    files.push_back( find_data.cFileName );
  } while ( FindNextFile( file_handle, &find_data ) );

  return files;
}

bool ReadTextFile( const std::wstring& filename, std::string* text ) {
  std::ifstream file( filename );

  if ( !file.is_open() )
    return false;

  std::string line;

  while ( std::getline( file, line ) )
    *text += line;

  return true;
}

void BotOptions::LoadOptions( const std::string character_name ) {
  const auto options_directory = optionutils::GetOptionsDirectory();

  std::vector<std::wstring> files = GetFilesInDirectory( options_directory );

  std::wstring correct_options_name = TEXT( "" );

  for ( auto& file : files ) {
    // did it find it?
    if ( file.find( stringutils::AnsiToWide( character_name ) ) !=
         std::wstring::npos ) {
      correct_options_name = file;
      break;
    }
  }

  // is there a option file?
  if ( correct_options_name == TEXT( "" ) )
    return;

  std::wstring options_file_directory =
      options_directory + TEXT( "\\" ) + correct_options_name;

  std::string options_file_buffer;
  bool ret = ReadTextFile( options_file_directory, &options_file_buffer );

  if ( !ret ) {
    gwingui::messagebox::Error(
        TEXT( "Failed to read the saved options. You have to be logged in when "
              "starting the bot in order to load the correct options "
              "automatically." ) );
    return;
  }

  const auto j = json::parse( options_file_buffer );

  for ( auto& option : options_ ) {
    option.second->LoadJson( j );
    option.second->RefreshControls();
  }

  SetOptionHasBeenChanged( false );

  // TODO: The options are loaded correctly, now i need to make it so the
  // controls changes their values to the options also check if the values are
  // read in the wrong order and shit.
}

void BotOptions::SetOptionHasBeenChanged( bool value ) {
  // TODO: Get combo box events and change this value and enabled the apply key
  // if has been changed
  has_options_been_changed_ = value;

  const auto button_apply_settings_handle = GWH( BUTTON_APPLY_SETTINGS );

  gwingui::control::EnableOrDisable( button_apply_settings_handle, value );
}

bool BotOptions::HasOptionsBeenChanged() {
  return has_options_been_changed_;
}

std::shared_ptr<WhitelistedNamesOption>
BotOptions::GetWhitelistedNamesOption() {
  return std::static_pointer_cast<WhitelistedNamesOption>(
      options_[ OptionType::kOptionTypeWhitelistedNames ] );
}

std::shared_ptr<AttackSequenceOption> BotOptions::GetAttackSequenceOption() {
  return std::static_pointer_cast<AttackSequenceOption>(
      options_[ OptionType::kOptionTypeAttackSequence ] );
}

std::shared_ptr<AutoHealingOption> BotOptions::GetAutoHealingFoodOption() {
  return std::static_pointer_cast<AutoHealingOption>(
      options_[ OptionType::kOptionTypeAutoHealingFood ] );
}

std::shared_ptr<AutoHealingPillsOption>
BotOptions::GetAutoHealingPillsOption() {
  return std::static_pointer_cast<AutoHealingPillsOption>(
      options_[ OptionType::kOptionTypeAutoHealingPills ] );
}

std::shared_ptr<WhitelistedPlayerNamesOption>
BotOptions::GetWhitelistedPlayerNamesOption() {
  return std::static_pointer_cast<WhitelistedPlayerNamesOption>(
      options_[ OptionType::kOptionTypeWhitelistedPlayerNames ] );
}

std::shared_ptr<CharacterRebuffListOption>
BotOptions::GetRebuffSequenceListOption() {
  return std::static_pointer_cast<CharacterRebuffListOption>(
      options_[ OptionType::kOptionTypeCharacterRebuffList ] );
}

std::shared_ptr<AverageYPosOption>
BotOptions::GetAverageYPositionTargetSelectionOption() {
  return std::static_pointer_cast<AverageYPosOption>(
      options_[ OptionType::kOptionTypeAverageYPositionTargetSelection ] );
}

std::shared_ptr<Option> BotOptions::GetRemoveAllObstaclesOption() {
  return std::static_pointer_cast<Option>(
      options_[ OptionType::kOptionTypeRemoveAllObstacles ] );
}

std::shared_ptr<Option> BotOptions::GetUpdateCharPosOption() {
  return std::static_pointer_cast<Option>(
      options_[ OptionType::kOptionTypeUpdateCharacterPos ] );
}

std::shared_ptr<LevelAreaOption> BotOptions::GetLevelAreaOption() {
  return std::static_pointer_cast<LevelAreaOption>(
      options_[ OptionType::kOptionTypeLevelArea ] );
}

std::shared_ptr<BotModeOption> BotOptions::GetBotModeOption() {
  return std::static_pointer_cast<BotModeOption>(
      options_[ OptionType::kOptionTypeBotMode ] );
}

std::shared_ptr<LureTargetKeyOption> BotOptions::GetLureKeyCodeOption() {
  return std::static_pointer_cast<LureTargetKeyOption>(
      options_[ OptionType::kOptionTypeLureTargetKeyCode ] );
}

void BotOptions::CheckOptionControlsChanged( uint32_t control_identifier ) {
  switch ( control_identifier ) {
    case EDIT_LEVEL_AREA_ORIGIN_POS_X:
    case EDIT_LEVEL_AREA_ORIGIN_POS_Z:
    case EDIT_LEVEL_AREA_LENGTH1:
    case EDIT_LEVEL_AREA_LENGTH2:
    case CHECK_LEVEL_AREA:
    case CHECK_UPDATE_POS_EACH_KILL:
    case CHECK_AUTO_HEALTH_PILLS:
    case COMBO_AUTO_HEALTH_PILLS_KEY:
    case EDIT_AUTO_PILLS_HP_LIMIT:
    case LISTBOX_REBUFF_SEQUENCES:
    case CHECK_REBUFF_SEQUENCES:
    case CHECK_REMOVE_ALL_OBSTACLES:
    case CHECK_AVERAGE_Y_POSITION:
    case LISTVIEW_REBUFF_KEY_SEQUENCE:
    case EDIT_REBUFF_SEQUENCE_INTERVAL:
    case EDIT_AUTO_FOOD_HP_LIMIT:
    case CHECK_WHITELIST_PLAYER_NAMES:
    case LISTBOX_WHITELIST_PLAYER_NAMES:
    case EDIT_WHITELISTED_PLAYER_NAME:
    case BUTTON_ADD_WHITELISTED_PLAYER_NAME:
    case BUTTON_WHITELIST_PLAYER_NAME_REMOVE:
    case COMBO_AUTO_HEALTH_FOOD_KEY:
    case CHECK_AUTO_HEALTH_FOOD:
    case CHECK_ATTACK_SEQUENCE:
    case CHECK_WHITELIST_NAMES:
    case BUTTON_WHITELIST_NAME_REMOVE:
    case BUTTON_ADD_WHITELISTED_NAME:
    case COMBO_BOT_MODE:
    case COMBO_TARGET_LURE_KEY:
    case LISTVIEW_ATTACKS: {
      SetOptionHasBeenChanged( true );
    } break;

    default:
      break;
  }
}

bool BotOptions::ApplyOptions( const std::string& character_name ) {
  using MemberFunctionPointer = decltype( &BotOptions::ApplyLureKey );

  std::vector<MemberFunctionPointer> apply_functions = {
    &BotOptions::ApplyBotMode,
    &BotOptions::ApplyLureKey,
    &BotOptions::ApplyAttackSequence,
    &BotOptions::ApplyWhitelistedNames,
    &BotOptions::ApplyAutoHealing,
    &BotOptions::ApplyWhitelistedPlayerNames,
    &BotOptions::ApplyRebuffSequence,
    &BotOptions::ApplyRebuffSequenceList,
    &BotOptions::ApplyAverageYPostionTargetSelection,
    &BotOptions::ApplyRemoveAllObstacles,
    &BotOptions::ApplyAutoHealingPills,
    &BotOptions::ApplyUpdateCharPos,
    &BotOptions::ApplyLevelArea,
  };

  for ( auto appl : apply_functions ) {
    const auto ret = ( this->*( appl ) )();

    if ( !ret )
      return false;
  }

  SaveOptions( character_name );

  SetOptionHasBeenChanged( false );

  return true;
}

bool BotOptions::ApplyBotMode() {
  const auto combo_bot_mode = GWH( COMBO_BOT_MODE );

  const auto bot_mode_index =
      gwingui::combobox::GetSelectedIndex( combo_bot_mode );

  if ( bot_mode_index == -1 ) {
    gwingui::messagebox::Error( TEXT( "You have to choose the bot mode." ) );
    return false;
  }

  auto bot_mode_option = GetBotModeOption();

  const auto bot_mode_str =
      gwingui::combobox::GetString( combo_bot_mode, bot_mode_index );

  if ( bot_mode_str == TEXT( "OneVsOne" ) )
    bot_mode_option->SetBotMode( BotMode::kBotModeOneVsOne );
  else
    bot_mode_option->SetBotMode( BotMode::kBotModeNone );

  return true;
}

bool BotOptions::ApplyLureKey() {
  const auto combo_target_lure_key = GWH( COMBO_TARGET_LURE_KEY );

  const auto lure_key_index =
      gwingui::combobox::GetSelectedIndex( combo_target_lure_key );

  if ( lure_key_index == -1 ) {
    gwingui::messagebox::Error(
        TEXT( "You have to choose a key to lure the target." ) );
    return false;
  }

  const auto lure_key_str =
      gwingui::combobox::GetString( combo_target_lure_key, lure_key_index );
  const auto lure_key = optionutils::AsciiKeyToVirtualKeyCode( lure_key_str );

  auto lure_key_code = GetLureKeyCodeOption();

  lure_key_code->SetLureKeyCode( lure_key );

  return true;
}

bool BotOptions::ApplyAttackSequence() {
  auto attack_sequence_option = GetAttackSequenceOption();

  attack_sequence_option->Clear();

  const auto checkbox_attack_sequence = GWH( CHECK_ATTACK_SEQUENCE );

  if ( gwingui::checkbox::IsChecked( checkbox_attack_sequence ) ) {
    attack_sequence_option->SetStatus( true );

    const auto listview_attacks = GWH( LISTVIEW_ATTACKS );

    for ( int i = 0,
              count = gwingui::listview::GetItemCount( listview_attacks );
          i < count; ++i ) {
      const auto row = gwingui::listview::GetItemRowText( listview_attacks, i );

      Key key;

      key.key_code = optionutils::AsciiKeyToVirtualKeyCode( row[ 0 ] );
      key.time_ms_after_last_attack = std::stoi( row[ 1 ] );

      attack_sequence_option->AddValue( key );
    }
  } else
    attack_sequence_option->SetStatus( false );

  return true;
}

bool BotOptions::ApplyWhitelistedNames() {
  auto whitelisted_names = GetWhitelistedNamesOption();

  whitelisted_names->Clear();

  const auto checkbox_whitelist_names = GWH( CHECK_WHITELIST_NAMES );

  if ( gwingui::checkbox::IsChecked( checkbox_whitelist_names ) ) {
    whitelisted_names->SetStatus( true );

    const auto listbox_whitelist_names = GWH( LISTBOX_WHITELIST_NAMES );

    for ( int i = 0,
              count = gwingui::listbox::GetCount( listbox_whitelist_names );
          i < count; ++i ) {
      const std::wstring name =
          gwingui::listbox::GetText( listbox_whitelist_names, i );
      whitelisted_names->AddValue( stringutils::WideToAnsi(name) );
    }
  } else
    whitelisted_names->SetStatus( false );

  return true;
}

bool BotOptions::ApplyAutoHealing() {
  auto auto_healing_option = GetAutoHealingFoodOption();

  const auto checkbox_auto_health_food = GWH( CHECK_AUTO_HEALTH_FOOD );

  if ( gwingui::checkbox::IsChecked( checkbox_auto_health_food ) ) {
    auto_healing_option->SetStatus( true );

    const auto combobox_auto_health_food_key =
        GWH( COMBO_AUTO_HEALTH_FOOD_KEY );

    const auto heal_key_index =
        gwingui::combobox::GetSelectedIndex( combobox_auto_health_food_key );

    if ( heal_key_index == -1 ) {
      gwingui::messagebox::Error(
          TEXT( "You have to choose a key to use the food." ) );
      return false;
    }

    const auto heal_key_str = gwingui::combobox::GetString(
        combobox_auto_health_food_key, heal_key_index );
    const auto heal_key = optionutils::AsciiKeyToVirtualKeyCode( heal_key_str );

    auto_healing_option->SetHealKeyCode( heal_key );

    const auto editcontrol_auto_food_hp_limit = GWH( EDIT_AUTO_FOOD_HP_LIMIT );
    auto health_limit =
        gwingui::editcontrol::GetInt( editcontrol_auto_food_hp_limit );

    if ( !health_limit ) {
      gwingui::messagebox::Error( TEXT( "You enter the health limit." ) );
      return false;
    }

    auto_healing_option->SetHealthLimit( health_limit );
  } else
    auto_healing_option->SetStatus( false );

  return true;
}

bool BotOptions::ApplyAutoHealingPills() {
  auto auto_healing_pills_option = GetAutoHealingPillsOption();

  const auto checkbox_auto_health_pills = GWH( CHECK_AUTO_HEALTH_PILLS );

  if ( gwingui::checkbox::IsChecked( checkbox_auto_health_pills ) ) {
    auto_healing_pills_option->SetStatus( true );

    const auto combo_auto_health_pills_key = GWH( COMBO_AUTO_HEALTH_PILLS_KEY );

    const auto heal_key_index =
        gwingui::combobox::GetSelectedIndex( combo_auto_health_pills_key );

    if ( heal_key_index == -1 ) {
      gwingui::messagebox::Error(
          TEXT( "You have to choose a key to use the pills." ) );
      return false;
    }

    const auto heal_key_str = gwingui::combobox::GetString(
        combo_auto_health_pills_key, heal_key_index );
    const auto heal_key = optionutils::AsciiKeyToVirtualKeyCode( heal_key_str );

    auto_healing_pills_option->SetHealKeyCode( heal_key );

    const auto editcontrol_auto_pills_hp_limit =
        GWH( EDIT_AUTO_PILLS_HP_LIMIT );
    auto health_limit =
        gwingui::editcontrol::GetInt( editcontrol_auto_pills_hp_limit );

    if ( !health_limit ) {
      gwingui::messagebox::Error( TEXT( "You enter the health limit." ) );
      return false;
    }

    auto_healing_pills_option->SetHealthLimit( health_limit );
  } else
    auto_healing_pills_option->SetStatus( false );

  return true;
}

bool BotOptions::ApplyWhitelistedPlayerNames() {
  auto whitelisted_player_names = GetWhitelistedPlayerNamesOption();

  whitelisted_player_names->Clear();

  const auto checkbox_whitelist_playernames =
      GWH( CHECK_WHITELIST_PLAYER_NAMES );

  if ( gwingui::checkbox::IsChecked( checkbox_whitelist_playernames ) ) {
    whitelisted_player_names->SetStatus( true );

    const auto listbox_whitelist_playernames =
        GWH( LISTBOX_WHITELIST_PLAYER_NAMES );

    for ( int i = 0, count = gwingui::listbox::GetCount(
                         listbox_whitelist_playernames );
          i < count; ++i ) {
      const std::wstring name =
          gwingui::listbox::GetText( listbox_whitelist_playernames, i );

      whitelisted_player_names->AddValue( stringutils::WideToAnsi(name) );
    }
  } else
    whitelisted_player_names->SetStatus( false );

  return true;
}

bool BotOptions::ApplyRebuffSequence() {
  /*auto rebuff_sequence_option = GetRebuffSequenceOption();

    rebuff_sequence_option->Clear();

    if (gWin::Checkbox::IsChecked(CHECK_REBUFF_KEY_SEQUENCE)) {
    rebuff_sequence_option->SetStatus(true);

    int sequence_interval =
    gWin::EditControl::GetInt(EDIT_REBUFF_SEQUENCE_INTERVAL);

    if (!sequence_interval) {
      gwingui::messagebox::Error(TEXT("Enter a rebuff sequence interval."));
      return false;
    }

    rebuff_sequence_option->SetRebuffInterval(sequence_interval);

    for (int i = 0, count =
    gWin::Listview::GetItemCount(LISTVIEW_REBUFF_KEY_SEQUENCE); i < count; ++i)
    { const auto row =
    gWin::Listview::GetItemRowText(LISTVIEW_REBUFF_KEY_SEQUENCE, i);

      Key key;

      key.key_code = bot_options::AsciiKeyToVirtualKeyCode(row[0]);
      key.time_ms_after_last_attack = std::stoi(row[1]);

      rebuff_sequence_option->AddValue(key);
    }
    }
    else {
    rebuff_sequence_option->SetStatus(false);
    }*/

  return true;
}

bool BotOptions::ApplyRebuffSequenceList() {
  // Save the currently selected sequence in case it has not been saved
  // the reason is because the user might have changed something without
  // selecting another sequence in the list that is the way it is saved
  auto character_rebuff_list = GetRebuffSequenceListOption();

  const auto listbox_rebuff_sequence = GWH( LISTBOX_REBUFF_SEQUENCES );

  const auto selected_index =
      gwingui::listbox::GetSelectedIndex( listbox_rebuff_sequence );

  if ( selected_index != -1 ) {
    const auto charcter_rebuff_of_options =
        ReadCharacterRebuffOptions( selected_index );

    character_rebuff_list->ReplaceRebuffSequence( selected_index,
                                                  charcter_rebuff_of_options );
  }

  const auto checkbox_rebuff_sequence = GWH( CHECK_REBUFF_SEQUENCES );

  if ( gwingui::checkbox::IsChecked( checkbox_rebuff_sequence ) )
    character_rebuff_list->SetStatus( true );
  else
    character_rebuff_list->SetStatus( false );

  return true;
}

CharacterRebuffOption BotOptions::ReadCharacterRebuffOptions( uint32_t index ) {
  CharacterRebuffOption rebuff_sequence_option(
      CharacterRebuffOption::kJSON_NAME + std::to_string( index ), 0 );

  rebuff_sequence_option.Clear();

  rebuff_sequence_option.SetStatus( true );

  const auto editcontrol_rebuff_sequence_interval =
      GWH( EDIT_REBUFF_SEQUENCE_INTERVAL );

  int sequence_interval =
      gwingui::editcontrol::GetInt( editcontrol_rebuff_sequence_interval );

  rebuff_sequence_option.SetRebuffInterval( sequence_interval );

  const auto listview_rebuff_key_sequence = GWH( LISTVIEW_REBUFF_KEY_SEQUENCE );

  for ( int i = 0, count = gwingui::listview::GetItemCount(
                       listview_rebuff_key_sequence );
        i < count; ++i ) {
    const auto row =
        gwingui::listview::GetItemRowText( listview_rebuff_key_sequence, i );

    Key key;

    key.key_code = optionutils::AsciiKeyToVirtualKeyCode( row[ 0 ] );
    key.time_ms_after_last_attack = std::stoi( row[ 1 ] );

    rebuff_sequence_option.AddValue( key );
  }

  return rebuff_sequence_option;
}

bool BotOptions::ApplyAverageYPostionTargetSelection() {
  auto average_y_pos_target_selection_option =
      GetAverageYPositionTargetSelectionOption();

  const auto checkbox_average_y_pos = GWH( CHECK_AVERAGE_Y_POSITION );

  if ( gwingui::checkbox::IsChecked( checkbox_average_y_pos ) )
    average_y_pos_target_selection_option->SetStatus( true );
  else
    average_y_pos_target_selection_option->SetStatus( false );

  return true;
}

bool BotOptions::ApplyRemoveAllObstacles() {
  auto remove_all_obstacles_option = GetRemoveAllObstaclesOption();

  const auto checkbox_remove_all_obstacles = GWH( CHECK_REMOVE_ALL_OBSTACLES );

  if ( gwingui::checkbox::IsChecked( checkbox_remove_all_obstacles ) )
    remove_all_obstacles_option->SetStatus( true );
  else
    remove_all_obstacles_option->SetStatus( false );

  return true;
}

bool BotOptions::ApplyUpdateCharPos() {
  auto update_char_pos_option = GetUpdateCharPosOption();

  const auto checkbox_update_pos_each_kill = GWH( CHECK_UPDATE_POS_EACH_KILL );

  if ( gwingui::checkbox::IsChecked( checkbox_update_pos_each_kill ) )
    update_char_pos_option->SetStatus( true );
  else
    update_char_pos_option->SetStatus( false );

  return true;
}

bool BotOptions::ApplyLevelArea() {
  auto level_area_option = GetLevelAreaOption();

  const auto checkbox_level_area = GWH( CHECK_LEVEL_AREA );

  if ( gwingui::checkbox::IsChecked( checkbox_level_area ) ) {
    level_area_option->SetStatus( true );

    const auto editcontrol_level_area_length1 = GWH( EDIT_LEVEL_AREA_LENGTH1 );
    auto area_length1 =
        gwingui::editcontrol::GetInt( editcontrol_level_area_length1 );

    if ( !area_length1 ) {
      gwingui::messagebox::Error(
          TEXT( "You enter the level area length 1." ) );
      return false;
    }

    level_area_option->SetLength1( area_length1 );

    const auto editcontrol_level_area_length2 = GWH( EDIT_LEVEL_AREA_LENGTH2 );
    auto area_length2 =
        gwingui::editcontrol::GetInt( editcontrol_level_area_length2 );

    if ( !area_length2 ) {
      gwingui::messagebox::Error(
          TEXT( "You enter the level area length 2." ) );
      return false;
    }

    level_area_option->SetLength2( area_length2 );

    const auto editcontrol_level_area_origin_pos_x =
        GWH( EDIT_LEVEL_AREA_ORIGIN_POS_X );
    auto origin_pos_x =
        gwingui::editcontrol::GetInt( editcontrol_level_area_origin_pos_x );

    if ( !origin_pos_x ) {
      gwingui::messagebox::Error( TEXT( "Please enter a origin x position" ) );
      return false;
    }

    const auto editcontrol_level_area_origin_pos_z =
        GWH( EDIT_LEVEL_AREA_ORIGIN_POS_Z );
    auto origin_pos_z =
        gwingui::editcontrol::GetInt( editcontrol_level_area_origin_pos_z );

    if ( !origin_pos_z ) {
      gwingui::messagebox::Error( TEXT( "Please enter a origin z position" ) );
      return false;
    }

    level_area_option->SetOriginPosition(
        D3DXVECTOR3( static_cast<float>( origin_pos_x ), 0.0f,
                     static_cast<float>( origin_pos_z ) ) );
  } else {
    level_area_option->SetStatus( false );
  }

  return true;
}

bool BotOptions::GetNoCollision() {
  return no_collision_;
}

void BotOptions::SetNoCollision( bool value ) {
  no_collision_ = value;
}

float BotOptions::GetSpeedMultiplier() {
  return speed_multiplier_;
}

void BotOptions::SetSpeedMultiplier( float value ) {
  speed_multiplier_ = value;
}

}  // namespace bot
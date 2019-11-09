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
#include "options/remove_all_obstacles_option.h"
#include "options/update_char_pos_option.h"

#include "../utils/string_utls.h"

using namespace nlohmann;

namespace bot {

BotOptions::BotOptions()
    : has_options_been_changed_( false ), speed_multiplier_( 0 ) {
  options_[ OptionType::kOptionTypeWhitelistedNames ] =
      std::make_unique<WhitelistedNamesOption>( "whitelisted_names",
                                                CHECK_WHITELIST_NAMES );
  options_[ OptionType::kOptionTypeAttackSequence ] =
      std::make_unique<AttackSequenceOption>( "attack_sequence",
                                              CHECK_ATTACK_SEQUENCE );
  options_[ OptionType::kOptionTypeAutoHealingFood ] =
      std::make_unique<AutoHealingOption>( "auto_healing",
                                           CHECK_AUTO_HEALTH_FOOD );
  options_[ OptionType::kOptionTypeWhitelistedPlayerNames ] =
      std::make_unique<WhitelistedPlayerNamesOption>(
          "whitelisted_player_names", CHECK_WHITELIST_PLAYER_NAMES );
  options_[ OptionType::kOptionTypeAverageYPositionTargetSelection ] =
      std::make_unique<AverageYPosOption>(
          "average_y_position_target_selection", CHECK_AVERAGE_Y_POSITION );
  options_[ OptionType::kOptionTypeRemoveAllObstacles ] =
      std::make_unique<RemoveAllObstacleOption>( "remove_all_obstacles",
                                                 CHECK_REMOVE_ALL_OBSTACLES );
  options_[ OptionType::kOptionTypeBotMode ] =
      std::make_unique<BotModeOption>( "botmode", 0 );
  options_[ OptionType::kOptionTypeLureTargetKeyCode ] =
      std::make_unique<LureTargetKeyOption>( "lure_target_key_code", 0 );
  options_[ OptionType::kOptionTypeCharacterRebuffList ] =
      std::make_unique<CharacterRebuffListOption>( "character_rebuff_list",
                                                   CHECK_REBUFF_SEQUENCES );
  options_[ OptionType::kOptionTypeAutoHealingPills ] =
      std::make_unique<AutoHealingPillsOption>( "auto_healing_pills",
                                                CHECK_AUTO_HEALTH_PILLS );
  options_[ OptionType::kOptionTypeUpdateCharacterPos ] =
      std::make_unique<UpdateCharPosOption>( "update_char_pos",
                                             CHECK_UPDATE_POS_EACH_KILL );

  options_[ OptionType::kOptionTypeLevelArea ] =
      std::make_unique<LevelAreaOption>( "level_area", CHECK_LEVEL_AREA );
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

bool IsCharacterNameValid( const std::string& character_name ) {
  for ( auto c : character_name ) {
    if ( !isascii( c ) ) {
      return false;
    }
  }

  return true;
}

void BotOptions::SaveOptions( const std::string& character_name ) {
  if ( !IsCharacterNameValid( character_name ) ) {
    gwingui::messagebox::Error(
        TEXT( "Invalid character name, not saving options." ) );
    return;
  }

  json json_parent;

  for ( const auto& option : options_ ) {
    const auto json_child = option.second->GetJson();
    json_parent[ option.second->GetName() ] = json_child;
  }

  const auto greyb1t_directory = optionutils::GetGreyb1tDirectory();

  if ( !CreateDirectory( greyb1t_directory.c_str(), 0 ) &&
       GetLastError() != ERROR_ALREADY_EXISTS ) {
    throw std::runtime_error( "Unable to create the greyb1t directory to " +
                              stringutils::WideToAnsi( greyb1t_directory ) );
  }

  const auto options_directory = optionutils::GetOptionsDirectory();

  if ( !CreateDirectory( options_directory.c_str(), 0 ) &&
       GetLastError() != ERROR_ALREADY_EXISTS ) {
    throw std::runtime_error( "Unable to create the option directory to " +
                              stringutils::WideToAnsi( options_directory ) );
  }

  const std::string json_dump = json_parent.dump( 4 );

  const bool ret = WriteTextFile(
      options_directory + TEXT( "\\options-" ) +
          stringutils::AnsiToWide( character_name ) + TEXT( ".rimjob" ),
      json_dump );

  if ( !ret ) {
    gwingui::messagebox::Error( TEXT( "Unable to save the options." ) );
  }
}

std::vector<std::wstring> GetFilesInDirectory( const std::wstring& dir ) {
  WIN32_FIND_DATA find_data;
  gwinmem::SafeFindHandle file_handle =
      FindFirstFile( ( dir + TEXT( "\\*" ) ).c_str(), &find_data );

  if ( file_handle.GetValue() == INVALID_HANDLE_VALUE ) {
    return {};
  }

  std::vector<std::wstring> files;

  do {
    files.push_back( find_data.cFileName );
  } while ( FindNextFile( file_handle.GetValue(), &find_data ) );

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

std::wstring FindCorrectOptionFile( const std::vector<std::wstring>& files,
                                    const std::wstring& character_name ) {
  if ( !files.size() ) {
    return std::wstring();
  }

  const auto result = std::find_if(
      files.begin(), files.end(), [&]( const std::wstring& file ) {
        return file.find( character_name ) != std::wstring::npos;
      } );

  // was it found? if not, return empty string
  if ( result == files.end() ) {
    return std::wstring();
  }

  return *result;
}

void BotOptions::LoadOptions( const std::string& character_name ) {
  const auto options_directory = optionutils::GetOptionsDirectory();

  const std::vector<std::wstring> files =
      GetFilesInDirectory( options_directory );

  const std::wstring correct_options_filename =
      FindCorrectOptionFile( files, stringutils::AnsiToWide( character_name ) );

  // is there a option file?
  if ( correct_options_filename.empty() )
    return;

  std::wstring options_file_directory =
      options_directory + TEXT( "\\" ) + correct_options_filename;

  std::string options_file_buffer;
  const bool ret = ReadTextFile( options_file_directory, &options_file_buffer );

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
}

void BotOptions::SetOptionHasBeenChanged( bool value ) {
  has_options_been_changed_ = value;

  const auto button_apply_settings_handle = GWH( BUTTON_APPLY_SETTINGS );

  gwingui::control::EnableOrDisable( button_apply_settings_handle, value );
}

bool BotOptions::HasOptionsBeenChanged() {
  return has_options_been_changed_;
}

void BotOptions::CheckOptionControlsChanged( uint32_t control_identifier ) {
  // TODO: All an array of these values in each options and iterate the options
  // looking for these values instead

  switch ( control_identifier ) {
    case HOTKEY_TARGET_LURE:
    case EDIT_LEVEL_AREA_ORIGIN_POS_X:
    case EDIT_LEVEL_AREA_ORIGIN_POS_Z:
    case EDIT_LEVEL_AREA_LENGTH1:
    case EDIT_LEVEL_AREA_LENGTH2:
    case CHECK_LEVEL_AREA:
    case CHECK_UPDATE_POS_EACH_KILL:
    case CHECK_AUTO_HEALTH_PILLS:
    case HOTKEY_AUTO_HEALTH_PILLS_KEY:
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
    case HOTKEY_AUTO_HEALTH_FOOD_KEY:
    case CHECK_AUTO_HEALTH_FOOD:
    case CHECK_ATTACK_SEQUENCE:
    case CHECK_WHITELIST_NAMES:
    case BUTTON_WHITELIST_NAME_REMOVE:
    case BUTTON_ADD_WHITELISTED_NAME:
    case COMBO_BOT_MODE:
    case LISTVIEW_ATTACKS: {
      SetOptionHasBeenChanged( true );
    } break;

    default:
      break;
  }
}

bool BotOptions::TryApplyOptions( const std::string& character_name ) {
  std::lock_guard<std::mutex> lock_guard( option_mutex_ );

  // Try applying all the options
  for ( const auto& option : options_ ) {
    if ( !option.second->TryApplyOption() ) {
      return false;
    }
  }

  // If all of the options were successfully applied, save them
  SaveOptions( character_name );

  SetOptionHasBeenChanged( false );

  return true;
}

float BotOptions::GetSpeedMultiplier() {
  return speed_multiplier_;
}

void BotOptions::SetSpeedMultiplier( float value ) {
  speed_multiplier_ = value;
}

}  // namespace bot
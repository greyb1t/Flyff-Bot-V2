#include "pch.h"
#include "tab_window_1.h"
#include "../bot/bot_initializer.h"
#include "../utils/string_utls.h"

#include "gwinguiv2/controls/editcontrol.h"
#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/listview.h"
#include "gwinguiv2/controls/listbox.h"
#include "gwinguiv2/controls/richedit.h"
#include "gwinguiv2/controls/hotkey.h"
#include "gwinguiv2/message_box.h"

#include "../bot/options/option_utils.h"
#include "../bot/options/level_area_option.h"
#include "../bot/options/auto_healing_pills_option.h"
#include "../bot/options/character_rebuff_list.h"
#include "../bot/options/whitelisted_player_names_option.h"
#include "../bot/options/attack_sequence_option.h"
#include "../bot/options/bot_mode_option.h"

namespace bot {

void TabWindow1::Initialize( const HWND window_handle ) {
  gwingui::editcontrol::SetPlaceholder( GWH( EDIT_WHITELISTED_NAME ),
                                        TEXT( "Monster Name" ) );
  gwingui::editcontrol::SetPlaceholder( GWH( EDIT_ATTACK_KEY_INTERVAL ),
                                        TEXT( "Interval e.g. 100" ) );
  gwingui::editcontrol::SetPlaceholder( GWH( EDIT_AUTO_FOOD_HP_LIMIT ),
                                        TEXT( "Health Limit" ) );
  gwingui::editcontrol::SetPlaceholder( GWH( EDIT_WHITELISTED_PLAYER_NAME ),
                                        TEXT( "Player Name" ) );
  gwingui::editcontrol::SetPlaceholder( GWH( EDIT_REBUFF_SEQUENCE_INTERVAL ),
                                        TEXT( "Sequence Interval" ) );
  gwingui::editcontrol::SetPlaceholder( GWH( EDIT_REBUFF_KEY_INTERVAL ),
                                        TEXT( "Interval e.g. 100" ) );
  gwingui::editcontrol::SetPlaceholder( GWH( EDIT_AUTO_PILLS_HP_LIMIT ),
                                        TEXT( "Health Limit" ) );
  gwingui::editcontrol::SetPlaceholder( GWH( EDIT_LEVEL_AREA_LENGTH1 ),
                                        TEXT( "Len1 e.g. 50" ) );
  gwingui::editcontrol::SetPlaceholder( GWH( EDIT_LEVEL_AREA_LENGTH2 ),
                                        TEXT( "Len2 e.g. 50" ) );

  const auto listview_attacks = GWH( LISTVIEW_ATTACKS );

  gwingui::listview::AddColumn(
      listview_attacks, TEXT( "Interval" ),
      gwingui::listview::GetItemCount( listview_attacks ) );
  gwingui::listview::AddColumn(
      listview_attacks, TEXT( "Attack Key" ),
      gwingui::listview::GetItemCount( listview_attacks ) );
  gwingui::listview::SetColumnWidth( listview_attacks, 0, 130 );
  gwingui::listview::SetColumnWidthAutoSize( listview_attacks, 1 );

  // gwingui::listview::EnableFullRowSelect( listview_attacks );

  const auto listview_rebuff_sequence = GWH( LISTVIEW_REBUFF_KEY_SEQUENCE );

  gwingui::listview::AddColumn(
      listview_rebuff_sequence, TEXT( "Interval" ),
      gwingui::listview::GetItemCount( listview_rebuff_sequence ) );
  gwingui::listview::AddColumn(
      listview_rebuff_sequence, TEXT( "Key" ),
      gwingui::listview::GetItemCount( listview_rebuff_sequence ) );
  gwingui::listview::SetColumnWidth( listview_rebuff_sequence, 0, 130 );
  gwingui::listview::SetColumnWidthAutoSize( listview_rebuff_sequence, 1 );

  // gwingui::listview::EnableFullRowSelect( LISTVIEW_REBUFF_KEY_SEQUENCE );

  gwingui::combobox::AddString( GWH( COMBO_BOT_MODE ), TEXT( "OneVsOne" ) );
}

void TabWindow1::OnControlHover( HWND control_handle,
                                 uint32_t control_identifier ) {
  std::wstring tip;

  switch ( control_identifier ) {
    case COMBO_BOT_MODE: {
      tip = TEXT( "Select the mode you want the bot to be operating in." );
    } break;

    case HOTKEY_TARGET_LURE: {
      tip = TEXT(
          "Select the key with the skill / action you would like the bot to "
          "use on the first time of attack." );
    } break;

    case CHECK_WHITELIST_NAMES: {
      tip = TEXT(
          "Whitelisted Names (Target Selection Specification): Add names of "
          "monsters that you want to bot to target. If the name is not found "
          "in the list, the monster will not target it." );
    } break;

    case EDIT_WHITELISTED_NAME: {
      tip = TEXT( "Enter the exact name (case-sensitive) of the monster." );
    } break;

    case BUTTON_WHITELIST_FILL_SELECTED_NAME: {
      tip = TEXT(
          "Fills the text box with the name of the current select target "
          "in-game." );
    } break;

    case CHECK_ATTACK_SEQUENCE: {
      tip = TEXT(
          "Attack Sequence (Attack): A sequence of keys that will continusly "
          "execute based on the user's input. It is used e.g. when the user "
          "would like to use a combination of skills on a monster." );
    } break;

    case HOTKEY_ATTACK_KEY: {
      tip = TEXT(
          "Select the key that you would like the bot to press for the "
          "specific action." );
    } break;

    case EDIT_ATTACK_KEY_INTERVAL: {
      tip = TEXT(
          "Enter the interval in milliseconds how long to wait before pressing "
          "the next key." );
    } break;

    case CHECK_AUTO_HEALTH_FOOD: {
      tip = TEXT(
          "Auto Food (Healing): The bot will automatically press the specified "
          "key when the character's health reaches below the specified "
          "limit." );
    } break;

    case HOTKEY_AUTO_HEALTH_FOOD_KEY: {
      tip = TEXT(
          "Select the key that you would like the bot to press to heal." );
    } break;

    case EDIT_AUTO_FOOD_HP_LIMIT: {
      tip = TEXT(
          "Enter the minimum amount of health when the bot should start "
          "healing." );
    } break;

    case CHECK_WHITELIST_PLAYER_NAMES: {
      tip = TEXT(
          "Whitelisted Player Names (Be Safe): If the bot see's another player "
          "that is not whitelisted, it automatically stops and idle's for 15 "
          "minutes." );
    } break;

    case EDIT_WHITELISTED_PLAYER_NAME: {
      tip = TEXT(
          "Enter the exact name (case-sensitive) of the player that you would "
          "like to whitelist." );
    } break;

    case BUTTON_WHITELIST_FILL_SELECTED_PLAYER_NAME: {
      tip = TEXT(
          "Fills the text box with the name of the current select target "
          "in-game." );
    } break;

      /*
        case CHECK_REBUFF_KEY_SEQUENCE:
        {
        tip = TEXT("Buff Key Sequence (Character Rebuff): Automatically executes
        the key sequence while the bot is waiting for it to finish."); } break;
      */

    case EDIT_REBUFF_SEQUENCE_INTERVAL: {
      tip = TEXT(
          "Enter the interval (how often) in milliseconds the bot will rebuff "
          "the character." );
    } break;

    case HOTKEY_REBUFF_KEY: {
      tip = TEXT(
          "Select the key of the specific skill / powerup or anything else." );
    } break;

    case EDIT_REBUFF_KEY_INTERVAL: {
      tip = TEXT(
          "Enter the interval in milliseconds to wait until the bot will "
          "execute the next key." );
    } break;

    case CHECK_AVERAGE_Y_POSITION: {
      tip = TEXT(
          "The bot will calculate the average y position of the last 30 "
          "monsters and offset it lower to avoid selecting monsters to "
          "low.\nNOTE: Should only be used in Traseria to avoid targeting "
          "monsters that are stuck on the sides of the islands." );
    } break;

    case CHECK_REMOVE_ALL_OBSTACLES: {
      tip = TEXT(
          "When the bot is started, it will destroys all visible obstacles "
          "that "
          "might block the character when ranged. Also acts as no collision." );
    } break;

    case CHECK_LEVEL_AREA: {
      tip = TEXT(
          "When starting the bot, it will only kill monsters in an area that "
          "is specified by the user with the two textboxes below. It will "
          "create a rectangle that will be displayed with four yellow "
          "corners." );
    } break;

    case EDIT_LEVEL_AREA_LENGTH1: {
      tip = TEXT( "Enter the first length of the level area rectangle." );
    } break;

    case EDIT_LEVEL_AREA_LENGTH2: {
      tip = TEXT( "Enter the second length of the level area rectangle." );
    } break;

    default:
      break;
  }

  gwingui::richedit::SetText( GWH( RICHEDIT_FEATURE_DESCRIPTION ), tip );
}

void TabWindow1::OnControlLeave( HWND control_handle,
                                 uint32_t control_identifier ) {
  gwingui::richedit::SetText( GWH( RICHEDIT_FEATURE_DESCRIPTION ), TEXT( "" ) );
}

void TabWindow1::OnListBoxSelectionChanged( uint32_t control_identifier,
                                            HWND control_handle ) {
  switch ( control_identifier ) {
    case LISTBOX_WHITELIST_NAMES_VICINITY: {
      const auto selected_index =
          gwingui::listbox::GetSelectedIndex( control_handle );
      const auto selected_monster_name =
          gwingui::listbox::GetText( control_handle, selected_index );

      const auto whitelisted_name_edit_control = GWH( EDIT_WHITELISTED_NAME );
      gwingui::editcontrol::SetText( whitelisted_name_edit_control,
                                     selected_monster_name );
    } break;

    case LISTBOX_REBUFF_SEQUENCES: {
      const auto botcore = Initializer().GetBotCore();
      auto& bot_options = botcore->GetBotOptions();
      auto& character_rebuff_list =
          bot_options.GetOption<CharacterRebuffListOption>();

      // Refresh the controls with the new values for the specific rebuff
      // sequence
      int selected_sequence_index =
          gwingui::listbox::GetSelectedIndex( control_handle );

      // Is a sequence selected in the listbox?
      if ( selected_sequence_index != -1 ) {
        // Does the selected index of sequence exists?
        if ( character_rebuff_list.RebuffExists( selected_sequence_index ) ) {
          auto& selected_char_rebuff_sequence =
              character_rebuff_list.GetCharacterRebuff(
                  selected_sequence_index );

          // selected_char_rebuff_sequence.EnableOrDisableControls(true);
          selected_char_rebuff_sequence.RefreshControls( &bot_options );
        } else {
          assert( false && "ehm" );

          CharacterRebuffOption rebuff_temp;
          rebuff_temp.SetStatus( true );
          rebuff_temp.RefreshControls( &bot_options );
        }
      }

      previous_listbox_sequence_index_ =
          gwingui::listbox::GetSelectedIndex( GWH( LISTBOX_REBUFF_SEQUENCES ) );
    } break;

    default:
      break;
  }
}

void TabWindow1::OnListBoxItemDeleted( const uint32_t control_id,
                                       const HWND control_handle,
                                       const uint32_t index ) {
  switch ( control_id ) {
    case LISTBOX_REBUFF_SEQUENCES: {
      const auto botcore = Initializer().GetBotCore();
      auto& bot_options = botcore->GetBotOptions();

      CharacterRebuffOption rebuff_temp;
      rebuff_temp.EnableOrDisableControls( false, &bot_options );
    } break;

    default:
      break;
  }
}

void TabWindow1::OnButtonClick( HWND hCtrl, UINT ctrlId ) {
  const auto botcore = Initializer().GetBotCore();
  auto& bot_options = botcore->GetBotOptions();
  bot_options.CheckOptionControlsChanged( ctrlId );

  switch ( ctrlId ) {
    case CHECK_WHITELIST_NAMES: {
      bool value = gwingui::checkbox::IsChecked( GWH( CHECK_WHITELIST_NAMES ) );
      bot_options.GetOption<WhitelistedNamesOption>().EnableOrDisableControls(
          value, &bot_options );
    } break;

    case BUTTON_ADD_WHITELISTED_NAME: {
      const auto editcontrol_whitelisted_name = GWH( EDIT_WHITELISTED_NAME );

      const std::wstring name =
          gwingui::editcontrol::GetText( editcontrol_whitelisted_name );

      if ( gwingui::listbox::FindString( editcontrol_whitelisted_name, 0,
                                         name ) == -1 ) {
        gwingui::messagebox::Error(
            TEXT( "The name is already added to the whitelist." ) );
        return;
      }

      gwingui::listbox::AddString( GWH( LISTBOX_WHITELIST_NAMES ), name );

      gwingui::editcontrol::SetText( editcontrol_whitelisted_name, TEXT( "" ) );
    } break;

    case BUTTON_WHITELIST_NAME_REMOVE: {
      const auto listbox_whitelisted_names = GWH( LISTBOX_WHITELIST_NAMES );

      const int selected_index =
          gwingui::listbox::GetSelectedIndex( listbox_whitelisted_names );
      const std::wstring selected_name =
          gwingui::listbox::GetSelectedString( listbox_whitelisted_names );

      if ( selected_index == -1 ) {
        gwingui::messagebox::Error(
            TEXT( "You have to select a name to remove from the listbox." ) );
        return;
      }

      gwingui::listbox::DeleteItem( listbox_whitelisted_names, selected_index );
    } break;

    case BUTTON_WHITELIST_FILL_SELECTED_NAME: {
      const auto botcore = Initializer().GetBotCore();
      const auto& client = botcore->GetFlyffClient();
      const auto local_player = client->CreateLocalPlayer();

      if ( local_player->IsDeletedOrInvalidMemory() ) {
        gwingui::messagebox::Error( TEXT(
            "Unable to fill the selected name, local player is invalid." ) );
        return;
      }

      if ( !local_player->IsEntitySelected() ) {
        gwingui::messagebox::Error( TEXT( "Please select an entity." ) );
        return;
      }

      const auto selected_entity = local_player->GetSelectedEntity();

      gwingui::editcontrol::SetText(
          GWH( EDIT_WHITELISTED_NAME ),
          stringutils::AnsiToWide( selected_entity->GetName() ) );
    } break;

    case CHECK_ATTACK_SEQUENCE: {
      bool value = gwingui::checkbox::IsChecked( GWH( CHECK_ATTACK_SEQUENCE ) );
      bot_options.GetOption<AttackSequenceOption>().EnableOrDisableControls(
          value, &bot_options );
    } break;

    case BUTTON_ADD_ATTACK: {
      const auto hotkey_attack_key_handle = GWH( HOTKEY_ATTACK_KEY );

      const auto selected_virtual_keycode =
          gwingui::hotkey::GetCurrentHotkey( hotkey_attack_key_handle );

      if ( !selected_virtual_keycode ) {
        gwingui::messagebox::Error(
            TEXT( "You have to select a key for the bot to press in the combo "
                  "box." ) );
        return;
      }

      const auto selected_key_string =
          gwingui::hotkey::VirtualKeycodeToString( selected_virtual_keycode );

      const auto editcontrol_attack_key_interval =
          GWH( EDIT_ATTACK_KEY_INTERVAL );
      const auto interval =
          gwingui::editcontrol::GetInt( editcontrol_attack_key_interval );

      if ( interval <= 0 ) {
        gwingui::messagebox::Error(
            TEXT( "The interval has to be entered into the text box or it has "
                  "to be higher than zero." ) );
        return;
      }

      const auto listview_attacks = GWH( LISTVIEW_ATTACKS );

      const auto item_index = gwingui::listview::AddItem(
          listview_attacks, selected_key_string,
          gwingui::listview::GetItemCount( listview_attacks ) );
      gwingui::listview::AddSubitem(
          listview_attacks, std::to_wstring( interval ), item_index, 2 );

      gwingui::hotkey::SetCurrentHotkey( hotkey_attack_key_handle, 0 );

      gwingui::editcontrol::SetText( editcontrol_attack_key_interval,
                                     TEXT( "" ) );
    } break;

    case BUTTON_REMOVE_ATTACK: {
      const auto listview_attacks = GWH( LISTVIEW_ATTACKS );

      const auto selected_attack_index =
          gwingui::listview::GetSelectedItem( listview_attacks );

      if ( selected_attack_index == -1 ) {
        gwingui::messagebox::Error(
            TEXT( "You have to select a attack in the list view." ) );
        return;
      }

      gwingui::listview::RemoveItem( listview_attacks, selected_attack_index );
    } break;

    case CHECK_AUTO_HEALTH_FOOD: {
      bool value =
          gwingui::checkbox::IsChecked( GWH( CHECK_AUTO_HEALTH_FOOD ) );
      bot_options.GetOption<AutoHealingOption>().EnableOrDisableControls(
          value, &bot_options );
    } break;

    case CHECK_AUTO_HEALTH_PILLS: {
      bool value =
          gwingui::checkbox::IsChecked( GWH( CHECK_AUTO_HEALTH_PILLS ) );
      bot_options.GetOption<AutoHealingPillsOption>().EnableOrDisableControls(
          value, &bot_options );
    } break;

    case CHECK_WHITELIST_PLAYER_NAMES: {
      bool value =
          gwingui::checkbox::IsChecked( GWH( CHECK_WHITELIST_PLAYER_NAMES ) );
      bot_options.GetOption<WhitelistedPlayerNamesOption>()
          .EnableOrDisableControls( value, &bot_options );
    } break;

    case CHECK_DO_NOT_KILL_OTHER_PLAYERS_MONSTERS: {
      bool value = gwingui::checkbox::IsChecked(
          GWH( CHECK_DO_NOT_KILL_OTHER_PLAYERS_MONSTERS ) );
      bot_options.GetOption<AvoidEngagedMonsterOption>()
          .EnableOrDisableControls( value, &bot_options );
    } break;

    case CHECK_LEVEL_AREA: {
      bool value = gwingui::checkbox::IsChecked( GWH( CHECK_LEVEL_AREA ) );
      bot_options.GetOption<LevelAreaOption>().EnableOrDisableControls(
          value, &bot_options );
    } break;

    case BUTTON_LEVEL_AREA_GET_CURRENT_POS: {
      const auto botcore = Initializer().GetBotCore();
      const auto client = botcore->GetFlyffClient();
      const auto local_player = client->CreateLocalPlayer();
      gwingui::editcontrol::SetInt(
          GWH( EDIT_LEVEL_AREA_ORIGIN_POS_X ),
          static_cast<int32_t>( local_player->GetPosition().x ) );
      gwingui::editcontrol::SetInt(
          GWH( EDIT_LEVEL_AREA_ORIGIN_POS_Z ),
          static_cast<int32_t>( local_player->GetPosition().z ) );
      bot_options.SetOptionHasBeenChanged( true );
      // bot_options.GetLevelAreaOption()->SetOriginPosition(local_player->GetPosition());
    } break;

    case BUTTON_ADD_WHITELISTED_PLAYER_NAME: {
      const auto editcontrol_whitelisted_player_name =
          GWH( EDIT_WHITELISTED_PLAYER_NAME );

      const std::wstring name =
          gwingui::editcontrol::GetText( editcontrol_whitelisted_player_name );

      if ( gwingui::listbox::FindString( editcontrol_whitelisted_player_name, 0,
                                         name ) == -1 ) {
        gwingui::messagebox::Error(
            TEXT( "The name is already added to the whitelist." ) );
        return;
      }

      gwingui::listbox::AddString( GWH( LISTBOX_WHITELIST_PLAYER_NAMES ),
                                   name );

      gwingui::editcontrol::SetText( editcontrol_whitelisted_player_name,
                                     TEXT( "" ) );
    } break;

    case BUTTON_WHITELIST_PLAYER_NAME_REMOVE: {
      const auto lisbox_whitelisted_player_names =
          GWH( LISTBOX_WHITELIST_PLAYER_NAMES );

      const int selected_index =
          gwingui::listbox::GetSelectedIndex( lisbox_whitelisted_player_names );
      const std::wstring selected_name = gwingui::listbox::GetSelectedString(
          lisbox_whitelisted_player_names );

      if ( selected_index == -1 ) {
        gwingui::messagebox::Error(
            TEXT( "You have to select a name to remove from the listbox." ) );
        return;
      }

      gwingui::listbox::DeleteItem( lisbox_whitelisted_player_names,
                                    selected_index );
    } break;

    case BUTTON_WHITELIST_FILL_SELECTED_PLAYER_NAME: {
      const auto botcore = Initializer().GetBotCore();
      const auto client = botcore->GetFlyffClient();
      const auto local_player = client->CreateLocalPlayer();

      if ( local_player->IsDeletedOrInvalidMemory() ) {
        gwingui::messagebox::Error( TEXT(
            "Unable to fill the selected name, local player is invalid." ) );
        return;
      }

      if ( !local_player->IsEntitySelected() ) {
        gwingui::messagebox::Error( TEXT( "Please select an entity." ) );
        return;
      }

      const auto selected_entity = local_player->GetSelectedEntity();

      gwingui::editcontrol::SetText(
          GWH( EDIT_WHITELISTED_PLAYER_NAME ),
          stringutils::AnsiToWide( selected_entity->GetName() ) );
    } break;

    case CHECK_REBUFF_SEQUENCES: {
      bool value =
          gwingui::checkbox::IsChecked( GWH( CHECK_REBUFF_SEQUENCES ) );

      bot_options.GetOption<CharacterRebuffListOption>()
          .EnableOrDisableControls( value, &bot_options );

      if ( !value ) {
        CharacterRebuffOption rebuff_temp;
        rebuff_temp.EnableOrDisableControls( false, &bot_options );
      }
    } break;

    case BUTTON_ADD_REBUFF_SEQUENCE: {
      const auto listbox_rebuff_sequences = GWH( LISTBOX_REBUFF_SEQUENCES );

      int count = gwingui::listbox::GetCount( listbox_rebuff_sequences );

      CharacterRebuffOption rebuff_temp;
      rebuff_temp.SetStatus( true );

      auto& character_rebuff_list =
          bot_options.GetOption<CharacterRebuffListOption>();
      character_rebuff_list.AddRebuffSequence( rebuff_temp );

      gwingui::listbox::AddString(
          listbox_rebuff_sequences,
          stringutils::AnsiToWide( CharacterRebuffOption::kJSON_NAME ) +
              TEXT( " " ) + std::to_wstring( count + 1 ) );
    } break;

    case BUTTON_REMOVE_REBUFF_SEQUENCE: {
      const auto listbox_rebuff_sequences = GWH( LISTBOX_REBUFF_SEQUENCES );

      int selected_index =
          gwingui::listbox::GetSelectedIndex( listbox_rebuff_sequences );

      auto& rebuff_sequence_list =
          bot_options.GetOption<CharacterRebuffListOption>();

      rebuff_sequence_list.RemoveRebuffSequence( selected_index );

      gwingui::listbox::DeleteItem( listbox_rebuff_sequences, selected_index );
    } break;

    case BUTTON_APPLY_SELECTED_REBUFF_SEQUENCE: {
      int selected_index =
          gwingui::listbox::GetSelectedIndex( GWH( LISTBOX_REBUFF_SEQUENCES ) );

      auto& rebuff_sequence_list =
          bot_options.GetOption<CharacterRebuffListOption>();

      const auto charcter_rebuff_of_options =
          rebuff_sequence_list.ReadCharacterRebuffOptions( selected_index );

      rebuff_sequence_list.ReplaceRebuffSequence( selected_index,
                                                  charcter_rebuff_of_options );
    } break;

    case BUTTON_REBUFF_KEY_ADD: {
      const auto hotkey_rebuff_handle = GWH( HOTKEY_REBUFF_KEY );

      const auto selected_virtual_keycode =
          gwingui::hotkey::GetCurrentHotkey( hotkey_rebuff_handle );

      if ( !selected_virtual_keycode ) {
        gwingui::messagebox::Error(
            TEXT( "You have to select a key for the bot to press in the combo "
                  "box." ) );
        return;
      }

      const auto selected_key_string =
          gwingui::hotkey::VirtualKeycodeToString( selected_virtual_keycode );

      const auto edit_rebuff_key_interval = GWH( EDIT_REBUFF_KEY_INTERVAL );

      const auto interval =
          gwingui::editcontrol::GetInt( edit_rebuff_key_interval );

      if ( interval <= 0 ) {
        gwingui::messagebox::Error(
            TEXT( "The interval has to be entered into the text box or it has "
                  "to be higher than zero." ) );
        return;
      }

      const auto listview_rebuff_key_sequence =
          GWH( LISTVIEW_REBUFF_KEY_SEQUENCE );

      const auto item_index = gwingui::listview::AddItem(
          listview_rebuff_key_sequence, selected_key_string,
          gwingui::listview::GetItemCount( listview_rebuff_key_sequence ) );

      gwingui::listview::AddSubitem( listview_rebuff_key_sequence,
                                     std::to_wstring( interval ), item_index,
                                     2 );

      gwingui::hotkey::SetCurrentHotkey( hotkey_rebuff_handle, 0 );
      gwingui::editcontrol::SetText( edit_rebuff_key_interval, TEXT( "" ) );
    } break;

    case BUTTON_REBUFF_KEY_REMOVE: {
      const auto listview_rebuff_key_sequence =
          GWH( LISTVIEW_REBUFF_KEY_SEQUENCE );

      const auto selected_rebuff_index =
          gwingui::listview::GetSelectedItem( listview_rebuff_key_sequence );

      if ( selected_rebuff_index == -1 ) {
        gwingui::messagebox::Error(
            TEXT( "You have to select a attack in the list view." ) );
        return;
      }

      gwingui::listview::RemoveItem( listview_rebuff_key_sequence,
                                     selected_rebuff_index );
    } break;

    default:
      break;
  }
}

void TabWindow1::OnComboboxSelectionChanged( uint32_t control_id,
                                             HWND control_handle ) {
  const auto botcore = Initializer().GetBotCore();
  auto& bot_options = botcore->GetBotOptions();
  bot_options.CheckOptionControlsChanged( control_id );
}

void TabWindow1::OnListViewItemAdded( NMLISTVIEW* lv ) {
  const auto botcore = Initializer().GetBotCore();
  auto& bot_options = botcore->GetBotOptions();
  bot_options.CheckOptionControlsChanged( lv->hdr.idFrom );
}

void TabWindow1::OnListViewItemDeleted( NMLISTVIEW* lv ) {
  const auto botcore = Initializer().GetBotCore();
  auto& bot_options = botcore->GetBotOptions();
  bot_options.CheckOptionControlsChanged( lv->hdr.idFrom );
}

void TabWindow1::OnTrackbarSliderChanging( const HWND trackbar_handle,
                                           int32_t position ) {
  auto slider_id = gwingui::control::GetWindowId( trackbar_handle );
}

void TabWindow1::OnRichEditTextChanged( uint32_t control_id,
                                        CHANGENOTIFY* cn ) {
  const auto botcore = Initializer().GetBotCore();
  auto& bot_options = botcore->GetBotOptions();
  bot_options.CheckOptionControlsChanged( control_id );
}

void TabWindow1::OnHotkeyChanged( const HWND hotkey_handle,
                                  const uint32_t control_id ) {
  const auto botcore = Initializer().GetBotCore();
  auto& bot_options = botcore->GetBotOptions();
  bot_options.CheckOptionControlsChanged( control_id );
}

}  // namespace bot
#include "pch.h"
#include "bot_ai_one_vs_one.h"
#include "botcore.h"
#include "entity_list.h"
#include "options/average_y_pos_option.h"
#include "options/level_area_option.h"
#include "options/character_rebuff_list.h"
#include "options/whitelisted_player_names_option.h"
#include "options/attack_sequence_option.h"
#include "options/lure_target_key_option.h"
#include "options/update_char_pos_option.h"

#include "../utils/string_utls.h"
#include "../utils/simulation.h"

namespace bot {

BotAIOneVsOne::BotAIOneVsOne( BotCore* botcore )
    : KillingBot( botcore,
                  static_cast<int>( OneVsOneStates::kFindingTarget ),
                  TEXT( "1v1 mode" ) ),
      focus_target_machine_( this ),
      select_target_machine_( this ),
      simulation_machine_( this ),
      blocked_state_machine_( this ),
      idle_machine_( this ),
      walk_to_pos_machine_( this ),
      has_stopped_running_( false ),
      has_started_running_( false ),
      has_started_standing_still_( false ),
      has_checked_hp_( false ),
      selected_entity_hp_when_started_hitting_( 0 ),
      check_around_counter_( 0 ),
      has_selected_unintended_target_count_( 0 ),
      local_player_health_start_( 0 ),
      started_walking_backwards_( false ),
      state_after_deselect_( OneVsOneStates::kFindingTarget ),
      entity_engaged_counter_( 0 ),
      who_engaged_prev_engaged_entity_( "" ) {
  auto& bot_options = botcore->GetBotOptions();
  auto& rebuff_sequence_list_option =
      bot_options.GetOption<CharacterRebuffListOption>();
  // bot_options.GetRebuffSequenceListOption();
  rebuff_list_ = rebuff_sequence_list_option.GetRebuffList();

  //////////////////////////////////////////////////////////////////////////
  const auto& whitelisted_names =
      bot_options.GetOption<WhitelistedNamesOption>();
  const auto& average_y_position = bot_options.GetOption<AverageYPosOption>();
  const auto& level_area = bot_options.GetOption<LevelAreaOption>();
  const auto& avoid_engaged_monsters =
      bot_options.GetOption<AvoidEngagedMonsterOption>();

  default_filter_.push_back( &whitelisted_names );
  default_filter_.push_back( &average_y_position );
  default_filter_.push_back( &level_area );
  default_filter_.push_back( &avoid_engaged_monsters );

  //////////////////////////////////////////////////////////////////////////

  // EntityList entity_list(botcore->GetFlyffClient());
  // auto &mover_entities = entity_list.GetMoverEntities();

  //// Check if the pick up pet is out
  // for (auto &e : mover_entities) {
  //  if (e->GetName() == TEXT("Baby Lawolf")) {
  //    pet_entity_ = std::move(e);
  //    break;
  //  }
  //}
}

BotAIOneVsOne::~BotAIOneVsOne() {
  logging::Log( TEXT( "Fixing modified bound boxes.\n" ) );

  RestoreSavedBoundBoxes();
  RestoreBlockedBoundBoxes();
}

void BotAIOneVsOne::UpdateInternal() {
  const auto botcore = GetBotCore();
  const auto client = botcore->GetFlyffClient();
  auto& bot_options = botcore->GetBotOptions();
  const auto& local_player = GetLocalPlayer();
  const auto& rebuff_sequence_list_option =
      bot_options.GetOption<CharacterRebuffListOption>();
  // const auto& whitelisted_names_options =
  //    bot_options.GetWhitelistedNamesOption();

  if ( local_player->IsDeletedOrInvalidMemory() ) {
    logging::Log( TEXT( "The local player became invalid.\n" ) );
    SetNextState( OneVsOneStates::kCharacterDied );
  }

  if ( !local_player->IsAlive() ) {
    logging::Log( TEXT( "The character has died, stopping the bot.\n" ) );
    SetNextState( OneVsOneStates::kCharacterDied );
  }

  // If we have an entity in the pointer
  if ( current_target_entity_ ) {
    // Check if it is valid
    if ( current_target_entity_->IsDeletedOrInvalidMemory() ) {
      logging::Log( TEXT( "POSSIBLY PREVENTED A CRASH, BRUH!\n" ) );
      current_target_entity_ = nullptr;
      SetNextState( OneVsOneStates::kFindingTarget );
    }
  }

  const auto current_state = GetCurrentState<OneVsOneStates>();

  switch ( current_state ) {
    case OneVsOneStates::DeselectEntity: {
      DO_ONCE( []() {
        logging::Log( TEXT( "Trying to deselect current entity.\n" ) );
      } );

      // Press escape with 0 delay because we want it as quickly as possible
      const auto simulation_status =
          simulation_machine_.KeyPress( VK_ESCAPE, 0 );

      if ( simulation_status == StateStatusReturnValue::kSucceeded ) {
        // Check if entity was successfully deselected
        if ( !local_player->IsEntitySelected() ) {
          DO_ONCE( []() {
            logging::Log( TEXT( "Successfully deselected entity.\n" ) );
          } );

          // Continue with the next state
          SetNextState( state_after_deselect_ );
        } else {
          DO_ONCE(
              []() { logging::Log( TEXT( "Failed deselected entity.\n" ) ); } );

          // If it failed to deselect, try again, we do not need to call
          // Reset because it does not internally, only do so for clarity
          simulation_machine_.Reset();
        }
      } else if ( simulation_status == StateStatusReturnValue::kInProgress ) {
        DO_ONCE( []() {
          logging::Log( TEXT( "Deselect entity in progress.\n" ) );
        } );
      }
    } break;

    case OneVsOneStates::kFindingTarget: {
      if ( !local_player->IsStandingStill() ) {
        DO_ONCE( []() {
          logging::Log(
              TEXT( "Character is not ready for another monster yet.\n" ) );
        } );

        return;
      }

      const auto& auto_health_food_option =
          bot_options.GetOption<AutoHealingOption>();

      // Check if the player health is above the limit
      if ( auto_health_food_option.IsEnabled() ) {
        if ( local_player->GetHealth() <
             auto_health_food_option.GetHealthLimit() ) {
          DO_ONCE( []() {
            logging::Log(
                TEXT( "Waiting for the player health to recover above the "
                      "limit.\n" ) );
          } );
          // If the health is lower than the limit, wait and let it recover
          // before killing more monsters
          return;
        }
      }

      EntityList entity_list( client );
      auto& entities = entity_list.GetMoverEntities();

      current_target_entity_ =
          FindNearestMonster( entities, *local_player, default_filter_ );

      // No near entity was found
      if ( !current_target_entity_ ) {
        logging::Log( TEXT( "No near entity was found, trying again.\n" ) );

        ++entities_not_found_counter_;

        // If the bot has not found any entities after 3 tries, then we reset
        // the blacklist
        if ( entities_not_found_counter_ >= 3 ) {
          logging::Log(
              TEXT( "The bot has not found entities after 3 tries, reseting "
                    "the blacklist.\n" ) );

          blacklisted_entities_temporary_.clear();

          entities_not_found_counter_ = 0;
        }

        // Adjust the camera to look the opposite direction for entities
        const float current_camera_degrees_x = local_player->GetCameraRotX();
        local_player->SetCameraRotX( current_camera_degrees_x + 90.f );

        const auto offset = local_player->GetScrollDistance() * 4;

        local_player->SetCameraRotY( 15.f - offset );

        // Wait for the camera to adjust before searching for entities again
        botcore->SkipUpdateForFrames( 35 );

        return;
      }

      const auto& whitelisted_player_names_option =
          bot_options.GetOption<WhitelistedPlayerNamesOption>();

      // Check for non whitelisted players
      if ( whitelisted_player_names_option.IsEnabled() ) {
        const auto entity_found = IsNonWhitelistedPlayerFound(
            whitelisted_player_names_option, entities, *local_player );

        if ( entity_found ) {
          logging::LogImportant(
              TEXT( "A non-whitelisted player was found, Name: " ) +
              stringutils::AnsiToWide( entity_found->GetName() ) +
              TEXT( "\n" ) );
          SetNextState( OneVsOneStates::kWaitUntilPlayerLeaves );
          return;
        }
      }

      for ( const auto& e : entities ) {
        if ( e->IsDeletedOrInvalidMemory() )
          continue;

        if ( e->IsPlayer() || e->IsPet() ) {
          // Exclude me
          if ( e->GetPointerAddress() == local_player->GetPointerAddress() )
            continue;

          bool exists = false;

          for ( auto& i : saved_bound_box_changed_entities_ ) {
            if ( i->GetPointerAddress() == e->GetPointerAddress() ) {
              exists = true;
              break;
            }
          }

          if ( !exists ) {
            auto entity_copy = client->CreateEntity( e->GetPointerAddress() );

            entity_copy->SaveBoundBox();
            entity_copy->RemoveBoundBox();

            saved_bound_box_changed_entities_.push_back(
                std::move( entity_copy ) );
          }
        }
      }

      if ( rebuff_sequence_list_option.IsEnabled() ) {
        if ( local_player->IsStandingStill() ) {
          for ( auto& rebuff_sequence : rebuff_list_ ) {
            const auto rebuff_status = rebuff_sequence.Update();

            switch ( rebuff_status ) {
              case CharacterRebuffReturnValue::kCharRebuffReady: {
                current_rebuff_ = &rebuff_sequence;
                idle_machine_.SetWaitMs( 500 );
                idle_machine_.NextState(
                    static_cast<uint32_t>( OneVsOneStates::kRebuffCharacter ) );
                SetNextState( OneVsOneStates::kIdle );
                return;
              } break;

              case CharacterRebuffReturnValue::kCharRebuffNotReady: {
                // do nothing
              } break;

              default:
                break;
            }
          }
        } else {
          DO_ONCE( []() {
            logging::Log(
                TEXT( "Waiting until the character is standing still before "
                      "doing the rebuff sequence.\n" ) );
          } );
          return;
        }
      }

      for ( size_t i = 0; i < entities.size(); ++i ) {
        const auto& entity = entities[ i ];

        if ( entity->IsDeletedOrInvalidMemory() )
          continue;

        if ( entity->IsMonster() ) {
          // prevent the vector from overgoing 30 values
          if ( last_entity_y_positions_.size() >= 30 )
            last_entity_y_positions_.erase( last_entity_y_positions_.begin() );

          last_entity_y_positions_.push_back( entity->GetPosition().y );
        }
      }

      // Calculate the average y-axis position on all visible entities
      float total_y = 0;

      for ( auto y_pos : last_entity_y_positions_ )
        total_y += y_pos;

      auto& avg_y_pos = bot_options.GetOption<AverageYPosOption>();

      avg_y_pos.SetYPos(
          total_y / static_cast<float>( last_entity_y_positions_.size() ) );

      // average_y_pos_ =
      //     total_y / static_cast<float>( last_entity_y_positions_.size() );

      // offseting the average y pos
      avg_y_pos.SetYPos( avg_y_pos.GetYPos() - 3.f );
      // average_y_pos_ -= 3.f;

      //current_target_entity_ =
      //    FindNearestMonster( entities, *local_player, default_filter_ );

      //// No near entity was found
      //if ( !current_target_entity_ ) {
      //  logging::Log( TEXT( "No near entity was found, trying again.\n" ) );

      //  ++entities_not_found_counter_;

      //  // If the bot has not found any entities after 3 tries, then we reset
      //  // the blacklist
      //  if ( entities_not_found_counter_ >= 3 ) {
      //    logging::Log(
      //        TEXT( "The bot has not found entities after 3 tries, reseting "
      //              "the blacklist.\n" ) );

      //    blacklisted_entities_temporary_.clear();

      //    entities_not_found_counter_ = 0;
      //  }

      //  // Adjust the camera to look the opposite direction for entities
      //  const float current_camera_degrees_x = local_player->GetCameraRotX();
      //  local_player->SetCameraRotX( current_camera_degrees_x + 90.f );

      //  const auto offset = local_player->GetScrollDistance() * 4;

      //  local_player->SetCameraRotY( 15.f - offset );

      //  // Wait for the camera to adjust before searching for entities again
      //  botcore->SkipUpdateForFrames( 35 );

      //  return;
      //}

      nearest_entity_hp_when_selected_ = current_target_entity_->GetHealth();

      std::wstring entity_info =
          TEXT( "Entity name: " ) +
          stringutils::AnsiToWide( current_target_entity_->GetName() ) +
          TEXT( "\nEntity HP: " ) +
          std::to_wstring( current_target_entity_->GetHealth() ) +
          TEXT( "\nEntity MP: " ) +
          std::to_wstring( current_target_entity_->GetMana() ) +
          TEXT( "\nEntity FP: " ) +
          std::to_wstring( current_target_entity_->GetFatigue() ) +
          TEXT( "\nEntity Speed: " ) +
          // std::to_wstring( current_target_entity_->GetSpeed() ) +
          TEXT( "\nEntity Level: " ) +
          std::to_wstring( current_target_entity_->GetLevel() ) +
          TEXT( "\nEntity Motion: " ) +
          std::to_wstring( current_target_entity_->GetMotion() ) +
          TEXT( "\nEntity Type: " ) +
          std::to_wstring( current_target_entity_->GetFlags() ) +
          TEXT( "\nEntity Distance: " ) +
          std::to_wstring(
              current_target_entity_->DistanceTo( *local_player ) ) +
          TEXT( "\nEntity PAddr: " ) +
          std::to_wstring( current_target_entity_->GetPointerAddress() ) +
          TEXT( "\n" );

      logging::Log(
          TEXT( "An entity was found, trying to select the entity.\n" ) +
          entity_info );

      SetNextState( OneVsOneStates::kFocusOnTarget );
    } break;

    case OneVsOneStates::kFocusOnTarget: {
      if ( !IsEntityValid( *current_target_entity_ ) ) {
        logging::Log(
            TEXT( "The bot is trying to focus on an invalid entity, back to "
                  "beginning.\n" ) );

        // Return to FindingTarget and redo the whole thing again
        SetNextState( OneVsOneStates::kFindingTarget );
        return;
      }

      const auto focus_target_status =
          focus_target_machine_.Focus( *current_target_entity_ );

      switch ( focus_target_status ) {
        case StateStatusReturnValue::kInProgress:
          break;

        case StateStatusReturnValue::kSucceeded: {
          SetNextState( OneVsOneStates::kSelectTarget );
        } break;

        default:
          break;
      }
    } break;

    case OneVsOneStates::kSelectTarget: {
      // TODO: Consider making this a function and using it, because this is
      // straight copy pasted from kFocusTarget
      // Added this code here because a crash has occured two times in this
      // state because of an invalid entity, this ensures that the entity is
      // valid before trying to select it
      if ( !IsEntityValid( *current_target_entity_ ) ) {
        logging::Log(
            TEXT( "The bot is trying to select an invalid entity, back to "
                  "beginning.\n" ) );

        // Return to FindingTarget and redo the whole thing again
        SetNextState( OneVsOneStates::kFindingTarget );
        return;
      }

      const auto select_target_status =
          select_target_machine_.Select( *current_target_entity_ );

      switch ( select_target_status ) {
        case StateStatusReturnValue::kSucceeded:
          SetNextState( OneVsOneStates::kVerifyTarget );
          break;

        case StateStatusReturnValue::kFailed:
          // Add the entity to a blacklist to avoid selecting it again and get
          // stuck again
          blacklisted_entities_temporary_.push_back( *current_target_entity_ );
          SetNextState( OneVsOneStates::kFindingTarget );
          break;

        case StateStatusReturnValue::kInProgress:
          break;

        default:
          break;
      }
    } break;

    case OneVsOneStates::kVerifyTarget: {
      // Ensure that a monster has been selected
      if ( !local_player->IsEntitySelected() ) {
        logging::Log( TEXT( "Failed to select the entity, trying again.\n" ) );

        // In case the bot clicked beside the monster and walks to the position,
        // we'll ensure that it stops by pressing 'S'
        const auto simulation_status = simulation_machine_.KeyPress( 'S' );

        switch ( simulation_status ) {
          case StateStatusReturnValue::kSucceeded: {
            if ( local_player->IsWalkingBackwards() ||
                 local_player->IsStandingStill() ) {
              logging::Log( TEXT(
                  "Sucessfully walked backwards and stopped character.\n" ) );

              RestoreBlockedBoundBoxes();

              SetNextState( OneVsOneStates::kFindingTarget );
              return;
            }
          } break;

          default:
            break;
        }

        break;
      }

      std::unique_ptr<Entity> selected_entity =
          local_player->GetSelectedEntity();

      if ( selected_entity->GetPointerAddress() == -1 ) {
        logging::Log( TEXT(
            "The selected entity could not be found in the entity list.\n" ) );

        SetNextState( OneVsOneStates::kFindingTarget );
        return;
      }

      // Is the selected entity different from the intended entity?
      if ( selected_entity->GetPointerAddress() !=
           current_target_entity_->GetPointerAddress() ) {
        logging::Log( TEXT( "An unintended target was selected.\n" ) );

        if ( has_selected_unintended_target_count_ > 5 ) {
          logging::Log( TEXT(
              "Five tries has been done, retrying from the beginning.\n" ) );

          // Reset
          has_selected_unintended_target_count_ = 0;

          RestoreBlockedBoundBoxes();

          SetNextState( OneVsOneStates::kFindingTarget );
          return;
        }

        logging::Log( TEXT( "Removing the blocked entity's bound box.\n" ) );

        // The bug is that the bound box is saved and removed twice
        // the next time, it saves the non-exisiting bound box.

        selected_entity->SaveBoundBox();
        selected_entity->RemoveBoundBox();

        // Add the entity to a list to later restore the bound box
        saved_selection_blocked_entities_.push_back(
            std::move( selected_entity ) );

        ++has_selected_unintended_target_count_;

        // Deselect the entity, then continue on focus target
        state_after_deselect_ = OneVsOneStates::kFocusOnTarget;
        SetNextState( OneVsOneStates::DeselectEntity );
        return;

        // Add the entity that the bot tried to select to a temporary blacklist
        // to avoid selecting it again risking that we select the same invalid
        // entity as we did this time.
        // blacklisted_entities_temporary_.push_back(*nearest_entity_);
      }

      // Reset
      has_selected_unintended_target_count_ = 0;

      // Check if the selected entity is not a monster or alive
      // Then it might have selected something that is in the way of the target
      if ( !selected_entity->IsMonster() || !selected_entity->IsAlive() ) {
        logging::Log( TEXT( "The entity selected is not valid, retrying.\n" ) );

        // Add the entity that the bot tried to select to a temporary blacklist
        // to avoid selecting it again risking that we select the same invalid
        // entity as we did this time.
        blacklisted_entities_temporary_.push_back( *current_target_entity_ );

        RestoreBlockedBoundBoxes();

        // Press escape to deselect current entity that was invalid
        // Go to the beginning again, it will find another target and
        // not the one we blacklisted that resulted in an invalid selection
        state_after_deselect_ = OneVsOneStates::kFindingTarget;
        SetNextState( OneVsOneStates::DeselectEntity );

        return;
      }

      logging::Log( TEXT( "An entity has been selected.\n" ) );

      // In case it somehow missed reseting the bound boxes, do it now.
      RestoreBlockedBoundBoxes();

      SetNextState( OneVsOneStates::kLureTarget );
    } break;

    case OneVsOneStates::kLureTarget: {
      // If the target is no longer selected, it has either disappeared or been
      // killed
      if ( !local_player->IsEntitySelected() ) {
        logging::Log( TEXT( "The target has been killed by someone else?\n" ) );

        // Return to FindingTarget and redo the whole thing again
        SetNextState( OneVsOneStates::kFindingTarget );
        return;
      }

      const StateStatusReturnValue simulation_status =
          simulation_machine_.KeyPress(
              bot_options.GetOption<LureTargetKeyOption>().GetLureKeyCode() );

      switch ( simulation_status ) {
        case StateStatusReturnValue::kInProgress:
          break;

        case StateStatusReturnValue::kSucceeded: {
          SetNextState( OneVsOneStates::kRunTowardsTarget );
        } break;

        default:
          assert( false && "Forgot to handle this case" );
          break;
      }
    } break;

    case OneVsOneStates::kRunTowardsTarget: {
      // If the target is no longer selected, it has either disappeared or been
      // killed
      if ( !local_player->IsEntitySelected() ) {
        logging::Log( TEXT( "The target has been killed by someone else?\n" ) );

        // Return to FindingTarget and redo the whole thing again
        SetNextState( OneVsOneStates::kFindingTarget );
        return;
      }

      // If the character started doing another motion other than standing still
      // in this case it started attacking I hope, then continue to next step
      if ( !local_player->IsStandingStill() ) {
        logging::Log(
            TEXT( "Started attacking or running for the entity.\n" ) );
        SetNextState( OneVsOneStates::kStartedAttackingTarget );
        return;
      } else {
        DO_ONCE( []() {
          logging::Log( TEXT( "The character is standing still.\n" ) );
        } );

        begin_target_attack_not_moving_timer.DoEachIntervalAfter(
            1000,
            [ & ]() {
              logging::Log(
                  TEXT( "The character has not moved for 1 second.\n" ) );
              SetNextState( OneVsOneStates::kBlockedTypeShootThroughObstacle );
            },
            [ & ]() { SetNextState( OneVsOneStates::kLureTarget ); } );
      }
    } break;

    case OneVsOneStates::kStartedAttackingTarget: {
      // TODO: Check if the monster hp has been changed in 4 seconds to check if
      // the monster has been hit or not. If true, enter the obstacle running
      // block state

      // Reset the time
      begin_target_attack_not_moving_timer.Reset();

      // Clear the blacklist when the player has started killing a target
      // TODO: Reset it in the if statement below, when a target has been killed
      // blacklisted_entities_temporary_.clear();

      // If the target is no longer selected, it has either disappeared or been
      // killed
      if ( !local_player->IsEntitySelected() ) {
        logging::Log( TEXT( "The target became invalid?\n" ) );

        // Return to FindingTarget and redo the whole thing again
        SetNextState( OneVsOneStates::kFindingTarget );
        return;
      }

      // If the character has started running
      if ( local_player->IsRunning() || has_started_running_ ) {
        has_started_running_ = true;

        // While the character is running to the target and has not stopped
        // running
        if ( local_player->IsRunning() && !has_stopped_running_ ) {
          const auto& avoid_engaged_monsters_option =
              bot_options.GetOption<AvoidEngagedMonsterOption>();

          if ( avoid_engaged_monsters_option.IsEnabled() ) {
            // Check if the target entity we are running towards is engaged,
            // that means someone else has started attacking it
            const auto is_entity_engaged =
                !avoid_engaged_monsters_option.IsEntityAllowed(
                    *current_target_entity_ );

            if ( is_entity_engaged ) {
              // If the last close player is the same as the previous time
              // and has not killed a monster since the last time

              EntityList entity_list( client );
              auto& entities = entity_list.GetMoverEntities();

              const auto closest_non_whitelisted_player =
                  avoid_engaged_monsters_option.GetClosestNonWhitelistedPlayer(
                      client, entities, *current_target_entity_ );

              const bool is_same_player_as_before =
                  closest_non_whitelisted_player->GetName() ==
                  who_engaged_prev_engaged_entity_;

              // If the previous close player is the same as the one this time and
              // if is has occured more 1 time or more in a row without killing a monster
              if ( entity_engaged_counter_ >= 1 && is_same_player_as_before ) {
                // Continue with this monster
                // Why? To avoid suspicious activity if a random player decides to follow you
                DO_ONCE( []() {
                  logging::Log(
                      TEXT( "Monster possibly taken by another player, but "
                            "this time ignoring it.\n" ) );
                } );
              } else {
                // Find another monster to target
                DO_ONCE( []() {
                  logging::Log(
                      TEXT( "Monster possibly taken by another player.\n" ) );
                } );

                entity_engaged_counter_++;
                who_engaged_prev_engaged_entity_ =
                    closest_non_whitelisted_player->GetName();

                // Find another target instead
                state_after_deselect_ = OneVsOneStates::kFindingTarget;
                SetNextState( OneVsOneStates::DeselectEntity );
                return;
              }
            }
          }

          DO_ONCE( []() {
            logging::Log( TEXT( "Looking for closer entities.\n" ) );
          } );

          EntityList entity_list( client );

          auto& entities = entity_list.GetMoverEntities();

          auto entity =
              FindNearestMonster( entities, *local_player, default_filter_ );

          if ( entity ) {
            // If the entity that was found is not the same one that is
            // currently selected
            if ( current_target_entity_->GetPointerAddress() !=
                 entity->GetPointerAddress() ) {
              const auto old_target_distance_to_player =
                  local_player->DistanceTo( *current_target_entity_ );
              const auto new_target_distance_to_player =
                  local_player->DistanceTo( *entity );

              if ( old_target_distance_to_player >
                   new_target_distance_to_player ) {
                logging::Log( TEXT( "Selecting a closer entity.\n" ) );

                current_target_entity_ = std::move( entity );

                // A close entity was found, deselect the current one and select
                // the new one
                state_after_deselect_ = OneVsOneStates::kFocusOnTarget;
                SetNextState( OneVsOneStates::DeselectEntity );

                return;
              }
            }
          }
        } else {
          // LogOnce(print_once_2, TEXT("The character has started attacking or
          // gotten stuck.\n"));

          DO_ONCE( []() {
            logging::Log( TEXT(
                "The character has started attacking or gotten stuck.\n" ) );
          } );

          // If the character is standing still while it has not started hitting
          // the target then the character is most likely stuck while running to
          // the target
          if ( local_player->IsStandingStill() ||
               has_started_standing_still_ ) {
            // LogOnce(print_once_9, TEXT("The character is not moving.\n"));

            DO_ONCE( []() {
              logging::Log( TEXT( "The character is not moving.\n" ) );
            } );

            has_started_standing_still_ = true;

            running_target_blocked_timer.DoEachIntervalAfter(
                2000,
                [ & ]() {
                  const bool has_been_damaged =
                      current_target_entity_->GetHealth() !=
                      nearest_entity_hp_when_selected_;

                  if ( !has_been_damaged ) {
                    logging::Log(
                        TEXT( "The character has been blocked by an obstacle "
                              "while running to the target.\n" ) );
                    SetNextState(
                        OneVsOneStates::kBlockedTypeRunningIntoObstacle );
                  }
                },
                [ & ]() {
                  if ( !local_player->IsStandingStill() &&
                       !local_player->IsRunning() ) {
                    logging::Log(
                        TEXT( "The character has done something else during "
                              "the two seconds.\n" ) );
                    SetNextState( OneVsOneStates::kStartedHittingTarget );
                  }
                } );

            break;
          } else {
            SetNextState( OneVsOneStates::kStartedHittingTarget );
            break;
          }

          has_stopped_running_ = true;
        }
      } else {
        // If it came here, then the character is not running or has not started
        // running The character probably started attacking a monster that it
        // was close enough

        // Check if the character is not standing still or running, then it
        // probably started attacking it.
        if ( !local_player->IsStandingStill() && !local_player->IsRunning() ) {
          // When using a psykeeper, the bot comes here immeditely due to not
          // having finished the skill A solution is to wait for the character
          // to reach a still standing state.

          // LogOnce(print_once_5, TEXT("Started hittin target immediately
          // without running for it.\n"));

          DO_ONCE( []() {
            logging::Log(
                TEXT( "Started hittin target immediately without running for "
                      "it.\n" ) );
          } );

          SetNextState( OneVsOneStates::kStartedHittingTarget );
          break;
        } else {
          block_check_while_attacking_timer2.DoEachIntervalAfter(
              4000, [ & ]() {
                // If the health has changed since the selection of the monster
                if ( current_target_entity_->GetHealth() ==
                     nearest_entity_hp_when_selected_ ) {
                  logging::Log(
                      TEXT( "The monster has not been damaged for 4 seconds, "
                            "anticipating that character is stuck.\n" ) );

                  SetNextState(
                      OneVsOneStates::kBlockedTypeRunningIntoObstacle );
                }
              } );
        }
      }

      // Has the character been running to the monster for 8 seconds, then
      // something it wrong
      block_check_while_attacking_timer2.DoEachIntervalAfter( 8000, [ & ]() {
        logging::Log(
            TEXT( "The monster has not been damaged for 8 seconds, "
                  "anticipating that character is stuck.\n" ) );
        SetNextState( OneVsOneStates::kBlockedTypeRunningIntoObstacle );
      } );
    } break;

    case OneVsOneStates::kBlockedTypeRunningIntoObstacle: {
      logging::Log( TEXT( "kBlockedTypeRunningIntoObstacle.\n" ) );

      // Add the entity that the bot tried to select to a temporary blacklist to
      // avoid selecting it again risking that we select the same invalid entity
      // as we did this time.
      blacklisted_entities_temporary_.push_back( *current_target_entity_ );

      // Press escape to deselect current entity that was invalid
      // Go to the beginning again, it will find another target and
      // not the one we blacklisted that resulted in an invalid selection
      state_after_deselect_ = OneVsOneStates::kFindingTarget;
      SetNextState( OneVsOneStates::DeselectEntity );

      return;

      // Simulate a monster beside the character and make it click on the ground
      // to move to the left
      // print_once_4.Do([&]() {
      // auto stuck_box = std::make_shared<EntityReplicateBox>(client,
      // local_player); botcore->AddEntityToDraw(stuck_box);
      //});

      // Fix this solution, be creative.
      // blocked_state_machine_.Update(local_player, simulation_machine_,
      // walk_to_pos_machine_);
    } break;

    case OneVsOneStates::kBlockedTypeShootThroughObstacle: {
      logging::Log( TEXT( "kBlockedTypeShootThroughObstacle.\n" ) );

      // Add the entity that the bot tried to select to a temporary blacklist to
      // avoid selecting it again risking that we select the same invalid entity
      // as we did this time.
      blacklisted_entities_temporary_.push_back( *current_target_entity_ );

      // Press escape to deselect current entity that was invalid
      // Go to the beginning again, it will find another target and
      // not the one we blacklisted that resulted in an invalid selection
      state_after_deselect_ = OneVsOneStates::kFindingTarget;
      SetNextState( OneVsOneStates::DeselectEntity );

      return;

      // print_once_6.Do([&]() {
      // auto stuck_box = std::make_shared<EntityReplicateBox>(client,
      // local_player); botcore->AddEntityToDraw(stuck_box);
      //});
    } break;

    case OneVsOneStates::kStartedHittingTarget: {
      // A bootleg version of blade damage hack, does not work on the first hit,
      // but does so on the ones afterwards. I removed this and added another
      // method by hooking SendMeleeAttack to get the first hit as well.
      // local_player->SetMotion(-16973826);

      blacklisted_entities_temporary_.clear();

      // If the target is no longer selected, it has either disappeared or been
      // killed
      if ( !local_player->IsEntitySelected() ) {
        // find the pet aaaa

        // EntityList entity_list(client);
        // auto &mover_entities = entity_list.GetMoverEntities();

        // bool has_found_pet = false;

        //// Check if the pick up pet is out
        // for (auto &e : mover_entities) {
        //  if (e->GetName() == TEXT("Baby Lawolf")) {
        //    has_found_pet = true;
        //    break;
        //  }
        //}

        // if (!has_found_pet) {
        //  logging::LogImportant(TEXT("PET WAS NOT FOUND, PRESS KEY AND THEN
        //  USE TIMER BEFORE CHECKING AGAIN.\n"));
        //}

        // if (pet_entity_) {
        //  if (pet_entity_->IsDeletedOrInvalidMemory() ||
        //    pet_entity_->GetName() != TEXT("Baby Lawolf")) {

        //    EntityList entity_list(client);
        //    auto &mover_entities = entity_list.GetMoverEntities();

        //    for (auto &e : mover_entities) {
        //      if (e->GetName() == TEXT("Baby Lawolf")) {
        //        pet_entity_ = std::move(e);
        //        break;
        //      }
        //    }

        //    logging::LogImportant(TEXT("PET WAS NOT FOUND, PRESS KEY AND THEN
        //    USE TIMER BEFORE CHECKING AGAIN.-") +
        //    std::to_wstring(pet_entity_->GetPointerAddress()) + TEXT("\n"));
        //  }
        //  else {
        //    logging::LogImportant(TEXT("PET VALID. Name: ") +
        //    pet_entity_->GetName() + TEXT(", Ptr: ") +
        //    std::to_wstring(pet_entity_->GetPointerAddress()) + TEXT("\n"));
        //  }
        //}

        DO_ONCE( [ & ]() {
          logging::Log( TEXT( "The target has been killed.\n" ) );
          monster_kill_count_++;
        } );

        // Reset the engaged counter after a monster has been killed
        entity_engaged_counter_ = 0;

        if ( bot_options.GetOption<UpdateCharPosOption>().IsEnabled() ) {
          // Walk backwards to update the characters position
          const auto simulation_status =
              simulation_machine_.KeyPress( 'S', 50 );

          switch ( simulation_status ) {
            case StateStatusReturnValue::kInProgress:
            case StateStatusReturnValue::kSucceeded: {
              // After we have pressed the 'S' key, check if the local player is
              // walking backwards before continuing. If we're not walking
              // backwards yet, it will continue in this state and keep pressing
              // 'S' until it does.
              if ( local_player->IsWalkingBackwards() ) {
                started_walking_backwards_ = true;
              }

              // If we have started walking backwards, wait for the key up event
              // before starting over. Otherwise the char will get stuck walking
              // backwards.
              if ( started_walking_backwards_ &&
                   simulation_status == StateStatusReturnValue::kSucceeded ) {
                simulation_machine_.Reset();

                // Return to FindingTarget and redo the whole thing again
                SetNextState( OneVsOneStates::kFindingTarget );
                return;
              }
            } break;

            default:
              break;
          }

          break;
        } else {
          // Return to FindingTarget and redo the whole thing again
          SetNextState( OneVsOneStates::kFindingTarget );
          return;
        }
      }

      // LogOnce(print_once_3, TEXT("Started hitting the target.\n"));
      DO_ONCE(
          []() { logging::Log( TEXT( "Started hitting the target.\n" ) ); } );

      block_check_while_attacking_timer.DoEachIntervalAfter( 4000, [ & ]() {
        if ( !has_checked_hp_ ) {
          // If the health has changed since the selection of the monster
          if ( current_target_entity_->GetHealth() ==
               nearest_entity_hp_when_selected_ ) {
            logging::Log(
                TEXT( "The monster has not been damaged at all for 4 seconds, "
                      "anticipating that character is stuck.\n" ) );

            SetNextState( OneVsOneStates::kBlockedTypeRunningIntoObstacle );
          }

          has_checked_hp_ = true;
        }

        // THE BELOW IS NOT WORKING PROPERLY
        // If the target has not been damaged for 4 seconds since last 4 seconds
        if ( current_target_entity_->GetHealth() ==
             selected_entity_hp_when_started_hitting_ ) {
          logging::Log(
              TEXT( "The target has not been damaged for 4 seconds, probably "
                    "stuck.\n" ) );

          SetNextState( OneVsOneStates::kBlockedTypeRunningIntoObstacle );
        } else
          selected_entity_hp_when_started_hitting_ =
              current_target_entity_->GetHealth();
      } );

      // TODO: The bot is stuck in here when the char is blocked and the monster
      // has already been hit once. Fix..

      const auto& attack_sequence_option =
          bot_options.GetOption<AttackSequenceOption>();

      if ( attack_sequence_option.IsEnabled() )
        attack_sequence_.ExecuteSequence( attack_sequence_option,
                                          simulation_machine_ );
    } break;

    case OneVsOneStates::kWaitUntilPlayerLeaves: {
      DO_ONCE( [ & ]() {
        // Store the health in order to check later on whether or not we have
        // been hurt
        local_player_health_start_ = local_player->GetHealth();
      } );

      const int minutes_5_ms = /*900000*/ 300000;
      wait_until_player_leaves_timer_.DoEachIntervalAfter(
          minutes_5_ms,
          [ & ]() { SetNextState( OneVsOneStates::kCheckAroundForPlayers ); },
          [ & ]() {
            // While waiting for a player to disappear, check if an aggressive
            // monster has started attacking us

            // If the health has went down
            if ( local_player->GetHealth() < local_player_health_start_ ) {
              // Find the closest monster with the speed multiplier of 2

              EntityList entity_list( client );
              auto& entities = entity_list.GetMoverEntities();

              std::vector<const EntityFilter*> empty_filters;

              std::unique_ptr<Entity> entity =
                  FindNearestMonster( entities, *local_player, empty_filters );

              // Find any monster with no filter, meaning that we find a monster
              // even though e.g. it is not whitelisted.
              if ( entity ) {
                logging::Log(
                    TEXT( "An aggressive monster has been found attacking the "
                          "player.\n" ) );

                // Go to the begin kill monster state and let the bot
                // continue as normal. It will then check around for a
                // player. In this case, add a bool that forces it to
                // check around or go to this state again?

                current_target_entity_ = std::move( entity );

                SetNextState( OneVsOneStates::kFocusOnTarget );
              } else {
                logging::Log(
                    TEXT( "The player health went down, but no aggressive "
                          "monster was found.\n" ) );
              }
            }
          } );
    } break;

    case OneVsOneStates::kCheckAroundForPlayers: {
      // Adjust the camera to look the opposite direction for entities
      const float current_camera_degrees_x = local_player->GetCameraRotX();
      local_player->SetCameraRotX( current_camera_degrees_x + 45.f );

      const auto offset = local_player->GetScrollDistance() * 4;

      local_player->SetCameraRotY( 15.f - offset );

      // Wait for the camera to adjust before searching for entities again
      botcore->SkipUpdateForFrames( 35 );

      EntityList entity_list( client );
      const auto& entities = entity_list.GetMoverEntities();

      const auto& whitelisted_player_names_option =
          bot_options.GetOption<WhitelistedPlayerNamesOption>();

      // TODO: Should not require this option-check in here, consider removing
      // it Check for non whitelisted players
      if ( whitelisted_player_names_option.IsEnabled() ) {
        const auto entity_found = IsNonWhitelistedPlayerFound(
            whitelisted_player_names_option, entities, *local_player );

        if ( entity_found ) {
          logging::LogImportant(
              TEXT( "A non-whitelisted player was found, Name: " ) +
              stringutils::AnsiToWide( entity_found->GetName() ) +
              TEXT( "\n" ) );
          SetNextState( OneVsOneStates::kWaitUntilPlayerLeaves );
        } else {
          if ( check_around_counter_ >= 12 ) {
            SetNextState( OneVsOneStates::kFindingTarget );
          }
        }
      } else {
        assert( false && "INVALID AND SHOULD NOT OCCUR" );
      }

      ++check_around_counter_;
    } break;

    case OneVsOneStates::kRebuffCharacter: {
      DO_ONCE( []() {
        logging::Log( TEXT( "Started the character rebuff sequence.\n" ) );
      } );

      KeySequenceReturnValues sequence_status =
          rebuff_sequence_.ExecuteSequence( *current_rebuff_,
                                            simulation_machine_ );

      if ( sequence_status == KeySequenceReturnValues::kFinishedSequence ) {
        logging::Log( TEXT( "The rebuff sequence has finished.\n" ) );

        // Change the start time the current time to reset and "start" the timer
        // now when it has finished
        // character_rebuff_timer_.SetStartTime(GetTickCount64());
        current_rebuff_->ResetRebuffTimer();

        idle_machine_.ResetTimer();

        SetNextState( OneVsOneStates::kFindingTarget );
      }
    } break;

    case OneVsOneStates::kIdle: {
      DO_ONCE( []() {
        logging::Log( TEXT( "Entering idle state to wait for rebuff.\n" ) );
      } );

      const auto idle_status = idle_machine_.Idle();

      if ( idle_status == StateStatusReturnValue::kSucceeded )
        SetNextState( idle_machine_.GetNextState() );
    } break;

    case OneVsOneStates::kCharacterDied: {
      DO_ONCE( []() {
        logging::Log( TEXT( "The bot is currently in a stopped state.\n" ) );
      } );

      SetInternalState( BaseBotStates::Stopped );
      botcore->SetStarted( false );
      botcore->ShowBotHasStoppedWindow();
    } break;

    default:
      break;
  }
}

std::wstring BotAIOneVsOne::BotStoppedLogMessage() {
  std::wstring s;

  s += TEXT( "Total monsters killed: " ) +
       std::to_wstring( monster_kill_count_ ) + TEXT( "\n" );

  // logging::Log( TEXT( "Total monsters killed: " ) +
  //               std::to_wstring( monster_kill_count_ ) + TEXT( "\n" ) );

  s += bot_name_ + TEXT( " running duration (hh:mm:ss:ms): " ) +
       bot_duration_stopwatch_.GetElapsedString() + TEXT( "\n" );

  // logging::Log( bot_name_ + TEXT( " running duration (hh:mm:ss:ms): " ) +
  //              bot_duration_stopwatch_.GetElapsedString() + TEXT( "\n" ) );

  return s;
}

void BotAIOneVsOne::OnStop() {
  logging::Log( BotStoppedLogMessage() );
}

void BotAIOneVsOne::OnStateChanging() {
  Bot::OnStateChanging();

  // Reset the variable here
  has_stopped_running_ = false;
  has_started_running_ = false;
  has_started_standing_still_ = false;
  has_checked_hp_ = false;

  block_check_while_attacking_timer.Reset();
  block_check_while_attacking_timer2.Reset();
  running_target_blocked_timer.Reset();
  blocked_move_forward_timer.Reset();
  wait_until_player_leaves_timer_.Reset();

  if ( current_target_entity_ &&
       !current_target_entity_->IsDeletedOrInvalidMemory() )
    selected_entity_hp_when_started_hitting_ =
        current_target_entity_->GetHealth();
  else
    selected_entity_hp_when_started_hitting_ = 0;

  check_around_counter_ = 0;

  started_walking_backwards_ = false;

  attack_sequence_.Reset();

  simulation_machine_.Reset();

  LogQueue().Notify();
}

}  // namespace bot
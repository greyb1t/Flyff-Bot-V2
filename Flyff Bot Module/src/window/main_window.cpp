#include "pch.h"
#include "main_window.h"
#include "waiting_window.h"
#include "..\bot\bot_initializer.h"
#include "..\bot\entity_list.h"
#include "../utils/string_utls.h"
#include "../build_time.h"
#include "../utils/trace.h"

#include "gwinguiv2/controls/richedit.h"
#include "gwinguiv2/message_box.h"

#include "gwinmem/process_memory_internal.h"
#include "../bot/logging.h"

typedef enum _PROCESSINFOCLASS {
  ProcessBasicInformation,  // q: PROCESS_BASIC_INFORMATION,
                            // PROCESS_EXTENDED_BASIC_INFORMATION
  ProcessQuotaLimits,  // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
  ProcessIoCounters,  // q: IO_COUNTERS
  ProcessVmCounters,  // q: VM_COUNTERS, VM_COUNTERS_EX, VM_COUNTERS_EX2
  ProcessTimes,  // q: KERNEL_USER_TIMES
  ProcessBasePriority,  // s: KPRIORITY
  ProcessRaisePriority,  // s: ULONG
  ProcessDebugPort,  // q: HANDLE
  ProcessExceptionPort,  // s: PROCESS_EXCEPTION_PORT
  ProcessAccessToken,  // s: PROCESS_ACCESS_TOKEN
  ProcessLdtInformation,  // qs: PROCESS_LDT_INFORMATION // 10
  ProcessLdtSize,  // s: PROCESS_LDT_SIZE
  ProcessDefaultHardErrorMode,  // qs: ULONG
  ProcessIoPortHandlers,  // (kernel-mode only)
  ProcessPooledUsageAndLimits,  // q: POOLED_USAGE_AND_LIMITS
  ProcessWorkingSetWatch,  // q: PROCESS_WS_WATCH_INFORMATION[]; s: void
  ProcessUserModeIOPL,
  ProcessEnableAlignmentFaultFixup,  // s: BOOLEAN
  ProcessPriorityClass,  // qs: PROCESS_PRIORITY_CLASS
  ProcessWx86Information,
  ProcessHandleCount,  // q: ULONG, PROCESS_HANDLE_INFORMATION // 20
  ProcessAffinityMask,  // s: KAFFINITY
  ProcessPriorityBoost,  // qs: ULONG
  ProcessDeviceMap,  // qs: PROCESS_DEVICEMAP_INFORMATION,
                     // PROCESS_DEVICEMAP_INFORMATION_EX
  ProcessSessionInformation,  // q: PROCESS_SESSION_INFORMATION
  ProcessForegroundInformation,  // s: PROCESS_FOREGROUND_BACKGROUND
  ProcessWow64Information,  // q: ULONG_PTR
  ProcessImageFileName,  // q: UNICODE_STRING
  ProcessLUIDDeviceMapsEnabled,  // q: ULONG
  ProcessBreakOnTermination,  // qs: ULONG
  ProcessDebugObjectHandle,  // q: HANDLE // 30
  ProcessDebugFlags,  // qs: ULONG
  ProcessHandleTracing,  // q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables,
                         // otherwise enables
  ProcessIoPriority,  // qs: IO_PRIORITY_HINT
  ProcessExecuteFlags,  // qs: ULONG
  ProcessResourceManagement,  // ProcessTlsInformation //
                              // PROCESS_TLS_INFORMATION
  ProcessCookie,  // q: ULONG
  ProcessImageInformation,  // q: SECTION_IMAGE_INFORMATION
  ProcessCycleTime,  // q: PROCESS_CYCLE_TIME_INFORMATION // since VISTA
  ProcessPagePriority,  // q: PAGE_PRIORITY_INFORMATION
  ProcessInstrumentationCallback,  // qs:
                                   // PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION
                                   // // 40
  ProcessThreadStackAllocation,  // s: PROCESS_STACK_ALLOCATION_INFORMATION,
                                 // PROCESS_STACK_ALLOCATION_INFORMATION_EX
  ProcessWorkingSetWatchEx,  // q: PROCESS_WS_WATCH_INFORMATION_EX[]
  ProcessImageFileNameWin32,  // q: UNICODE_STRING
  ProcessImageFileMapping,  // q: HANDLE (input)
  ProcessAffinityUpdateMode,  // qs: PROCESS_AFFINITY_UPDATE_MODE
  ProcessMemoryAllocationMode,  // qs: PROCESS_MEMORY_ALLOCATION_MODE
  ProcessGroupInformation,  // q: USHORT[]
  ProcessTokenVirtualizationEnabled,  // s: ULONG
  ProcessConsoleHostProcess,  // q: ULONG_PTR // ProcessOwnerInformation
  ProcessWindowInformation,  // q: PROCESS_WINDOW_INFORMATION // 50
  ProcessHandleInformation,  // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since
                             // WIN8
  ProcessMitigationPolicy,  // s: PROCESS_MITIGATION_POLICY_INFORMATION
  ProcessDynamicFunctionTableInformation,
  ProcessHandleCheckingMode,  // qs: ULONG; s: 0 disables, otherwise enables
  ProcessKeepAliveCount,  // q: PROCESS_KEEPALIVE_COUNT_INFORMATION
  ProcessRevokeFileHandles,  // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
  ProcessWorkingSetControl,  // s: PROCESS_WORKING_SET_CONTROL
  ProcessHandleTable,  // q: ULONG[] // since WINBLUE
  ProcessCheckStackExtentsMode,
  ProcessCommandLineInformation,  // q: UNICODE_STRING // 60
  ProcessProtectionInformation,  // q: PS_PROTECTION
  ProcessMemoryExhaustion,  // PROCESS_MEMORY_EXHAUSTION_INFO // since THRESHOLD
  ProcessFaultInformation,  // PROCESS_FAULT_INFORMATION
  ProcessTelemetryIdInformation,  // PROCESS_TELEMETRY_ID_INFORMATION
  ProcessCommitReleaseInformation,  // PROCESS_COMMIT_RELEASE_INFORMATION
  ProcessDefaultCpuSetsInformation,
  ProcessAllowedCpuSetsInformation,
  ProcessSubsystemProcess,
  ProcessJobMemoryInformation,  // PROCESS_JOB_MEMORY_INFO
  ProcessInPrivate,  // since THRESHOLD2 // 70
  ProcessRaiseUMExceptionOnInvalidHandleClose,  // qs: ULONG; s: 0 disables,
                                                // otherwise enables
  ProcessIumChallengeResponse,
  ProcessChildProcessInformation,  // PROCESS_CHILD_PROCESS_INFORMATION
  ProcessHighGraphicsPriorityInformation,
  ProcessSubsystemInformation,  // q: SUBSYSTEM_INFORMATION_TYPE // since
                                // REDSTONE2
  ProcessEnergyValues,  // PROCESS_ENERGY_VALUES, PROCESS_EXTENDED_ENERGY_VALUES
  ProcessActivityThrottleState,  // PROCESS_ACTIVITY_THROTTLE_STATE
  ProcessActivityThrottlePolicy,  // PROCESS_ACTIVITY_THROTTLE_POLICY
  ProcessWin32kSyscallFilterInformation,
  ProcessDisableSystemAllowedCpuSets,  // 80
  ProcessWakeInformation,  // PROCESS_WAKE_INFORMATION
  ProcessEnergyTrackingState,  // PROCESS_ENERGY_TRACKING_STATE
  ProcessManageWritesToExecutableMemory,  // MANAGE_WRITES_TO_EXECUTABLE_MEMORY
                                          // // since REDSTONE3
  ProcessCaptureTrustletLiveDump,
  ProcessTelemetryCoverage,
  ProcessEnclaveInformation,
  ProcessEnableReadWriteVmLogging,  // PROCESS_READWRITEVM_LOGGING_INFORMATION
  ProcessUptimeInformation,  // PROCESS_UPTIME_INFORMATION
  ProcessImageSection,  // q: HANDLE
  ProcessDebugAuthInformation,  // since REDSTONE4 // 90
  ProcessSystemResourceManagement,  // PROCESS_SYSTEM_RESOURCE_MANAGEMENT
  ProcessSequenceNumber,  // q: ULONGLONG
  ProcessLoaderDetour,  // since REDSTONE5
  ProcessSecurityDomainInformation,  // PROCESS_SECURITY_DOMAIN_INFORMATION
  ProcessCombineSecurityDomainsInformation,  // PROCESS_COMBINE_SECURITY_DOMAINS_INFORMATION
  ProcessEnableLogging,  // PROCESS_LOGGING_INFORMATION
  ProcessLeapSecondInformation,  // PROCESS_LEAP_SECOND_INFORMATION
  ProcessFiberShadowStackAllocation,  // PROCESS_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION
                                      // // since 19H1
  ProcessFreeFiberShadowStackAllocation,  // PROCESS_FREE_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION
  MaxProcessInfoClass
} PROCESSINFOCLASS;

using tNtSetInformationProcess =
    NTSTATUS( NTAPI* )( IN HANDLE ProcessHandle,
                        IN PROCESSINFOCLASS ProcessInformationClass,
                        IN PVOID ProcessInformation,
                        IN ULONG ProcessInformationLength );

using tNtQueryInformationProcess =
    NTSTATUS( NTAPI* )( IN HANDLE ProcessHandle,
                        IN PROCESSINFOCLASS ProcessInformationClass,
                        OUT PVOID ProcessInformation,
                        IN ULONG ProcessInformationLength,
                        OUT PULONG ReturnLength );

const auto ntdll = LoadLibrary( TEXT( "ntdll.dll" ) );

const auto NtQueryInformationProcess =
    reinterpret_cast<tNtQueryInformationProcess>(
        GetProcAddress( ntdll, "NtQueryInformationProcess" ) );

const auto NtSetInformationProcess = reinterpret_cast<tNtSetInformationProcess>(
    GetProcAddress( ntdll, "NtSetInformationProcess" ) );

NTSTATUS SetDebugFlags( const bool debugging ) {
  ULONG no_debug_inherit = !debugging;

  return NtSetInformationProcess(
      reinterpret_cast<HANDLE>( -1 ), PROCESSINFOCLASS::ProcessDebugFlags,
      &no_debug_inherit, sizeof( no_debug_inherit ) );
}

bool HasDebugFlags() {
  ULONG no_debug_inherit = 0;
  ULONG return_length = 0;

  // Check debugging, it SHOULD be true because we set it earlier
  const NTSTATUS status = NtQueryInformationProcess(
      reinterpret_cast<HANDLE>( -1 ), PROCESSINFOCLASS::ProcessDebugFlags,
      &no_debug_inherit, sizeof( no_debug_inherit ), &return_length );

  // TODO: Figure out what to do if fails
  if ( status != 0 )
    return false;

  if ( return_length != sizeof( no_debug_inherit ) )
    return true;

  return no_debug_inherit == FALSE;
}

bool IsAntiAntiDebugFlagsDetected() {
  // Explicitly enable the debug flags to say that we are being debugged
  SetDebugFlags( true );

  // Is the method returning debug flags that are saying we're debugging?
  // If not, then we know it has been hooked and returning that we are not under
  // a debugger.
  const auto has_debug_flags = HasDebugFlags();

  // Reset the debugger flag for the next call
  SetDebugFlags( false );

  return !has_debug_flags;
}

bool IsDebuggerAttached() {
  // Is a debugger attached?
  if ( HasDebugFlags() )
    return true;

  // If we did not find a debugger attached above, check if an anti anti
  // debugging is present
  if ( IsAntiAntiDebugFlagsDetected() )
    return true;

  return false;
}

namespace bot {

void MainWindow::Initialize( const HWND window_handle ) {
  // std::wstring title = TEXT( "RIMJOB - RIghteously Mighty JOllygood Bot" );
  std::wstring title = TEXT( "RIMJOB" );
  gwingui::control::SetText( GetHandle(), title );

  const auto richedit_log_handle = GWH( RICHEDIT_LOG );
  gwingui::richedit::EnableTextChangedNotification( richedit_log_handle );

  const auto main_tabcontrol_handle = GWH( TAB_MAIN );

  AddTabWindow( main_tabcontrol_handle, DIALOG_TAB1, TEXT( "Bot Settings" ),
                new TabWindow1 );

  AddTabWindow( main_tabcontrol_handle, DIALOG_TAB2, TEXT( "Cheats" ),
                new TabWindow2 );

  AddTabWindow( main_tabcontrol_handle, DIALOG_TAB3, TEXT( "Events" ),
                new TabWindow3 );

  // Should no longer be needed, we'll see tho.
  // gWin::Tabcontrol::SetSelected( TAB_MAIN, 0 );

  std::string compilation_string( MAX_PATH, 0 );

  snprintf( &compilation_string[ 0 ], compilation_string.size(), "%s - %s",
            build_time::BuildDate, build_time::BuildTime );

  const auto compliation_date_handle = GWH( STATIC_COMPILATION_DATE );

  gwingui::control::SetText( compliation_date_handle,
                             stringutils::AnsiToWide( compilation_string ) );

  const auto richedit_feature_desc_handle = GWH( RICHEDIT_FEATURE_DESCRIPTION );
  gwingui::richedit::SetTextItalic( richedit_feature_desc_handle, true );

  // TODO: Replace SetTimer/KillTimer solution with another because the game
  // crashes when you inject the dll the second time to the game
  // NOTE: A better solution would be to just use GetTickCount in the Update()
  // loop
  /*
  if ( !SetTimer( window_handle, kBadboyTimerId, 1000, NULL ) ) {
    gwingui::messagebox::Error( TEXT( "Failed to initialize time." ) );
  } else {
    gwingui::messagebox::Error( std::to_wstring( ( uint32_t )window_handle ) );
  }
  */
}

void MainWindow::OnButtonClick( const HWND button_handle,
                                const uint32_t button_id ) {
  switch ( button_id ) {
    case BUTTON_APPLY_SETTINGS: {
      auto botcore = Initializer().GetBotCore();
      auto& bot_options = botcore->GetBotOptions();
      auto client = botcore->GetFlyffClient();
      auto local_player = client->CreateLocalPlayer();

      if ( local_player->IsDeletedOrInvalidMemory() ) {
        gwingui::messagebox::Error(
            TEXT( "Unable to apply the options, local player is invalid." ) );
        return;
      }

      bot_options.TryApplyOptions( local_player->GetName() );
    } break;

    case BUTTON_HIDE_WINDOW: {
      const auto botcore = Initializer().GetBotCore();
      const auto target_window = botcore->GetTargetWindow();

      if ( IsWindowVisible( target_window ) )
        ShowWindow( target_window, SW_HIDE );
      else
        SetWindowPos( target_window, 0, 0, 0, 0, 0,
                      SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );

      return;

      auto client = botcore->GetFlyffClient();
      auto local_player = client->CreateLocalPlayer();

      if ( local_player->IsDeletedOrInvalidMemory() ) {
        gwingui::messagebox::Error(
            TEXT( "Unable to view the stats, local player is invalid." ) );
        return;
      }

      /*
      bool new_val = !botcore->GetBladeDamageHack();

      if ( new_val )
        logging::Log( TEXT( "BLADE DAMAGE HACK - ON\n" ) );
      else
        logging::Log( TEXT( "BLADE DAMAGE HACK - OFF\n" ) );

      LogQueue().Notify();

      botcore->SetBladeDamageHack( new_val );

      return;
      */

      struct BaseStats {
        int32_t STR;
        int32_t STA;
        int32_t DEX;
        int32_t INT;
      };

      const uint32_t stats_offset = 0x6e4;

      auto selected_entity = local_player->GetSelectedEntity();

      /*
        auto test1 = selected_entity->GetFatigue();
        auto test2 = selected_entity->GetHealth();
        auto test3 = selected_entity->GetLevel();
        auto test4 = selected_entity->GetMana();
        auto test5 = selected_entity->GetMotion();
        auto test6 = selected_entity->GetName();
        auto test7 = selected_entity->GetSpeed();
        auto test8 = selected_entity->GetSpeedMultiplier();
        auto test9 = selected_entity->GetType();
      */

      /*
        const uint32_t visibility_offset = 0x19ec;
        gWin::ProcessInternal().Write(selected_entity->GetPointerAddress() +
        visibility_offset, 1024);

        logging::Log(TEXT("Make invisible nice!\n"));

        static UniquePtrEntity last_selected_entity;

        if (last_selected_entity) {
        logging::Log(TEXT("Made visible again\n"));
        gWin::ProcessInternal().Write(last_selected_entity->GetPointerAddress()
        + visibility_offset, 0);
        }

        last_selected_entity = std::move(selected_entity);

      */
      // TODO: Add a important log
      // TODO: Try changing the visibility of all object to shoot through them
      // Otherwise, try delete them like FlyFF Does SAFFE_DELETE

      // const uint32_t visiblity_offset = 0x19ec;
      //
      // gWin::ProcessInternal().Write(selected_entity->GetPointerAddress() +
      // visiblity_offset, 1024);

      // Ignite Flyff only registers and responds to keypresses when the window
      // is in focus

      // SetForegroundWindow(botcore->GetTargetWindow());

      BaseStats stats = gwinmem::CurrentProcess().Read<BaseStats>(
          selected_entity->GetPointerAddress() + stats_offset );

      const auto health_offset =
          client->GetClientVar( MemoryContants::kHealthOffset );

      int health =
          *( int* )( selected_entity->GetPointerAddress() + health_offset );
      int health2;
      memcpy( &health2,
              ( void* )( selected_entity->GetPointerAddress() + health_offset ),
              sizeof( health2 ) );

      gwingui::messagebox::Error(
          TEXT( "The selected user's base stats:\n" ) TEXT( "PtrAddress: " ) +
          std::to_wstring( selected_entity->GetPointerAddress() ) +
          TEXT( "\n" ) + TEXT( "STR: " ) + std::to_wstring( stats.STR ) +
          TEXT( "\n" ) + TEXT( "STA: " ) + std::to_wstring( stats.STA ) +
          TEXT( "\n" ) + TEXT( "DEX: " ) + std::to_wstring( stats.DEX ) +
          TEXT( "\n" ) + TEXT( "INT: " ) + std::to_wstring( stats.INT ) +
          TEXT( "\n" ) + TEXT( "HP: " ) +
          std::to_wstring( selected_entity->GetHealth() ) + TEXT( "\n" ) +
          TEXT( "HP (Cast): " ) + std::to_wstring( health ) + TEXT( "\n" ) +
          TEXT( "HP (Cast2): " ) + std::to_wstring( health2 ) + TEXT( "\n" ) +
          TEXT( "MP: " ) + std::to_wstring( selected_entity->GetMana() ) +
          TEXT( "\n" ) + TEXT( "FP: " ) +
          std::to_wstring( selected_entity->GetFatigue() ) + TEXT( "\n" ) );

      // selected_entity.GetPointerAddress();
      // int32_t shit = 70;
      // gWin::ProcessInternal().Write(selected_entity.GetPointerAddress() +
      // 0x760, shit);
    } break;

    case BUTTON_TOGGLEBOT: {
      const auto botcore = Initializer().GetBotCore();

      botcore->ToggleBot();
    } break;

    case BTN_TEST: {
      //       const auto botcore = Initializer().GetBotCore();
      //       const auto client = botcore->GetFlyffClient();
      //       const auto local_player = client->CreateLocalPlayer();
      //
      //       const auto selected_entity = local_player->GetSelectedEntity();
      //
      //       gwingui::messagebox::Warning(
      //           std::to_wstring( selected_entity->GetMotion() ) );

      static bool lol = []() -> bool {
        __try {
          *( int* )0 = 0;
        } __except ( 1 ) {
          MessageBox( 0, TEXT( "worked" ), TEXT( "wporked" ), 0 );
        }

        return true;
      }();

      try {
        throw std::exception( "hello, std::exception it worked" );
      } catch ( std::exception& e ) {
        MessageBoxA( 0, e.what(), e.what(), 0 );
      }

      const auto botcore = Initializer().GetBotCore();
      const auto client = botcore->GetFlyffClient();

      EntityList entity_list( client );
      auto& mover_entities = entity_list.GetMoverEntities();

      bool gm_found = false;
      std::string name;
      int32_t auth = 0;

      for ( const auto& entity : mover_entities ) {
        auth = gwinmem::CurrentProcess().Read<int32_t>(
            entity->GetPointerAddress() + 0x760 );

        if ( entity->GetName() == "Hope" || ( auth > 70 && auth <= 90 ) ) {
          gm_found = true;
          name = entity->GetName();
          break;
        }
      }

      if ( gm_found )
        gwingui::messagebox::Error( TEXT( "A GM WAS FOUND Auth: " ) +
                                    std::to_wstring( auth ) +
                                    stringutils::AnsiToWide( name ) );
      else
        gwingui::messagebox::Error(
            TEXT( "NOTHING WAS FOUND, GM NOT VISIBLE FUCK SHIT DAMN" ) );

      const auto address_window_handle = GWH( DIALOG_ADDRESSES );
      ShowWindow( address_window_handle, SW_SHOW );

      /*
        auto botcore = Initializer().GetBotCore();
        auto client = botcore->GetFlyffClient();
        auto entitylist = bot::entitylist::ReadEntityList(client);
        auto local_player = client->CreateLocalPlayer();

        auto selected_entity = local_player->GetSelectedEntity();

        if (selected_entity.IsMonster()) {
        selected_entity.SetPosition(local_player->GetPosition());
        }
      */

      // for (auto &entity : entitylist) {
      //  if (entity->IsMonster()) {
      //    if (entity->GetPosition() != local_player->GetPosition()) {
      //      auto entity_pos = entity->GetPosition();
      //      entity->SetPosition(local_player_position);
      //      break;
      //    }
      //  }
      //}

      // TODO: Test the idea I had with teleporting monster to me and back again
      // see if it crashes
    } break;

    default: {
      break;
    }
  }
}

INT_PTR CALLBACK MainWindow::DialogProc( HWND window_handle,
                                         UINT message,
                                         WPARAM wparam,
                                         LPARAM lparam ) {
  switch ( message ) {
    case WM_TIMER: {
      if ( wparam == 0x65 ) {
        // A timer event that goes off every 1000 ms, created by SetTimer
        auto botcore = Initializer().GetBotCore();
        botcore->SetIsDebuggerAttached( IsDebuggerAttached() );
      }
    } break;

    case WM_CLOSE: {
      // TODO: Replace SetTimer/KillTimer solution with another because the game
      // crashes when you inject the dll the second time to the game
      /*
      if ( !KillTimer( window_handle, kBadboyTimerId ) ) {
        gwingui::messagebox::Error( TEXT( "killing badboy timer error." ) );
      } else {
        gwingui::messagebox::Error(
            std::to_wstring( ( uint32_t )window_handle ) );
      }
      */

      auto botcore = Initializer().GetBotCore();

      if ( botcore->GetStarted() ) {
        gwingui::messagebox::Warning(
            TEXT( "Please stop the bot before closing the bot." ) );
        return 1;
      }

      // WM_QUIT makes GetMessage return 0 and exit the message loop
      SendMessage( window_handle, WM_QUIT, 0, 0 );

      const auto local_player = botcore->GetFlyffClient()->CreateLocalPlayer();

      if ( local_player ) {
        if ( !local_player->IsDeletedOrInvalidMemory() ) {
          const auto name = local_player->GetName();

          if ( name != "" )
            botcore->GetBotOptions().SaveOptions( name );
        }
      }

      DestroyWindow( window_handle );
    } break;

    case WM_DESTROY: {
      PostQuitMessage( 0 );
    } break;

    default:
      break;
  }

  return FALSE;
}

void MainWindow::OnRichEditTextChanged( uint32_t ctrlId, CHANGENOTIFY* pCn ) {
  if ( ctrlId == RICHEDIT_LOG || ctrlId == RICHEDIT_LOG_IMPORTANT )
    gwingui::richedit::ScrollToBottom( GWH( ctrlId ) );
}

}  // namespace bot
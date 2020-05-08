#include "pch.h"
#include "flyff_client_eclipse.h"
#include "gwinmem/process_memory_internal.h"
#include "../bot/bot_initializer.h"

/*
  kD3dVec3ProjectAddress: 0x6c2006e6
  kPlayerBaseAddress: 0xa7b788
  kScrollDistanceAddress: 0xb11aa0
  kAccountNameAddress: 0xa06728
  kSelectedEntityAddress: 0xb18f9c
  kEntityListAddress: 0xab8fb0
  kMovementOffset: 0x38c
  kModelOffset: 0x170
  kBoundBoxOffset: 0xc
  kWorldMatrixOffset: 0xe8
  kPositionOffset: 0x190
  kHealthOffset: 0x770
  kManaOffset: 0x774
  kFatigueOffset: 0x778
  kLevelOffset: 0x734
  kNameOffset: 0x1b84
  kObjectFlagsOffset: 0x4
  kSelectedEntityOffset: 0x20
  kSpeedMultiplierOffset: 0x8C0
  kMoveOffset: 0x4
  kObjectTypeOffset: 0x168
  kCameraAddress: 0xb119a4
  kProjectionMatrixAddress: 0xaf0e00
  kViewMatrixAddress: 0xb119b4
*/
HHOOK bot::FlyffClientEclipseFlyff::wnd_proc_hook_handle_ = 0;

bot::FlyffClientEclipseFlyff::FlyffClientEclipseFlyff()
    : FlyffClient( TEXT( "Neuz.exe" ) ) {
  AddSearchFunction( MemoryContants::kD3dVec3ProjectAddress, [=]() {
    const auto d3dx_handle = GetModuleHandle( TEXT( "d3dx9_43.dll" ) );

    return reinterpret_cast<uint32_t>(
        GetProcAddress( d3dx_handle, "D3DXVec3Project" ) );
  } );

  AddSearchFunction( MemoryContants::kPlayerBaseAddress, [=]() {
    uint32_t addr =
        pattern_matcher_.FindIdaSignature( "A1 ? ? ? ? 75 08 3B C7" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 1 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kScrollDistanceAddress, [=]() {
    uint32_t addr = pattern_matcher_.FindIdaSignature(
        "C7 05 ? ? ? ? ? ? ? ? 80 3D ? ? ? ? ? 74 55" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kAccountNameAddress, [=]() {
    uint32_t addr = pattern_matcher_.FindIdaSignature(
        "BA ? ? ? ? 8B ? ? ? ? ? 8A 01 8D 49 01" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 1 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kSelectedEntityAddress, [=]() {
    uint32_t addr = pattern_matcher_.FindIdaSignature(
        "8B ? ? ? ? ? 85 F6 74 4A 8B 56 20" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kEntityListAddress, [=]() {
    uint32_t addr = pattern_matcher_.FindIdaSignature(
        "8B ? ? ? ? ? ? 85 F6 0F 84 ? ? ? ? 83 BF" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 3 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kMovementOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindIdaSignature(
        "8B ? ? ? ? ? F7 40 ? ? ? ? ? 75 4E" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kModelOffset, [=]() { return 0x170; } );

  AddSearchFunction( MemoryContants::kBoundBoxOffset, [=]() { return 0xc; } );

  AddSearchFunction( MemoryContants::kWorldMatrixOffset,
                     [=]() { return 0xe8; } );

  AddSearchFunction( MemoryContants::kPositionOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindIdaSignature(
        "F3 ? ? ? ? ? ? ? 8B ? ? ? ? ? 6A 01 83 EC 0C 8B CC 66 0F D6 01 89 41 "
        "08 8B CE" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 4 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kHealthOffset, [=]() {
    uint32_t addr =
        pattern_matcher_.FindIdaSignature( "8B ? ? ? ? ? 0B ? ? ? ? ? 74 40" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kManaOffset, [=]() {
    uint32_t addr =
        pattern_matcher_.FindIdaSignature( "8B ? ? ? ? ? 0B ? ? ? ? ? 74 40" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset + 0x4;
  } );

  AddSearchFunction( MemoryContants::kFatigueOffset, [=]() {
    uint32_t addr =
        pattern_matcher_.FindIdaSignature( "8B ? ? ? ? ? 0B ? ? ? ? ? 74 40" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset + 0x8;
  } );

  AddSearchFunction( MemoryContants::kLevelOffset, [=]() {
    uint32_t addr =
        pattern_matcher_.FindIdaSignature( "FF B0 ? ? ? ? 8D 45 F0" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kNameOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindIdaSignature(
        "8D ? ? ? ? ? 8D ? ? ? ? ? 8B C8 2B D0" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kObjectFlagsOffset,
                     [=]() { return 0x4; } );

  AddSearchFunction( MemoryContants::kSelectedEntityOffset, [=]() {
    return ( uint32_t )0x20;

    uint32_t addr =
        pattern_matcher_.FindIdaSignature( "8B 56 20 8B C2 8B 4E 24" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  // Find a steady offset by logging in and out while finding out what is
  // accessing the value
  AddSearchFunction( MemoryContants::kSpeedMultiplierOffset, [=]() {
    /*
      0042282E - Neuz.exe+2282E (+4)
      F3 ? ? ? ? ? ? ? 0F 2F ? ? ? ? ? 76 7A

      004224C8 - Neuz.exe+224C8 (+4) (best?)
      F3 ? ? ? ? ? ? ? 83 F8 05 0F 95 C1

      004A0910 - Neuz.exe+A0910 (+4)
      F3 ? ? ? ? ? ? ? 0F 57 C9 F3 0F 59 D0

    */

    //uint32_t addr = pattern_matcher_.FindIdaSignature(
    //    "F3 ? ? ? ? ? ? ? F3 0F 59 C8 F3 ? ? ? ? ? ? ? 0F 2E" );

    uint32_t addr = pattern_matcher_.FindIdaSignature(
        "F3 ? ? ? ? ? ? ? 83 F8 05 0F 95 C1" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 4 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kMoveOffset, [=]() { return 0x4; } );

  AddSearchFunction( MemoryContants::kObjectTypeOffset, [=]() {
    uint32_t addr =
        pattern_matcher_.FindIdaSignature( "8B ? ? ? ? ? 8B 4D 28" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );
}

LRESULT bot::FlyffClientEclipseFlyff::CallWndProcHook( int code,
                                                       WPARAM wParam,
                                                       LPARAM lParam ) {
  auto botcore = Initializer().GetBotCore();

  if ( code < 0 )
    CallNextHookEx( wnd_proc_hook_handle_, code, wParam, lParam );

  if ( code == HC_ACTION ) {
    CWPSTRUCT* cwp = reinterpret_cast<CWPSTRUCT*>( lParam );

    switch ( cwp->message ) {
      case WM_KILLFOCUS: {
        SendMessage( botcore->GetTargetWindow(), WM_ACTIVATE, WA_ACTIVE, 0 );
      } break;

      default:
        break;
    }
  }

  return CallNextHookEx( 0, code, wParam, lParam );
}

void bot::FlyffClientEclipseFlyff::PreAddressSearch() {
  /*
    Eclipse Flyff has protection against keypresses that is not 1-9 or F-keys
    when the window is not in focus. They detect if the window is in focus or
    not by handling WM_ACTIVATE I detect when the window goes out of focus and
    then send a message with WM_ACTIVATE to say we are in focus again but in
    reality we are not.
  */

  // Do it once to avoid having window out of focus when it never gets into
  // focus
  auto botcore = Initializer().GetBotCore();
  SendMessage( botcore->GetTargetWindow(), WM_ACTIVATE, WA_ACTIVE, 0 );

  const auto target_window = botcore->GetTargetWindow();

  assert( target_window );

  uint32_t window_thread_id = GetWindowThreadProcessId( target_window, 0 );

  wnd_proc_hook_handle_ = SetWindowsHookEx( WH_CALLWNDPROC, CallWndProcHook,
                                            NULL, window_thread_id );

  if ( !wnd_proc_hook_handle_ ) {
    gwingui::messagebox::Error( TEXT( "SetWindowsHookEx failed" ) );
  }
}
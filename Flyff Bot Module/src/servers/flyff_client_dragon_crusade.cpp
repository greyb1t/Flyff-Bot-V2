#include "pch.h"
#include "flyff_client_dragon_crusade.h"
#include "../bot/bot_initializer.h"

#include "gwinmem/process_memory_internal.h"

#include "gwinguiv2/message_box.h"

bot::FlyffClientDragonCrusade::FlyffClientDragonCrusade()
    : FlyffClient( TEXT( "data.dll" ) ) {
  AddSearchFunction( MemoryContants::kD3dVec3ProjectAddress, [=]() {
    /*
      How to get this value:
      Follow the instructions in the file on desktop (Flyff Matrix.txt) to get
      the address of D3DXVec3Project
    */

    // The patterns for Ignite Flyff. Seems to be using a newer version of
    // D3dx9.lib
    /*
    int addr = pattern_matcher_.FindHexPattern(
        "\x55\x8B\xEC\x83\xE4\xF0\x83\xEC\x44\x8B\x4D\x1C\x33\xC0\x85",
        "xxxxxxxxxxxxxxx" );

    return addr;
    */

    const auto d3dx_handle = GetModuleHandle( TEXT( "d3dx9_43.dll" ) );
    return reinterpret_cast<uint32_t>(
        GetProcAddress( d3dx_handle, "D3DXVec3Project" ) );
  } );

  AddSearchFunction( MemoryContants::kPlayerBaseAddress, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x00\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00"
        "\x85\xC0\x0F\x84\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\xE8\x00\x00"
        "\x00\x00\x83\xB8",
        "x?????x?????x????xxxx????x?????x????xx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kScrollDistanceAddress, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\xF3\x00\x00\x00\x00\x00\x00\x00\x0F\x2F\x00\x00\x00\x00\x00\x76\x33"
        "\xC7\x45\x00\x00\x00\x00\x00\x68\x00\x00\x00\x00\x8B\x4D\xF4\xE8\x00"
        "\x00\x00\x00\x89\x45\xFC",
        "x???????xx?????xxxx?????x????xxxx????xxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 4 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kAccountNameAddress, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x08\x83\x3D\x00\x00"
        "\x00\x00\x00",
        "x????x????xxxxx?????" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 1 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kSelectedEntityAddress, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x89\x00\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B"
        "\x4D\xC8",
        "x?????x????x????xxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return address;
  } );

  /*
    Finding Entity List Address on Ignite:
    Search for the string ClosestPlayer in IDA.
    That function iterates the entity list for entities.
  */
  AddSearchFunction( MemoryContants::kEntityListAddress, [=]() {
    /*
      int addr = pattern_matcher_.FindHexPattern(
      "\x8B\x34\xBD\x00\x00\x00\x00\x66\x39\x86\xC0\x02\x00\x00",
      "xxx????xxxxxxx");
    */

    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x00\x00\x00\x00\x00\x00\x89\x4D\xC0\x83\x7D\xC0\x00",
        "x??????xxxxxxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 3 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kMovementOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x88\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x75\x15\x8B\x4D"
        "\xFC",
        "xx????x????xxxxxxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kCollisionAddress, [=]() {
    return static_cast<uint32_t>( 1 );

    // TODO: Won't work after the function has been patched

    // IF IT STOPS WORKING, TRY THE SIGNATURE BELOW SINCE update 2.2.0
    // I make the jmp's ? instead of x
    // \x81\x3D\x00\x00\x00\x00\x00\x00\x00\x00\x74\x66\x85\xC0\x74\x62
    // xx??????????xx??

    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x0D\x00\x00\x00\x00\x51\x8B\x4D\xC0\xE8\x00\x00\x00\x00\x68\x00"
        "\x00\x00\x00\x8B\x4D\xBC",
        "xx????xxxxx????x????xxx" );

    ////uint32_t addr2 = pattern_matcher_.FindHexPattern(
    ////    "\x83\x3D\x00\x00\x00\x00\x00\x90\x90\x90\x0F\x57\xED",
    ////    "xx?????xxxxx?" );

    ////// If the first pattern is invalid, the user has attached the bot more
    ////// than two times to the same client because the bot has already
    /// patched
    ////// the collision. So we have to use another pattern for the patched
    ////// memory.
    ////if ( IsAddressInvalid( addr ) )
    ////  addr = addr2;

    // collision_check_addr_ = addr;

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kModelOffset, [=]() { return 0x188; } );

  AddSearchFunction( MemoryContants::kBoundBoxOffset, [=]() { return 0xc; } );

  AddSearchFunction( MemoryContants::kWorldMatrixOffset,
                     [=]() { return 0xe4 + 0x4; } );

  AddSearchFunction( MemoryContants::kPositionOffset, [=]() {
    return uint32_t( 0x170 );

    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\xF3\x00\x00\x00\x00\x00\x00\x00\x50\xE8\x00\x00\x00\x00\x8B\x00\x00"
        "\x00\x00\x00\x0F\xB6\x00",
        "x???????xx????x?????xxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 4 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kSpeedOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x84\x81\x00\x00\x00\x00\xEB\x02\x33\xC0\x8B\xE5\x5D\xC2\x04\x00",
        "xxx????xxxxxxxxxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 3 );

    offset = offset + ( 0xb * 4 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kHealthOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x88\x00\x00\x00\x00\x51\x6A\x27", "xx????xxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset - 0x4;
  } );

  AddSearchFunction( MemoryContants::kManaOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x88\x00\x00\x00\x00\x51\x6A\x27", "xx????xxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kFatigueOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x88\x00\x00\x00\x00\x51\x6A\x27", "xx????xxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset + 0x4;
  } );

  AddSearchFunction( MemoryContants::kLevelOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x88\x00\x00\x00\x00\x83\xC1\x01\xC1\xE1\x05\x8B\x81\x00\x00\x00"
        "\x00\x8B\x91\x00\x00\x00\x00\x8B\xE5",
        "xx????xxxxxxxx????xx????xx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kNameOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x0F\xBE\x00\x00\x00\x00\x00\x00\x85\xC0\x75\x19", "xx??????xxxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 4 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kObjectFlagsOffset,
                     [=]() { return 0x8; } );

  AddSearchFunction( MemoryContants::kSelectedEntityOffset,
                     [=]() { return 0x20; } );

  // Find a steady offset by logging in and out while finding out what is
  // accessing the value
  AddSearchFunction( MemoryContants::kSpeedMultiplierOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\xD9\x80\x00\x00\x00\x00\x8B\xE5\x5D\xC3", "xx????xxxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kMoveOffset, [=]() { return 0x4; } );

  AddSearchFunction( MemoryContants::kObjectTypeOffset, [=]() {
    return uint32_t( 0x180 );

    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x83\xB9\x00\x00\x00\x00\x00\x75\x29", "xx?????xx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kGetCmdParamAddress, [=]() {
    // The unchanged bytes pattern
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\xE8\x00\x00\x00\x00\x89\x00\x00\x00\x00\x00\x6A\x01\x8B\x00\x00\x00"
        "\x00\x00\xE8\x00\x00\x00\x00\x89\x00\x00\x00\x00\x00\x6A\x02",
        "x????x?????xxx?????x????x?????xx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t relative_call_offset =
        gwinmem::CurrentProcess().Read<uint32_t>( addr + 1 );

    return relative_call_offset + addr + 5;
  } );

  AddSearchFunction( MemoryContants::kIsAggroOffset, [=]() {
    uint32_t addr =
        pattern_matcher_.FindIdaSignature( "83 B9 ? ? ? ? ? 74 36 6A 00" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );
}

void bot::FlyffClientDragonCrusade::PreAddressSearch() {}

void bot::FlyffClientDragonCrusade::OnExit() {}

void bot::FlyffClientDragonCrusade::PostAddressSearch() {}

void bot::FlyffClientDragonCrusade::PreHookInitialization() {}

inline void bot::FlyffClientDragonCrusade::PostHookDeinitialization() {}

void bot::FlyffClientDragonCrusade::PostHookInitialization() {}

void bot::FlyffClientDragonCrusade::PreHookDeInitialization() {}

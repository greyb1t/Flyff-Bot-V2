#pragma once

#include "flyff_client.h"

#include "gwinmem/process_memory_internal.h"

namespace bot {

class FlyffClientMazeyFlyff : public FlyffClient {
 public:
  FlyffClientMazeyFlyff() : FlyffClient( TEXT( "Neuz.exe" ) ) {
    AddSearchFunction( MemoryContants::kD3dVec3ProjectAddress, [=]() {
      /*
        How to get this value:
        Follow the instructions in the file on desktop (Flyff Matrix.txt) to get
        the address of D3DXVec3Project
      */

      // A pattern that is unique for Origin Flyff only
      int addr3 = pattern_matcher_.FindHexPattern(
          "\xFF\x25\x00\x00\x00\x00\x55\x8B\xEC\x6A\x01\xE8\x00\x00\x00\x00\x5D"
          "\xFF\x25\x00\x00\x00\x00\xFF\x25\x00\x00\x00\x00\x55\x8B\xEC\xD9"
          "\xE8",
          "xx????xxxxxx????xxx????xx????xxxxx" );

      // The pattern for D3DXVec3Project for all Flyffs with D3D9 static link
      // When statically linked, the source of the function gets copied into the
      // exe statically. This is the pattern for that. I found it by following
      // the call to the function in a debugger to find the vtable call.
      int addr = pattern_matcher_.FindHexPattern(
          "\x53\x8B\xDC\x51\x51\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C"
          "\x24\x04\x8B\xEC\x83\xEC\x58\x56\x8B",
          "xxxxxxxxxxxxxxxxxxxxxxxxxx" );

      // The patterns for Ignite Flyff. Seems to be using a newer version of
      // D3dx9.lib
      int addr2 = pattern_matcher_.FindHexPattern(
          "\x55\x8B\xEC\x83\xE4\xF0\x83\xEC\x44\x8B\x4D\x1C\x33\xC0\x85",
          "xxxxxxxxxxxxxxx" );

      return addr;
    } );

    AddSearchFunction( MemoryContants::kPlayerBaseAddress, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\xA1\x00\x00\x00\x00\x5D\xC3\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x51"
          "\x89\x4D\xFC\x8B\x45\xFC\xC7\x00\x00\x00\x00\x00\x8B\x45\xFC",
          "x????xxxxxxxxxxxxxxxxxxxx????xxx" );

      uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 1 );

      return address;
    } );

    AddSearchFunction( MemoryContants::kScrollDistanceAddress, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\xD9\x05\x00\x00\x00\x00\xD8\x0D\x00\x00\x00\x00\xD8\x2D\x00\x00\x00"
          "\x00"
          "\xD8\x1D\x00\x00\x00\x00\xDF\xE0\xF6\xC4\x05\x7A\x18\xD9\x05\x00\x00"
          "\x00"
          "\x00\xD8\x0D\x00\x00\x00\x00\xD8\x2D\x00\x00\x00\x00\xD9\x1D\x00\x00"
          "\x00"
          "\x00\xD9\x05\x00\x00\x00\x00\xD8\x1D\x00\x00\x00\x00\xDF\xE0\xF6\xC4"
          "\x05"
          "\x7A\x0A\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xEB\x27",
          "xx????xx????xx????xx????xxxxxxxxx????xx????xx????xx????xx????xx????"
          "xxxxxxxxx????????xx" );

      uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

      return address;
    } );

    AddSearchFunction( MemoryContants::kAccountNameAddress, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x08\x83\x3D\x00\x00"
          "\x00\x00\x00",
          "x????x????xxxxx?????" );

      uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 1 );

      return address;
    } );

    AddSearchFunction( MemoryContants::kSelectedEntityAddress, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x51",
          "x????x????xxxx" );

      uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 1 );

      return address + 0x10;
    } );

    AddSearchFunction( MemoryContants::kEntityListAddress, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\x8B\x0C\x85\x00\x00\x00\x00\x89\x4D\xC0\x83\x7D\xC0\x00\x74\x43",
          "xxx????xxxxxxxxx" );

      uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 3 );

      return address;
    } );

    AddSearchFunction( MemoryContants::kMovementOffset, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\x8B\x8A\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x19",
          "xx????x????xxxx" );

      uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

      return offset;
    } );

    AddSearchFunction( MemoryContants::kCollisionAddress, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\x8B\x0D\x00\x00\x00\x00\x51\x8B\x4D\xC0\xE8\x00\x00\x00\x00\x68\x00"
          "\x00\x00\x00\x8B\x4D\xBC\xE8\x00\x00\x00\x00\x89\x45\xC0",
          "xx????xxxxx????x????xxxx????xxx" );

      uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

      return address;
    } );

    AddSearchFunction( MemoryContants::kModelOffset, [=]() { return 0x17c; } );

    AddSearchFunction( MemoryContants::kBoundBoxOffset, [=]() { return 0xc; } );

    AddSearchFunction( MemoryContants::kWorldMatrixOffset,
                       [=]() { return 0xe4; } );

    AddSearchFunction( MemoryContants::kPositionOffset,
                       [=]() { return 0x164; } );

    AddSearchFunction( MemoryContants::kSpeedOffset, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\x8B\x84\x81\x00\x00\x00\x00\xEB\x02\x33\xC0\x8B\xE5\x5D\xC2\x04"
          "\x00",
          "xxx????xxxxxxxxxx" );

      uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 3 );

      return offset + ( 0xb * 4 );
    } );

    AddSearchFunction( MemoryContants::kHealthOffset, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\x8B\x88\x00\x00\x00\x00\x51\x6A\x26\x8B\x4D\xFC", "xx????xxxxxx" );

      uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

      return offset;
    } );

    AddSearchFunction( MemoryContants::kManaOffset, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\x8B\x88\x00\x00\x00\x00\x51\x6A\x27\x8B\x4D\xFC\xE8\x00\x00\x00"
          "\x00",
          "xx????xxxxxxx????" );

      uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

      return offset;
    } );

    AddSearchFunction( MemoryContants::kFatigueOffset, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\x8B\x88\x00\x00\x00\x00\x51\x6A\x27\x8B\x4D\xFC\xE8\x00\x00\x00"
          "\x00",
          "xx????xxxxxxx????" );

      uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

      return offset + 0x4;
    } );

    AddSearchFunction( MemoryContants::kLevelOffset, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\xDB\x80\x00\x00\x00\x00\x8B\x4D\xF0\xD8\x49\x04\xD8\x35\x00\x00\x00"
          "\x00",
          "xx????xxxxxxxx????" );

      uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

      return offset;
    } );

    AddSearchFunction( MemoryContants::kNameOffset, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\x0F\xBE\x88\x80\x17\x00\x00\x85\xC9\x75\x18", "xxxx?xxxxxx" );

      uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 3 );

      return offset;
    } );

    AddSearchFunction( MemoryContants::kObjectFlagsOffset, [=]() { return 0x4; } );

    AddSearchFunction( MemoryContants::kSelectedEntityOffset,
                       [=]() { return 0x20; } );

    AddSearchFunction( MemoryContants::kSpeedMultiplierOffset, [=]() {
      int addr = pattern_matcher_.FindHexPattern(
          "\xD9\x80\x00\x00\x00\x00\x8B\xE5\x5D\xC3", "xx????xxxx" );

      uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

      return offset;
    } );

    AddSearchFunction( MemoryContants::kMoveOffset, [=]() { return 0x4; } );
  }

  ~FlyffClientMazeyFlyff() {}
};

}  // namespace bot
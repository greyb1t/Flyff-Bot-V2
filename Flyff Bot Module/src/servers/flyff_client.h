#pragma once

#include "..\res\resource.h"
#include "..\bot\entity.h"

#include "../bot/hook_manager.h"

#include "gwinmem/pattern_searcher.h"

enum class MemoryContants {
  // Addresses
  kGetCmdParamAddress,
  kD3dVec3ProjectAddress,
  kProjectionMatrixAddress,
  kViewMatrixAddress,
  kCameraAddress,
  kPlayerBaseAddress,
  kScrollDistanceAddress,
  kAccountNameAddress,
  kSelectedEntityAddress,
  kEntityListAddress,
  kCollisionAddress,
  // kSendMeleeAttackAddress,
  // kDoAttackMeleeAddress,

  // Offsets
  kMovementOffset,
  kModelOffset,
  kBoundBoxOffset,
  kWorldMatrixOffset,
  kPositionOffset,
  kSpeedOffset,
  kHealthOffset,
  kManaOffset,
  kFatigueOffset,
  kLevelOffset,
  kNameOffset,
  kObjectFlagsOffset,
  kSelectedEntityOffset,
  kSpeedMultiplierOffset,
  kMoveOffset,
  kObjectTypeOffset,  // Read about no cast delay in

  // kIsAggroOffset is used to find if a monster is aggro (red name). The offset
  // is located about 40 bytes below auth value in the player structure.
  kIsAggroOffset,

  // Specialized Offsets
  kIgniteEntityIdOffset
};

namespace bot {

class LocalPlayer;
class Entity;

class FlyffClient {
 public:
  FlyffClient( const std::wstring& module_name );

  ~FlyffClient() {}

  // Searches for the memory patterns to find the memory memory contants
  // addresses and offset
  bool Search( HWND loading_window_handle );

  void InitializeDefaultHooks();
  void RemoveDefaultHooks();

  virtual bool Initialize() {
    return true;
  }

  virtual void PreAddressSearch() {}
  virtual void PostAddressSearch() {}
  virtual void OnExit() {}
  virtual void PreHookInitialization() {}
  virtual void PostHookInitialization() {}
  virtual void PreHookDeInitialization() {}
  virtual void PostHookDeinitialization() {}

  // Converts a memory contant to a readable string
  std::wstring MemoryConstantToString( MemoryContants constant );

  // Create the local player for the specified server
  virtual std::unique_ptr<LocalPlayer> CreateLocalPlayer();

  // Create the entity for the specified server
  virtual std::unique_ptr<Entity> CreateEntity( uint32_t address_ptr );

  void SetServerVar( MemoryContants type, uint32_t value );

  uint32_t GetClientVar( MemoryContants type ) {
    return server_addresses_[ type ];
  }

  std::wstring GetModuleName() const {
    return module_name_;
  }

  HookManager* GetHookManager();

 protected:
  void AddSearchFunction( MemoryContants value_type,
                          std::function<uint32_t()> search_func );
  bool IsAddressInvalid( uint32_t addr );

  gwinmem::PatternSearcher pattern_matcher_;
  std::wstring module_name_;
  std::wstring window_identification_;

  HookManager hook_manager_;

 private:
  std::map<MemoryContants, uint32_t> server_addresses_;
  std::map<MemoryContants, std::function<uint32_t()>>
      server_addresses_finder_functions_;
};

}  // namespace bot
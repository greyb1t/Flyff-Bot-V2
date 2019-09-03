#include "pch.h"
#include "hook_manager.h"

#include "gwinguiv2/message_box.h"

void HookManager::Begin() {
  auto result = DetourTransactionBegin();

  if ( result != NO_ERROR ) {
    throw std::runtime_error( "DetourTransactionBegin error code: " +
                              std::to_string( result ) );
  }

  result = DetourUpdateThread( GetCurrentThread() );

  if ( result != NO_ERROR ) {
    throw std::runtime_error( "DetourUpdateThread error code: " +
                              std::to_string( result ) );
  }
}

void HookManager::End() {
  const auto result = DetourTransactionCommit();

  if ( result != NO_ERROR ) {
    throw std::runtime_error( "DetourTransactionCommit error code: " +
                              std::to_string( result ) );
  }
}

void HookManager::AddHook( const HookType type, Hook& hook ) {
  // Add it immediately to allocate memory for it in the map
  // this way the DetourAttach uses that memory location when
  // DetourTransactionCommit is called to update original_function variable
  auto& newly_added_hook_ref =
      hooks_.insert( std::make_pair( type, hook ) ).first->second;

  const auto result = DetourAttach(
      reinterpret_cast<PVOID*>( &newly_added_hook_ref.original_function ),
      reinterpret_cast<PVOID>( newly_added_hook_ref.hook_callback ) );

  if ( result != NO_ERROR ) {
    throw std::runtime_error( "DetourAttach hook type: " +
                              std::to_string( static_cast<uint32_t>( type ) ) +
                              " with error code: " + std::to_string( result ) );

    // remove it from the list if we failed
    hooks_.erase( type );
  }
}

void HookManager::RemoveHook( const HookType type ) {
  auto& hook = hooks_.at( type );

  const auto result =
      DetourDetach( reinterpret_cast<PVOID*>( &hook.original_function ),
                    reinterpret_cast<PVOID>( hook.hook_callback ) );

  if ( result != NO_ERROR ) {
    throw std::runtime_error( "DetourDetach hook type: " +
                              std::to_string( static_cast<uint32_t>( type ) ) +
                              " with error code: " + std::to_string( result ) );
  }
}

const Hook& HookManager::GetHook( const HookType type ) const {
  try {
    return hooks_.at( type );
  } catch ( ... ) {
    gwingui::messagebox::Error( TEXT( "The hook type was not found" ) );
    ExitProcess( -1 );
    return {};
  }
}

#include "pch.h"
#include "software_bp_hook.h"
#include "trace.h"

#include "gwinmem/process_memory_internal.h"
#include "../bot/crash_handler.h"

std::map<uint32_t, HookData> hooks_;

LONG software_bp_hook::BreakpointExceptionHandler(
    EXCEPTION_POINTERS* ex_info ) {
  if ( ex_info->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT ) {
    for ( auto it = hooks_.rbegin(); it != hooks_.rend(); it++ ) {
      const auto& hook_entry = *it;
      const auto& hook_data = hook_entry.second;

      const auto exception_addr = reinterpret_cast<uintptr_t>(
          ex_info->ExceptionRecord->ExceptionAddress );

      if ( exception_addr == hook_data.address_to_hook ) {
        hook_data.callback( ex_info, hook_data );

        // Unhook to reset the instruction to make the rest of the instruction flow correct
        software_bp_hook::UnHook( hook_entry.first );

        return EXCEPTION_CONTINUE_EXECUTION;
      }
    }
  }

  // I previously used EXCEPTION_CONTINUE_SEARCH
  // But that caused bad stuff in my shit
  // return EXCEPTION_CONTINUE_SEARCH;

  return EXCEPTION_CONTINUE_EXECUTION;
}

PVOID exception_handler_handle = 0;

void software_bp_hook::RegisterExceptionHandler() {
  // NOTE: i am currently using my global exception handler and calling the above exception handler in that
  /*
  exception_handler_handle = AddVectoredExceptionHandler(
      1, reinterpret_cast<PVECTORED_EXCEPTION_HANDLER>(
             BreakpointExceptionHandler ) );
  */
}

void software_bp_hook::UnRegisterExceptionHandler() {
  if ( exception_handler_handle ) {
    RemoveVectoredExceptionHandler( exception_handler_handle );
  }
}

void software_bp_hook::Hook( const uint32_t hook_id,
                             const uintptr_t address,
                             bp_callback_t callback,
                             void* custom_data ) {
  uint8_t saved_byte = gwinmem::CurrentProcess().Read<uint8_t>( address );

  DWORD old_protection;
  VirtualProtect( ( LPVOID )address, sizeof( uint8_t ), PAGE_EXECUTE_READWRITE,
                  &old_protection );

  gwinmem::CurrentProcess().Write<uint8_t>( address, 0xCC );

  HookData hook_data;
  hook_data.address_to_hook = address;
  hook_data.saved_byte = saved_byte;
  hook_data.callback = callback;
  hook_data.custom_data = custom_data;

  hooks_[ hook_id ] = hook_data;
}

struct CustomData {
  uint32_t hook_id;
  uint32_t proxy_hook_id;
  uintptr_t address;
  uintptr_t address_of_next_instruction;
  bp_callback_t callback;
};

void ApplyNextBreakpoint( EXCEPTION_POINTERS* ex_info,
                          const HookData& hook_data );

void ApplyFirstBreakpoint( EXCEPTION_POINTERS* ex_info,
                           const HookData& hook_data ) {
  const auto custom_data =
      reinterpret_cast<CustomData*>( hook_data.custom_data );

  // Re-apply the hook
  software_bp_hook::Hook( custom_data->hook_id, custom_data->address,
                          ApplyNextBreakpoint, custom_data );
}

void ApplyNextBreakpoint( EXCEPTION_POINTERS* ex_info,
                          const HookData& hook_data ) {
  const auto custom_data =
      reinterpret_cast<CustomData*>( hook_data.custom_data );

  custom_data->callback( ex_info, hook_data );

  // Add hook to next instruction and make that hook re-apply this current hook
  // NOTE: Make sure the next bp is on the beginning of an instruction
  software_bp_hook::Hook( custom_data->proxy_hook_id,
                          custom_data->address_of_next_instruction,
                          ApplyFirstBreakpoint, custom_data );
}

void software_bp_hook::HookGood( const uint32_t hook_id,
                                 const uint32_t proxy_hook_id,
                                 const uintptr_t address,
                                 const uintptr_t address_of_next_instruction,
                                 bp_callback_t callback ) {
  CustomData* custom_data = new CustomData;
  custom_data->address = address;
  custom_data->address_of_next_instruction = address_of_next_instruction;
  custom_data->callback = callback;
  custom_data->hook_id = hook_id;
  custom_data->proxy_hook_id = proxy_hook_id;

  Hook( hook_id, address, ApplyNextBreakpoint, custom_data );
}

void software_bp_hook::UnHook( const uint32_t hook_id ) {
  if ( hooks_.find( hook_id ) != hooks_.end() ) {
    const HookData hook_data = hooks_[ hook_id ];

    gwinmem::CurrentProcess().Write<uint8_t>( hook_data.address_to_hook,
                                              hook_data.saved_byte );

    hooks_.erase( hook_id );
  }
}

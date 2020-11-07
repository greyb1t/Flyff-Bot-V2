#pragma once

struct HookData;

/*
using bp_callback_t = void ( * )( EXCEPTION_POINTERS* ex_info,
                                  const HookData& hook_data );
*/

using bp_callback_t = std::function<void( EXCEPTION_POINTERS* ex_info,
                                          const HookData& hook_data )>;

struct HookData {
  uintptr_t address_to_hook = 0;
  uint8_t saved_byte = 0;
  bp_callback_t callback = nullptr;
  void* custom_data = nullptr;
};

namespace software_bp_hook {

LONG BreakpointExceptionHandler( EXCEPTION_POINTERS* ex_info );

void RegisterExceptionHandler();
void UnRegisterExceptionHandler();

// Sets a breakpoint on the address and executes the callback onces breakpoint has been hit
// Then automatically removes the breakpoint to continue execution.
// Meaning that it only ever executes once.
// If you want a hook that re-applies itself, use HookGood
void Hook( const uint32_t hook_id,
           const uintptr_t address,
           bp_callback_t callback,
           void* custom_data = nullptr );

// NOTE: Due to limitations, the hook is only executed once, then automatically unhooked
// Therefore you need to hook one instruction after the initial instruction that hooks
// the first instruction again
void HookGood( const uint32_t hook_id,
               const uint32_t proxy_hook_id,
               const uintptr_t address,
               const uintptr_t address_of_next_instruction,
               bp_callback_t callback );

void UnHook( const uint32_t hook_id );

}  // namespace software_bp_hook
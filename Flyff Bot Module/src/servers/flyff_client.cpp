#include "pch.h"
#include "flyff_client.h"
#include "..\bot\local_player.h"
#include "..\bot\entity.h"

#include "../bot/botcore.h"

// #include "gwin/win32/gui/controls/RichEdit.h"
// #include "gwin/win32/misc/Logger.h"

#include "gwinguiv2/message_box.h"
#include "gwinguiv2/controls/richedit.h"
#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/progressbar.h"

#include "gwinmem/process_memory_internal.h"

#include "../utils/string_utls.h"

namespace bot {

FlyffClient::FlyffClient( const std::wstring& module_name )
    : module_name_( module_name ) {}

HookManager* FlyffClient::GetHookManager() {
  return &hook_manager_;
}

void FlyffClient::AddSearchFunction( MemoryContants value_type,
                                     std::function<uint32_t()> search_func ) {
  server_addresses_finder_functions_[ value_type ] = search_func;
}

bool FlyffClient::Search( HWND loading_window_handle ) {
  const auto neuz_module = gwinmem::CurrentProcess().GetModule( module_name_ );

  if ( !pattern_matcher_.LoadProcess( gwinmem::CurrentProcess(), neuz_module ) )
    return false;

  PreAddressSearch();

  const auto total_addresses = server_addresses_finder_functions_.size();
  const float progress_increment_value =
      static_cast<const float>( 100 / total_addresses );
  float progress_increment_value_extra;
  std::modf( progress_increment_value, &progress_increment_value_extra );

  float excess_inc_val = 0.f;

  std::vector<std::future<std::pair<MemoryContants, uint32_t>>>
      address_search_results;

  for ( auto addr : server_addresses_finder_functions_ ) {
    auto result = std::async(
        std::launch::async, [ = ]() -> std::pair<MemoryContants, uint32_t> {
          return std::make_pair( addr.first, addr.second() );
        } );

    address_search_results.push_back( std::move( result ) );
  }

  try {
    for ( auto& search_result : address_search_results ) {
      const auto result = search_result.get();

      const auto found_addr = result.second;

      if ( IsAddressInvalid( found_addr ) ) {
        gwingui::messagebox::Error(
            TEXT(
                "An invalid address has been found with an id of " +
                std::to_wstring( static_cast<uint32_t>( result.first ) ) +
                TEXT( ", the bot might not work properly. Contact greyb1t for "
                      "more information." ) ),
            TEXT( "Error" ), loading_window_handle );
      }

      server_addresses_[ result.first ] = found_addr;

      const auto string_to_hex = []( const uint64_t value ) -> std::wstring {
        std::wstringstream ss;
        ss << std::hex << value;
        return ss.str();
      };

      const auto richedit_addresses_offsets = GWH( RICHEDIT_ADDRESSES_OFFSETS );

      gwingui::richedit::AppendText(
          richedit_addresses_offsets,
          MemoryConstantToString( result.first ) + TEXT( ": 0x" ) +
              string_to_hex( found_addr ) + TEXT( "\n" ) );

      float more = 0;

      if ( excess_inc_val > 1.f ) {
        more = excess_inc_val;
        excess_inc_val = 0.f;
      }

      const auto progressbar_load_addresses = GWH( PROGRESS_LOAD_ADDRESSES );
      uint32_t old_pos =
          gwingui::progressbar::GetPos( progressbar_load_addresses );

      // Remove the slow progress bar animation
      // https :  //
      // stackoverflow.com/questions/5332616/disabling-net-progressbar-animation-when-changing-value
      // Increase and decrease immediately
      gwingui::progressbar::SetPos(
          progressbar_load_addresses,
          old_pos + static_cast<int>( progress_increment_value + more ) + 1 );
      gwingui::progressbar::SetPos(
          progressbar_load_addresses,
          old_pos + static_cast<int>( progress_increment_value + more ) - 1 );

      // gWin::Progressbar::IncementPos(PROGRESS_LOAD_ADDRESSES,
      // static_cast<int>(progress_increment_value + more));
      excess_inc_val += progress_increment_value_extra;
    }
  } catch ( std::exception& ex ) {
    gwingui::messagebox::Error(
        TEXT( "An exception occured during search for focks sake!\n" ) +
        stringutils::AnsiToWide( ex.what() ) );
  }

  PostAddressSearch();

  return true;
}

LPDIRECT3DDEVICE9 CreateD3DDevice( HRESULT* result ) {
  D3DPRESENT_PARAMETERS present_params = { 0 };

  present_params.BackBufferWidth = 1;
  present_params.BackBufferHeight = 1;
  present_params.BackBufferFormat = D3DFMT_UNKNOWN;

  present_params.Windowed = TRUE;
  present_params.SwapEffect = D3DSWAPEFFECT_DISCARD;

  present_params.EnableAutoDepthStencil = TRUE;
  present_params.AutoDepthStencilFormat = D3DFMT_D16;

  LPDIRECT3DDEVICE9 device_temp = NULL;

  *result = Direct3DCreate9( D3D_SDK_VERSION )
                ->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 0,
                                D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                &present_params, &device_temp );

  return device_temp;
}

void FlyffClient::InitializeDefaultHooks() {
  HRESULT result = 0;
  const auto device = CreateD3DDevice( &result );

  if ( !device ) {
    throw std::runtime_error( "Cannot create d3d9 device, HRESULT: " +
                              std::to_string( result ) );
  }

  const int kEncSceneVTableIndex = 42;

  DWORD endscene_addr = *reinterpret_cast<DWORD*>( device ) +
                        ( sizeof( DWORD* ) * kEncSceneVTableIndex );
  DWORD endscene_ptr_addr = *reinterpret_cast<DWORD*>( endscene_addr );

  device->Release();

  Hook endscene_hook;
  endscene_hook.original_function = ( void* )endscene_ptr_addr;
  endscene_hook.hook_callback = BotCore::EndSceneHooked;

  Hook d3dvec3project_hook;
  d3dvec3project_hook.original_function = reinterpret_cast<void*>(
      GetClientVar( MemoryContants::kD3dVec3ProjectAddress ) );
  d3dvec3project_hook.hook_callback = BotCore::D3DXVec3ProjectHooked;

  Hook getcursorpos_hook;
  getcursorpos_hook.original_function = GetCursorPos;
  getcursorpos_hook.hook_callback = BotCore::GetCursorPosHooked;

  Hook getkeystate_hook;
  getkeystate_hook.original_function = GetKeyState;
  getkeystate_hook.hook_callback = BotCore::GetKeyStateHooked;

  hook_manager_.Begin();

  hook_manager_.AddHook( HookType::EndScene, endscene_hook );
  hook_manager_.AddHook( HookType::D3DVec3Project, d3dvec3project_hook );
  hook_manager_.AddHook( HookType::GetCursorPos, getcursorpos_hook );
  hook_manager_.AddHook( HookType::GetKeyState, getkeystate_hook );

  hook_manager_.End();
}

void FlyffClient::RemoveDefaultHooks() {
  hook_manager_.Begin();

  hook_manager_.RemoveHook( HookType::EndScene );
  hook_manager_.RemoveHook( HookType::D3DVec3Project );
  hook_manager_.RemoveHook( HookType::GetCursorPos );
  hook_manager_.RemoveHook( HookType::GetKeyState );

  hook_manager_.End();
}

bool FlyffClient::IsAddressInvalid( uint32_t addr ) {
  if ( addr == 0 )
    return true;
  else if ( addr == 0xffffffff || addr == 0xfffffff || addr == 0xffffff ||
            addr == 0xfffff || addr == 0xffff || addr == 0xfff ||
            addr == 0xff || addr == 0xf )
    return true;

  return false;
}

std::wstring FlyffClient::MemoryConstantToString( MemoryContants constant ) {
  switch ( constant ) {
    case MemoryContants::kGetCmdParamAddress:
      return TEXT( "kGetCmdParamAddress" );
      break;

    case MemoryContants::kD3dVec3ProjectAddress:
      return TEXT( "kD3dVec3ProjectAddress" );
      break;

    case MemoryContants::kProjectionMatrixAddress:
      return TEXT( "kProjectionMatrixAddress" );
      break;

    case MemoryContants::kViewMatrixAddress:
      return TEXT( "kViewMatrixAddress" );
      break;

    case MemoryContants::kCameraAddress:
      return TEXT( "kCameraAddress" );
      break;

    case MemoryContants::kPlayerBaseAddress:
      return TEXT( "kPlayerBaseAddress" );
      break;

    case MemoryContants::kScrollDistanceAddress:
      return TEXT( "kScrollDistanceAddress" );
      break;

    case MemoryContants::kAccountNameAddress:
      return TEXT( "kAccountNameAddress" );
      break;

    case MemoryContants::kSelectedEntityAddress:
      return TEXT( "kSelectedEntityAddress" );
      break;

    case MemoryContants::kEntityListAddress:
      return TEXT( "kEntityListAddress" );
      break;

    case MemoryContants::kMovementOffset:
      return TEXT( "kMovementOffset" );
      break;

    case MemoryContants::kCollisionAddress:
      return TEXT( "kCollisionAddress" );
      break;

    case MemoryContants::kModelOffset:
      return TEXT( "kModelOffset" );
      break;

    case MemoryContants::kBoundBoxOffset:
      return TEXT( "kBoundBoxOffset" );
      break;

    case MemoryContants::kWorldMatrixOffset:
      return TEXT( "kWorldMatrixOffset" );
      break;

    case MemoryContants::kPositionOffset:
      return TEXT( "kPositionOffset" );
      break;

    case MemoryContants::kSpeedOffset:
      return TEXT( "kSpeedOffset" );
      break;

    case MemoryContants::kHealthOffset:
      return TEXT( "kHealthOffset" );
      break;

    case MemoryContants::kManaOffset:
      return TEXT( "kManaOffset" );
      break;

    case MemoryContants::kFatigueOffset:
      return TEXT( "kFatigueOffset" );
      break;

    case MemoryContants::kLevelOffset:
      return TEXT( "kLevelOffset" );
      break;

    case MemoryContants::kNameOffset:
      return TEXT( "kNameOffset" );
      break;

    case MemoryContants::kObjectFlagsOffset:
      return TEXT( "kObjectFlagsOffset" );
      break;

    case MemoryContants::kSelectedEntityOffset:
      return TEXT( "kSelectedEntityOffset" );
      break;

    case MemoryContants::kSpeedMultiplierOffset:
      return TEXT( "kSpeedMultiplierOffset" );
      break;

    case MemoryContants::kMoveOffset:
      return TEXT( "kMoveOffset" );
      break;

    case MemoryContants::kIgniteEntityIdOffset:
      return TEXT( "kIgniteEntityIdOffset" );

    case MemoryContants::kObjectTypeOffset:
      return TEXT( "kObjectTypeOffset" );

    default:
      return TEXT( "Invalid" );
      break;
  }
}

std::unique_ptr<LocalPlayer> FlyffClient::CreateLocalPlayer() {
  return std::unique_ptr<LocalPlayer>( new LocalPlayer( this ) );
}

std::unique_ptr<Entity> FlyffClient::CreateEntity( uint32_t address_ptr ) {
  return std::unique_ptr<Entity>( new Entity( this, address_ptr ) );
}

void FlyffClient::SetServerVar( MemoryContants type, uint32_t value ) {
  const auto string_to_hex = []( const uint64_t value ) -> std::wstring {
    std::wstringstream ss;
    ss << std::hex << value;
    return ss.str();
  };

  gwingui::richedit::AppendText( GWH( RICHEDIT_ADDRESSES_OFFSETS ),
                                 MemoryConstantToString( type ) +
                                     TEXT( ": 0x" ) + string_to_hex( value ) +
                                     TEXT( "\n" ) );

  server_addresses_[ type ] = value;
}

}  // namespace bot
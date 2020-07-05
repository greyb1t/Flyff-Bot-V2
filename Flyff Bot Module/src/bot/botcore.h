#pragma once

#include "..\servers\flyff_client.h"
#include "flyff_client_factory.h"
#include "../utils/stopwatch.h"
#include "bot.h"
#include "bot_options.h"
#include "../utils/d3d_drawing.h"

#pragma comment( lib, "Winmm.lib" )

#define NORMAL_TEXT TEXT( "NormalText" )
#define NORMAL_TEXT_BIG TEXT( "NormalTextBig" )
#define BIG_TEXT TEXT( "BiggerText" )

extern std::mutex g_hooks_mutex;

namespace bot {

class Bot;
class Entity;

using tEndScene = HRESULT( WINAPI* )( LPDIRECT3DDEVICE9 device );

using tD3DXVec3Project = D3DXVECTOR3*( WINAPI* )( D3DXVECTOR3* pOut,
                                                  const D3DXVECTOR3* pV,
                                                  const D3DVIEWPORT9* pViewport,
                                                  const D3DXMATRIX* pProjection,
                                                  const D3DXMATRIX* pView,
                                                  const D3DXMATRIX* pWorld );

using tGetCursorPos = BOOL( WINAPI* )( LPPOINT lpPoint );

using tGetKeyState = SHORT( WINAPI* )( int nVirtKey );

using tSendMeleeAttack = int( __thiscall* )( void* this_,
                                             uint32_t attack_message,
                                             uint32_t obj_id,
                                             __int32 nparam2,
                                             __int32 nparam3,
                                             float f_val );

using tDoAttackMelee = signed int( __thiscall* )( void* this_,
                                                  int target,
                                                  int attack_message,
                                                  int item_id );

using tGetCmdParam = int( __thiscall* )( void* this_, int index );

class BotCore {
 public:
  BotCore();
  ~BotCore() = default;

  static HRESULT WINAPI EndSceneHooked( LPDIRECT3DDEVICE9 device );

  static D3DXVECTOR3* WINAPI
  D3DXVec3ProjectHooked( D3DXVECTOR3* out,
                         const D3DXVECTOR3* pV,
                         const D3DVIEWPORT9* pViewport,
                         const D3DXMATRIX* pProjection,
                         const D3DXMATRIX* pView,
                         const D3DXMATRIX* pWorld );

  static BOOL WINAPI GetCursorPosHooked( LPPOINT lpPoint );

  static SHORT WINAPI GetKeyStateHooked( int nVirtKey );

#if 0
  static int __fastcall SendMeleeAttackHooked( void* this_,
                                               void* unknown,
                                               uint32_t attack_message,
                                               uint32_t obj_id,
                                               __int32 nparam2,
                                               __int32 nparam3,
                                               float f_val );

  static signed int __fastcall DoAttackMeleeHooked( void* this_,
                                                    void* unknown,
                                                    int target,
                                                    int attack_message,
                                                    int item_id );

  static int __fastcall GetCmdParamHooked( void* this_,
                                           void* unknown,
                                           int index );
#endif

  static LRESULT CALLBACK KeyboardCallback( int code,
                                            WPARAM wparam,
                                            LPARAM lparam );

  void ShowBotHasStoppedWindow();

  void DrawEntity( const Entity& local_player,
                   const Entity& entity,
                   const D3DCOLOR box_color );

  void Render( LPDIRECT3DDEVICE9 device );
  void Update();
  void UpdateCheatsValues();

  void ToggleBot();

  void OnBotStatusChange();

  // Skips calling Update for specified amount of frames
  void SkipUpdateForFrames( int64_t frames );

  void AddEntityToDraw( std::unique_ptr<Entity>& entity );

  std::vector<std::unique_ptr<Bot>>& GetActiveBots();

  LPDIRECT3DDEVICE9 GetDevice();

  HWND GetTargetWindow();
  D3DVIEWPORT9 GetViewport() const;

  // DO NOT MAKE IT RETURN UNIQUE PTR, IT IS A FUCKIN' MESS
  FlyffClient* GetFlyffClient();

  bool GetStarted() const;
  int64_t GetFrameCount() const;
  BotOptions& GetBotOptions();

  void SetActiveBots( std::vector<std::unique_ptr<Bot>>&& bots );

  void SetSimulatedCursorPos( POINT& pos );
  POINT GetSimulatedCursorPos() const;
  void SetViewport( D3DVIEWPORT9 viewport );
  void SetStarted( bool started );
  void SetIsDebuggerAttached( bool value );

 public:
  std::vector<std::string> names_of_entities_to_draw;

 private:
  D3DVIEWPORT9 viewport_;
  D3DDrawing drawing_;

  bool has_initialized_d3d_;
  bool has_initialized_matrix_addresses_;

  HWND target_window_handle_;

  bool started_;

  int64_t frame_counter_;
  int64_t last_frame_counter_;
  int64_t skipped_frames_;

  // fps
  uint32_t last_time_;
  uint32_t ms_elapsed_;
  uint32_t fps_frame_counter_;

  bool is_stopwatch_started_;

  uint32_t fps_;

  uint64_t last_cheats_update_ms_;
  uint64_t last_gui_entity_population_ms_;

  HHOOK kb_hook_handle_;

  std::vector<std::unique_ptr<Bot>> active_bots_;

  std::unique_ptr<FlyffClient> client_;
  std::vector<std::unique_ptr<Entity>> extra_entities_;

  POINT simulated_cursor_pos_;

  BotOptions bot_options_;

  bool bad_boy_present_;

  uintptr_t endscene_ret_addr_;

  friend class BotInitializer;
};

}  // namespace bot
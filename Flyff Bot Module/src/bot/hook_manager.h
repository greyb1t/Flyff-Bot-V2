#pragma once

enum class HookType {
  D3DVec3Project,
  EndScene,
  GetCursorPos,
  GetKeyState,
};

struct Hook {
  void *original_function = 0;
  void *hook_callback = 0;
};

class HookManager {
 public:
  HookManager() = default;
  ~HookManager() = default;

  void Begin();
  void End();
  void AddHook( const HookType type, Hook& hook );
  void RemoveHook( const HookType type );

  const Hook& GetHook(const HookType type) const;

 private:
  std::unordered_map<HookType, Hook> hooks_;
};
#pragma once

#include <Windows.h>

#include "flyff_client.h"

namespace bot {

class FlyffClientDragonCrusade : public FlyffClient {
 public:
  FlyffClientDragonCrusade();
  ~FlyffClientDragonCrusade() {}

  virtual void PreAddressSearch();

  void OnExit() override;

  void PostAddressSearch() override;

  virtual void PreHookInitialization();

  void PostHookDeinitialization() override;

  virtual void PostHookInitialization();

  virtual void PreHookDeInitialization();
};

}  // namespace bot
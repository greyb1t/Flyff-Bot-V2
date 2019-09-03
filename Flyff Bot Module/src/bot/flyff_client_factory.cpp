#include "pch.h"
#include "flyff_client_factory.h"

#include "../servers/flyff_client_dragon_crusade.h"
#include "../servers/flyff_client_ignite.h"
#include "../servers/flyff_client_mazey.h"

using namespace bot;
using namespace bot::flyff_client_factory;

std::unique_ptr<FlyffClient> flyff_client_factory::CreateFlyffClient(
    const ServerType type ) {
  std::unique_ptr<FlyffClient> client = nullptr;

  switch ( type ) {
    case ServerType::IgniteFlyff:
      client = std::make_unique<FlyffClientIgniteFlyff>();
      break;

    case ServerType::MazeyFlyff:
      client = std::make_unique<FlyffClientMazeyFlyff>();
      break;

    case ServerType::DragonCrusade:
      client = std::make_unique<FlyffClientDragonCrusade>();
      break;

    default:
      break;
  }

  return client;
}

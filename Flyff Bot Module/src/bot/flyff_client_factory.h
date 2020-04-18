#pragma once

#include "../servers/flyff_client.h"

enum class ServerType {
  MazeyFlyff,
  IgniteFlyff,
  DragonCrusade,
  EclipseFlyff,

  Unknown
};

namespace bot {

namespace flyff_client_factory {

std::unique_ptr<FlyffClient> CreateFlyffClient( const ServerType type );

}

}  // namespace bot
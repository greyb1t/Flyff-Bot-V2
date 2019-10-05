#pragma once

#include "..\utils\d3d_drawing.h"
#include "entity.h"

class BotCore;

namespace bot {

class Entity;

namespace math {

bool FlyffW2SObsolete( BotCore& botcore,
                       const D3DXVECTOR3& src_origin_pos,
                       const D3DXMATRIX& mat_rotation,
                       const D3DXVECTOR3& src_position,
                       D3DXVECTOR3* screen_out );

// Returns true if visible, false otherwise
bool FlyffWorldToScreen( const BotCore* botcore,
                         const Entity& entity,
                         const D3DXVECTOR3& src_position,
                         D3DXVECTOR3* screen_out );

D3DXVECTOR3 CalculateBoxCenter( const BOUND_BOX* bound_box );

float CalcDistance( const D3DXVECTOR3& vec1, const D3DXVECTOR3& vec2 );

}  // namespace math

}  // namespace bot
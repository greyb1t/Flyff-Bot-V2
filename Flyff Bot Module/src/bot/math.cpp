#include "pch.h"
#include "math.h"
#include "botcore.h"

namespace bot {

namespace math {

bool FlyffW2SObsolete( BotCore& botcore,
                       const D3DXVECTOR3& src_origin_pos,
                       const D3DXMATRIX& mat_rotation,
                       const D3DXVECTOR3& src_position,
                       D3DXVECTOR3* screen_out ) {
  // Calculate the world matrix myself, can also use the precalculated world
  // matrix in the CObj class.
  D3DXMATRIX mat_world;
  D3DXMatrixIdentity( &mat_world );

  D3DXMATRIX mat_trans;
  D3DXMatrixTranslation( &mat_trans, src_origin_pos.x, src_origin_pos.y,
                         src_origin_pos.z );

  D3DXMatrixMultiply( &mat_world, &mat_world, &mat_rotation );
  D3DXMatrixMultiply( &mat_world, &mat_world, &mat_trans );

  const auto projection_matrix_addr = botcore.GetFlyffClient()->GetClientVar(
      MemoryContants::kProjectionMatrixAddress );
  const auto view_matrix_addr = botcore.GetFlyffClient()->GetClientVar(
      MemoryContants::kViewMatrixAddress );

  D3DXVec3Project( screen_out, &src_position, &botcore.GetViewport(),
                   reinterpret_cast<D3DXMATRIX*>( projection_matrix_addr ),
                   reinterpret_cast<D3DXMATRIX*>( view_matrix_addr ),
                   &mat_world );

  // Prevents drawing if the position is outside of view
  if ( screen_out->z < 0.01f || screen_out->z > 1.00f )
    return false;

  return true;
}

bool FlyffWorldToScreen( const BotCore* botcore,
                         const UniquePtrEntity& entity,
                         const D3DXVECTOR3& src_position,
                         D3DXVECTOR3* screen_out ) {
  FlyffClient* client = entity->GetFlyffClient();

  D3DXMATRIX mat_world = entity->GetWorldMatrix();

  const auto projection_matrix_addr =
      client->GetClientVar( MemoryContants::kProjectionMatrixAddress );
  const auto view_matrix_addr =
      client->GetClientVar( MemoryContants::kViewMatrixAddress );

  D3DXMATRIX* mat_proj =
      reinterpret_cast<D3DXMATRIX*>( projection_matrix_addr );
  D3DXMATRIX* mat_view = reinterpret_cast<D3DXMATRIX*>( view_matrix_addr );

  D3DXVec3Project( screen_out, &src_position, &botcore->GetViewport(), mat_proj,
                   mat_view, &mat_world );

  // Prevents drawing if the position is outside of view
  if ( screen_out->z < 0.0f || screen_out->z > 1.00f )
    return false;

  return true;
}

D3DXVECTOR3 CalculateBoxCenter( const BOUND_BOX* bound_box ) {
  D3DXVECTOR3 temp = { 0, 0, 0 };

  for ( int i = 0; i < BOUND_BOX::kBoxCorners; ++i )
    temp += bound_box->pos[ i ];

  return ( temp / static_cast<float>( BOUND_BOX::kBoxCorners ) );
}

float CalcDistance( const D3DXVECTOR3& vec1, const D3DXVECTOR3& vec2 ) {
  float dist = sqrt( pow( vec1.x - vec2.x, 2 ) + pow( vec1.y - vec2.y, 2 ) +
                     pow( vec1.z - vec2.z, 2 ) );

  return dist;
}

}  // namespace math

}  // namespace bot
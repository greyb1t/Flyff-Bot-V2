#include "pch.h"
#include "level_area_option.h"
#include "../../res/resource.h"
#include "option_utils.h"

#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/editcontrol.h"

namespace bot {

const std::string LevelAreaOption::kLevelAreaLength1Name =
    "level_area_length_1";
const std::string LevelAreaOption::kLevelAreaLength2Name =
    "level_area_length_2";

const std::string LevelAreaOption::kLevelAreaOriginPosX =
    "level_area_origin_pos_x";
const std::string LevelAreaOption::kLevelAreaOriginPosZ =
    "level_area_origin_pos_z";

void LevelAreaOption::SetLength1( uint32_t length ) {
  level_area_length1 = length;
}

uint32_t LevelAreaOption::GetLength1() const {
  return level_area_length1;
}

void LevelAreaOption::SetLength2( uint32_t length ) {
  level_area_length2 = length;
}

uint32_t LevelAreaOption::GetLength2() const {
  return level_area_length2;
}

void LevelAreaOption::SetOriginPosition( const D3DXVECTOR3 origin_position ) {
  origin_position_ = origin_position;
}

D3DXVECTOR3 LevelAreaOption::GetOriginPosition() const {
  return origin_position_;
}

json LevelAreaOption::GetJson() {
  auto j = Option::GetJson();

  j[ kLevelAreaLength1Name ] = level_area_length1;
  j[ kLevelAreaLength2Name ] = level_area_length2;

  j[ kLevelAreaOriginPosX ] = ( uint32_t )origin_position_.x;
  j[ kLevelAreaOriginPosZ ] = ( uint32_t )origin_position_.z;

  return j;
}

void LevelAreaOption::LoadJson( const json& json_parent ) {
  Option::LoadJson( json_parent );

  if ( JsonExists( json_parent, name_ ) ) {
    auto json_child = json_parent.at( name_ );

    if ( JsonExists( json_child, kLevelAreaLength1Name ) &&
         JsonExists( json_child, kLevelAreaLength2Name ) ) {
      level_area_length1 =
          json_child.at( kLevelAreaLength1Name ).get<uint32_t>();
      level_area_length2 =
          json_child.at( kLevelAreaLength2Name ).get<uint32_t>();
    }

    if ( JsonExists( json_child, kLevelAreaOriginPosX ) &&
         JsonExists( json_child, kLevelAreaOriginPosZ ) ) {
      origin_position_.x = static_cast<float>(
          json_child.at( kLevelAreaOriginPosX ).get<uint32_t>() );
      origin_position_.z = static_cast<float>(
          json_child.at( kLevelAreaOriginPosZ ).get<uint32_t>() );
    }
  }
}

void LevelAreaOption::RefreshControls() {
  Option::RefreshControls();

  const auto editcontrol_levelarea_length2 = GWH( EDIT_LEVEL_AREA_LENGTH2 );

  if ( level_area_length2 != -1 )
    gwingui::editcontrol::SetInt( editcontrol_levelarea_length2,
                                  level_area_length2 );
  else
    gwingui::editcontrol::SetText( editcontrol_levelarea_length2, TEXT( "" ) );

  const auto editcontrol_levelarea_length1 = GWH( EDIT_LEVEL_AREA_LENGTH1 );

  if ( level_area_length1 != -1 )
    gwingui::editcontrol::SetInt( editcontrol_levelarea_length1,
                                  level_area_length1 );
  else
    gwingui::editcontrol::SetText( editcontrol_levelarea_length1, TEXT( "" ) );

  const auto editcontrol_levelarea_origin_x =
      GWH( EDIT_LEVEL_AREA_ORIGIN_POS_X );

  if ( origin_position_.x != 0 )
    gwingui::editcontrol::SetInt( editcontrol_levelarea_origin_x,
                                  static_cast<int32_t>( origin_position_.x ) );
  else
    gwingui::editcontrol::SetText( editcontrol_levelarea_origin_x, TEXT( "" ) );

  const auto editcontrol_levelarea_origin_z =
      GWH( EDIT_LEVEL_AREA_ORIGIN_POS_Z );

  if ( origin_position_.z != 0 )
    gwingui::editcontrol::SetInt( editcontrol_levelarea_origin_z,
                                  static_cast<int32_t>( origin_position_.z ) );
  else
    gwingui::editcontrol::SetText( editcontrol_levelarea_origin_z, TEXT( "" ) );
}

void LevelAreaOption::EnableOrDisableControls( bool enable ) {
  gwingui::control::EnableOrDisable( GWH( EDIT_LEVEL_AREA_LENGTH2 ), enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_LEVEL_AREA_LENGTH1 ), enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_LEVEL_AREA_ORIGIN_POS_X ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_LEVEL_AREA_ORIGIN_POS_Z ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_LEVEL_AREA_GET_CURRENT_POS ),
                                     enable );
}

D3DXVECTOR3 LevelAreaOption::GetCornerPosition(
    const D3DXVECTOR3& start_position,
    const uint32_t index ) const {
  D3DXVECTOR3 corner_pos = start_position;

  const uint32_t length1 = GetLength1();
  const uint32_t length2 = GetLength2();

  switch ( index ) {
    case 0:
      corner_pos.x -= length1;
      corner_pos.z -= length2;
      break;
    case 1:
      corner_pos.x += length1;
      corner_pos.z += length2;
      break;
    case 2:
      corner_pos.x += length1;
      corner_pos.z -= length2;
      break;
    case 3:
      corner_pos.x -= length1;
      corner_pos.z += length2;
      break;
    default:
      assert( false && "out of bounds" );
      break;
  }

  return corner_pos;
}

bool LevelAreaOption::IsInsideArea( const D3DXVECTOR3& start_position,
                                    const D3DXVECTOR3& position ) const {
  D3DXVECTOR3 corners[ 4 ]{
    GetCornerPosition( start_position, 0 ),
    GetCornerPosition( start_position, 1 ),
    GetCornerPosition( start_position, 2 ),
    GetCornerPosition( start_position, 3 ),
  };

  const auto length_x = std::abs( corners[ 0 ].x - corners[ 1 ].x );
  const auto length_z = std::abs( corners[ 0 ].z - corners[ 1 ].z );

  // Works by seeing if the distance between the point is bigger than the
  // distance of each corner
  for ( int i = 0; i < 4; ++i ) {
    if ( !( std::abs( corners[ i ].x - position.x ) < length_x &&
            std::abs( corners[ i ].z - position.z ) < length_z ) ) {
      return false;
    }
  }

  return true;
}

bool LevelAreaOption::IsEntityAllowed(
    const bot::UniquePtrEntity& entity ) const {
  if ( IsEnabled() ) {
    if ( !IsInsideArea( GetOriginPosition(), entity->GetPosition() ) )
      return false;
  }

  return true;
}

}  // namespace bot
#pragma once

#include "option.h"

namespace bot {

class LevelAreaOption : public Option {
 public:
  LevelAreaOption( const std::string& name, const uint32_t control_identifier )
      : Option( name, control_identifier ),
        level_area_length1( -1 ),
        level_area_length2( -1 ),
        origin_position_{} {}

  void SetLength1( uint32_t length );
  uint32_t GetLength1() const;

  void SetLength2( uint32_t length );
  uint32_t GetLength2() const;

  void SetOriginPosition( const D3DXVECTOR3 origin_position );
  D3DXVECTOR3 GetOriginPosition() const;

  json GetJson() override;

  void LoadJson( const json& json_parent ) override;

  void RefreshControls() override;

  void EnableOrDisableControls( bool enable ) override;

  D3DXVECTOR3 GetCornerPosition( const D3DXVECTOR3& start_position,
                                 const uint32_t index ) const;
  bool IsInsideArea( const D3DXVECTOR3& start_position,
                     const D3DXVECTOR3& position ) const;

  virtual bool IsEntityAllowed( const bot::UniquePtrEntity& entity ) const;

 protected:
  uint32_t level_area_length1;
  uint32_t level_area_length2;

  D3DXVECTOR3 origin_position_;

  const static std::string kLevelAreaLength1Name;
  const static std::string kLevelAreaLength2Name;

  const static std::string kLevelAreaOriginPosX;
  const static std::string kLevelAreaOriginPosZ;
};

}  // namespace bot
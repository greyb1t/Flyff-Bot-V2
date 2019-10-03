#include "pch.h"
#include "entity.h"
#include "math.h"
#include "botcore.h"
#include "logging.h"
#include "../utils/string_utls.h"

#include "gwinmem/process_memory_internal.h"

namespace bot {

Entity::Entity( const Entity& entity )
    : client_( entity.client_ ), address_ptr_( entity.address_ptr_ ) {}

Entity::Entity( FlyffClient* client, uint32_t address_ptr )
    : client_( client ), address_ptr_( address_ptr ) {}

Entity::~Entity() {}

bool Entity::IsDeleted() const {
  return GetFlags() & OBJ_FLAG_DELETE;
}

bool Entity::IsInvalidMemory() const {
  try {
    if ( !address_ptr_ )
      return true;

    int ptr = gwinmem::CurrentProcess().Read<int>( address_ptr_ );

    if ( ptr == 0 )
      return true;

    return false;
  } catch ( gwinmem::BadMemoryException ex ) {
    return true;
  }
}

bool Entity::IsDeletedOrInvalidMemory() const {
  // check if the memory is valid before checking if deleted
  if ( IsInvalidMemory() )
    return true;

  return IsDeleted();
}

bool Entity::IsPlayer() const {
  return GetFlags() == 2;
}

bool Entity::IsItem() const {
  return GetHealth() == 0 && GetMana() == 0 && GetFatigue() == 0;
}

bool Entity::IsMonster() const {
  return !IsPlayer() && GetLevel() > 0 && GetLevel() <= 300 && !IsPet() &&
         !IsItem() && !IsNpc() && !IsUnknownEntity();
}

bool Entity::IsAlive() const {
  if ( IsDeletedOrInvalidMemory() )
    return false;

  return GetHealth() > 0;
}

bool Entity::IsPet() const {
  return GetHealth() == 2297 && GetLevel() == 1;
}

bool Entity::IsRunning() const {
  return GetMotion() == 4;
}

bool Entity::IsStandingStill() const {
  return GetMotion() == 1;
}

ObjectType Entity::GetObjectType() const {
  const auto type_offset =
      client_->GetClientVar( MemoryContants::kObjectTypeOffset );

  ObjectType type =
      gwinmem::CurrentProcess().Read<ObjectType>( address_ptr_ + type_offset );

  return type;
}

bool Entity::IsNpc() const {
  return GetHealth() == 1 && GetLevel() == 1;
}

bool Entity::IsAggro() const {
  const auto is_aggro_offset =
      client_->GetClientVar( MemoryContants::kIsAggroOffset );

  return !!gwinmem::CurrentProcess().Read<int32_t>( address_ptr_ +
                                                    is_aggro_offset );
}

bool Entity::IsUnknownEntity() const {
  return GetMotion() == 0;
  // return GetSpeed() < 0 || GetMotion() == 0;
}

float Entity::DistanceTo( const Entity* entity ) const {
  return math::CalcDistance( GetPosition(), entity->GetPosition() );
}

D3DXVECTOR3 Entity::GetPosition() const {
  D3DXVECTOR3 pos = { 0.f, 0.f, 0.f };

  if ( IsDeletedOrInvalidMemory() )
    return pos;

  const auto position_offset =
      client_->GetClientVar( MemoryContants::kPositionOffset );

  pos = gwinmem::CurrentProcess().Read<D3DXVECTOR3>( address_ptr_ +
                                                     position_offset );

  return pos;
}

int32_t Entity::GetHealth() const {
  const auto health_offset =
      client_->GetClientVar( MemoryContants::kHealthOffset );

  const auto health =
      gwinmem::CurrentProcess().Read<int32_t>( address_ptr_ + health_offset );

  return health;
}

int Entity::GetMana() const {
  const auto mana_offset = client_->GetClientVar( MemoryContants::kManaOffset );

  int mana = gwinmem::CurrentProcess().Read<int>( address_ptr_ + mana_offset );

  return mana;
}

int Entity::GetFatigue() const {
  const auto fatigue_offset =
      client_->GetClientVar( MemoryContants::kFatigueOffset );

  int fatigue =
      gwinmem::CurrentProcess().Read<int>( address_ptr_ + fatigue_offset );

  return fatigue;
}

/*
int Entity::GetSpeed() const {
  const auto speed_offset =
      client_->GetClientVar( MemoryContants::kSpeedOffset );

  int speed =
      gwinmem::CurrentProcess().Read<int>( address_ptr_ + speed_offset );

  return speed;
}
*/

int Entity::GetFlags() const {
  const auto flags_offset =
      client_->GetClientVar( MemoryContants::kObjectFlagsOffset );

  return gwinmem::CurrentProcess().Read<int>( address_ptr_ + flags_offset );
}

void Entity::SetFlags( const uint32_t flags ) const {
  const auto flags_offset =
      client_->GetClientVar( MemoryContants::kObjectFlagsOffset );

  gwinmem::CurrentProcess().Write( address_ptr_ + flags_offset, flags );
}

int Entity::GetLevel() const {
  const auto level_offset =
      client_->GetClientVar( MemoryContants::kLevelOffset );

  int level =
      gwinmem::CurrentProcess().Read<int>( address_ptr_ + level_offset );

  return level;
}

int Entity::GetMotion() const {
  if ( IsDeletedOrInvalidMemory() )
    return 0;

  const auto movement_base_address =
      client_->GetClientVar( MemoryContants::kMovementOffset );
  const auto move_offset = client_->GetClientVar( MemoryContants::kMoveOffset );

  int motion = 0;

  // Use a try block because the pActionMover can be NULL
  // try {
    motion = gwinmem::CurrentProcess().Read<int>(
        address_ptr_ + movement_base_address, { move_offset } );
  // } catch ( gwinmem::BadMemoryException ) {
  //   return 0;
  // }

  return motion;
}

float Entity::GetSpeedMultiplier() {
  const auto speed_multiplier_offset =
      client_->GetClientVar( MemoryContants::kSpeedMultiplierOffset );

  float speed_multiplier = gwinmem::CurrentProcess().Read<float>(
      address_ptr_ + speed_multiplier_offset );

  return speed_multiplier;
}

void Entity::SetSpeedMultiplier( float value ) const {
  const auto speed_multiplier_offset =
      client_->GetClientVar( MemoryContants::kSpeedMultiplierOffset );

  gwinmem::CurrentProcess().Write( address_ptr_ + speed_multiplier_offset,
                                   value );
}

void Entity::SetPosition( const D3DXVECTOR3 position ) {
  const auto position_offset =
      client_->GetClientVar( MemoryContants::kPositionOffset );

  gwinmem::CurrentProcess().Write( address_ptr_ + position_offset, position );
}

std::string Entity::GetName() const {
  if ( IsDeletedOrInvalidMemory() )
    return "";

  // Is the entity not a mover? Then it has no name.
  if ( GetObjectType() != ObjectType::kObjectTypeMover )
    return "";

  const auto name_offset = client_->GetClientVar( MemoryContants::kNameOffset );
  const int kMaxNameLength = 250;

  std::string name = gwinmem::CurrentProcess().ReadString(
      address_ptr_ + name_offset, kMaxNameLength );

  return name;
}

D3DXMATRIX Entity::GetWorldMatrix() const {
  const auto world_matrix_offset =
      client_->GetClientVar( MemoryContants::kWorldMatrixOffset );

  D3DXMATRIX matrix = gwinmem::CurrentProcess().Read<D3DXMATRIX>(
      address_ptr_ + world_matrix_offset );

  return matrix;
}

void Entity::SetWorldMatrix( const D3DXMATRIX& matrix ) const {
  const auto world_matrix_offset =
      client_->GetClientVar( MemoryContants::kWorldMatrixOffset );

  gwinmem::CurrentProcess().Write<D3DXMATRIX>(
      address_ptr_ + world_matrix_offset, matrix );
}

BOUND_BOX Entity::GetBoundBox() const {
  const auto model_offset =
      client_->GetClientVar( MemoryContants::kModelOffset );
  const auto bound_box_offset =
      client_->GetClientVar( MemoryContants::kBoundBoxOffset );

  DWORD model_ptr_address =
      gwinmem::CurrentProcess().Read<DWORD>( address_ptr_ + model_offset );

  BOUND_BOX bound_box = BOUND_BOX::EMPTY;

  // Added the try catch because Render and DrawEntity failed because it seems
  // to have tried to draw an object that has no bound box
  try {
    bound_box = gwinmem::CurrentProcess().Read<BOUND_BOX>( model_ptr_address +
                                                           bound_box_offset );
  } catch ( gwinmem::BadMemoryException ) {
  }

  return bound_box;
}

void Entity::SetBoundBox( const BOUND_BOX& bound_box ) {
  const auto model_offset =
      client_->GetClientVar( MemoryContants::kModelOffset );
  const auto bound_box_offset =
      client_->GetClientVar( MemoryContants::kBoundBoxOffset );

  DWORD model_ptr_address =
      gwinmem::CurrentProcess().Read<DWORD>( address_ptr_ + model_offset );

  gwinmem::CurrentProcess().Write( model_ptr_address + bound_box_offset,
                                   bound_box );
}

void Entity::RemoveBoundBox() {
  SetBoundBox( BOUND_BOX::EMPTY );
}

void Entity::SaveBoundBox() {
  original_bound_box_ = GetBoundBox();

  if ( original_bound_box_ == BOUND_BOX::EMPTY )
    logging::Log( TEXT( "UNEXPECTED EMPTY BB\n" ) );
}

void Entity::RestoreBoundBox() {
  SetBoundBox( original_bound_box_ );
}

}  // namespace bot
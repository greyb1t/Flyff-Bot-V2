#pragma once

#include "bound_box.h"

constexpr DWORD OBJ_FLAG_DELETE = 0x00000001;
constexpr DWORD OBJ_FLAG_VISIBLE = 0x00000002;
constexpr DWORD OBJ_FLAG_VIRTUAL = 0x00000004;
constexpr DWORD OBJ_FLAG_UPDATEMATRIX = 0x00000008;
constexpr DWORD OBJ_FLAG_POSCHANGED = 0x00000010;
constexpr DWORD OBJ_FLAG_MAX = OBJ_FLAG_DELETE | OBJ_FLAG_VISIBLE |
                               OBJ_FLAG_VIRTUAL | OBJ_FLAG_UPDATEMATRIX |
                               OBJ_FLAG_POSCHANGED;

enum class ObjectType : uint32_t {
  kObjectTypeObject,
  kObjectTypeAni,

  // Ignite Flyff: Profession Gathering (Tree's, Mining
  // Stuff...)
  kObjectTypeCommonCtrl,

  kObjectTypeSfx,
  kObjectTypeItem,

  // Players, Monsters, Moving Objects...
  kObjectTypeMover,

  kObjectTypeRegion,
  kObjectTypeShip,
  kObjectTypePath,

  kMaxObjectTypes
};

namespace bot {

class FlyffClient;

class Entity {
public:
  Entity() = default;

  Entity( const Entity& entity );

  Entity( FlyffClient* client, uint32_t addrptr );
  virtual ~Entity() {}

  bool operator<( const Entity& rhs ) const {
    return address_ptr_ < rhs.address_ptr_;
  }

  bool IsDeleted() const;
  bool IsInvalidMemory() const;
  bool IsDeletedOrInvalidMemory() const;

  bool IsPlayer() const;
  bool IsItem() const;
  virtual bool IsMonster() const;
  virtual bool IsAlive() const;
  bool IsPet() const;
  virtual bool IsRunning() const;
  virtual bool IsStandingStill() const;
  ObjectType GetObjectType() const;

  // Returns true or false based on whether the entity is an npc, works on
  // ignite
  bool IsNpc() const;

  bool IsAggro() const;

  // Removes the profession system entities from Ignite Flyff
  // based on the fact that their motion is always 0 I think
  bool IsUnknownEntity() const;

  float DistanceTo( const Entity& entity ) const;

  virtual D3DXVECTOR3 GetPosition() const;
  int32_t GetHealth() const;
  int GetMana() const;
  int GetFatigue() const;
  // int GetSpeed() const;
  int GetFlags() const;
  void SetFlags( const uint32_t flags ) const;
  int GetLevel() const;
  int GetMotion() const;

  bool IsEngaged() const;

  float GetSpeedMultiplier() const;
  void SetSpeedMultiplier( float value ) const;
  virtual void SetPosition( const D3DXVECTOR3 position );

  virtual std::string GetName() const;
  virtual D3DXMATRIX GetWorldMatrix() const;

  virtual void SetWorldMatrix( const D3DXMATRIX& matrix ) const;

  virtual BOUND_BOX GetBoundBox() const;
  virtual void SetBoundBox( const BOUND_BOX& bound_box );

  void RemoveBoundBox();
  void SaveBoundBox();
  void RestoreBoundBox();

  uint32_t GetPointerAddress() const {
    return address_ptr_;
  }

  FlyffClient* GetFlyffClient() const {
    return client_;
  }

 protected:
  FlyffClient* client_;
  uint32_t address_ptr_;
  BOUND_BOX original_bound_box_;
};

}  // namespace bot
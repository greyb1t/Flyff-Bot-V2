#pragma once

//constexpr DWORD OBJ_FLAG_DELETE = 0x00000001;
//constexpr DWORD OBJ_FLAG_VISIBLE = 0x00000002;
//constexpr DWORD OBJ_FLAG_VIRTUAL = 0x00000004;
//constexpr DWORD OBJ_FLAG_UPDATEMATRIX = 0x00000008;
//constexpr DWORD OBJ_FLAG_POSCHANGED = 0x00000010;
//constexpr DWORD OBJ_FLAG_MAX = OBJ_FLAG_DELETE | OBJ_FLAG_VISIBLE |
//                               OBJ_FLAG_VIRTUAL | OBJ_FLAG_UPDATEMATRIX |
//                               OBJ_FLAG_POSCHANGED;
//
//enum class ObjectType : uint32_t {
//  kObjectTypeObject,
//  kObjectTypeAni,
//
//  // Ignite Flyff: Profession Gathering (Tree's, Mining
//  // Stuff...)
//  kObjectTypeCommonCtrl,
//
//  kObjectTypeSfx,
//  kObjectTypeItem,
//
//  // Players, Monsters, Moving Objects...
//  kObjectTypeMover,
//
//  kObjectTypeRegion,
//  kObjectTypeShip,
//  kObjectTypePath,
//
//  kMaxObjectTypes
//};
//
//class FlyffClient;
//
//class GameObject {
// public:
//  GameObject() = default;
//  GameObject( const GameObject& game_object ) = default;
//  virtual ~GameObject() = default;
//
//  int GetFlags() const;
//
//  bool IsDeleted() const;
//
// protected:
//  FlyffClient* client_;
//  uintptr_t address_ptr_;
//};
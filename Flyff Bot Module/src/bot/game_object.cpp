#include "pch.h"
#include "game_object.h"

#include "gwinmem/process_memory_internal.h"
#include "../servers/flyff_client.h"

//int GameObject::GetFlags() const {
//  const auto flags_offset =
//      client_->GetClientVar( MemoryContants::kObjectFlagsOffset );
//
//  return gwinmem::CurrentProcess().Read<int>( address_ptr_ + flags_offset );
//}
//
//bool GameObject::IsDeleted() const {
//  return GetFlags() & OBJ_FLAG_DELETE;
//}

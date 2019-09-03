#pragma once

#include "option.h"
#include "character_rebuff.h"

namespace bot {

class CharacterRebuffListOption : public Option {
 public:
  CharacterRebuffListOption( const std::string& name,
                             const uint32_t control_identifier )
      : Option( name, control_identifier ) {}

  void AddRebuffSequence( const CharacterRebuffOption& rebuff );
  void ReplaceRebuffSequence( uint32_t index,
                              const CharacterRebuffOption& rebuff );
  void RemoveRebuffSequence( const uint32_t index );

  CharacterRebuffOption& GetCharacterRebuff( uint32_t index );

  bool RebuffExists( uint32_t index );

  std::vector<CharacterRebuffOption>& GetRebuffList();

  json GetJson() override;

  void LoadJson( const json& json_parent ) override;

  void RefreshControls() override;

  void EnableOrDisableControls( bool enable ) override;

 private:
  std::vector<CharacterRebuffOption> rebuff_list_;

  const static std::string kRebuffListJsonName;
};

}  // namespace bot
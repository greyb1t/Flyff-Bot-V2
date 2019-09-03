#include "pch.h"
#include "whitelisted_names_option.h"

#include "../../res/resource.h"
#include "option_utils.h"
#include "../utils/string_utls.h"

#include "gwinguiv2/controls/listbox.h"

namespace bot {

const std::string WhitelistedNamesOption::kWhitelistedNamesJsonName =
    "whitelisted_names";

json WhitelistedNamesOption::GetJson() {
  auto j = Option::GetJson();

  j[ kWhitelistedNamesJsonName ] = json::array();

  for ( const auto& value : values_ ) {
    j[ kWhitelistedNamesJsonName ].push_back( value );
  }

  return j;
}

void WhitelistedNamesOption::LoadJson( const json& json_parent ) {
  Option::LoadJson( json_parent );

  if ( JsonExists( json_parent, kWhitelistedNamesJsonName ) ) {
    auto json_names = json_parent.at( name_ ).at( kWhitelistedNamesJsonName );

    for ( auto& elem_name : json_names ) {
      auto value = elem_name.get<std::string>();
      values_.push_back( value );
    }
  }
}

void WhitelistedNamesOption::RefreshControls() {
  Option::RefreshControls();

  for ( auto& value : values_ )
    gwingui::listbox::AddString( GWH( LISTBOX_WHITELIST_NAMES ),
                                 stringutils::AnsiToWide( value ) );
}

void WhitelistedNamesOption::EnableOrDisableControls( bool enable ) {
  gwingui::control::EnableOrDisable( GWH( LISTBOX_WHITELIST_NAMES ), enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_WHITELISTED_NAME ), enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_ADD_WHITELISTED_NAME ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_WHITELIST_NAME_REMOVE ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_WHITELIST_FILL_SELECTED_NAME ),
                                     enable );
}

bool WhitelistedNamesOption::IsEntityAllowed(
    const UniquePtrEntity& entity ) const {
  if ( IsEnabled() ) {
    if ( ValueExists( entity->GetName() ) )
      return true;

    return false;
  }

  return true;
}

}  // namespace bot
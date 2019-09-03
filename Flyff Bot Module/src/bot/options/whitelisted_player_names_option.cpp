#include "pch.h"
#include "whitelisted_player_names_option.h"
#include "../../res/resource.h"

#include "gwinguiv2/controls/listbox.h"

#include "../utils/string_utls.h"

void bot::WhitelistedPlayerNamesOption::RefreshControls() {
  Option::RefreshControls();

  for ( auto& value : values_ ) {
    const auto listbox_handle = GWH( LISTBOX_WHITELIST_PLAYER_NAMES );
    gwingui::listbox::AddString( listbox_handle, stringutils::AnsiToWide(value) );
  }
}

void bot::WhitelistedPlayerNamesOption::EnableOrDisableControls( bool enable ) {
  gwingui::control::EnableOrDisable( GWH( LISTBOX_WHITELIST_PLAYER_NAMES ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( EDIT_WHITELISTED_PLAYER_NAME ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_ADD_WHITELISTED_PLAYER_NAME ),
                                     enable );
  gwingui::control::EnableOrDisable( GWH( BUTTON_WHITELIST_PLAYER_NAME_REMOVE ),
                                     enable );
  gwingui::control::EnableOrDisable(
      GWH( BUTTON_WHITELIST_FILL_SELECTED_PLAYER_NAME ), enable );
}

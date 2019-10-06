#include "pch.h"
#include "option.h"

const std::string Option::kOptionJsonName = "enabled";

void Option::SetStatus( bool enabled ) {
  enabled_ = enabled;
}

bool Option::IsEnabled() const {
  return enabled_;
}

json Option::GetJson() {
  json j;

  j[ kOptionJsonName ] = enabled_;

  return j;
}

void Option::LoadJson( const json& json_parent ) {
  if ( JsonExists( json_parent, name_ ) ) {
    auto json_child = json_parent.at( name_ );

    if ( JsonExists( json_child, kOptionJsonName ) )
      enabled_ = json_child.at( kOptionJsonName ).get<bool>();
  }
}

bool Option::JsonExists( const json& j, const std::string& key ) {
  return j.find( key ) != j.end();
}

void Option::RefreshControls() {
  if ( control_identifier_ ) {
    const auto checkbox_handle = GWH( control_identifier_ );
    gwingui::checkbox::SetCheck( checkbox_handle, enabled_ );
  }

  EnableOrDisableControls( enabled_ );
}

std::string Option::GetName() const {
  return name_;
}

bool Option::IsEntityAllowed( const std::unique_ptr<bot::Entity>& entity ) const {
  assert( false && "unhandled filter" );
  return false;
}

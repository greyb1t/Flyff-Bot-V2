#include "pch.h"
#include "option.h"

const std::string Option::kOptionJsonName =
  "enabled";

json Option::GetJson() {
  json j;

  j[kOptionJsonName] = enabled_;

  return j;
}

void Option::LoadJson( const json &json_parent ) {
  if ( JsonExists( json_parent, name_ ) ) {
    auto json_child = json_parent.at( name_ );

    if ( JsonExists( json_child, kOptionJsonName ) )
      enabled_ = json_child.at(
                   kOptionJsonName ).get<bool>();
  }
}

bool Option::JsonExists( const json &j,
                         const std::string &key ) {
  return j.find( key ) != j.end();
}
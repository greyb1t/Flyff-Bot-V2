#include "pch.h"
#include "remove_all_obstacles_option.h"

#include "../../res/resource.h"

bool RemoveAllObstacleOption::TryApplyOption() {
  const auto checkbox_remove_all_obstacles = GWH( CHECK_REMOVE_ALL_OBSTACLES );

  if ( gwingui::checkbox::IsChecked( checkbox_remove_all_obstacles ) )
    SetStatus( true );
  else
    SetStatus( false );

  return true;
}

#pragma once

#include "gwinguiv2/window.h"

using namespace gwingui;

class ProcessSelectorWindow : public Window {
 public:
  virtual void Initialize( const HWND window_handle ) override;

  virtual void OnButtonClick( const HWND button_handle,
                              const uint32_t button_id ) override;
  virtual void OnListViewDoubleClick( NMITEMACTIVATE* item ) override;
};
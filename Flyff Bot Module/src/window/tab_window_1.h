#pragma once

#include "gwinguiv2/window.h"

namespace bot {

struct RebuffSequenceUiData {};

class TabWindow1 : public gwingui::Window {
 public:
  TabWindow1() : previous_listbox_sequence_index_( -1 ) {}

  virtual void Initialize( const HWND window_handle ) override;

  void OnControlHover( HWND control_handle,
                       uint32_t control_identifier ) override;
  void OnControlLeave( HWND control_handle,
                       uint32_t control_identifier ) override;
  void OnListBoxSelectionChanged( uint32_t control_identifier,
                                  HWND control_handle ) override;
  void OnListBoxItemDeleted( uint32_t control_identifier,
                             HWND control_handle,
                             uint32_t index ) override;
  void OnButtonClick( HWND hCtrl, UINT ctrlId ) override;
  void OnComboboxSelectionChanged( uint32_t control_id,
                                   HWND control_handle ) override;
  void OnListViewItemAdded( NMLISTVIEW* lv ) override;
  void OnListViewItemDeleted( NMLISTVIEW* lv ) override;
  void OnTrackbarSliderChanging( const HWND trackbar_handle,
                                 int32_t position ) override;
  void OnRichEditTextChanged( uint32_t ctrlId, CHANGENOTIFY* pCn ) override;

 private:
  std::map<int, RebuffSequenceUiData> rebuff_sequences_map_;
  int previous_listbox_sequence_index_;
};

}  // namespace bot
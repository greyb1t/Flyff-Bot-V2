#pragma once

#include "d3dx9.h"

namespace bot {

class BackCamera {
 public:
  char pad_0x0000[ 0x4 ];  // 0x0000
  D3DXVECTOR3 m_vPos;  // 0x0004
  D3DXMATRIX m_matView;  // 0x0010
  D3DXMATRIX m_matInvView;  // 0x0050
  D3DXVECTOR3 m_vLookAt;  // 0x0090
  D3DXVECTOR3 m_vPosDest;  // 0x009C
  D3DXVECTOR3 m_vLookAtDest;  // 0x00A8
  D3DXVECTOR3 m_vOffset;  // 0x00B4
  D3DXVECTOR3 m_vOffsetDest;  // 0x00C0
  DWORD m_dwCamStyle;  // 0x00CC
  float m_fRotx;  // 0x00D0
  float m_fRoty;  // 0x00D4
  float m_fCurRotx;  // 0x00D8
  float m_fCurRoty;  // 0x00DC
  __int32 m_nQuakeSec;  // 0x00E0
  float m_fQuakeSize;  // 0x00E4
  __int32 m_bLock;  // 0x00E8
  float m_fLength1;  // 0x00EC
  float m_fLength2;  // 0x00F0
  __int32 m_bOld;  // 0x00F4
  __int32 m_bStart;  // 0x00F8
  float m_fZoom;  // 0x00FC
  char pad_0x0100[ 0x334 ];  // 0x0100
};  // Size=0x0434

}  // namespace bot
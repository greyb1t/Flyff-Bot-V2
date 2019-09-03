#pragma once

#include "d3dx9math.h"

struct BOUND_BOX {
 public:
  static const int kBoxCorners = 8;

  static const BOUND_BOX EMPTY;

  BOUND_BOX() = default;

  BOUND_BOX( std::array<D3DXVECTOR3, BOUND_BOX::kBoxCorners> pos2 )
      : pos( pos2 ) {}

  BOUND_BOX( const BOUND_BOX& other ) {
    operator=( other );
  }

  BOUND_BOX& operator=( const BOUND_BOX& other ) {
    for ( int i = 0; i < kBoxCorners; ++i )
      pos[ i ] = other.pos[ i ];

    return *this;
  }

  bool operator==( const BOUND_BOX& other ) const {
    bool is_equal = true;

    for ( int i = 0; i < BOUND_BOX::kBoxCorners; ++i ) {
      if ( pos[ i ] != other.pos[ i ] ) {
        is_equal = false;
        break;
      }
    }

    return is_equal;
  }

  std::array<D3DXVECTOR3, 8> pos;
};

struct VERTEX {
  float x, y, z, rhw;
  DWORD color;
};

class D3DDrawing {
 public:
  D3DDrawing();
  ~D3DDrawing();

  void SetRenderStates( bool antialiasing );

  void DrawBoundBoxOutline( const BOUND_BOX* bound_box, const D3DCOLOR color );

  void DrawBoundBoxOutlineUnOptimized( const BOUND_BOX* bound_box,
                                       const D3DCOLOR color );

  void DrawBoundBoxFilled( const BOUND_BOX* bound_box, const D3DCOLOR color );

  void DrawRectangle( float x1,
                      float y1,
                      float x2,
                      float y2,
                      float x3,
                      float y3,
                      float x4,
                      float y4,
                      D3DCOLOR color );

  void DrawRectangle( float x,
                      float y,
                      float width,
                      float height,
                      D3DCOLOR color );

  void DrawTriangle( float x1,
                     float y1,
                     float x2,
                     float y2,
                     float x3,
                     float y3,
                     D3DCOLOR color );

  void DrawLine( float x, float y, float x2, float y2, D3DCOLOR color );

  void DrawTextC( const std::wstring& s,
                  int x,
                  int y,
                  D3DCOLOR color,
                  const std::wstring& font_name );

  int GetTextWidth( const std::wstring& s, const std::wstring& unique_name );
  int GetTextHeight( const std::wstring& s, const std::wstring& unique_name );

  inline void SetDevice( LPDIRECT3DDEVICE9 device ) {
    device_ = device;
  }

  inline LPDIRECT3DDEVICE9 GetDevice() const {
    return device_;
  }

  void CreateFontToMap( const std::wstring& font_name,
                        const std::wstring& unique_name,
                        int size );

 private:
  std::map<std::wstring, LPD3DXFONT> font_map_;

  LPDIRECT3DDEVICE9 device_;
};
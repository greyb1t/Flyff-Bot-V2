#include "pch.h"
#include "d3d_drawing.h"

const BOUND_BOX BOUND_BOX::EMPTY( { { { -0.00f, 0.00f, -0.00f },
                                      { -0.00f, 0.00f, -0.00f },
                                      { -0.00f, 0.00f, -0.00f },
                                      { -0.00f, 0.00f, -0.00f },
                                      { -0.00f, 0.00f, -0.00f },
                                      { -0.00f, 0.00f, -0.00f },
                                      { -0.00f, 0.00f, -0.00f },
                                      { -0.00f, 0.00f, -0.00f } } } );

D3DDrawing::D3DDrawing() : device_( 0 ) {}

D3DDrawing::~D3DDrawing() {
  /*
  for ( auto& font : font_map_ )
    font.second->Release();
  */
}

void D3DDrawing::SetRenderStates( bool antialiasing ) {
  /*
  device_->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
  device_->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

  device_->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, antialiasing );

  device_->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
  device_->SetRenderState( D3DRS_LIGHTING, FALSE );

  // Fixes the issue when hovering over something in Flyff, then it stopped
  // drawing
  device_->SetTexture( 0, NULL );
  */
}

void D3DDrawing::DrawBoundBoxOutline( const BOUND_BOX* bound_box,
                                      const D3DCOLOR color ) {
  /*
  // TODO: Can probably optimize even more with DrawIndexedPrimitive()
  // (see this:
  // https://www.gamedev.net/topic/563020-rendering-bounding-boxes-direct3d/)

  // Bounding box outline vertices
  const VERTEX vertices[] = {
    // Top
    { bound_box->pos[ 0 ].x, bound_box->pos[ 0 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 1 ].x, bound_box->pos[ 1 ].y, 0.0f, 1.0f, color },

    { bound_box->pos[ 1 ].x, bound_box->pos[ 1 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 2 ].x, bound_box->pos[ 2 ].y, 0.0f, 1.0f, color },

    { bound_box->pos[ 2 ].x, bound_box->pos[ 2 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 3 ].x, bound_box->pos[ 3 ].y, 0.0f, 1.0f, color },

    { bound_box->pos[ 3 ].x, bound_box->pos[ 3 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 0 ].x, bound_box->pos[ 0 ].y, 0.0f, 1.0f, color },

    // Body
    { bound_box->pos[ 0 ].x, bound_box->pos[ 0 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 4 ].x, bound_box->pos[ 4 ].y, 0.0f, 1.0f, color },

    { bound_box->pos[ 1 ].x, bound_box->pos[ 1 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 5 ].x, bound_box->pos[ 5 ].y, 0.0f, 1.0f, color },

    { bound_box->pos[ 2 ].x, bound_box->pos[ 2 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 6 ].x, bound_box->pos[ 6 ].y, 0.0f, 1.0f, color },

    { bound_box->pos[ 3 ].x, bound_box->pos[ 3 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 7 ].x, bound_box->pos[ 7 ].y, 0.0f, 1.0f, color },

    // Bottom
    { bound_box->pos[ 4 ].x, bound_box->pos[ 4 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 5 ].x, bound_box->pos[ 5 ].y, 0.0f, 1.0f, color },

    { bound_box->pos[ 5 ].x, bound_box->pos[ 5 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 6 ].x, bound_box->pos[ 6 ].y, 0.0f, 1.0f, color },

    { bound_box->pos[ 6 ].x, bound_box->pos[ 6 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 7 ].x, bound_box->pos[ 7 ].y, 0.0f, 1.0f, color },

    { bound_box->pos[ 7 ].x, bound_box->pos[ 7 ].y, 0.0f, 1.0f, color },
    { bound_box->pos[ 4 ].x, bound_box->pos[ 4 ].y, 0.0f, 1.0f, color },
  };

  device_->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

  const int total_vertices = sizeof( vertices ) / sizeof( VERTEX );

  const unsigned short indexes[] = {
    0,  1,  // Vertex top pair 1
    2,  3,  // Vertex top pair 2
    4,  5,  // Vertex top pair 3
    6,  7,  // Vertex top pair 4
    8,  9,  // Vertex body pair 1
    10, 11,  // Vertex body pair 2
    12, 13,  // Vertex body pair 3
    14, 15,  // Vertex body pair 4
    16, 17,  // Vertex bottom pair 1
    18, 19,  // Vertex bottom pair 2
    20, 21,  // Vertex bottom pair 3
    22, 23,  // Vertex bottom pair 4
  };

  device_->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, total_vertices,
                                   total_vertices / 2, indexes, D3DFMT_INDEX16,
                                   vertices, sizeof( VERTEX ) );
  */
}

void D3DDrawing::DrawBoundBoxOutlineUnOptimized( const BOUND_BOX* bound_box,
                                                 const D3DCOLOR color ) {
  /*
  auto draw_line = [ = ]( int x1, int y1, int x2, int y2 ) {
    DrawLine( bound_box->pos[ x1 ].x, bound_box->pos[ y1 ].y,
              bound_box->pos[ x2 ].x, bound_box->pos[ y2 ].y, color );
  };

  draw_line( 0, 0, 1, 1 );
  draw_line( 1, 1, 2, 2 );
  draw_line( 2, 2, 3, 3 );
  draw_line( 3, 3, 0, 0 );
  draw_line( 0, 0, 4, 4 );
  draw_line( 1, 1, 5, 5 );
  draw_line( 2, 2, 6, 6 );
  draw_line( 3, 3, 7, 7 );
  draw_line( 4, 4, 5, 5 );
  draw_line( 5, 5, 6, 6 );
  draw_line( 6, 6, 7, 7 );
  draw_line( 7, 7, 4, 4 );
  */
}

void D3DDrawing::DrawBoundBoxFilled( const BOUND_BOX* bound_box,
                                     const D3DCOLOR color ) {
  /*
  // Front Face
  DrawRectangle( bound_box->pos[ 0 ].x, bound_box->pos[ 0 ].y,
                 bound_box->pos[ 4 ].x, bound_box->pos[ 4 ].y,
                 bound_box->pos[ 1 ].x, bound_box->pos[ 1 ].y,
                 bound_box->pos[ 5 ].x, bound_box->pos[ 5 ].y, color );

  // Left Face
  DrawRectangle( bound_box->pos[ 1 ].x, bound_box->pos[ 1 ].y,
                 bound_box->pos[ 5 ].x, bound_box->pos[ 5 ].y,
                 bound_box->pos[ 2 ].x, bound_box->pos[ 2 ].y,
                 bound_box->pos[ 6 ].x, bound_box->pos[ 6 ].y, color );

  // Right Face
  DrawRectangle( bound_box->pos[ 3 ].x, bound_box->pos[ 3 ].y,
                 bound_box->pos[ 7 ].x, bound_box->pos[ 7 ].y,
                 bound_box->pos[ 0 ].x, bound_box->pos[ 0 ].y,
                 bound_box->pos[ 4 ].x, bound_box->pos[ 4 ].y, color );

  // Back Face
  DrawRectangle( bound_box->pos[ 2 ].x, bound_box->pos[ 2 ].y,
                 bound_box->pos[ 6 ].x, bound_box->pos[ 6 ].y,
                 bound_box->pos[ 3 ].x, bound_box->pos[ 3 ].y,
                 bound_box->pos[ 7 ].x, bound_box->pos[ 7 ].y, color );

  // Top Face
  DrawRectangle( bound_box->pos[ 1 ].x, bound_box->pos[ 1 ].y,
                 bound_box->pos[ 2 ].x, bound_box->pos[ 2 ].y,
                 bound_box->pos[ 0 ].x, bound_box->pos[ 0 ].y,
                 bound_box->pos[ 3 ].x, bound_box->pos[ 3 ].y, color );

  // Bottom Face
  DrawRectangle( bound_box->pos[ 5 ].x, bound_box->pos[ 5 ].y,
                 bound_box->pos[ 6 ].x, bound_box->pos[ 6 ].y,
                 bound_box->pos[ 4 ].x, bound_box->pos[ 4 ].y,
                 bound_box->pos[ 7 ].x, bound_box->pos[ 7 ].y, color );
  */
}

void D3DDrawing::DrawRectangle( float x1,
                                float y1,
                                float x2,
                                float y2,
                                float x3,
                                float y3,
                                float x4,
                                float y4,
                                D3DCOLOR color ) {
  /*
  VERTEX vertices[] = {
    { x1, y1, 0.0f, 1.0f, color },
    { x2, y2, 0.0f, 1.0f, color },
    { x3, y3, 0.0f, 1.0f, color },
    { x4, y4, 0.0f, 1.0f, color },
  };

  device_->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
  device_->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &vertices,
                            sizeof( VERTEX ) );
  */
}

void D3DDrawing::DrawRectangle( float x,
                                float y,
                                float width,
                                float height,
                                D3DCOLOR color ) {
  /*
  DrawRectangle( x, y, x, y + height, x + width, y, x + width, y + height,
                 color );
  */
}

void D3DDrawing::DrawTriangle( float x1,
                               float y1,
                               float x2,
                               float y2,
                               float x3,
                               float y3,
                               D3DCOLOR color ) {
  /*
  VERTEX vertices[] = {
    { x1, y1, 0.0f, 1.0f, color },
    { x2, y2, 0.0f, 1.0f, color },
    { x3, y3, 0.0f, 1.0f, color },
  };

  device_->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
  device_->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 1, &vertices,
                            sizeof( VERTEX ) );
  */
}

void D3DDrawing::DrawLine( float x,
                           float y,
                           float x2,
                           float y2,
                           D3DCOLOR color ) {
  /*
  VERTEX vertices[] = {
    { x, y, 0.0f, 1.0f, color },
    { x2, y2, 0.0f, 1.0f, color },
  };

  device_->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
  device_->DrawPrimitiveUP( D3DPT_LINELIST, 1, vertices, sizeof( VERTEX ) );
  */
}

void D3DDrawing::DrawTextC( const std::wstring& s,
                            int x,
                            int y,
                            D3DCOLOR color,
                            const std::wstring& unique_name ) {
  /*
  RECT rect;
  rect.left = x;
  rect.top = y;

  font_map_[ unique_name ]->DrawText( NULL, s.c_str(), s.size(), &rect,
                                      DT_NOCLIP, color );
  */
}

int D3DDrawing::GetTextWidth( const std::wstring& s,
                              const std::wstring& unique_name ) {
  /*
  RECT rcRect = { 0 };

  font_map_[ unique_name ]->DrawText( NULL, s.c_str(), s.size(), &rcRect,
                                      DT_CALCRECT, D3DCOLOR_XRGB( 0, 0, 0 ) );

  return rcRect.right - rcRect.left;
  */
  return 0;
}

int D3DDrawing::GetTextHeight( const std::wstring& s,
                               const std::wstring& unique_name ) {
  /*
  RECT rcRect = { 0 };

  font_map_[ unique_name ]->DrawText( NULL, s.c_str(), s.size(), &rcRect,
                                      DT_CALCRECT, D3DCOLOR_XRGB( 0, 0, 0 ) );

  return rcRect.bottom - rcRect.top;
  */
  return 0;
}

void D3DDrawing::CreateFontToMap( const std::wstring& font_name,
                                  const std::wstring& unique_name,
                                  int size ) {
  /*
  LPD3DXFONT font;

  // Added the code below for crash identification purposes
  int is_all_ascii = -1;

  for ( auto c : unique_name ) {
    if ( !isascii( c ) ) {
      is_all_ascii = 1337;
      break;
    } else
      is_all_ascii++;
  }

  auto ret =
      D3DXCreateFont( device_, size, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET,
                      OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                      DEFAULT_PITCH | FF_DONTCARE, font_name.c_str(), &font );

  assert( !( ret != S_OK ) );

  font_map_[ unique_name ] = font;
  */
}
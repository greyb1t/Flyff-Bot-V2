#pragma once

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
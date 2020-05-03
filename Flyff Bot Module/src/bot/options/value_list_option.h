#pragma once

#include "option.h"

#include <vector>

namespace bot {

template <typename T>
class ValueListOption : public Option {
 public:
  ValueListOption( const std::string& name, const uint32_t control_identifier )
      : Option( name, control_identifier ) {}

  ~ValueListOption() {}

  void AddValue( const T& value );
  void RemoveValue( const T& value );

  bool ValueExists( const T& value ) const;

  const T& GetAt( uint32_t index ) const;
  uint32_t Count() const;

  void Clear();

  void EnableOrDisableControls( bool enable,
                                bot::BotOptions* bot_options ) override {}

 protected:
  std::vector<T> values_;
};

template <typename T>
void ValueListOption<T>::AddValue( const T& value ) {
  values_.push_back( value );
}

template <typename T>
void ValueListOption<T>::RemoveValue( const T& value ) {
  for ( int i = 0; i < values_.size(); ++i ) {
    if ( values_[ i ] == value )
      values_.erase( values_.begin() + i );
  }
}

template <typename T>
bool ValueListOption<T>::ValueExists( const T& value ) const {
  for ( auto& val : values_ ) {
    if ( val == value )
      return true;
  }

  return false;
}

template <typename T>
const T& ValueListOption<T>::GetAt( uint32_t index ) const {
  return values_[ index ];
}

template <typename T>
uint32_t ValueListOption<T>::Count() const {
  return values_.size();
}

template <typename T>
void ValueListOption<T>::Clear() {
  values_.clear();
}

}  // namespace bot
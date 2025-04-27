#ifndef COMMON_UTILITY_STRING_LITERAL_HPP_
#define COMMON_UTILITY_STRING_LITERAL_HPP_

#include <utility>
#include <string_view>
#include <array>
#include <limits>
#include <algorithm>

namespace common::utility {

template<typename Character, std::size_t Size>
class basic_string_literal {

public:

  using character_type = Character;
  using size_type = std::size_t;
  using iterator = const character_type*;
  using string_view_type = std::basic_string_view<character_type>;

  static constexpr auto npos = std::numeric_limits<size_type>::max();

  constexpr basic_string_literal(const character_type (&data)[Size]) noexcept {
    std::copy_n(data, Size, _data.data());
  }

  constexpr auto begin() const noexcept -> iterator {
    return std::begin(_data);
  }

  constexpr auto end() const noexcept -> iterator {
    return std::end(_data.data());
  }

  constexpr auto data() const noexcept -> const character_type* {
    return _data.data();
  }

  constexpr auto size() const noexcept -> size_type {
    return Size;
  }

  constexpr auto is_empty() const noexcept -> bool {
    return size() == 0;
  }

  constexpr auto operator[](size_type index) const noexcept -> character_type {
    return _data[index];
  }

  constexpr operator string_view_type() const noexcept {
    return string_view_type{_data.data(), Size};
  }

  std::array<character_type, Size> _data;

}; // class basic_string_literal

template<std::size_t Size>
using string_literal = basic_string_literal<char, Size>;

} // namespace common::utility

#endif // COMMON_UTILITY_STRING_LITERAL_HPP_

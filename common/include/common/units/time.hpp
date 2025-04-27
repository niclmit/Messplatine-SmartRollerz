#ifndef COMMON_UNITS_TIME_HPP_
#define COMMON_UNITS_TIME_HPP_

#include <type_traits>

#include <common/units/quantity.hpp>

namespace common::units {

namespace dimension {

struct time { };

} // namespace dimension

using seconds = quantity<dimension::time, std::uint32_t>;

using milli_seconds = quantity<dimension::time, std::uint32_t, std::milli>;
using micro_seconds = quantity<dimension::time, std::uint32_t, std::micro>;
using nano_seconds = quantity<dimension::time, std::uint32_t, std::nano>;

namespace literals {

inline constexpr auto operator""_s(const unsigned long long int value) -> seconds {
  return seconds{static_cast<seconds::value_type>(value)};
}

inline constexpr auto operator""_s(const long double value) -> seconds {
  return seconds{static_cast<seconds::value_type>(value)};
}

inline constexpr auto operator""_ms(const unsigned long long int value) -> milli_seconds {
  return milli_seconds{static_cast<milli_seconds::value_type>(value)};
}

inline constexpr auto operator""_ms(const long double value) -> milli_seconds {
  return milli_seconds{static_cast<milli_seconds::value_type>(value)};
}

inline constexpr auto operator""_us(const unsigned long long int value) -> micro_seconds {
  return micro_seconds{static_cast<micro_seconds::value_type>(value)};
}

inline constexpr auto operator""_us(const long double value) -> micro_seconds {
  return micro_seconds{static_cast<micro_seconds::value_type>(value)};
}

inline constexpr auto operator""_ns(const unsigned long long int value) -> nano_seconds {
  return nano_seconds{static_cast<nano_seconds::value_type>(value)};
}

inline constexpr auto operator""_ns(const long double value) -> nano_seconds {
  return nano_seconds{static_cast<nano_seconds::value_type>(value)};
}

} // namespace literals


} // namespace common::units

#endif // COMMON_UNITS_TIME_HPP_

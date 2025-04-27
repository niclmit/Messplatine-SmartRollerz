#ifndef COMMON_UNITS_ANGLE_HPP_
#define COMMON_UNITS_ANGLE_HPP_

#include <numbers>

#include <common/units/quantity.hpp>

namespace common::units {

namespace dimension {

struct angle { };

} // namespace dimension

using radians = quantity<dimension::angle, std::float_t, float_ratio<1.0f>>;
using degrees = quantity<dimension::angle, std::float_t, float_ratio<std::numbers::pi_v<std::float_t>, 180.0f>>;

namespace literals {

inline constexpr auto operator""_rad(const unsigned long long int value) -> radians {
  return radians{static_cast<radians::value_type>(value)};
}

inline constexpr auto operator""_rad(const long double value) -> radians {
  return radians{static_cast<radians::value_type>(value)};
}

inline constexpr auto operator""_deg(const unsigned long long int value) -> degrees {
  return degrees{static_cast<degrees::value_type>(value)};
}

inline constexpr auto operator""_deg(const long double value) -> degrees {
  return degrees{static_cast<degrees::value_type>(value)};
}

} // namespace literals


} // namespace common::units

#endif // COMMON_UNITS_ANGLE_HPP_

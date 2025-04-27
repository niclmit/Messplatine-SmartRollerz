#ifndef COMMON_UNITS_FREQUENCY_HPP_
#define COMMON_UNITS_FREQUENCY_HPP_

#include <common/units/quantity.hpp>

namespace common::units {

namespace dimension {

struct frequency { };

} // namespace dimension

using hertz = quantity<dimension::frequency, std::uint32_t>;

using kilo_hertz = quantity<dimension::frequency, std::uint32_t, std::kilo>;

namespace literals {

inline constexpr auto operator""_Hz(const unsigned long long int value) -> hertz {
  return hertz{static_cast<hertz::value_type>(value)};
}

inline constexpr auto operator""_Hz(const long double value) -> hertz {
  return hertz{static_cast<hertz::value_type>(value)};
}

inline constexpr auto operator""_kHz(const unsigned long long int value) -> kilo_hertz {
  return kilo_hertz{static_cast<kilo_hertz::value_type>(value)};
}

inline constexpr auto operator""_kHz(const long double value) -> kilo_hertz {
  return kilo_hertz{static_cast<kilo_hertz::value_type>(value)};
}

} // namespace literals


} // namespace common::units

#endif // COMMON_UNITS_FREQUENCY_HPP_

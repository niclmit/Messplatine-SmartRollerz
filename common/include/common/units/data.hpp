#ifndef COMMON_UNITS_DATA_HPP_
#define COMMON_UNITS_DATA_HPP_

#include <common/units/quantity.hpp>

namespace common::units {

namespace dimension {

struct data { };

struct baut { };

} // namespace dimension

using kibi = std::ratio<1024>;

using byte = quantity<dimension::data, std::uint32_t>;

using kilo_byte = quantity<dimension::data, std::uint32_t, std::kilo>;

using kibi_byte = quantity<dimension::data, std::uint32_t, kibi>;

using baut_rate = quantity<dimension::baut, std::uint32_t>;

namespace literals {

inline constexpr auto operator""_Byte(const unsigned long long int value) -> byte {
  return byte{static_cast<byte::value_type>(value)};
}

inline constexpr auto operator""_Byte(const long double value) -> byte {
  return byte{static_cast<byte::value_type>(value)};
}

inline constexpr auto operator""_KByte(const unsigned long long int value) -> kilo_byte {
  return kilo_byte{static_cast<kilo_byte::value_type>(value)};
}

inline constexpr auto operator""_KByte(const long double value) -> kilo_byte {
  return kilo_byte{static_cast<kilo_byte::value_type>(value)};
}

inline constexpr auto operator""_KiByte(const unsigned long long int value) -> kibi_byte {
  return kibi_byte{static_cast<kibi_byte::value_type>(value)};
}

inline constexpr auto operator""_KiByte(const long double value) -> kibi_byte {
  return kibi_byte{static_cast<kibi_byte::value_type>(value)};
}

inline constexpr auto operator""_baut(const unsigned long long int value) -> baut_rate {
  return baut_rate{static_cast<baut_rate::value_type>(value)};
}

inline constexpr auto operator""_baut(const long double value) -> baut_rate {
  return baut_rate{static_cast<baut_rate::value_type>(value)};
}

} // namespace literals


} // namespace common::units

#endif // COMMON_UNITS_DATA_HPP_

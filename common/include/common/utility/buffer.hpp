#ifndef COMMON_UTILITY_BUFFER_HPP_
#define COMMON_UTILITY_BUFFER_HPP_

#include <cstdint>
#include <vector>
#include <array>
#include <span>

namespace common::utility {

using byte = std::uint8_t;

template<std::size_t Size>
using buffer = std::array<byte, Size>;

using buffer_view = std::span<const byte, std::dynamic_extent>;

using mutable_buffer_view = std::span<byte, std::dynamic_extent>;

using dynamic_buffer = std::vector<byte>;

} // namespace common::utility

#endif // COMMON_UTILITY_BUFFER_HPP_

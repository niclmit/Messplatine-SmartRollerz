#ifndef COMMON_UTILITY_FORMAT_HPP_
#define COMMON_UTILITY_FORMAT_HPP_

#include <cstdint>

#include <type_traits>
#include <concepts>
#include <string_view>
#include <span>
#include <limits>
#include <charconv>
#include <algorithm>
#include <array>
#include <ranges>

#include <esp_log.h>

#include <common/utility/enum.hpp>

namespace common::utility {

template<typename Type>
struct formatter {

  template<typename Context>
  static auto format(Context& context, const Type& value) -> bool;

}; // struct formatter

namespace detail {

enum class base : std::uint8_t {
  binary = 2u,
  octal = 8u,
  decimal = 10u,
  hexadecimal = 16u
}; // enum class base

template<std::unsigned_integral Type>
constexpr auto format_primitive(std::span<char> buffer, Type value, const base base = base::decimal) -> std::size_t {
  static constexpr auto digits = std::array<char, 16u>{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

  auto written = std::size_t{0u};

  auto storage = std::array<char, std::numeric_limits<Type>::digits10 + 1u>{};
  auto index = std::size_t{0u};

  do {
    storage[index++] = digits[value % static_cast<std::uint8_t>(base)];
    value /= static_cast<std::uint8_t>(base);
  } while (value > 0);

  for (auto i = 0u; i < index && written < buffer.size(); ++i) {
    buffer[written++] = storage[index - i - 1u];
  }

  return written;
}

template<std::signed_integral Type>
constexpr auto format_primitive(std::span<char> buffer, Type value) -> std::size_t {
  auto written = std::size_t{0u};

  if (value < 0) {
    if (written < buffer.size()) {
      buffer[written++] = '-';
    }

    value = -value;
  }

  return written + format_primitive(buffer.subspan(written), static_cast<std::make_unsigned_t<Type>>(value));
}

template<std::floating_point Type>
constexpr auto format_primitive(std::span<char> buffer, std::floating_point auto value, const std::size_t precision = 6u) -> std::size_t {
  auto written = 0;

  if (value < 0) {
    if (written < buffer.size()) {
      buffer[written++] = '-';
    }

    value = -value;
  }

  auto integer_part = static_cast<std::uint64_t>(value);
  auto fractional_part = value - integer_part;

  written += format_primitive(buffer.subspan(written), integer_part);

  if (written < buffer.size() && precision > 0) {
    buffer[written++] = '.';
  }

  for (int i = 0; i < precision && written < buffer.size(); ++i) {
    fractional_part *= 10;
    auto digit = static_cast<int>(fractional_part);

    if (written < buffer.size()) {
      std::to_chars(buffer.data() + written, buffer.data() + written + 1u, digit);
      ++written;
    }

    fractional_part -= digit;
  }

  return written;
}

using format_string = std::string_view;

struct format_context {

  using character_type = char;
  using storage_type = std::span<character_type>;

  using pointer = storage_type::pointer;
  using size_type = storage_type::size_type;

  storage_type buffer;
  size_type written;

  constexpr auto data() noexcept -> pointer {
    return buffer.data() + written;
  }

  constexpr auto size() noexcept -> size_type {
    return buffer.size() - written;
  }

}; // struct context

template<typename Type, typename... Rest>
inline constexpr auto format_to(format_context& context, format_string format, const Type& value, Rest&&... rest) -> bool {
  const auto next_placeholder = format.find_first_of("{}");

  if (next_placeholder != format_string::npos) {
    if (next_placeholder >= context.size()) {
      std::copy_n(format.data(), context.size(), context.data());
      context.written += context.size();

      return false;
    }

    std::copy_n(format.data(), next_placeholder, context.data());
    context.written += next_placeholder;

    if (!formatter<Type>::format(context, value)) {
      return false;
    }

    return format_to(context, format.substr(next_placeholder + 2u), std::forward<Rest>(rest)...);
  } else {
    if (format.size() >= context.size()) {
      std::copy_n(format.data(), context.size(), context.data());
      context.written += context.size();

      return false;
    }

    std::copy_n(format.data(), format.size(), context.data());
    context.written += format.size();

    return true;
  }
}

inline constexpr auto format_to(format_context& context, format_string format) -> bool {
  if (format.size() >= context.size()) {
    std::copy_n(format.data(), context.size(), context.data());
    context.written += context.size();

    return false;
  }

  std::copy_n(format.data(), format.size(), context.data());

  context.written += format.size();

  return true;
}

} // namespace detail

template<typename Type>
concept formattable = requires(const Type& value) {
  { formatter<Type>::format(std::declval<detail::format_context&>(), value) } -> std::same_as<bool>;
}; // concept formattable

template<typename Type>
requires(std::is_integral_v<Type> || std::is_floating_point_v<Type>)
struct formatter<Type> {

  template<typename Context>
  static auto format(Context& context, const Type& value) -> bool {
    const auto result = detail::format_primitive<Type>(std::span{context.data(), context.size()}, value);

    if (result >= context.size()) {
      context.written += context.size();
      return false;
    }

    context.written += result;
    
    return true;
  }
}; // struct formatter

template<>
struct formatter<const char*> {

  template<typename Context>
  static auto format(Context& context, const char* value) -> bool {
    const auto view = std::string_view{value};

    if (view.size() >= context.size()) {
      std::copy_n(value, context.size(), context.data());
      context.written += context.size();

      return false;
    }

    std::copy_n(value, view.size(), context.data());
    context.written += view.size();

    return true;
  }

}; // struct formatter

template<typename Type>
requires (std::is_enum_v<Type>)
struct formatter<Type> {

  using base_type = formatter<std::underlying_type_t<Type>>;

  template<typename Context>
  static auto format(Context& context, const Type& value) -> bool {
    return base_type::format(context, to_underlying(value));
  }

}; // struct formatter

struct hex {

  std::uint8_t value;

  explicit constexpr hex(const std::uint8_t val) noexcept 
  : value{val} { }

}; // struct hex

template<>
struct formatter<hex> {

  template<typename Context>
  static auto format(Context& context, const hex& value) -> bool {
    const auto result = detail::format_primitive(std::span{context.data(), context.size()}, value.value, detail::base::hexadecimal);

    if (result >= context.size()) {
      context.written += context.size();
      return false;
    }

    context.written += result;

    return true;
  }

}; // struct formatter

template<typename Type>
struct formatter<Type*> {

  using base_type = formatter<std::uintptr_t>;

  template<typename Context>
  static auto format(Context& context, const Type* value) -> bool {
    return base_type::format(context, reinterpret_cast<std::uintptr_t>(value));
  }

}; // struct formatter

template<formattable Type>
struct range {

  std::span<const Type> value;
  std::string_view separator;

  explicit constexpr range(std::span<const Type> val, std::string_view sep = ", ") noexcept
  : value{val},
    separator{sep} { }

  explicit constexpr range(std::span<Type> val, std::string_view sep = ", ") noexcept
  : value{val},
    separator{sep} { }

  template<std::size_t Size>
  explicit constexpr range(const std::array<Type, Size>& val, std::string_view sep = ", ") noexcept
  : value{val},
    separator{sep} { }

}; // struct range

template<formattable Type>
struct formatter<range<Type>> {

  template<typename Context>
  static auto format(Context& context, const range<Type>& value) -> bool {
    auto written = std::size_t{0u};
    auto first = true;

    for (const auto& element : value.value) {
      if (!first && written < context.size() - value.separator.size()) {
        std::copy_n(value.separator.data(), value.separator.size(), context.data());
        context.written += value.separator.size();
      } else {
        first = false;
      }

      if (!formatter<Type>::format(context, element)) {
        return false;
      }
    }

    return true;
  }

}; // struct formatter

template<formattable Type>
requires (std::is_constructible_v<hex, Type>)
struct hexrange {

  std::span<const Type> value;
  std::string_view separator;
  std::string_view prefix;

  explicit constexpr hexrange(std::span<const Type> val, std::string_view sep = ", ", std::string_view pre = "0x") noexcept
  : value{val},
    separator{sep},
    prefix{pre} { }

  explicit constexpr hexrange(std::span<Type> val, std::string_view sep = ", ", std::string_view pre = "0x") noexcept
  : value{val},
    separator{sep},
    prefix{pre} { }

  template<std::size_t Size>
  explicit constexpr hexrange(const std::array<Type, Size>& val, std::string_view sep = ", ", std::string_view pre = "0x") noexcept
  : value{val},
    separator{sep},
    prefix{pre} { }

}; // struct range

template<formattable Type>
struct formatter<hexrange<Type>> {

  template<typename Context>
  static auto format(Context& context, const hexrange<Type>& value) -> bool {
    auto written = std::size_t{0u};
    auto first = true;

    for (const auto& element : value.value) {
      if (!first && written < context.size() - value.separator.size()) {
        std::copy_n(value.separator.data(), value.separator.size(), context.data());
        context.written += value.separator.size();
      } else {
        first = false;
      }

      if (!value.prefix.empty()) {
        std::copy_n(value.prefix.data(), value.prefix.size(), context.data());
        context.written += value.prefix.size();
      }

      if (!formatter<hex>::format(context, hex{element})) {
        return false;
      }
    }

    return true;
  }

}; // struct formatter

struct format_to_result {

  using pointer = detail::format_context::pointer;
  using size_type = detail::format_context::size_type;

  pointer end;
  size_type size;

}; // struct format_to_result

using format_string = detail::format_string;

template<formattable... Args>
auto format_to(std::span<char> buffer, format_string format, Args&&... args) -> format_to_result {
  auto context = detail::format_context{
    .buffer = buffer,
    .written = 0u
  };

  if (!detail::format_to(context, format, std::forward<Args>(args)...)) {
    return format_to_result{context.data(), context.written};
  }

  return format_to_result{context.data(), context.size()};
}

} // namespace common::utility

#endif // COMMON_UTILITY_FORMAT_HPP_

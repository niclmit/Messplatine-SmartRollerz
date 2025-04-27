#ifndef COMMON_UTILITY_RESULT_HPP_
#define COMMON_UTILITY_RESULT_HPP_

#include <variant>

namespace common::utility {

template<typename Type>
class error {

public:

  using value_type = Type;

  constexpr error(const value_type value) noexcept
  : _value{value} { }

  constexpr error(value_type&& value) noexcept
  : _value{std::move(value)} { }

  template<typename... Args>
  requires (std::is_constructible_v<value_type, Args...>)
  constexpr error(Args&&... args) noexcept
  : _value{std::forward<Args>(args)...} { }

  constexpr error(const error& other) noexcept
  : _value{other._value} { }

  constexpr error(error&& other) noexcept
  : _value{std::move(other._value)} { }

  constexpr auto value() const& noexcept -> const value_type& {
    return _value;
  }

  constexpr auto value() & noexcept -> value_type& {
    return _value;
  }

  constexpr auto value() const&& noexcept -> const value_type&& {
    return std::move(_value);
  }

  constexpr auto value() && noexcept -> value_type&& {
    return std::move(_value);
  }

private:

  value_type _value;

}; // class error

template<typename Type, typename Error>
class result {

public:

  using value_type = Type;
  using error_type = Error;

  result(const value_type& value) {

  }

private:

  std::variant<value_type, error_type> _value;

}; // class result

} // namespace common::utility

#endif // COMMON_UTILITY_RESULT_HPP_

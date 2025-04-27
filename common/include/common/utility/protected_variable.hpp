#ifndef COMMON_UTILITY_PROTECTED_VARIABLE_HPP_
#define COMMON_UTILITY_PROTECTED_VARIABLE_HPP_

#include <type_traits>
#include <mutex>

#include <common/utility/mutex.hpp>

namespace common::utility {

/**
 * @brief Wrapper type that secures access to its value with a mutex
 * 
 * @tparam Type Underlying type
 */
template<typename Type, mutex Mutex>
class basic_protected_variable {

public:

  using value_type = Type;
  using mutex_type = Mutex;

  basic_protected_variable() requires (std::default_initializable<Type>) = default;

  basic_protected_variable(const value_type& value)
  : _value{value},
    _mutex{} { }

  basic_protected_variable(value_type&& value)
  : _value{std::move(value)},
    _mutex{} { }

  template<typename... Args>
  requires (std::is_constructible_v<value_type, Args...>)
  basic_protected_variable(Args&&... args)
  : _value{std::forward<Args>(args)...},
    _mutex{} { }

  basic_protected_variable(const basic_protected_variable& other) = delete;

  basic_protected_variable(basic_protected_variable&& other) = delete;

  ~basic_protected_variable() = default;

  auto operator=(const value_type& value) -> basic_protected_variable& {
    auto lock = std::lock_guard{_mutex};

    _value = value;

    return *this;
  }

  auto operator=(value_type&& value) -> basic_protected_variable& {
    auto lock = std::lock_guard{_mutex};

    _value = std::move(value);

    return *this;
  }

  template<typename... Args>
  requires (std::is_constructible_v<value_type, Args...>)
  auto operator=(Args&&... args) -> basic_protected_variable& {
    auto lock = std::lock_guard{_mutex};

    _value = value_type{std::forward<Args>(args)...};

    return *this;
  }

  auto operator=(const basic_protected_variable& other) -> basic_protected_variable& = delete;

  auto operator=(basic_protected_variable&& other) -> basic_protected_variable& = delete;

  auto value() const -> const value_type& {
    auto lock = std::lock_guard{_mutex};
    return _value;
  }

private:

  value_type _value;
  mutable mutex_type _mutex;

}; // class basic_protected_variable

template<typename Type>
using protected_variable_mt = basic_protected_variable<Type, rtos_mutex>;

template<typename Type>
using protected_variable_st = basic_protected_variable<Type, null_mutex>;

template<typename Type>
using protected_variable = protected_variable_mt<Type>;

} // namespace common::utility

#endif // COMMON_UTILITY_PROTECTED_VARIABLE_HPP_

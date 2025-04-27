#ifndef COMMON_UTILITY_MUTEX_HPP_
#define COMMON_UTILITY_MUTEX_HPP_

#include <concepts>
#include <array>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <common/utility/type.hpp>

namespace common::utility {

template<typename Type>
concept mutex = requires(Type m) {
  { m.lock() } -> std::same_as<void>;
  { m.unlock() } -> std::same_as<void>;
}; // concept mutex

template<mutex Mutex>
class guard {

public:

  using mutex_type = Mutex;
  using reference = mutex_type&;

  guard(reference mutex)
  : _mutex{mutex} {
    _mutex.lock();
  }

  ~guard() {
    _mutex.unlock();
  }

private:

  reference _mutex;

}; // class guard

template<mutex Mutex>
guard(Mutex&) -> guard<Mutex>;

template<mutex Mutex>
constexpr auto make_guard(Mutex& mutex) -> guard<Mutex> {
  return guard<Mutex>{mutex};
}

struct null_mutex {
  auto lock() -> void { }
  auto unlock() -> void { }
}; // struct null_mutex

class rtos_mutex {

public:

  using handle_type = SemaphoreHandle_t;
  using buffer_type = StaticSemaphore_t;

  rtos_mutex()
  : _handle{xSemaphoreCreateMutexStatic(&_buffer)} {

  }

  ~rtos_mutex() {
    // vSemaphoreDelete(_handle);
  }

  auto lock() const -> void {
    xSemaphoreTake(_handle, portMAX_DELAY);
  }

  auto unlock() const -> void {
    xSemaphoreGive(_handle);
  }

private:

  buffer_type _buffer;
  handle_type _handle;

}; // class rtos_mutex

template<std::size_t Mod>
constexpr auto fast_mod(const std::size_t value) noexcept -> std::size_t {
  return (value >= Mod) ? value % Mod : value;
}

template<typename Type, std::size_t Size, mutex Mutex = rtos_mutex>
requires (std::is_trivial_v<Type>)
class fixed_queue {

public:

  using value_type = Type;
  using mutex_type = Mutex;
  using size_type = std::size_t;

  fixed_queue()
  : _head{0},
    _tail{0} { }

  constexpr auto size() const noexcept -> size_type {
    return Size;
  }

  auto is_empty() const noexcept -> bool {
    return _head == _tail;
  }

  auto is_full() const noexcept -> bool {
    return fast_mod<Size>(_head + 1) == _tail;
  }

  auto push(const value_type& value) -> bool {
    auto guard = make_guard(_mutex);

    if (is_full()) {
      return false;
    }

    _buffer[_head] = value;
    _head = fast_mod<Size>(_head + 1);

    return true;
  }

  auto front() -> value_type& {
    auto guard = make_guard(_mutex);

    return _buffer[_tail];
  }

  auto front() const -> const value_type& {
    auto guard = make_guard(_mutex);

    return _buffer[_tail];
  }

  auto pop() -> bool {
    auto guard = make_guard(_mutex);

    if (is_empty()) {
      return false;
    }

    _tail = fast_mod<Size>(_tail + 1);

    return true;
  }

private:

  std::array<value_type, Size> _buffer;
  size_type _head;
  size_type _tail;
  mutable mutex_type _mutex;

}; // class fixed_queue


} // namespace common::utility

#endif // COMMON_UTILITY_MUTEX_HPP_

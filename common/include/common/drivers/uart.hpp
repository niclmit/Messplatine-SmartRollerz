#ifndef COMMON_DRIVERS_UART_HPP_
#define COMMON_DRIVERS_UART_HPP_

#include <span>

#include <driver/uart.h>
#include <driver/gpio.h>

#include <common/units/data.hpp>
#include <common/units/time.hpp>

#include <common/utility/buffer.hpp>
#include <common/utility/type.hpp>

namespace common::drivers {

class uart {

public:

  struct config {
    uart_port_t port;
    gpio_num_t rx;
    gpio_num_t tx;
    common::units::byte rx_buffer_size;
    common::units::byte tx_buffer_size;
    common::units::baut_rate baut_rate;
  }; // struct config

  enum class read_result : std::uint8_t {
    success,
    frame_overflow,
    frame_underflow
  }; // enum class read_result

  uart(const config& config);

  ~uart();

  template<typename Type>
  requires (std::is_trivially_copyable_v<Type>)
  auto write(const Type& value) -> void {
    write({reinterpret_cast<const utility::byte*>(&value), sizeof(Type)});
  }

  auto write(utility::buffer_view buffer) -> void;

  template<typename Type>
  requires (std::is_trivially_copyable_v<Type>)
  auto read(Type& value, const units::milli_seconds& wait) -> std::size_t {
    return read({reinterpret_cast<utility::byte*>(&value), sizeof(Type)}, wait);
  }

  auto read(utility::mutable_buffer_view buffer, const units::milli_seconds& wait) -> std::size_t;

private:

  config _config;
  bool _is_synced;

}; // class uart

} // namespace common::drivers

#endif // COMMON_DRIVERS_UART_HPP_

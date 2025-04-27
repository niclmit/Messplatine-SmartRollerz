#ifndef COMMON_DRIVERS_GPIO_HPP_
#define COMMON_DRIVERS_GPIO_HPP_

#include <array>
#include <algorithm>
#include <cinttypes>

#include <driver/gpio.h>

namespace common::drivers {

class gpio {

public:

  struct config {
    gpio_num_t pin;
    gpio_mode_t mode;
    gpio_pullup_t pullup{GPIO_PULLUP_DISABLE};
    gpio_pulldown_t pulldown{GPIO_PULLDOWN_DISABLE};
    gpio_int_type_t interrupt_type{GPIO_INTR_DISABLE};
  }; // struct config

  enum class level : std::uint8_t {
    low = 0,
    high = 1
  }; // enum class level

  gpio(gpio_num_t pin, gpio_mode_t mode, gpio_pullup_t pullup = GPIO_PULLUP_DISABLE, gpio_pulldown_t pulldown = GPIO_PULLDOWN_DISABLE, gpio_int_type_t interrupt_type = GPIO_INTR_DISABLE);

  gpio(const config& config);

  ~gpio();

  auto set_level(const level level) const -> void;

  auto get_level() const -> level;

  config _config;

private:

  

}; // class gpio

} // namespace common::drivers

#endif // COMMON_DRIVERS_GPIO_HPP_

#include <common/drivers/gpio.hpp>

#include <common/utility/enum.hpp>

namespace common::drivers {

gpio::gpio(gpio_num_t pin, gpio_mode_t mode, gpio_pullup_t pullup, gpio_pulldown_t pulldown, gpio_int_type_t interrupt_type)
: gpio{gpio::config{pin, mode, pullup, pulldown, interrupt_type}} { }

gpio::gpio(const config& c)
: _config{c} {
  auto config = gpio_config_t{
    .pin_bit_mask = (1ull << _config.pin),
    .mode = _config.mode,
    .pull_up_en = _config.pullup,
    .pull_down_en = _config.pulldown,
    .intr_type = _config.interrupt_type
  };

  ESP_ERROR_CHECK(gpio_config(&config));
}

gpio::~gpio() {
  gpio_reset_pin(_config.pin);
}

auto gpio::set_level(const level level) const -> void {
  ESP_ERROR_CHECK(gpio_set_level(_config.pin, utility::to_underlying(level)));
}

auto gpio::get_level() const -> level {
  return utility::from_underlying<level>(gpio_get_level(_config.pin));
}

} // namespace common::drivers

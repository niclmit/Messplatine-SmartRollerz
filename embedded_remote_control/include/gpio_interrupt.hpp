#ifndef COMMON_GPIO_INTERRUPT_HPP_
#define COMMON_GPIO_INTERRUPT_HPP_

#include <common/drivers/gpio.hpp>

#include <functional>
#include <unordered_map>

#include <driver/gpio.h>
#include <esp_attr.h> // Include this for IRAM_ATTR macro

namespace common::drivers {

  class gpio_interrupt {
  public:
    using callback_type = std::function<void()>;

    explicit gpio_interrupt(gpio& gpio_instance);

    auto attach(callback_type callback) -> void;
    auto detach() -> void;

  private:
    gpio& _gpio;
    bool _isr_installed = false;
    gpio_num_t _pin;

    static void IRAM_ATTR internal_isr(void* arg);
    inline static std::unordered_map<gpio_num_t, callback_type> _callbacks{};
  };

} // namespace common::drivers

#endif // COMMON_GPIO_INTERRUPT_HPP_

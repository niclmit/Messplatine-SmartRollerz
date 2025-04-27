#include <gpio_interrupt.hpp>
#include <common/drivers/gpio.hpp>



namespace common::drivers {
  gpio_interrupt::gpio_interrupt(gpio& gpio_instance) : _gpio{gpio_instance}, _pin{gpio_instance._config.pin} {
    if(!_isr_installed) {
      gpio_install_isr_service(0); // ISR-Service von ESP-IDF starten
      _isr_installed = true;
    }
  }

  auto gpio_interrupt::attach(callback_type callback) -> void {
    _callbacks[_pin] = std::move(callback);
    gpio_isr_handler_add(_pin, &gpio_interrupt::internal_isr, reinterpret_cast<void*>(static_cast<intptr_t>(_pin)));
  }

  auto gpio_interrupt::detach() -> void {
    _callbacks.erase(_pin);
    gpio_isr_handler_remove(_pin);
  }

  void IRAM_ATTR gpio_interrupt::internal_isr(void* arg) {
    auto pin = static_cast<gpio_num_t>(reinterpret_cast<intptr_t>(arg));
    auto it = _callbacks.find(pin);
    if(it != _callbacks.end()) {
      it->second(); // Callback aufrufen
    }
  }
} // namespace common::drivers
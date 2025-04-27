#ifndef COMMON_DRIVERS_PWM_HPP_
#define COMMON_DRIVERS_PWM_HPP_

#include <cinttypes>
#include <cmath>

#include <driver/gpio.h>
#include <driver/ledc.h>

#include <common/units/frequency.hpp>

namespace common::drivers {

class pwm final {

public:

  struct config {
  gpio_num_t pin;
  ledc_mode_t speed_mode;
  units::hertz frequency;
  ledc_timer_bit_t resolution;
  ledc_timer_t timer_number;
  ledc_channel_t channel;
}; // struct config

  pwm(const config& config)
  : _config{config} {
    const auto timer_config = ledc_timer_config_t{
      .speed_mode = _config.speed_mode,
      .duty_resolution = _config.resolution,
      .timer_num = _config.timer_number,
      .freq_hz = _config.frequency,
      .clk_cfg = LEDC_AUTO_CLK
    };

    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    const auto channel_config = ledc_channel_config_t {
      .gpio_num = _config.pin,
      .speed_mode = _config.speed_mode,
      .channel = _config.channel,
      .intr_type = LEDC_INTR_DISABLE,
      .timer_sel = _config.timer_number,
      .duty = 0,
      .hpoint = 0,
      .flags = {
        .output_invert = false
      }
    };

    ESP_ERROR_CHECK(ledc_channel_config(&channel_config));
  }

  ~pwm() {
    ESP_ERROR_CHECK(ledc_stop(_config.speed_mode, _config.channel, 0));
    ESP_ERROR_CHECK(ledc_timer_rst(_config.speed_mode, _config.timer_number));
  }

  auto set_duty(const std::uint32_t duty) -> void {
    ESP_ERROR_CHECK(ledc_set_duty(_config.speed_mode, _config.channel, duty));
    ESP_ERROR_CHECK(ledc_update_duty(_config.speed_mode, _config.channel));
  }

  auto get_duty() const -> std::uint32_t {
    return ledc_get_duty(_config.speed_mode, _config.channel);
  }

private:

  config _config;

}; // class pwm

} // namespace common::drivers

#endif // COMMON_DRIVERS_PWM_HPP_

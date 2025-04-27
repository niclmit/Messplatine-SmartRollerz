#include <common/drivers/servo.hpp>

namespace common::drivers {

servo::servo(const config& config) noexcept
: _config{config},
  _full_duty{(1u << _config.resolution) - 1u} {
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

servo::~servo() noexcept {
  ESP_ERROR_CHECK(ledc_stop(_config.speed_mode, _config.channel, 0));
  ESP_ERROR_CHECK(ledc_timer_rst(_config.speed_mode, _config.timer_number));
}

auto servo::write_angle(const units::degrees& angle) noexcept -> void {
  const auto duty = _calculate_duty(angle);

  ESP_ERROR_CHECK(ledc_set_duty(_config.speed_mode, _config.channel, duty));
  ESP_ERROR_CHECK(ledc_update_duty(_config.speed_mode, _config.channel));
}

auto servo::read_angle() noexcept -> units::degrees {
  const auto duty = ledc_get_duty(_config.speed_mode, _config.channel);

  return _calculate_angle(duty);
}

auto servo::_calculate_duty(const units::degrees& angle) const noexcept -> duty_circle_type {
  const auto angle_fraction = angle / _config.max_angle; 
  
  const auto angle_us = angle_fraction * static_cast<std::float_t>((_config.max_width - _config.min_width)) + static_cast<std::float_t>(_config.min_width);

  return static_cast<std::uint32_t>(static_cast<std::float_t>(_full_duty) * angle_us * static_cast<std::float_t>(_config.frequency) / 1000000.0f);
}

auto servo::_calculate_angle(const duty_circle_type duty) const noexcept -> units::degrees {
  auto angle_us = static_cast<std::float_t>(duty) * 1000000.0f / static_cast<std::float_t>(_full_duty) / static_cast<std::float_t>(_config.frequency);

  angle_us -= static_cast<std::float_t>(_config.min_width);

  if (angle_us < 0.0f) {
    angle_us = 0.0f;
  }

  return units::degrees{angle_us * static_cast<std::float_t>(_config.max_angle) / (static_cast<std::float_t>(_config.max_width) - static_cast<std::float_t>(_config.min_width))};
}

} // namespace common::drivers

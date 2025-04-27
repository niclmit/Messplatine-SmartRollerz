#ifndef DRIVERS_SERVO_HPP_
#define DRIVERS_SERVO_HPP_

#include <cinttypes>
#include <cmath>

#include <driver/gpio.h>
#include <driver/ledc.h>

#include <common/units/frequency.hpp>
#include <common/units/angle.hpp>
#include <common/units/time.hpp>

namespace common::drivers {

class servo final {

public:

  struct config {
    gpio_num_t pin;
    ledc_mode_t speed_mode;
    units::hertz frequency;
    ledc_timer_bit_t resolution;
    ledc_timer_t timer_number;
    ledc_channel_t channel;
    units::degrees max_angle;
    units::micro_seconds min_width;
    units::micro_seconds max_width;
  }; // struct config

  using duty_circle_type = std::uint32_t;

  servo(const config& config) noexcept;

  ~servo() noexcept;

  auto write_angle(const units::degrees& angle) noexcept -> void;

  auto read_angle() noexcept -> units::degrees;

private:

  auto _calculate_duty(const units::degrees& angle) const noexcept -> duty_circle_type;

  auto _calculate_angle(const duty_circle_type duty) const noexcept -> units::degrees;

  config _config;
  duty_circle_type _full_duty;

}; // class servo

} // namespace common::drivers

#endif // DRIVERS_SERVO_HPP_

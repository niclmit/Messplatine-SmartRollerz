#ifndef COMMON_DRIVERS_I2C_HPP_
#define COMMON_DRIVERS_I2C_HPP_

#include <span>

#include <driver/i2c.h>

#include <common/units/frequency.hpp>

#include <common/utility/buffer.hpp>

namespace common::drivers {

class i2c_slave_config {
    gpio_num_t scl;
    gpio_num_t sda;
    i2c_port_t port;
}; // class i2c_slave_config

class i2c_slave {

public:

  using address = std::uint8_t;

  i2c_slave(const i2c_slave_config& config);

  ~i2c_slave();

private:

  i2c_slave_config _config;

}; // class i2c_slave

class i2c_master {

public:

  struct config {
    i2c_port_t port;
    gpio_num_t scl;
    gpio_num_t sda;
    units::hertz frequency;
  }; // struct config

  i2c_master(const config& config);

  ~i2c_master();

  template<typename Type>
  requires (std::is_trivially_copyable_v<Type>)
  auto write(const i2c_slave::address address, const Type& value) -> bool {
    return write(address, std::span{reinterpret_cast<const std::uint8_t*>(&value), sizeof(Type)});
  }

  auto write(const i2c_slave::address address, utility::buffer_view buffer) -> bool;

  auto read(const i2c_slave::address address, utility::mutable_buffer_view buffer) -> bool;

private:

  config _config;

}; // class i2c_master

} // namespace common::drivers

#endif // COMMON_DRIVERS_I2C_HPP_

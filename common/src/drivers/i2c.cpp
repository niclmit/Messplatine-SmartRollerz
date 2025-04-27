#include <common/drivers/i2c.hpp>

namespace common::drivers {

i2c_slave::i2c_slave(const i2c_slave_config& config) {

}

i2c_slave::~i2c_slave() {

}

i2c_master::i2c_master(const config& config)
: _config{config} {
  auto i2c_config = i2c_config_t{
    .mode = I2C_MODE_MASTER,
    .sda_io_num = _config.sda,       
    .scl_io_num = _config.scl,
    .sda_pullup_en = GPIO_PULLUP_DISABLE,
    .scl_pullup_en = GPIO_PULLUP_DISABLE,
    .master = {
      .clk_speed = _config.frequency,
    },
    .clk_flags = 0,     
  };

  ESP_ERROR_CHECK(i2c_param_config(_config.port , &i2c_config));

  ESP_ERROR_CHECK(i2c_driver_install(_config.port, i2c_config.mode, 0u, 0u, 0u));
}

i2c_master::~i2c_master() {
  i2c_driver_delete(_config.port);
}

auto i2c_master::write(const i2c_slave::address address, utility::buffer_view buffer) -> bool {
  auto cmd = i2c_cmd_link_create();

  i2c_master_start(cmd);

  i2c_master_write_byte(cmd, (address << 1u) | I2C_MASTER_WRITE, true);
  
  i2c_master_write(cmd, buffer.data(), buffer.size(), I2C_MASTER_ACK);
  
  i2c_master_stop(cmd);

  const auto result = i2c_master_cmd_begin(_config.port, cmd, 1000 / portTICK_PERIOD_MS);

  i2c_cmd_link_delete(cmd);

  return (result == ESP_OK);
}

auto i2c_master::read(const i2c_slave::address address, utility::mutable_buffer_view buffer) -> bool {
  auto cmd = i2c_cmd_link_create();

  i2c_master_start(cmd);

  i2c_master_write_byte(cmd, (address << 1u) | I2C_MASTER_READ, true);

  if (buffer.size() > 1) {
    i2c_master_read(cmd, buffer.data(), buffer.size() - 1, I2C_MASTER_ACK);
  }

  i2c_master_read_byte(cmd, buffer.data() + buffer.size() - 1, I2C_MASTER_NACK);

  i2c_master_stop(cmd);

  const auto result = i2c_master_cmd_begin(_config.port, cmd, 1000 / portTICK_PERIOD_MS);

  i2c_cmd_link_delete(cmd);

  return (result == ESP_OK);
}

} // namespace common::drivers

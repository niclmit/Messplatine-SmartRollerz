#include <INA236.hpp>
#include <optional> // Für std::optional
//adress ina 0x40
namespace common::drivers{
    INA236::INA236(i2c_master& master, uint8_t address) : _address(address), _i2c_master(master) {
    }
 
    INA236::~INA236() {
    }

    auto INA236::read(uint8_t reg, utility::mutable_buffer_view buffer) -> bool {
        _i2c_master.write(_address, reg); // Registeradresse an Slave senden
        return _i2c_master.read(_address, buffer); // Daten vom Slave lesen
    }

    auto INA236::write(uint8_t reg, uint16_t data) -> bool {
        common::utility::buffer<3> buffer = {0};
        buffer[0] = reg;
        uint8_t data_lsb = data & 0xFF;
        uint8_t data_msb = (data >> 8) & 0xFF;
        buffer[1] = data_msb;
        buffer[2] = data_lsb;
        return _i2c_master.write(_address, buffer);
    }
    
    auto INA236::system_reset() -> bool {
        return write(static_cast<uint8_t>(Register::CONFIG), static_cast<uint16_t>(ConfigRegister::RESET));
    }

    auto INA236::disable_full_scale_adc_range(bool disable) -> bool {
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::CONFIG), buf))
        {
            uint16_t data = (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
            if (disable){
                data |= static_cast<uint16_t>(ConfigRegister::NOT_FULLSCALE_ADCRANGE);
            } else {
                data &= ~static_cast<uint16_t>(ConfigRegister::NOT_FULLSCALE_ADCRANGE);
            }
            return write(static_cast<uint8_t>(Register::CONFIG), data);
        }
        return false;
    }

    auto INA236::set_averaging_conversions(ConfigRegister avg) -> bool {
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::CONFIG), buf))
        {
            uint16_t data = (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
            data |= static_cast<uint16_t>(avg); 
            return write(static_cast<uint8_t>(Register::CONFIG), data);
        }
        return false;
    }

    auto INA236::set_vbus_conversion_time(ConfigRegister vbus_ct) -> bool {
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::CONFIG), buf))
        {
            uint16_t data = (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
            data |= static_cast<uint16_t>(vbus_ct);
            return write(static_cast<uint8_t>(Register::CONFIG), data);
        }
        return false;
    }

    auto INA236::set_shunt_conversion_time(ConfigRegister shunt_ct) -> bool {
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::CONFIG), buf))
        {
            uint16_t data = (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
            data |= static_cast<uint16_t>(shunt_ct);
            return write(static_cast<uint8_t>(Register::CONFIG), data);
        }
        return false;
    }



    auto INA236::set_mode(ConfigRegister mode) -> bool {
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::CONFIG), buf))
        {
            uint16_t data = (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
            data |= static_cast<uint16_t>(mode);
            return write(static_cast<uint8_t>(Register::CONFIG), data);
        }
        return false;
    }

    auto INA236::read_shunt_voltage(utility::mutable_buffer_view buffer) -> bool {
        return read(static_cast<uint8_t>(Register::SHUNT_VOLTAGE), buffer);
    }


    auto INA236::set_calibration(uint16_t calibration) -> bool {
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::CALIBRATION), buf))
        {
            uint16_t data = (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
            data |= static_cast<uint16_t>(calibration);
            return write(static_cast<uint8_t>(Register::CALIBRATION), data);
        }
        return false;
    }

    auto INA236::read_bus_voltage(utility::mutable_buffer_view buffer) -> bool {
        return read(static_cast<uint8_t>(Register::BUS_VOLTAGE), buffer);
    }

    auto INA236::read_power(utility::mutable_buffer_view buffer) -> bool {
        return read(static_cast<uint8_t>(Register::POWER), buffer);
    }

    auto INA236::read_current(utility::mutable_buffer_view buffer) -> bool {
        return read(static_cast<uint8_t>(Register::CURRENT), buffer);
    }

    auto INA236::enable_alert_pin(MaskEnableRegister limit_type) -> bool {
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::MASK_ENABLE), buf))
        {
            uint16_t data = (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
            data |= static_cast<uint16_t>(limit_type);
            return write(static_cast<uint8_t>(Register::MASK_ENABLE), data);
        }
        return false;
    }

    auto INA236::set_alert_limit(uint16_t limit) -> bool {
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::ALERT_LIMIT), buf))
        {
            uint16_t data = (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
            data |= static_cast<uint16_t>(limit);
            return write(static_cast<uint8_t>(Register::ALERT_LIMIT), data);
        }
        return false;
    }
    
    bool INA236::is_conversion_ready(){
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::MASK_ENABLE), buf))
        {
            return (buf[0] & static_cast<uint8_t>(MaskEnableRegister::CONVERSION_READY));
        }
        return false;
    }

    bool INA236::memory_error(){
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::MASK_ENABLE), buf))
        {
            return (buf[0] & static_cast<uint8_t>(MaskEnableRegister::MEM_ERROR));
        }
        return false;
    }

    bool INA236::is_alert_function_flag_set(){
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::MASK_ENABLE), buf))
        {
            return (buf[0] & static_cast<uint8_t>(MaskEnableRegister::ALERT_FUNCTION_FLAG));
        }
        return false;
    }

    bool INA236::is_conversion_ready_flag_set(){
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::MASK_ENABLE), buf))
        {
            return (buf[0] & static_cast<uint8_t>(MaskEnableRegister::CONVERSION_READY_FLAG));
        }
        return false;
    }

    bool INA236::math_overflow(){
        utility::buffer<2> buf = {0};
        if (read(static_cast<uint8_t>(Register::MASK_ENABLE), buf))
        {
            return (buf[0] & static_cast<uint8_t>(MaskEnableRegister::MATH_OVER_FLOW));
        }
        return false;
    }

    
}
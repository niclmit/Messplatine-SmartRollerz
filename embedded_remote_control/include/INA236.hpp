#ifndef INA236_HPP_
#define INA236_HPP_

#include <common/drivers/i2c.hpp>

namespace common::drivers {
    class INA236 {
        public:
            const i2c_slave::address _address;
            i2c_master& _i2c_master;

            // Register addresses
            enum class Register : uint8_t {
                CONFIG = 0x00,
                SHUNT_VOLTAGE = 0x01,
                BUS_VOLTAGE = 0x02,
                POWER = 0x03,
                CURRENT = 0x04,
                CALIBRATION = 0x05,
                MASK_ENABLE = 0x06,
                ALERT_LIMIT = 0x07,
                MANUFACTURER_ID = 0xFE,
                DIE_ID = 0xFF
            };

            // Bitmasks for the config register
            enum class ConfigRegister : uint16_t {
                RESET = (1 << 15),
                NOT_FULLSCALE_ADCRANGE = (1 << 12),
                AVG_CONVERSIONS_1 = (0b000 << 9),
                AVG_CONVERSIONS_4 = (0b001 << 9),
                AVG_CONVERSIONS_16 = (0b010 << 9),
                AVG_CONVERSIONS_64 = (0b011 << 9),
                AVG_CONVERSIONS_128 = (0b100 << 9),
                AVG_CONVERSIONS_256 = (0b101 << 9),
                AVG_CONVERSIONS_512 = (0b110 << 9),
                AVG_CONVERSIONS_1024 = (0b111 << 9),
                VBUS_CT_140US = (0b000 << 6),
                VBUS_CT_204US = (0b001 << 6),
                VBUS_CT_332US = (0b010 << 6),
                VBUS_CT_588US = (0b011 << 6),
                VBUS_CT_1100US = (0b100 << 6),
                VBUS_CT_2116US = (0b101 << 6),
                VBUS_CT_4156US = (0b110 << 6),
                VBUS_CT_8244US = (0b111 << 6),
                SHUNT_CT_140US = (0b000 << 3),
                SHUNT_CT_204US = (0b001 << 3),
                SHUNT_CT_332US = (0b010 << 3),
                SHUNT_CT_588US = (0b011 << 3),
                SHUNT_CT_1100US = (0b100 << 3),
                SHUNT_CT_2116US = (0b101 << 3),
                SHUNT_CT_4156US = (0b110 << 3),
                SHUNT_CT_8244US = (0b111 << 3),
                MODE_SHUNTDOWN_1 = (0b000 << 0),
                MODE_SHUNT_SINGLE_SHOT = (0b001 << 0),
                MODE_BUS_SINGLE_SHOT = (0b010 << 0),
                MODE_SHUNT_BUS_SINGLE_SHOT = (0b011 << 0),
                MODE_SHUNTDOWN_2 = (0b100 << 0),
                MODE_SHUNT_CONTINUOUS = (0b101 << 0),
                MODE_BUS_CONTINUOUS = (0b110 << 0),
                MODE_SHUNT_BUS_CONTINUOUS = (0b111 << 0)
            };

            //Bitmask for the calibration register
            enum class CalibrationRegister : uint16_t {
                CALIBRATION = 0x7FFF
            };

            // Bitmasks for the mask enable register
            enum class MaskEnableRegister : uint16_t {
                SHUNT_OVER_LIMIT = (1 << 15),
                SHUNT_UNDER_LIMIT = (1 << 14),
                BUS_OVER_LIMIT = (1 << 13),
                BUS_UNDER_LIMIT = (1 << 12),
                POWER_OVER_LIMIT = (1 << 11),
                CONVERSION_READY = (1 << 10),
                MEM_ERROR = (1 << 5),
                ALERT_FUNCTION_FLAG = (1 << 4),
                CONVERSION_READY_FLAG = (1 << 3),
                MATH_OVER_FLOW = (1 << 2),
                ALERT_ALERT_POLARITY = (1 << 1),
                ALERT_LATCH_ENABLE = (1 << 0)
            };
        
        INA236(i2c_master& i2c_master, uint8_t address);
        ~INA236();
        
        auto read(uint8_t reg, utility::mutable_buffer_view buffer) -> bool;
        auto write(uint8_t reg, uint16_t data) -> bool;
        auto system_reset() -> bool;
        auto disable_full_scale_adc_range(bool disable) -> bool;
        auto set_averaging_conversions(ConfigRegister avg) -> bool;
        auto set_vbus_conversion_time(ConfigRegister vbus_ct) -> bool;
        auto set_shunt_conversion_time(ConfigRegister shunt_ct) -> bool;
        auto set_mode(ConfigRegister mode) -> bool;
        auto read_shunt_voltage(utility::mutable_buffer_view buffer) -> bool;
        auto read_bus_voltage(utility::mutable_buffer_view buffer) -> bool;
        auto read_power(utility::mutable_buffer_view buffer) -> bool;
        auto read_current(utility::mutable_buffer_view buffer) -> bool;
        auto set_calibration(uint16_t calibration_value) -> bool;
        auto enable_alert_pin(MaskEnableRegister limit_type) -> bool;
        auto set_alert_limit(uint16_t limit) -> bool;
        bool is_conversion_ready();
        bool memory_error();
        bool is_alert_function_flag_set();
        bool is_conversion_ready_flag_set();
        bool math_overflow();
    };
}

#endif // INA236_HPP_
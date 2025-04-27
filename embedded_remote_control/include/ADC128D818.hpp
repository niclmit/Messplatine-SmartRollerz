#ifndef ADC128D818_HPP_
#define ADC128D818_HPP_

#include <common/drivers/i2c.hpp>

namespace common::drivers {
    class ADC128D818 {
        public:
            uint8_t _address;
            i2c_master& _i2c_master;

            // Register addresses
            enum class Register : uint8_t {
                CONFIG = 0x00,
                INTERRUPT_STATUS = 0x01,
                INTERRUPT_MASK = 0x03,
                CONVERSION_RATE = 0x07,
                CHANNEL_DISABLE = 0x08,
                ONE_SHOT = 0x09,
                DEEP_SHUTDOWN = 0x0A,
                ADVANCED_CONFIG = 0x0B,
                BUSY_STATUS = 0x0C,
                CHANNEL_READINGS_START = 0x20,
                CHANNEL_READINGS_END = 0x27,
                IN0_READ = 0x20,
                IN1_READ = 0x21,
                IN2_READ = 0x22,
                IN3_READ = 0x23,
                IN4_READ = 0x24,
                IN5_READ = 0x25,
                IN6_READ = 0x26,
                TEMPERATURE_READ = 0x27,
                IN0_HIGH_LIMIT = 0x2A,
                IN0_LOW_LIMIT = 0x2B,
                IN1_HIGH_LIMIT = 0x2C,
                IN1_LOW_LIMIT = 0x2D,
                IN2_HIGH_LIMIT = 0x2E,
                IN2_LOW_LIMIT = 0x2F,
                IN3_HIGH_LIMIT = 0x30,
                IN3_LOW_LIMIT = 0x31,
                IN4_HIGH_LIMIT = 0x32,
                IN4_LOW_LIMIT = 0x33,
                IN5_HIGH_LIMIT = 0x34,
                IN5_LOW_LIMIT = 0x35,
                IN6_HIGH_LIMIT = 0x36,
                IN6_LOW_LIMIT = 0x37,
                TEMPERATURE_HIGH_LIMIT = 0x38,
                TEMPERATURE_HYSTERESIS_LIMIT = 0x39
            };

            // Bitmasks for the config register
            enum class ConfigRegister : uint8_t {
                START = (1 << 0),
                INT_ENABLE = (1 << 1),
                INT_CLEAR = (1 << 3),
                RESET_REGISTER = (1 << 7)
            };

            // Bitmasks for the interrupt status register
            enum class InterruptStatusRegister : uint8_t {
                IN0_ERROR = (1 << 0),
                IN1_ERROR = (1 << 1),
                IN2_ERROR = (1 << 2),
                IN3_ERROR = (1 << 3),
                IN4_ERROR = (1 << 4),
                IN5_ERROR = (1 << 5),
                IN6_ERROR = (1 << 6),
                HOT_TEMP_ERROR = (1 << 7)
            };

            // Bitmasks for the interrupt mask register
            enum class InterruptMaskRegister : uint8_t {
                IN0_MASK = (1 << 0),
                IN1_MASK = (1 << 1),
                IN2_MASK = (1 << 2),
                IN3_MASK = (1 << 3),
                IN4_MASK = (1 << 4),
                IN5_MASK = (1 << 5),
                IN6_MASK = (1 << 6),
                TEMP_MASK = (1 << 7)
            };

            // Bitmasks for the conversion rate register
            enum class ConversionRateRegister : uint8_t {
                LOW_POWER_CONVERSION_MODE = (0 << 0),
                HIGH_POWER_CONVERSION_MODE = (1 << 0),
            };

            // Bitmasks for the channel disable register
            enum class ChannelDisableRegister : uint8_t {
                IN0_DISABLE = (1 << 0),
                IN1_DISABLE = (1 << 1),
                IN2_DISABLE = (1 << 2),
                IN3_DISABLE = (1 << 3),
                IN4_DISABLE = (1 << 4),
                IN5_DISABLE = (1 << 5),
                IN6_DISABLE = (1 << 6),
                TEMP_DISABLE = (1 << 7)
            };

            // Bitmasks for the one shot register
            enum class OneShotRegister : uint8_t {
                ONE_SHOT_MODE = (1 << 0)
            };

            // Bitmasks for the deep shutdown register
            enum class DeepShutdownRegister : uint8_t {
                DEEP_SHUTDOWN_ENABLE = (1 << 0)
            };

            // Bitmasks for the advanced config register
            enum class AdvancedConfigRegister : uint8_t {
                EXTERNAL_REF_ENABLE = (1 << 0),
                MODE_0 = (0b00 << 1),
                MODE_1 = (0b01 << 1),
                MODE_2 = (0b10 << 1),
                MODE_3 = (0b11 << 1)
            };

            // Bitmasks for the busy status register
            enum class BusyStatusRegister : uint8_t {
                BUSY = (1 << 0),
                NOT_READY = (1 << 1)
            };

        
            ADC128D818(uint8_t addr, i2c_master& master);
            ~ADC128D818();
            auto read(uint8_t reg, utility::mutable_buffer_view buffer) -> bool;
            auto write(uint8_t reg, uint8_t data) -> bool;
            auto reset_all_registers() -> bool;
            auto enable_interrupt() -> bool;
            auto clear_interrupt() -> bool;
            auto set_initialization() -> bool;
            auto get_interrupt_status(utility::mutable_buffer_view buffer) -> bool;
            auto interrupt_mask(InterruptMaskRegister mask) -> bool;
            auto set_conversion_rate(ConversionRateRegister rate) -> bool;
            auto disable_channel(ChannelDisableRegister channel) -> bool;
            auto enable_channel(ChannelDisableRegister channel) -> bool;
            auto one_shot_conversion() -> bool;
            auto deep_shutdown() -> bool;
            auto exit_deep_shutdown() -> bool;
            auto set_external_ref(bool external) -> bool;
            auto set_mode(AdvancedConfigRegister mode) -> bool;
            auto set_high_limit(Register channel, uint8_t limit) -> bool;
            auto set_low_limit(Register channel, uint8_t limit) -> bool;
            bool is_not_ready();
            auto read_channel(Register channel, utility::mutable_buffer_view buffer) -> bool;
            auto read_temperature(utility::mutable_buffer_view buffer) -> bool;

    };
} // namespace common::drivers
#endif // ADC128D818_HPP_
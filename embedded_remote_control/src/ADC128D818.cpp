#include <ADC128D818.hpp>
#include <optional> // Für std::optional


//buffer muss noch auf die richtige Größe geändert werden!!!!
namespace common::drivers {
    ADC128D818::ADC128D818(uint8_t address, i2c_master& master) : _address(address), _i2c_master(master) {
    }

    ADC128D818::~ADC128D818() {
    }

    

    auto ADC128D818::read(uint8_t reg, utility::mutable_buffer_view buffer) -> bool {
        _i2c_master.write(_address, reg); // Registeradresse an Slave senden
        return _i2c_master.read(_address, buffer); // Daten vom Slave lesen
    }


    auto ADC128D818::write(uint8_t reg, uint8_t data) -> bool{
        common::utility::buffer<2> buffer = {0};
        buffer[0] = reg;
        buffer[1] = data;
        return _i2c_master.write(_address, buffer);
    }

    auto ADC128D818::reset_all_registers() -> bool {
        utility::buffer<1> config = {0};
        if (read(static_cast<uint8_t>(Register::CONFIG), config))
        {
            config[0] = static_cast<uint8_t>(ConfigRegister::RESET_REGISTER);
            return write(static_cast<uint8_t>(Register::CONFIG), config[0]);
        }
        return false;
    }

    auto ADC128D818::enable_interrupt() -> bool {
        utility::buffer<1> config = {0};
        if (read(static_cast<uint8_t>(Register::CONFIG), config))
        {
            config[0] |= static_cast<uint8_t>(ConfigRegister::INT_ENABLE);
            return write(static_cast<uint8_t>(Register::CONFIG), config[0]);
        }
        return false;
    }

    auto ADC128D818::clear_interrupt() -> bool {
        utility::buffer<1> config = {0};
        if (read(static_cast<uint8_t>(Register::CONFIG), config))
        {
            config[0] |= static_cast<uint8_t>(ConfigRegister::INT_CLEAR);
            return write(static_cast<uint8_t>(Register::CONFIG), config[0]);
        }
        return false;
    }

    auto ADC128D818::set_initialization() -> bool {
        utility::buffer<1> config = {0};
        if (read(static_cast<uint8_t>(Register::CONFIG), config))
        {
            // printf("CONFIG: 0x%02X\n", config[0]);
            config[0] |= static_cast<uint8_t>(ConfigRegister::START);
            config[0] &= ~static_cast<uint8_t>(ConfigRegister::INT_CLEAR);
            return write(static_cast<uint8_t>(Register::CONFIG), config[0]);
        }
        return false;
    }

    auto ADC128D818::interrupt_mask(InterruptMaskRegister mask) -> bool {
        utility::buffer<1> buf = {0};
        if (read(static_cast<uint8_t>(Register::INTERRUPT_MASK), buf))
        {
            buf[0] |= static_cast<uint8_t>(mask);
            return write(static_cast<uint8_t>(Register::INTERRUPT_MASK), buf[0]);
            
        }
        return false;
    }

    auto ADC128D818::set_conversion_rate(ConversionRateRegister rate) -> bool {
        utility::buffer<1> buf = {0};
        if (read(static_cast<uint8_t>(Register::CONVERSION_RATE), buf))
        {
            buf[0] |= static_cast<uint8_t>(rate);
            return write(static_cast<uint8_t>(Register::CONVERSION_RATE), buf[0]);
        }
        return false;
    }

    auto ADC128D818::disable_channel(ChannelDisableRegister disable) -> bool {
        utility::buffer<1> buf = {0};
        if (read(static_cast<uint8_t>(Register::CHANNEL_DISABLE), buf))
        {
            buf[0] |= static_cast<uint8_t>(disable);
            return write(static_cast<uint8_t>(Register::CHANNEL_DISABLE), buf[0]);
        }
        return false;
    }

    auto ADC128D818::enable_channel(ChannelDisableRegister enable) -> bool {
        utility::buffer<1> buf = {0};
        if (read(static_cast<uint8_t>(Register::CHANNEL_DISABLE), buf))
        {
            buf[0] &= ~static_cast<uint8_t>(enable);
            return write(static_cast<uint8_t>(Register::CHANNEL_DISABLE), buf[0]);
        }
        return false;
    }

    auto ADC128D818::one_shot_conversion() -> bool {
        utility::buffer<1> buf = {0};
        if (read(static_cast<uint8_t>(Register::ONE_SHOT), buf))
        {
            buf[0] |= static_cast<uint8_t>(OneShotRegister::ONE_SHOT_MODE);
            return write(static_cast<uint8_t>(Register::ONE_SHOT), buf[0]);
        }
        return false;
    }

    auto ADC128D818::deep_shutdown() -> bool {
        utility::buffer<1> buf = {0};
        if (read(static_cast<uint8_t>(Register::DEEP_SHUTDOWN), buf))
        {
            buf[0] |= static_cast<uint8_t>(DeepShutdownRegister::DEEP_SHUTDOWN_ENABLE);
            return write(static_cast<uint8_t>(Register::DEEP_SHUTDOWN), buf[0]);
        }
        return false;
    }

    auto ADC128D818::exit_deep_shutdown() -> bool {
        utility::buffer<1> buf = {0};
        if (read(static_cast<uint8_t>(Register::DEEP_SHUTDOWN), buf))
        {
            buf[0] &= ~static_cast<uint8_t>(DeepShutdownRegister::DEEP_SHUTDOWN_ENABLE);
            return write(static_cast<uint8_t>(Register::DEEP_SHUTDOWN), buf[0]);
        }
        return false;
    }

    auto ADC128D818::set_external_ref(bool external) -> bool {
        utility::buffer<1> buf = {0};
        if (read(static_cast<uint8_t>(Register::ADVANCED_CONFIG), buf))
        {
            if (external)
            {
                buf[0] |= static_cast<uint8_t>(AdvancedConfigRegister::EXTERNAL_REF_ENABLE);
            }
            else
            {
                buf[0] &= ~static_cast<uint8_t>(AdvancedConfigRegister::EXTERNAL_REF_ENABLE);
            }
            return write(static_cast<uint8_t>(Register::ADVANCED_CONFIG), buf[0]);
        }
        return false;
    }

    bool ADC128D818::is_not_ready(){
        utility::buffer<1> buffer = {0};
        if(read(static_cast<uint8_t>(Register::BUSY_STATUS), buffer))
        {
            return (buffer[0] & static_cast<uint8_t>(BusyStatusRegister::NOT_READY));
        }
        printf("Busy Status not read\n");
        return false;
    }
    
    auto ADC128D818::set_mode(AdvancedConfigRegister mode) -> bool {
        utility::buffer<8> buf = {0};
        if (read(static_cast<uint8_t>(Register::ADVANCED_CONFIG), buf))
        {
            buf[0] |= static_cast<uint8_t>(mode);
            return write(static_cast<uint8_t>(Register::ADVANCED_CONFIG), buf[0]);
        }
        return false;
    }

    auto ADC128D818::set_high_limit(Register channel, uint8_t limit) -> bool {
        utility::buffer<8> buf = {0};
        if (read(static_cast<uint8_t>(channel), buf))
        {
            buf[0] = limit;
            return write(static_cast<uint8_t>(channel), buf[0]);
        }
        return false;
    }

    auto ADC128D818::set_low_limit(Register channel, uint8_t limit) -> bool {
        utility::buffer<8> buf = {0};
        if (read(static_cast<uint8_t>(channel), buf))
        {
            buf[0] = limit;
            return write(static_cast<uint8_t>(channel), buf[0]);
        }
        return false;
    }

    auto ADC128D818::get_interrupt_status(utility::mutable_buffer_view buffer) -> bool {
        return read(static_cast<uint8_t>(Register::INTERRUPT_STATUS), buffer);
    }

    auto ADC128D818::read_channel(Register channel, utility::mutable_buffer_view buffer) -> bool {
        return read(static_cast<uint8_t>(channel), buffer);
    }

    auto ADC128D818::read_temperature(utility::mutable_buffer_view buffer) -> bool {
        return read(static_cast<uint8_t>(Register::TEMPERATURE_READ), buffer);
    }
} // namespace common::drivers
#include <tasks.hpp>
#include <common/units/time.hpp>
#include <globals_freertos.hpp>


const float V_REF = 2.56;  // Referenzspannung des ADC
const int ADC_MAX = 4096;  // 12-Bit-ADC → 2^12 = 4096

common::drivers::gpio ON_PIN(GPIO_NUM_47, GPIO_MODE_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_DISABLE);

void print_values(uint8_t value, uint8_t callback_id) {
    printf("PWM Value: %d\n", value);
    printf("Callback ID: %d\n", callback_id); // Hier kannst du die Callback-ID anpassen
}

void print_values(uint64_t value, uint8_t callback_id) {
    printf("PWM Value: %" PRIu64 "\n", value);
    printf("Callback ID: %d\n", callback_id); // Hier kannst du die Callback-ID anpassen
}

void print_values(float value, uint8_t callback_id) {
    printf("Temperature Value: %f\n", value);
    printf("Callback ID: %d\n", callback_id); // Hier kannst du die Callback-ID anpassen
}


void send_data_uart(uint8_t data) {
    // Hier kannst du den Code zum Senden der Daten über UART hinzufügen
    // uart_write_bytes(UART_NUM_1, (const char*)&data, sizeof(data));
    printf("UART Data: %d\n", data); // Beispielausgabe
}


uint8_t mapTo8Bit(int64_t x, int64_t in_min, int64_t in_max) {
    if (in_min == in_max) return 0; // Vermeidung von Division durch 0

    // Clamp den Wert auf den Eingabebereich
    if (x < in_min) x = in_min;
    if (x > in_max) x = in_max;

    // Berechnung
    return static_cast<uint8_t>((x - in_min) * 255 / (in_max - in_min));
}



ADC_Channel_Config adc_channels[8] = {
    {common::drivers::ADC128D818::Register::IN0_READ, 0.4563},
    {common::drivers::ADC128D818::Register::IN1_READ, 0.2271},
    {common::drivers::ADC128D818::Register::IN2_READ, 0.1483},
    {common::drivers::ADC128D818::Register::IN3_READ, 0.1155},
    {common::drivers::ADC128D818::Register::IN4_READ, 0.0909},
    {common::drivers::ADC128D818::Register::IN5_READ, 0.0750},
    {common::drivers::ADC128D818::Register::IN6_READ, 0.0646},
    {common::drivers::ADC128D818::Register::TEMPERATURE_READ, 1.0}
};

uint8_t adc_raw_channel_voltage[5] = {0};
uint8_t adc_cell_voltage[5] = {0};

void set_adc_limits(common::drivers::ADC128D818& adc) {
    adc.set_high_limit(common::drivers::ADC128D818::Register::IN0_HIGH_LIMIT, 0xFF);
    adc.set_low_limit(common::drivers::ADC128D818::Register::IN0_LOW_LIMIT, 0x00);
    adc.set_high_limit(common::drivers::ADC128D818::Register::IN1_HIGH_LIMIT, 0xFF);
    adc.set_low_limit(common::drivers::ADC128D818::Register::IN1_LOW_LIMIT, 0x00);
    adc.set_high_limit(common::drivers::ADC128D818::Register::IN2_HIGH_LIMIT, 0xFF);
    adc.set_low_limit(common::drivers::ADC128D818::Register::IN2_LOW_LIMIT, 0x00);
    adc.set_high_limit(common::drivers::ADC128D818::Register::IN3_HIGH_LIMIT, 0xFF);
    adc.set_low_limit(common::drivers::ADC128D818::Register::IN3_LOW_LIMIT, 0x00);
    adc.set_high_limit(common::drivers::ADC128D818::Register::IN4_HIGH_LIMIT, 0xFF);
    adc.set_low_limit(common::drivers::ADC128D818::Register::IN4_LOW_LIMIT, 0x00);
    adc.set_high_limit(common::drivers::ADC128D818::Register::IN5_HIGH_LIMIT, 0xFF);
    adc.set_low_limit(common::drivers::ADC128D818::Register::IN5_LOW_LIMIT, 0x00);
    adc.set_high_limit(common::drivers::ADC128D818::Register::IN6_HIGH_LIMIT, 0xFF);
    adc.set_low_limit(common::drivers::ADC128D818::Register::IN6_LOW_LIMIT, 0x00);
    adc.set_high_limit(common::drivers::ADC128D818::Register::TEMPERATURE_HIGH_LIMIT, 0xFF);
    adc.set_low_limit(common::drivers::ADC128D818::Register::TEMPERATURE_HYSTERESIS_LIMIT, 0x00);
}


void set_adc_enable(common::drivers::ADC128D818& adc) {
    adc.enable_channel(common::drivers::ADC128D818::ChannelDisableRegister::IN0_DISABLE);
    adc.enable_channel(common::drivers::ADC128D818::ChannelDisableRegister::IN1_DISABLE);
    adc.enable_channel(common::drivers::ADC128D818::ChannelDisableRegister::IN2_DISABLE);
    adc.enable_channel(common::drivers::ADC128D818::ChannelDisableRegister::IN3_DISABLE);
    adc.enable_channel(common::drivers::ADC128D818::ChannelDisableRegister::IN4_DISABLE);
    adc.enable_channel(common::drivers::ADC128D818::ChannelDisableRegister::IN5_DISABLE);
    adc.enable_channel(common::drivers::ADC128D818::ChannelDisableRegister::IN6_DISABLE);
    adc.enable_channel(common::drivers::ADC128D818::ChannelDisableRegister::TEMP_DISABLE);
}

void init_adc(common::drivers::ADC128D818& adc) {
    adc.reset_all_registers();
    bool ready = !adc.is_not_ready();
    while (!ready)
    {
        printf("ADC128D818 is busy\n");
        ready = adc.is_not_ready();
    }
    adc.set_mode(common::drivers::ADC128D818::AdvancedConfigRegister::MODE_0);
    adc.set_external_ref(false);
    adc.set_conversion_rate(common::drivers::ADC128D818::ConversionRateRegister::HIGH_POWER_CONVERSION_MODE);
    set_adc_enable(adc);
    // set_adc_limits(adc);
    adc.set_initialization();

}

void calculate_cell_voltage(){
    adc_cell_voltage[0] = adc_raw_channel_voltage[0];
    for(int cell_counter = 1; cell_counter < num_channels; cell_counter++){
        adc_cell_voltage[cell_counter] = adc_raw_channel_voltage[cell_counter] - adc_raw_channel_voltage[cell_counter - 1];
    }
}



void init_ina(common::drivers::INA236& ina) {
    // ina.system_reset();
    // ina.disable_full_scale_adc_range(false);
    // ina.set_mode(common::drivers::INA236::ConfigRegister::MODE_SHUNT_CONTINUOUS);
    // ina.set_calibration(0x8000); // Set calibration value
    // ina.set_averaging_conversions(common::drivers::INA236::ConfigRegister::AVG_CONVERSIONS_128);
    // ina.set_vbus_conversion_time(common::drivers::INA236::ConfigRegister::VBUS_CT_1100US);
    // ina.set_shunt_conversion_time(common::drivers::INA236::ConfigRegister::SHUNT_CT_8244US);
}

auto read_i2c_sensors_task(common::task::parameters parameters) -> void
{
    using namespace common::units::literals;
    
    auto i2c_master = common::drivers::i2c_master{{
        .port = I2C_NUM_0,
        .scl = GPIO_NUM_11,
        .sda = GPIO_NUM_10,
        .frequency = 1000_Hz,
    }};


    // Initialize the INA236
    auto i2c_slave_ina = 0x40;
    common::drivers::INA236 ina1(i2c_master, i2c_slave_ina);
    init_ina(ina1);

    auto i2c_slave_ina2 = 0x41;
    common::drivers::INA236 ina2(i2c_master, i2c_slave_ina2);
    init_ina(ina2);

    auto i2c_slave_ina3 = 0x42;
    common::drivers::INA236 ina3(i2c_master, i2c_slave_ina3);
    init_ina(ina3);
    printf("INA236 initialized\n");

    auto i2c_slave_adc = 0x2D;  // 0x2D war davor jz 0x2E
    common::drivers::ADC128D818 adc(i2c_slave_adc, i2c_master);
    init_adc(adc);
    common::utility::buffer<2> buffer = {0};
    while(true)
    {
        //read adc values
        for (int current_channel = 0; current_channel < num_channels; current_channel++)
        {
            adc.read_channel(adc_channels[current_channel].channel, buffer);
            uint16_t adcValue = (buffer[0] << 8) | buffer[1];
            adcValue = adcValue >> 4; // Falls der ADC 12-Bit-Daten liefert

            float v_adc = (float(adcValue) / float(ADC_MAX)) * V_REF;
            float v_real = v_adc / adc_channels[current_channel].teiler_faktor;

            adc_raw_channel_voltage[current_channel] = v_real; // Speichern des Rohwerts in einem Array
            printf("Channel %d - VBAT: %f V\n", current_channel, v_real);
        }
        calculate_cell_voltage();
        // send_uart(adc_raw_channel_voltage[4]);
        common::task::delay(1_s);

        //read ina values
    }
}


auto read_temp_task(common::task::parameters parameters) -> void{
    using namespace common::units::literals;
    DS18B20 sensors(GPIO_NUM_18);

    if (sensors.init() == ESP_OK) {
        printf("Sensor gefunden! Anzahl: %d\n", sensors.get_device_count());
    } else {
        printf("Kein DS18B20 erkannt.\n");
    }
    while(true){
        for(int sensor_count = 0; sensor_count < sensors.get_device_count(); sensor_count++) {
            float temp = sensors.get_temperature(sensor_count);
            if (!isnan(temp)) {
                print_values(temp, sensor_count); // Hier kannst du die Callback-ID anpassen
            } else {
                printf("Sensor %d nicht verbunden.\n", sensor_count);
            }
        }
        common::task::delay(1_s);
    }

}


auto pwm_uart_task(common::task::parameters parameters) -> void{

    common::drivers::remote_controller::setup_interrupts_pwm();
    uint64_t last_pwm_value = esp_timer_get_time();
    while (true) {
        const uint64_t timeout = 1000000;
        PwmData pwm_data;
        if (xQueueReceive(pwm_queue, &(pwm_data), portMAX_DELAY) == pdPASS) {
            // UART senden (ohne Blockierung!)
            last_pwm_value = esp_timer_get_time(); // Aktueller Zeitstempel
            uint8_t mapped_value = mapTo8Bit(pwm_data.pulse_width, 1000, 2000); // Beispielwerte für min und max
            if(mapped_value < 10) mapped_value = 0; // Wenn der Wert kleiner als 100 ist, setze ihn auf 0
            // print_values(mapped_value, pwm_data.callback_id);
            // uart_write_bytes(UART_NUM_1, (const char*)&pwm_value, sizeof(pwm_value));
        }
        uint64_t current_time = esp_timer_get_time(); // Aktueller Zeitstempel
        uint64_t elapsed_time = current_time - last_pwm_value; // Zeitdifferenz seit der letzten PWM-Aktualisierung
        if (elapsed_time > timeout) { // Beispiel: 1 Sekunde
            uint8_t timeout_value_for_uart = 0; // Standardwert, wenn keine Aktualisierung erfolgt ist
            // print_values(timeout_value_for_uart, pwm_data.callback_id); // Hier kannst du die Callback-ID anpassen
            // uart_write_bytes(UART_NUM_1, (const char*)&default_value, sizeof(default_value));
        }

        if(pwm_data.callback_id == static_cast<uint8_t>(common::drivers::remote_controller::channel::SWITCH_D))
        {
            if(pwm_data.pulse_width > 1500) {
                ON_PIN.set_level(common::drivers::gpio::level::high);
            } else {
                ON_PIN.set_level(common::drivers::gpio::level::low);
            }
        }

        using namespace common::units::literals;
        common::task::delay(10_us);
    }
    

}

#ifndef TASKS_HPP_
#define TASKS_HPP_

#include <common/task.hpp>
#include <common/drivers/i2c.hpp>
#include <common/utility/buffer.hpp>
#include <ADC128D818.hpp>
#include <INA236.hpp>
#include <ds18b20.h>
#include <onewire.h>
#include <remote_controller.hpp>
#include <esp_timer.h>





// Struktur für die ADC-Kanäle
typedef struct {
    common::drivers::ADC128D818::Register channel;
    float teiler_faktor;
} ADC_Channel_Config;

extern common::drivers::gpio ON_PIN;

void init_adc(common::drivers::ADC128D818 &adc);
void init_ina(common::drivers::INA236 &ina);


const int num_channels = 5; // Anzahl der Kanäle


// auto read_ina_task(common::task::parameters parameters) -> void;

auto read_i2c_sensors_task(common::task::parameters parameters) -> void;

auto read_temp_task(common::task::parameters parameters) -> void;

// auto send_data_uart_task(common::task::parameters parameters) -> void;
auto pwm_uart_task(common::task::parameters parameters) -> void;

#endif // TASKS_HPP_

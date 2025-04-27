#include <common/drivers/i2c.hpp>
#include <common/utility/buffer.hpp>
#include <ADC128D818.hpp>
#include <INA236.hpp>
#include <common/units/time.hpp>
#include <common/task.hpp>
#include <tasks.hpp>
#include <common/drivers/gpio.hpp>
#include <gpio_interrupt.hpp>
#include <remote_controller.hpp>
#include <esp_timer.h>
#include <globals_freertos.hpp>
#include <ds18b20.h>
#include <stdio.h> 


QueueHandle_t pwm_queue = NULL; // Queue für PWM-Werte



extern "C" void app_main() {
    pwm_queue = xQueueCreate(10, sizeof(struct PwmData));
    if (pwm_queue == NULL) {
        printf("Fehler: Queue konnte nicht erstellt werden!\n");
        return;
    }
    
    ON_PIN.set_level(common::drivers::gpio::level::low);

    using namespace common::units::literals;
    common::task::create("pwm_uart_task", &pwm_uart_task, 16_KiByte, common::task::priority::high, nullptr, 1);
    // common::task::create("read_temp_task", &read_temp_task, 16_KiByte, common::task::priority::low, nullptr, 0);
    common::task::create("read_i2c_sensors_task", &read_i2c_sensors_task, 16_KiByte, common::task::priority::low, nullptr, 0);


    while(true){
        common::task::delay(1_s);
    }
}

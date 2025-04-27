#ifndef GLOBALS_FREERTOS_HPP_
#define GLOBALS_FREERTOS_HPP_

#include <freertos/FreeRTOS.h>
#include <common/drivers/gpio.hpp>

extern QueueHandle_t pwm_queue; // Queue für PWM-Werte

struct PwmData {
    uint8_t callback_id;   // 0 for callback1, 1 for callback2
    uint64_t pulse_width;  // The pulse width
};


#endif // GLOBALS_FREERTOS_HPP_
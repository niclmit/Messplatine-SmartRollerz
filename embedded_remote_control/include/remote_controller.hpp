#ifndef REMOTE_CONTROLLER_HPP
#define REMOTE_CONTROLLER_HPP

#include <cstdint>
#include <functional>
#include <common/utility/buffer.hpp>
#include <common/utility/utility.hpp>
#include <common/drivers/gpio.hpp>
#include <gpio_interrupt.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <globals_freertos.hpp>
#include <atomic>
#include <esp_timer.h>

namespace common::drivers::remote_controller
{
    // GPIO-Pins für den Empfänger
    extern gpio CH1_PIN_RIGHT_STICK_X;
    extern gpio CH2_PIN_RIGHT_STICK_Y;
    extern gpio CH3_PIN_LEFT_STICK_Y;
    extern gpio CH4_PIN_LEFT_STICK_X;
    extern gpio CH5_PIN_SWITCH_A;
    extern gpio CH6_PIN_SWITCH_B;
    extern gpio CH7_PIN_SWITCH_C;
    extern gpio CH8_PIN_SWITCH_D;
    extern gpio CH9_PIN_SWITCH_E;
    extern gpio CH10_PIN_SWITCH_F;
    extern gpio CH11_PIN_SWITCH_G;

    enum class channel : uint8_t
    {
        RIGHT_STICK_X = 0,
        RIGHT_STICK_Y = 1,
        LEFT_STICK_X = 2,
        LEFT_STICK_Y = 3,
        SWITCH_A = 4,
        SWITCH_B = 5,
        SWITCH_C = 6,
        SWITCH_D = 7,
        SWITCH_E = 8,
        SWITCH_F = 9,
        SWITCH_G = 10,
    };

    void interrupt_callback_right_stick_x();
    void interrupt_callback_right_stick_y();
    void interrupt_callback_left_stick_x();
    void interrupt_callback_left_stick_y();
    void interrupt_callback_switch_a();
    void interrupt_callback_switch_b();
    void interrupt_callback_switch_c();
    void interrupt_callback_switch_d();
    void interrupt_callback_switch_e();
    void interrupt_callback_switch_f();
    void interrupt_callback_switch_g();

    void setup_interrupts_pwm();
}

#endif // REMOTE_CONTROLLER_HPP

#include "remote_controller.hpp"

namespace common::drivers::remote_controller
{


    // // GPIO-Pins für den Empfänger
    gpio CH1_PIN_RIGHT_STICK_X = gpio{GPIO_NUM_17, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_ANYEDGE};
    gpio CH2_PIN_RIGHT_STICK_Y = gpio{GPIO_NUM_16, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_ANYEDGE};
    gpio CH3_PIN_LEFT_STICK_Y = gpio{GPIO_NUM_15, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_ANYEDGE};
    gpio CH4_PIN_LEFT_STICK_X = gpio{GPIO_NUM_7, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_ANYEDGE};
    gpio CH5_PIN_SWITCH_A = gpio{GPIO_NUM_38, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_ANYEDGE};
    gpio CH6_PIN_SWITCH_B = gpio{GPIO_NUM_39, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_ANYEDGE};
    gpio CH7_PIN_SWITCH_C = gpio{GPIO_NUM_40, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_ANYEDGE};
    gpio CH8_PIN_SWITCH_D = gpio{GPIO_NUM_41, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_ANYEDGE};
    gpio CH9_PIN_SWITCH_E = gpio{GPIO_NUM_42, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_ANYEDGE};
    gpio CH10_PIN_SWITCH_F = gpio{GPIO_NUM_2, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_ANYEDGE};
    gpio CH11_PIN_SWITCH_G = gpio{GPIO_NUM_1, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_ANYEDGE};


    void interrupt_callback_right_stick_x() {
        static uint64_t rising_edge_time = 0;
    
        if (common::drivers::remote_controller::CH1_PIN_RIGHT_STICK_X.get_level() == common::drivers::gpio::level::high) {
            rising_edge_time = esp_timer_get_time(); // High-Flanke
        } else {
            uint64_t pulse_width = esp_timer_get_time() - rising_edge_time;
            PwmData pwm_data = { static_cast<uint8_t>(common::drivers::remote_controller::channel::RIGHT_STICK_X), pulse_width };  // 0 for callback1
            xQueueSendFromISR(pwm_queue, &pwm_data, NULL); // Send the struct
        }
    
    
    }
    

    void interrupt_callback_right_stick_y() {
        static uint64_t rising_edge_time = 0;

        if (CH2_PIN_RIGHT_STICK_Y.get_level() == gpio::level::high) {
            rising_edge_time = esp_timer_get_time(); // High-Flanke
        } else {
            uint64_t pulse_width = esp_timer_get_time() - rising_edge_time;
            PwmData pwm_data = {static_cast<uint8_t>(channel::RIGHT_STICK_Y), pulse_width};
            xQueueSendFromISR(pwm_queue, &pwm_data, NULL); // Send the struct
        }
    }

    void interrupt_callback_left_stick_x() {
        static uint64_t rising_edge_time = 0;

        if (CH4_PIN_LEFT_STICK_X.get_level() == gpio::level::high) {
            rising_edge_time = esp_timer_get_time(); // High-Flanke
        } else {
            uint64_t pulse_width = esp_timer_get_time() - rising_edge_time;
            PwmData pwm_data = {static_cast<uint8_t>(channel::LEFT_STICK_X), pulse_width};
            xQueueSendFromISR(pwm_queue, &pwm_data, NULL); // Send the struct
        }
    }

    void interrupt_callback_left_stick_y() {
        static uint64_t rising_edge_time = 0;

        if (CH3_PIN_LEFT_STICK_Y.get_level() == gpio::level::high) {
            rising_edge_time = esp_timer_get_time(); // High-Flanke
        } else {
            uint64_t pulse_width = esp_timer_get_time() - rising_edge_time;
            PwmData pwm_data = {static_cast<uint8_t>(channel::LEFT_STICK_Y), pulse_width};
            xQueueSendFromISR(pwm_queue, &pwm_data, NULL); // Send the struct
        }
    }

    void interrupt_callback_switch_a() {
        static uint64_t rising_edge_time = 0;

        if (CH5_PIN_SWITCH_A.get_level() == gpio::level::high) {
            rising_edge_time = esp_timer_get_time(); // High-Flanke
        } else {
            uint64_t pulse_width = esp_timer_get_time() - rising_edge_time;
            PwmData pwm_data = {static_cast<uint8_t>(channel::SWITCH_A), pulse_width};
            xQueueSendFromISR(pwm_queue, &pwm_data, NULL); // Send the struct
        }
    }

    void interrupt_callback_switch_b() {
        static uint64_t rising_edge_time = 0;

        if (CH6_PIN_SWITCH_B.get_level() == gpio::level::high) {
            rising_edge_time = esp_timer_get_time(); // High-Flanke
        } else {
            uint64_t pulse_width = esp_timer_get_time() - rising_edge_time;
            PwmData pwm_data = {static_cast<uint8_t>(channel::SWITCH_B), pulse_width};
            xQueueSendFromISR(pwm_queue, &pwm_data, NULL); // Send the struct
        }
    }

    void interrupt_callback_switch_c() {
        static uint64_t rising_edge_time = 0;

        if (CH7_PIN_SWITCH_C.get_level() == gpio::level::high) {
            rising_edge_time = esp_timer_get_time(); // High-Flanke
        } else {
            uint64_t pulse_width = esp_timer_get_time() - rising_edge_time;
            PwmData pwm_data = {static_cast<uint8_t>(channel::SWITCH_C), pulse_width};
            xQueueSendFromISR(pwm_queue, &pwm_data, NULL); // Send the struct
        }
    }

    void interrupt_callback_switch_d() {
        static uint64_t rising_edge_time = 0;

        if (CH8_PIN_SWITCH_D.get_level() == gpio::level::high) {
            rising_edge_time = esp_timer_get_time(); // High-Flanke
        } else {
            uint64_t pulse_width = esp_timer_get_time() - rising_edge_time;
            PwmData pwm_data = {static_cast<uint8_t>(channel::SWITCH_D), pulse_width};
            xQueueSendFromISR(pwm_queue, &pwm_data, NULL); // Send the struct
        }
    }

    void interrupt_callback_switch_e() {
        static uint64_t rising_edge_time = 0;

        if (CH9_PIN_SWITCH_E.get_level() == gpio::level::high) {
            rising_edge_time = esp_timer_get_time(); // High-Flanke
        } else {
            uint64_t pulse_width = esp_timer_get_time() - rising_edge_time;
            PwmData pwm_data = {static_cast<uint8_t>(channel::SWITCH_E), pulse_width};
            xQueueSendFromISR(pwm_queue, &pwm_data, NULL); // Send the struct
        }
    }

    void interrupt_callback_switch_f() {
        static uint64_t rising_edge_time = 0;

        if (CH10_PIN_SWITCH_F.get_level() == gpio::level::high) {
            rising_edge_time = esp_timer_get_time(); // High-Flanke
        } else {
            uint64_t pulse_width = esp_timer_get_time() - rising_edge_time;
            PwmData pwm_data = {static_cast<uint8_t>(channel::SWITCH_F), pulse_width};
            xQueueSendFromISR(pwm_queue, &pwm_data, NULL); // Send the struct
        }
    }

    void interrupt_callback_switch_g() {
        static uint64_t rising_edge_time = 0;

        if (CH11_PIN_SWITCH_G.get_level() == gpio::level::high) {
            rising_edge_time = esp_timer_get_time(); // High-Flanke
        } else {
            uint64_t pulse_width = esp_timer_get_time() - rising_edge_time;
            PwmData pwm_data = {static_cast<uint8_t>(channel::SWITCH_G), pulse_width};
            xQueueSendFromISR(pwm_queue, &pwm_data, NULL); // Send the struct
        }
    }
    
    
    void setup_interrupts_pwm(){
        gpio_interrupt gpio_interrupt_right_stick_x(CH1_PIN_RIGHT_STICK_X);
        gpio_interrupt gpio_interrupt_right_stick_y(CH2_PIN_RIGHT_STICK_Y);
        gpio_interrupt gpio_interrupt_left_stick_x(CH4_PIN_LEFT_STICK_X);
        gpio_interrupt gpio_interrupt_left_stick_y(CH3_PIN_LEFT_STICK_Y);
        gpio_interrupt gpio_interrupt_switch_a(CH5_PIN_SWITCH_A);
        gpio_interrupt gpio_interrupt_switch_b(CH6_PIN_SWITCH_B);
        gpio_interrupt gpio_interrupt_switch_c(CH7_PIN_SWITCH_C);
        gpio_interrupt gpio_interrupt_switch_d(CH8_PIN_SWITCH_D);
        gpio_interrupt gpio_interrupt_switch_e(CH9_PIN_SWITCH_E);
        gpio_interrupt gpio_interrupt_switch_f(CH10_PIN_SWITCH_F);
        gpio_interrupt gpio_interrupt_switch_g(CH11_PIN_SWITCH_G);
    
        // 3. Callback-Funktion anhängen
        gpio_interrupt_right_stick_x.attach(interrupt_callback_right_stick_x);
        // gpio_interrupt_right_stick_y.attach(interrupt_callback_right_stick_y);
        // gpio_interrupt_left_stick_x.attach(interrupt_callback_left_stick_x);
        gpio_interrupt_left_stick_y.attach(interrupt_callback_left_stick_y);
        gpio_interrupt_switch_a.attach(interrupt_callback_switch_a);
        // gpio_interrupt_switch_b.attach(interrupt_callback_switch_b);
        gpio_interrupt_switch_c.attach(interrupt_callback_switch_c);
        // gpio_interrupt_switch_d.attach(interrupt_callback_switch_d);
        // gpio_interrupt_switch_e.attach(interrupt_callback_switch_e);
        // gpio_interrupt_switch_f.attach(interrupt_callback_switch_f);
        // gpio_interrupt_switch_g.attach(interrupt_callback_switch_g);
    }
        
}

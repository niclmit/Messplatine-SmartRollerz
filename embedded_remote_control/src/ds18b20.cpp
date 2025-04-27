#include "ds18b20.h"
// Define OWR_OK if not already defined


DS18B20::DS18B20(gpio_num_t pin) : onewire(static_cast<uint8_t>(pin)) {}

esp_err_t DS18B20::init() {
    device_count = onewire.search(addresses, 10);
    return device_count > 0 ? ESP_OK : ESP_FAIL;
}

bool DS18B20::is_connected() const {
    return device_count > 0;
}

float DS18B20::get_temperature(uint8_t index) {
    if (index >= device_count) {
        return NAN;
    }

    onewire.request(); // Request temperature conversion
    vTaskDelay(pdMS_TO_TICKS(750)); // Wait for conversion (750ms for DS18B20)

    float temp = NAN;
    uint8_t result = onewire.getTemp(addresses[index], temp);
    return (result == OWR_OK) ? temp : NAN;
}

uint64_t DS18B20::get_address(uint8_t index) const {
    if (index < device_count) {
        return addresses[index];
    }
    return 0;
}

uint8_t DS18B20::get_device_count() const {
    return device_count;
}

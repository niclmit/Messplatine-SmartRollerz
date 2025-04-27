#ifndef DS18B20_H
#define DS18B20_H


#include "onewire.h"
#include <math.h>

class DS18B20 {
public:
    explicit DS18B20(gpio_num_t pin);

    esp_err_t init();
    bool is_connected() const;
    float get_temperature(uint8_t index);
    uint64_t get_address(uint8_t index) const;
    uint8_t get_device_count() const;
    

private:
    OneWire32 onewire;
    uint64_t addresses[10];  // bis zu 4 Sensoren, erweiterbar
    uint8_t device_count = 0;
};

#endif // DS18B20_H
#include "TracePin.h"
#include <driver/gpio.h>

#define TRACE_PIN GPIO_NUM_18
#define TRACE_PIN_BIT (1ULL << TRACE_PIN)

void TracePin_Initialize(void) {
    // Zero-initialize the config structure
    gpio_config_t io_conf = {};
    
    // Set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    
    // Bit mask of the pin
    io_conf.pin_bit_mask = TRACE_PIN_BIT;
    
    // Disable pull-up and pull-down
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    
    // Disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    
    // Configure the GPIO
    gpio_config(&io_conf);
    
    // Start with pin OFF
    GPIO.out_w1tc = TRACE_PIN_BIT;  // Clear pin (OFF)
}

void TracePin_On(void) {
    GPIO.out_w1ts = TRACE_PIN_BIT;  // Set pin (ON)
}

void TracePin_Off(void) {
    GPIO.out_w1tc = TRACE_PIN_BIT;  // Clear pin (OFF)
}
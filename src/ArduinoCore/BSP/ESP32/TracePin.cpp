#include "TracePin.h"
#include "soc/gpio_struct.h"
#include "soc/io_mux_reg.h"

// Use the SOC-provided register structures [citation:9]
#define TRACE_GPIO_NUM  17
#define TRACE_GPIO_BIT  (1 << TRACE_GPIO_NUM)

void TracePin_Initialize(void)
{
    // Configure IO MUX for GPIO function
    REG_SET_FIELD(IO_MUX_GPIO17_REG, MCU_SEL, 1);  // Function 1 = GPIO
    
    // Enable output
    GPIO.enable_w1ts.val = TRACE_GPIO_BIT;
    
    // Start with output low
    GPIO.out_w1tc.val = TRACE_GPIO_BIT;
}

void TracePin_On(void)
{
    GPIO.out_w1ts.val = TRACE_GPIO_BIT;
}

void TracePin_Off(void)
{
    GPIO.out_w1tc.val = TRACE_GPIO_BIT;
}
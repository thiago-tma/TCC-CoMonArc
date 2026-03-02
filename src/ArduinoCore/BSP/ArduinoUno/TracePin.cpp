#include "TracePin.h"
#include <avr/io.h>

void TracePin_Initialize(void)
{
    // Set pin A2 (PC2) as output
    // A2 corresponds to Port C, bit 2 (PC2)
    DDRC |= (1 << DDC2);  // Set bit 2 of Data Direction Register for Port C to 1 (output)
    
    // Initialize pin to OFF state (low)
    PORTC &= ~(1 << PORTC2);  // Clear bit 2 of Port C output register
}

void TracePin_On(void)
{
    // Set pin A2 high
    PORTC |= (1 << PORTC2);   // Set bit 2 of Port C to 1 (high)
}

void TracePin_Off(void)
{
    // Set pin A2 low
    PORTC &= ~(1 << PORTC2);  // Clear bit 2 of Port C (low)
}
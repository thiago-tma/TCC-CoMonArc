#ifndef H_FAKECS_ADC_D
#define H_FAKECS_ADC_D

#include <stdbool.h>
#include <CS_ADC.h>

void FakeCS_ADC_Reset(void);

bool FakeCS_ADC_GetInitialized (void);
void FakeCS_ADC_SetParameters (CS_ADC_Parameters_t parameters);
void FakeCS_ADC_SetRead (CS_ADC_Value_t rawValue);

#endif /*H_FAKECS_ADC_D*/
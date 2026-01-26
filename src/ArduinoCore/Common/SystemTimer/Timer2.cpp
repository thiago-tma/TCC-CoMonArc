//#include <Arduino.h>
//#include <SystemTimer.h>
//
//// ===== System timer configuration =====
//#define SYSTEM_TIMER_PRESCALER       64
//#define SYSTEM_TIMER_PERIOD_US       1020  // interrupt every 1000 µs (1 ms)
//
//#define TIMER_TICK_US ((SYSTEM_TIMER_PRESCALER * 1000000UL) / F_CPU)
//#define OCR2A_VALUE   ((SYSTEM_TIMER_PERIOD_US / TIMER_TICK_US) - 1)
//
//#if OCR2A_VALUE > 255
//#error "Timer2 OCR2A overflow: period too long"
//#endif
//
///* System timer uses timer2 on the Arduino */
//
//static Callback storedTimerCallback = nullptr;
//static volatile bool timerRunning = false;
//
///* ====================================================== */
//
//ISR(TIMER2_COMPA_vect)
//{
//    if (storedTimerCallback && timerRunning)
//    {
//        storedTimerCallback(SYSTEM_TIMER_PERIOD_US);
//    }
//}
//
///* Receives callback, initializes and runs timer */
//void SystemTimer_Create(Callback timerCallback)
//{
//    TIMSK2 &= ~(1 << OCIE2A);  // disable interrupt
//
//    storedTimerCallback = timerCallback;
//
//    /* Reset timer configuration */
//    TCCR2A = 0;
//    TCCR2B = 0;
//    TCNT2  = 0;
//
//    /* CTC mode */
//    TCCR2A |= (1 << WGM21);
//
//    /* Compare value */
//    OCR2A = OCR2A_VALUE;
//
//    /* Prescaler */
//    #if SYSTEM_TIMER_PRESCALER == 1
//        TCCR2B |= (1 << CS20);
//    #elif SYSTEM_TIMER_PRESCALER == 8
//        TCCR2B |= (1 << CS21);
//    #elif SYSTEM_TIMER_PRESCALER == 32
//        TCCR2B |= (1 << CS21) | (1 << CS20);
//    #elif SYSTEM_TIMER_PRESCALER == 64
//        TCCR2B |= (1 << CS22);
//    #elif SYSTEM_TIMER_PRESCALER == 128
//        TCCR2B |= (1 << CS22) | (1 << CS20);
//    #elif SYSTEM_TIMER_PRESCALER == 256
//        TCCR2B |= (1 << CS22) | (1 << CS21);
//    #elif SYSTEM_TIMER_PRESCALER == 1024
//        TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
//    #else
//        #error "Invalid SYSTEM_TIMER_PRESCALER"
//    #endif
//
//    timerRunning = true;
//
//    /* Enable compare match interrupt */
//    TIMSK2 |= (1 << OCIE2A);
//}
//
///* Stops timer and erases the callback */
//void SystemTimer_Destroy(void)
//{
//    TIMSK2 &= ~(1 << OCIE2A);  // disable interrupt
//    TCCR2B = 0;               // stop timer
//    storedTimerCallback = nullptr;
//    timerRunning = false;
//}
//
///* Stops timer (keeps configuration) */
//void SystemTimer_Pause(void)
//{
//    TIMSK2 &= ~(1 << OCIE2A);  // disable interrupt
//}
//
///* Continues timer */
//void SystemTimer_Continue(void)
//{
//    /* Enable compare match interrupt */
//    TIMSK2 |= (1 << OCIE2A);
//}
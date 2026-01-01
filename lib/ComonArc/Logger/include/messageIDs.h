#ifndef H_MESSAGEIDS_D
#define H_MESSAGEIDS_D



typedef enum MessageID_t
{
    /* Logger */
    LOG_ID_LOGGER_TRACE_INITIALIZED,          /* Initialized */

    /* Command Handler*/
    LOG_ID_COMMAND_TRACE_INITIALIZED,         /* Initialized */

    /* Servo Controller */
    LOG_ID_SERVO_TRACE_INITIALIZED,           /* Initialized */

    /* Magnetometer */
    LOG_ID_SERVO_TRACE_INITIALIZED,           /* Initialized */

    /* Current Sensor */
    LOG_ID_CURRENTSENSOR_TRACE_INITIALIZED,   /* Initialized */

    /* User Interface */
    LOG_ID_USERINTERFACE_TRACE_INITIALIZED    /* Initialized */
    
} MessageID_t;

#endif /*H_MESSAGEIDS_D*/
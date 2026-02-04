// AUTO-GENERATED FILE — DO NOT EDIT

#ifndef D_LOG_API_H
#define D_LOG_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "log_ids.h"
#include "Logger.h"

static inline void log_command_error_false_alarm()
{
    Logger_Log(
        LOG_SUBSYS_COMMAND,
        LOG_LEVEL_ERROR,
        LOG_COMMAND_ERROR_FALSE_ALARM, NULL, 0);
}

static inline void log_command_error_receiver_buffer_overflow()
{
    Logger_Log(
        LOG_SUBSYS_COMMAND,
        LOG_LEVEL_ERROR,
        LOG_COMMAND_ERROR_RECEIVER_BUFFER_OVERFLOW, NULL, 0);
}

static inline void log_command_error_receiver_error_code(uint8_t arg0)
{
    uint8_t payload[1];
    payload[0] = (uint8_t)arg0;

    Logger_Log(
        LOG_SUBSYS_COMMAND,
        LOG_LEVEL_ERROR,
        LOG_COMMAND_ERROR_RECEIVER_ERROR_CODE, payload, sizeof(payload));
}

static inline void log_command_error_receiver_initialization_failed()
{
    Logger_Log(
        LOG_SUBSYS_COMMAND,
        LOG_LEVEL_ERROR,
        LOG_COMMAND_ERROR_RECEIVER_INITIALIZATION_FAILED, NULL, 0);
}

static inline void log_command_event_command_executed()
{
    Logger_Log(
        LOG_SUBSYS_COMMAND,
        LOG_LEVEL_EVENT,
        LOG_COMMAND_EVENT_COMMAND_EXECUTED, NULL, 0);
}

static inline void log_command_event_ping()
{
    Logger_Log(
        LOG_SUBSYS_COMMAND,
        LOG_LEVEL_EVENT,
        LOG_COMMAND_EVENT_PING, NULL, 0);
}

static inline void log_command_event_receiver_timeout()
{
    Logger_Log(
        LOG_SUBSYS_COMMAND,
        LOG_LEVEL_EVENT,
        LOG_COMMAND_EVENT_RECEIVER_TIMEOUT, NULL, 0);
}

static inline void log_command_trace_command_handler_initialized()
{
    Logger_Log(
        LOG_SUBSYS_COMMAND,
        LOG_LEVEL_TRACE,
        LOG_COMMAND_TRACE_COMMAND_HANDLER_INITIALIZED, NULL, 0);
}

static inline void log_command_trace_receiver_initialized()
{
    Logger_Log(
        LOG_SUBSYS_COMMAND,
        LOG_LEVEL_TRACE,
        LOG_COMMAND_TRACE_RECEIVER_INITIALIZED, NULL, 0);
}

static inline void log_current_data_reading(int16_t arg0)
{
    uint8_t payload[2];
    payload[0] = (uint8_t)(arg0 >> 0);
    payload[1] = (uint8_t)(arg0 >> 8);

    Logger_Log(
        LOG_SUBSYS_CURRENT,
        LOG_LEVEL_DATA,
        LOG_CURRENT_DATA_READING, payload, sizeof(payload));
}

static inline void log_current_error_initialization_failed()
{
    Logger_Log(
        LOG_SUBSYS_CURRENT,
        LOG_LEVEL_ERROR,
        LOG_CURRENT_ERROR_INITIALIZATION_FAILED, NULL, 0);
}

static inline void log_current_error_initialization_failed_adc()
{
    Logger_Log(
        LOG_SUBSYS_CURRENT,
        LOG_LEVEL_ERROR,
        LOG_CURRENT_ERROR_INITIALIZATION_FAILED_ADC, NULL, 0);
}

static inline void log_current_trace_initialization()
{
    Logger_Log(
        LOG_SUBSYS_CURRENT,
        LOG_LEVEL_TRACE,
        LOG_CURRENT_TRACE_INITIALIZATION, NULL, 0);
}

static inline void log_logger_error_buffer_overflow()
{
    Logger_Log(
        LOG_SUBSYS_LOGGER,
        LOG_LEVEL_ERROR,
        LOG_LOGGER_ERROR_BUFFER_OVERFLOW, NULL, 0);
}

static inline void log_logger_trace_logger_initialized()
{
    Logger_Log(
        LOG_SUBSYS_LOGGER,
        LOG_LEVEL_TRACE,
        LOG_LOGGER_TRACE_LOGGER_INITIALIZED, NULL, 0);
}

static inline void log_logger_trace_transmitter_initialized()
{
    Logger_Log(
        LOG_SUBSYS_LOGGER,
        LOG_LEVEL_TRACE,
        LOG_LOGGER_TRACE_TRANSMITTER_INITIALIZED, NULL, 0);
}

static inline void log_magnetometer_data_reading(int16_t arg0)
{
    uint8_t payload[2];
    payload[0] = (uint8_t)(arg0 >> 0);
    payload[1] = (uint8_t)(arg0 >> 8);

    Logger_Log(
        LOG_SUBSYS_MAGNETOMETER,
        LOG_LEVEL_DATA,
        LOG_MAGNETOMETER_DATA_READING, payload, sizeof(payload));
}

static inline void log_magnetometer_error_initialization_failed()
{
    Logger_Log(
        LOG_SUBSYS_MAGNETOMETER,
        LOG_LEVEL_ERROR,
        LOG_MAGNETOMETER_ERROR_INITIALIZATION_FAILED, NULL, 0);
}

static inline void log_magnetometer_event_new_read_failed()
{
    Logger_Log(
        LOG_SUBSYS_MAGNETOMETER,
        LOG_LEVEL_EVENT,
        LOG_MAGNETOMETER_EVENT_NEW_READ_FAILED, NULL, 0);
}

static inline void log_magnetometer_trace_initialization()
{
    Logger_Log(
        LOG_SUBSYS_MAGNETOMETER,
        LOG_LEVEL_TRACE,
        LOG_MAGNETOMETER_TRACE_INITIALIZATION, NULL, 0);
}

static inline void log_servo_data_direction(int16_t arg0)
{
    uint8_t payload[2];
    payload[0] = (uint8_t)(arg0 >> 0);
    payload[1] = (uint8_t)(arg0 >> 8);

    Logger_Log(
        LOG_SUBSYS_SERVO,
        LOG_LEVEL_DATA,
        LOG_SERVO_DATA_DIRECTION, payload, sizeof(payload));
}

static inline void log_servo_data_reference(int16_t arg0)
{
    uint8_t payload[2];
    payload[0] = (uint8_t)(arg0 >> 0);
    payload[1] = (uint8_t)(arg0 >> 8);

    Logger_Log(
        LOG_SUBSYS_SERVO,
        LOG_LEVEL_DATA,
        LOG_SERVO_DATA_REFERENCE, payload, sizeof(payload));
}

static inline void log_servo_trace_initialized()
{
    Logger_Log(
        LOG_SUBSYS_SERVO,
        LOG_LEVEL_TRACE,
        LOG_SERVO_TRACE_INITIALIZED, NULL, 0);
}

static inline void log_system_trace_initialized()
{
    Logger_Log(
        LOG_SUBSYS_SYSTEM,
        LOG_LEVEL_TRACE,
        LOG_SYSTEM_TRACE_INITIALIZED, NULL, 0);
}

static inline void log_system_trace_loop_time(int32_t arg0)
{
    uint8_t payload[4];
    payload[0] = (uint8_t)(arg0 >> 0);
    payload[1] = (uint8_t)(arg0 >> 8);
    payload[2] = (uint8_t)(arg0 >> 16);
    payload[3] = (uint8_t)(arg0 >> 24);

    Logger_Log(
        LOG_SUBSYS_SYSTEM,
        LOG_LEVEL_TRACE,
        LOG_SYSTEM_TRACE_LOOP_TIME, payload, sizeof(payload));
}

static inline void log_user_interface_data_button_state(uint8_t arg0)
{
    uint8_t payload[1];
    payload[0] = (uint8_t)arg0;

    Logger_Log(
        LOG_SUBSYS_USER_INTERFACE,
        LOG_LEVEL_DATA,
        LOG_USER_INTERFACE_DATA_BUTTON_STATE, payload, sizeof(payload));
}

static inline void log_user_interface_error_initialization_failed()
{
    Logger_Log(
        LOG_SUBSYS_USER_INTERFACE,
        LOG_LEVEL_ERROR,
        LOG_USER_INTERFACE_ERROR_INITIALIZATION_FAILED, NULL, 0);
}

static inline void log_user_interface_event_button()
{
    Logger_Log(
        LOG_SUBSYS_USER_INTERFACE,
        LOG_LEVEL_EVENT,
        LOG_USER_INTERFACE_EVENT_BUTTON, NULL, 0);
}

static inline void log_user_interface_trace_initialized()
{
    Logger_Log(
        LOG_SUBSYS_USER_INTERFACE,
        LOG_LEVEL_TRACE,
        LOG_USER_INTERFACE_TRACE_INITIALIZED, NULL, 0);
}

#ifdef __cplusplus
}
#endif

#endif /* D_LOG_API_H */
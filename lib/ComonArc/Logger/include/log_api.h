// AUTO-GENERATED FILE — DO NOT EDIT

#ifndef D_LOG_API_H
#define D_LOG_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "log_ids.h"
#include "Logger.h"

static inline void log_command_event_ping()
{
    Logger_Log(
        LOG_SUBSYS_COMMAND,
        LOG_LEVEL_EVENT,
        LOG_COMMAND_EVENT_PING, NULL, 0);
}

static inline void log_command_trace_initialized()
{
    Logger_Log(
        LOG_SUBSYS_COMMAND,
        LOG_LEVEL_TRACE,
        LOG_COMMAND_TRACE_INITIALIZED, NULL, 0);
}

static inline void log_logger_error_buffer_overflow()
{
    Logger_Log(
        LOG_SUBSYS_LOGGER,
        LOG_LEVEL_ERROR,
        LOG_LOGGER_ERROR_BUFFER_OVERFLOW, NULL, 0);
}

static inline void log_logger_trace_initialized()
{
    Logger_Log(
        LOG_SUBSYS_LOGGER,
        LOG_LEVEL_TRACE,
        LOG_LOGGER_TRACE_INITIALIZED, NULL, 0);
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

static inline void log_system_trace_initialized()
{
    Logger_Log(
        LOG_SUBSYS_SYSTEM,
        LOG_LEVEL_TRACE,
        LOG_SYSTEM_TRACE_INITIALIZED, NULL, 0);
}

#ifdef __cplusplus
}
#endif

#endif /* D_LOG_API_H */
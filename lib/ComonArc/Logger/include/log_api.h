// AUTO-GENERATED FILE — DO NOT EDIT
// MODE: EMPTY

#ifndef D_LOG_API_H
#define D_LOG_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "log_ids.h"

static inline void log_command_error_false_alarm()
{
}

static inline void log_command_error_receiver_buffer_overflow()
{
}

static inline void log_command_error_receiver_error_code(uint8_t arg0)
{
}

static inline void log_command_error_receiver_initialization_failed()
{
}

static inline void log_command_event_command_executed()
{
}

static inline void log_command_event_ping()
{
}

static inline void log_command_event_receiver_timeout()
{
}

static inline void log_command_trace_command_handler_initialized()
{
}

static inline void log_command_trace_receiver_initialized()
{
}

static inline void log_current_data_reading(int32_t arg0)
{
}

static inline void log_current_error_initialization_failed()
{
}

static inline void log_current_error_initialization_failed_adc()
{
}

static inline void log_current_event_no_current()
{
}

static inline void log_current_trace_initialization()
{
}

static inline void log_current_trace_raw_reading(int32_t arg0)
{
}

static inline void log_logger_error_buffer_overflow()
{
}

static inline void log_logger_trace_buffer_usage(uint16_t arg0)
{
}

static inline void log_logger_trace_logger_initialized()
{
}

static inline void log_logger_trace_transmitter_initialized()
{
}

static inline void log_magnetometer_data_reading(int16_t arg0)
{
}

static inline void log_magnetometer_error_initialization_failed()
{
}

static inline void log_magnetometer_event_new_read_failed()
{
}

static inline void log_magnetometer_trace_initialization()
{
}

static inline void log_servo_data_direction(int16_t arg0)
{
}

static inline void log_servo_data_reference(int16_t arg0)
{
}

static inline void log_servo_trace_initialized()
{
}

static inline void log_system_error_loop_time_exceeded()
{
}

static inline void log_system_trace_command_handler_running()
{
}

static inline void log_system_trace_command_handler_time(uint32_t arg0)
{
}

static inline void log_system_trace_current_sensor_running()
{
}

static inline void log_system_trace_current_sensor_time(uint32_t arg0)
{
}

static inline void log_system_trace_initialized()
{
}

static inline void log_system_trace_logger_running()
{
}

static inline void log_system_trace_logger_time(uint32_t arg0)
{
}

static inline void log_system_trace_loop_time(uint32_t arg0)
{
}

static inline void log_system_trace_magnetometer_running()
{
}

static inline void log_system_trace_magnetometer_time(uint32_t arg0)
{
}

static inline void log_system_trace_user_interface_running()
{
}

static inline void log_system_trace_user_interface_time(uint32_t arg0)
{
}

static inline void log_system_trace_yaw_controller_running()
{
}

static inline void log_system_trace_yaw_controller_time(uint32_t arg0)
{
}

static inline void log_user_interface_data_button_state(uint8_t arg0)
{
}

static inline void log_user_interface_data_buzzer_state(uint8_t arg0)
{
}

static inline void log_user_interface_data_led_state(uint8_t arg0)
{
}

static inline void log_user_interface_error_initialization_failed()
{
}

static inline void log_user_interface_event_button()
{
}

static inline void log_user_interface_trace_initialized()
{
}

#ifdef __cplusplus
}
#endif

#endif /* D_LOG_API_H */
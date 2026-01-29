# AUTO-GENERATED FILE — DO NOT EDIT

LOG_SUBSYSTEMS ={
        0: 'LOGGER',
        1: 'COMMAND',
        2: 'SERVO',
        3: 'MAGNETOMETER',
        4: 'CURRENT',
        5: 'USER_INTERFACE',
        6: 'SYSTEM',
}

LOG_LEVELS ={
        0: 'ERROR',
        1: 'EVENT',
        2: 'DATA',
        3: 'TRACE',
}

LOG_MESSAGES = {
    1: {
        'name': 'LOG_COMMAND_ERROR_FALSE_ALARM',
        'subsystem': 'COMMAND',
        'level': 'ERROR',
        'args': [],
        'format': "Error command triggered!",
    },
    2: {
        'name': 'LOG_COMMAND_EVENT_PING',
        'subsystem': 'COMMAND',
        'level': 'EVENT',
        'args': [],
        'format': "Pingback",
    },
    3: {
        'name': 'LOG_COMMAND_TRACE_INITIALIZED',
        'subsystem': 'COMMAND',
        'level': 'TRACE',
        'args': [],
        'format': "Initialized",
    },
    4: {
        'name': 'LOG_LOGGER_ERROR_BUFFER_OVERFLOW',
        'subsystem': 'LOGGER',
        'level': 'ERROR',
        'args': [],
        'format': "Message buffer overflow, too many messages were accumulated",
    },
    5: {
        'name': 'LOG_LOGGER_TRACE_INITIALIZED',
        'subsystem': 'LOGGER',
        'level': 'TRACE',
        'args': [],
        'format': "Initialized",
    },
    6: {
        'name': 'LOG_MAGNETOMETER_DATA_READING',
        'subsystem': 'MAGNETOMETER',
        'level': 'DATA',
        'args': ['i16'],
        'format': "Magnetometer direction: {0} deg",
    },
    7: {
        'name': 'LOG_SERVO_DATA_DIRECTION',
        'subsystem': 'SERVO',
        'level': 'DATA',
        'args': ['i16'],
        'format': "Servo arm: {0} deg",
    },
    8: {
        'name': 'LOG_SERVO_DATA_REFERENCE',
        'subsystem': 'SERVO',
        'level': 'DATA',
        'args': ['i16'],
        'format': "Yaw Controller Reference: {0} deg",
    },
    9: {
        'name': 'LOG_SYSTEM_TRACE_INITIALIZED',
        'subsystem': 'SYSTEM',
        'level': 'TRACE',
        'args': [],
        'format': "Initialized",
    },
}

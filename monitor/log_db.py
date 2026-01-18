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
        2: 'TRACE',
        3: 'DATA',
}

LOG_MESSAGES = {
    1: {
        'name': 'LOG_COMMAND_TRACE_INITIALIZED',
        'subsystem': 'COMMAND',
        'level': 'TRACE',
        'args': [],
        'format': "Initialized",
    },
    2: {
        'name': 'LOG_LOGGER_ERROR_BUFFER_OVERFLOW',
        'subsystem': 'LOGGER',
        'level': 'ERROR',
        'args': [],
        'format': "Message buffer overflow, too many messages were accumulated",
    },
    3: {
        'name': 'LOG_LOGGER_TRACE_INITIALIZED',
        'subsystem': 'LOGGER',
        'level': 'TRACE',
        'args': [],
        'format': "Initialized",
    },
    4: {
        'name': 'LOG_SERVO_DATA_DIRECTION',
        'subsystem': 'SERVO',
        'level': 'DATA',
        'args': ['i16'],
        'format': "Servo arm: {0} deg",
    },
    5: {
        'name': 'LOG_SYSTEM_TRACE_INITIALIZED',
        'subsystem': 'SYSTEM',
        'level': 'TRACE',
        'args': [],
        'format': "Initialized",
    },
}

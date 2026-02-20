# Simplified CMakeLists.txt template

# === CONFIGURATION ===

# Main file (required)
set(MAIN_FILE "ArduinoCore/Programs/ESP32/app_config_check.cpp")

# Additional source files (list them individually or use directories)
set(ADDITIONAL_SOURCES
    "ArduinoCore/Common"
    "ArduinoCore/BSP/ESP32"
    "ArduinoCore/Arduino_UART"
)

# === BUILD SETUP ===

# Convert relative paths to absolute
set(ALL_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/${MAIN_FILE}
)

foreach(SOURCE ${ADDITIONAL_SOURCES})
    list(APPEND ALL_SOURCES ${CMAKE_SOURCE_DIR}/src/${SOURCE})
endforeach()

# Register component
idf_component_register(SRCS ${ALL_SOURCES})
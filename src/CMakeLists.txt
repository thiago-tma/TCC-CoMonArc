# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources 
    ${CMAKE_SOURCE_DIR}/src/ArduinoCore/app.cpp     # Main file
    ${CMAKE_SOURCE_DIR}/src/ArduinoCore/Common/*.*
    ${CMAKE_SOURCE_DIR}/src/ArduinoCore/BSP/ESP32/*.*
#   ${CMAKE_SOURCE_DIR}/src/ArduinoCore/Arduino_UART/*.*
    ${CMAKE_SOURCE_DIR}/src/ArduinoCore/BitBang_UART/BitBang_ESP32/*.*
    )

idf_component_register(SRCS ${app_sources})

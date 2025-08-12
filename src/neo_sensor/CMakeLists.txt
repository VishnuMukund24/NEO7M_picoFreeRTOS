# GPS driver library
add_library(neo_sensor_lib STATIC
    neo7m.cpp
)

# Public include dirs for other modules using this driver
target_include_directories(neo_sensor_lib PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}          # src/neo_sensor
)

# Link with necessary Pico SDK and FreeRTOS
target_link_libraries(neo_sensor_lib PUBLIC
    pico_stdlib
    hardware_uart
    hardware_gpio
    FreeRTOS-Kernel-Heap4
)

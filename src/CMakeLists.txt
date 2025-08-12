# Add the neo_sensor module
add_subdirectory(neo_sensor)

# Main application
add_executable(${ProjectName}
    main.cpp
)

# Include dirs for main.cpp (config is already global)
target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}          # src/
    ${CMAKE_CURRENT_SOURCE_DIR}/neo_sensor
)

# Link order: FreeRTOS before hardware libs, then modules
target_link_libraries(${ProjectName} 
    pico_stdlib
    hardware_gpio
    hardware_uart
    hardware_irq
    FreeRTOS-Kernel-Heap4              # FreeRTOS kernel
    neo_sensor_lib                     # Our GPS driver
)

pico_add_extra_outputs(${ProjectName})

#pragma once

#include <string>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "FreeRTOS.h"
#include "stream_buffer.h"

class NEO7M {
public:
    NEO7M(uart_inst_t* uart_port, uint tx_pin, uint rx_pin, uint32_t baudrate);

    void init();                             // Initialize UART and IRQ
    static void on_uart_rx();                // UART RX ISR handler
    bool readLine(std::string& line);        // Read a complete line

private:
    uart_inst_t* uart;                       // UART instance
    uint tx;                                 // TX pin
    uint rx;                                 // RX pin
    uint32_t baud;                           // Baudrate

    static StreamBufferHandle_t rxStream;    // Stream buffer for RX data
    static NEO7M* instance;                  // Singleton instance for ISR access
};

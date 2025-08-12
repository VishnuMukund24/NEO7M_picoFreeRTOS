#include "neo7m.h"
#include <cstdio>
#include "hardware/irq.h"

StreamBufferHandle_t NEO7M::rxStream = nullptr;
NEO7M* NEO7M::instance = nullptr;

NEO7M::NEO7M(uart_inst_t* uart_port, uint tx_pin, uint rx_pin, uint32_t baudrate)
    : uart(uart_port), tx(tx_pin), rx(rx_pin), baud(baudrate) {
    instance = this; // Store instance for ISR
}

void NEO7M::init() {
    // Create stream buffer (size: 512, trigger: 1 byte)
    rxStream = xStreamBufferCreate(512, 1);

    uart_init(uart, baud);
    gpio_set_function(tx, GPIO_FUNC_UART);
    gpio_set_function(rx, GPIO_FUNC_UART);
    uart_set_format(uart, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(uart, false);

    int irq_num = uart_get_index(uart) ? UART1_IRQ : UART0_IRQ;
    irq_set_exclusive_handler(irq_num, NEO7M::on_uart_rx);
    irq_set_enabled(irq_num, true);
    uart_set_irq_enables(uart, true, false);

    printf("NEO7M: UART interrupt initialized\n");
}

void NEO7M::on_uart_rx() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    while (uart_is_readable(instance->uart)) {
        char ch = uart_getc(instance->uart);
        xStreamBufferSendFromISR(rxStream, &ch, 1, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

bool NEO7M::readLine(std::string& line) {
    static std::string local_buf;
    char ch;
    while (xStreamBufferReceive(rxStream, &ch, 1, 0) == 1) {
        if (ch == '\n') {
            line = local_buf;
            local_buf.clear();
            return true;
        } else if (ch != '\r') {
            local_buf += ch;
        }
    }
    return false;
}

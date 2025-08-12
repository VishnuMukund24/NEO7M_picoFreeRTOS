#include "FreeRTOS.h"
#include "task.h"
#include <cstdio>
#include "pico/stdlib.h"
#include "neo_sensor/neo7m.h"
#include <cmath>

extern "C" void led_task(void* pvParameters)
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    while (true) {
        gpio_put(LED_PIN, 1);
        vTaskDelay(200);
        // printf("LED ON\n");
        gpio_put(LED_PIN, 0);
        vTaskDelay(1000);
        // printf("LED OFF\n");
    }
}

extern "C" void gps_task(void* pvParameters)
{
    // Retrieve the pointer to your gps instance:
    NEO7M* gps      = static_cast<NEO7M*>(pvParameters);
    int loop_count = 0;
    std::string line;

    for (;;) {
        loop_count++;
        if (gps->readLine(line)) {
            printf("Loop #%d\n", loop_count);
            printf("NMEA: %s\n", line.c_str());
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }   
}


int main()
{
    stdio_init_all();
    static NEO7M gps(uart1, 8, 9, 9600);
    gps.init();
    sleep_ms(3000); // vTaskDelay(pdMS_TO_TICKS(3000));  // SHOULD NOT CALL FREERTOS ELEMENTS BEFORE INITIALIZING SCHEDULER

    if (xTaskCreate(
            gps_task,
            "GPSTask",
            1024,
            &gps,
            2,
            nullptr
        ) != pdPASS)
        {
            printf("ERROR: could not create gps task\n");
        }
    
    xTaskCreate(led_task, "LED_Task", 512, NULL, 1, NULL);
    vTaskStartScheduler();
    while(1){};
    return 0;
}
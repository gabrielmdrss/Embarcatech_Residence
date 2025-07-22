#include "defines_functions.h"

int main()
{
    stdio_init_all();
    adc_init();
    ssd1306_Init();

    esquematico_Display();

    // Inicializa os pinos dos LEDs
    gpio_init(LED_PIN_12);
    gpio_set_dir(LED_PIN_12, GPIO_OUT);

    // Cria as duas tarefas, mas suspende Task2 imediatamente
    xTaskCreate(vLedTask1, "LED Task 1", 256, NULL, 1, &xTask1Handle);
    xTaskCreate(vTempTask2, "LED Task 2", 256, NULL, 1, &xTask2Handle);
    xTaskCreate(vJoystickTask3, "Joystick Task 3", 256, NULL, 1, &xTask3Handle);
    xTaskCreate(vDisplayTask, "Display Task", 256, NULL, 1, NULL);

    // Inicia o escalonador do FreeRTOS
    vTaskStartScheduler();

    while (1)
        ;
    return 0;
}
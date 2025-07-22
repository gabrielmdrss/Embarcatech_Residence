#ifndef DEFINES_FUNCTIONS_H
#define DEFINES_FUNCTIONS_H

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ssd1306/ssd1306.h"
#include "ssd1306/ssd1306_fonts.h"

// --- Pinos do LED ---
const uint LED_PIN_12 = 12;

// Alocar handles para controlar suspend/resume
typedef TaskHandle_t xTaskHandle;
static xTaskHandle xTask1Handle = NULL;
static xTaskHandle xTask2Handle = NULL;
static xTaskHandle xTask3Handle = NULL;

// Função para exibir o esqueleto de separação no display
void esquematico_Display(void)
{      
    ssd1306_DrawRectangle(0, 0, 127, 63, 1);
    ssd1306_DrawRectangle(64, 0, 63, 128, 1);
    ssd1306_SetCursor(15, 10);
    ssd1306_WriteString("Sensor", Font_6x8, 1);
    ssd1306_DrawRectangle(9, 27, 55, 27, 1);
    ssd1306_SetCursor(72, 10);
    ssd1306_WriteString("Joystick", Font_6x8, 1);
    ssd1306_DrawRectangle(73, 27, 119, 27, 1);
    ssd1306_UpdateScreen();
}

// Função para ler a temperatura do sensor onboard
// Retorna a temperatura em Celsius
float read_onboard_temperature() {
    
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4); // Seleciona o sensor de temperatura interno

    /* Conversão de 12 bits, assume valor máximo == ADC_VREF == 3.3 V */
    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float)adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    return tempC;
}

// Tarefa para controlar o primeiro LED
void vLedTask1(void *pvParameters)
{
    for (;;)
    {
        gpio_put(LED_PIN_12, 1);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        gpio_put(LED_PIN_12, 0);
        vTaskDelay(950 / portTICK_PERIOD_MS);
    }
}

// Tarefa para ler a temperatura no sensor onboard a cada 1 segundo
void vTempTask2(void *pvParameters)
{
    char buffer[20];
    for (;;)
    {
        float temperature = read_onboard_temperature(); // Lê a temperatura em Celsius
        sprintf(buffer, "%.2f C\n", temperature);

        // Printar a temperatura no display OLED
        ssd1306_SetCursor(8, 40);
        ssd1306_WriteString(buffer, Font_7x10, 1);
        ssd1306_DrawCircle(48, 40, 1, White);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void vJoystickTask3(void *pvParameters)
{
    const int ADC_MID    = 1900;    // ponto médio de 0–4095
    const int DEADZONE   = 300;     // ±200 counts de zona morta
    char *direction;                // Variável que armazenará direção do joystick

    for (;;)
    {
        // lê Y
        adc_select_input(0);
        uint adc_y_raw = adc_read();

        // lê X
        adc_select_input(1);
        uint adc_x_raw = adc_read();

        if(adc_y_raw > ADC_MID + DEADZONE && adc_y_raw > ADC_MID - DEADZONE)
        {
            if(adc_y_raw > adc_x_raw) direction = "Cima";
            else direction = "Direita";
        }
        else if(adc_y_raw < ADC_MID - DEADZONE && adc_y_raw < ADC_MID + DEADZONE)
        {
            if(adc_y_raw < adc_x_raw) direction = "Baixo";
            else direction = "Esquerda";
        }
        else if(adc_y_raw > ADC_MID + DEADZONE) direction = "Cima";
        else if(adc_y_raw < ADC_MID - DEADZONE) direction = "Baixo";     
        else if(adc_x_raw > ADC_MID + DEADZONE) direction = "Direita";
        else if(adc_x_raw < ADC_MID - DEADZONE) direction = "Esquerda";
    
        if(direction == "Cima")
        {
            ssd1306_FillRectangle(71, 39, 123, 48, Black);
            ssd1306_SetCursor(84, 40);
            ssd1306_WriteString("Cima", Font_6x8, 1);
        }
        else if(direction == "Baixo")
        {
            ssd1306_FillRectangle(71, 39, 123, 48, Black);
            ssd1306_SetCursor(82, 40);
            ssd1306_WriteString("Baixo", Font_6x8, 1);
        }
        else if(direction == "Esquerda")
        {
            ssd1306_FillRectangle(71, 39, 123, 48, Black);
            ssd1306_SetCursor(73, 40);
            ssd1306_WriteString("Esquerda", Font_6x8, 1);
        }
        else if(direction == "Direita")
        {
            ssd1306_FillRectangle(71, 39, 123, 48, Black);
            ssd1306_SetCursor(77, 40);
            ssd1306_WriteString("Direita", Font_6x8, 1);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/* Tarefa para atualizar o display OLED a cada segundo separadamente, para não 
gerar concorrência entre as duas outras tarefas que atualizam o display */
void vDisplayTask(void *pvParameters)
{
    for (;;)
    {
        ssd1306_UpdateScreen(); // Atualiza a tela OLED
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

#endif /* DEFINES_FUNCTIONS_H */
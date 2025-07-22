#include <stdio.h>
#include "pico/stdlib.h"
#include "menu/menu.h"


int main()
{
    stdio_init_all();

    init_pins();         // Inicializa os pinos usados no desenvolvimento     

    ssd1306_Init();      // Inicializa o display OLED
    adc_init();          // Inicializa o ADC
    ssd1306_Fill(Black); // Limpa o display OLED

    while (true) {
        
        menu();          // Função que armazena toda a lógica do menu interativo
    }
}

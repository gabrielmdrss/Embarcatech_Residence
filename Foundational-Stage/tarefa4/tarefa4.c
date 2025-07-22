#include <stdio.h>
#include "pico/stdlib.h"
#include "defines_functions.h"

int main()
{
    stdio_init_all();
    ssd1306_Init();
    adc_init();
    init_pins();
    LedMatrix_Init(&ws2812_config);

    while (true) {
        ct = !pt || (gr && ho && di);
        ssd1306_Fill(Black);

        if(gr == 0) LedMatrix_SetPixel(0, 0, LED_WEAK, LED_OFF, LED_OFF);
        else LedMatrix_SetPixel(0, 0, LED_OFF, LED_WEAK, LED_OFF);
        if(ho == 0) LedMatrix_SetPixel(1, 0, LED_WEAK, LED_OFF, LED_OFF);
        else LedMatrix_SetPixel(1, 0, LED_OFF, LED_WEAK, LED_OFF);
        if(di == 0) LedMatrix_SetPixel(2, 0, LED_WEAK, LED_OFF, LED_OFF);
        else LedMatrix_SetPixel(2, 0, LED_OFF, LED_WEAK, LED_OFF);
        if(pt == 0) LedMatrix_SetPixel(3, 0, LED_WEAK, LED_OFF, LED_OFF);
        else LedMatrix_SetPixel(3, 0, LED_OFF, LED_WEAK, LED_OFF);

        if (ct == 0) {
            pwm_set_chan_level(slice_red, chan_red, LED_PWM_INTENSITY);
            pwm_set_chan_level(slice_green, chan_green, 0);
        }
        // Se output == 1, acende LED verde com intensidade reduzida
        else {
            pwm_set_chan_level(slice_red, chan_red, 0);
            pwm_set_chan_level(slice_green, chan_green, LED_PWM_INTENSITY);
        }

        update_cursor();
        LedMatrix_Update();
        ssd1306_UpdateScreen();
    }
}

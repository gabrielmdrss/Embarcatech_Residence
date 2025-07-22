#include <stdio.h>
#include "pico/stdlib.h"
#include "ledMatrix/ledMatrix.h"
#include "hardware/pwm.h"

void buzzer_init(uint gpio) {
    // configura o pino como PWM
    gpio_set_function(gpio, GPIO_FUNC_PWM);
}

void buzzer_tone(uint gpio, uint freq_hz) {
    uint slice = pwm_gpio_to_slice_num(gpio);
    uint chan  = pwm_gpio_to_channel(gpio);
    pwm_config config = pwm_get_default_config();
    uint32_t sys_clk = clock_get_hz(clk_sys);

    // sem divisor (clkdiv = 1), wrap para definir a frequência
    pwm_config_set_clkdiv(&config, 1.f);
    pwm_config_set_wrap(&config, sys_clk / freq_hz - 1);

    // inicia o PWM e define 50% de duty
    pwm_init(slice, &config, true);
    pwm_set_chan_level(slice, chan, (sys_clk / freq_hz - 1) / 2);
}

void buzzer_off(uint gpio) {
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_set_enabled(slice, false);
    gpio_set_dir(gpio, GPIO_OUT);
    gpio_put(gpio, 0);
}

void fogo_fraco (void){

    // Leds amarelos para fogo
    LedMatrix_SetPixel(2, 0, LED_WEAK, LED_WEAK, LED_OFF);
    LedMatrix_SetPixel(1, 1, LED_WEAK, LED_WEAK, LED_OFF);
    LedMatrix_SetPixel(2, 1, LED_WEAK, LED_WEAK, LED_OFF);
    LedMatrix_SetPixel(3, 1, LED_WEAK, LED_WEAK, LED_OFF);
    LedMatrix_SetPixel(2, 2, LED_WEAK, LED_WEAK, LED_OFF);

    LedMatrix_SetPixel(1, 0, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(0, 1, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(1, 2, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(2, 3, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(3, 2, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(4, 1, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(3, 0, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_Update();
}

void fogo_forte_1 (void){

    // Leds amarelos para fogo
    LedMatrix_SetPixel(2, 0, LED_MEDIUM, LED_MEDIUM, LED_OFF);
    LedMatrix_SetPixel(1, 1, LED_MEDIUM, LED_MEDIUM, LED_OFF);
    LedMatrix_SetPixel(2, 1, LED_MEDIUM, LED_MEDIUM, LED_OFF);
    LedMatrix_SetPixel(3, 1, LED_MEDIUM, LED_MEDIUM, LED_OFF);
    LedMatrix_SetPixel(2, 2, LED_MEDIUM, LED_MEDIUM, LED_OFF);

    LedMatrix_SetPixel(1, 0, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(0, 1, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(1, 2, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(2, 3, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(3, 2, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(4, 1, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(3, 0, LED_WEAK, LED_OFF, LED_OFF);
    LedMatrix_Update();
}

void fogo_forte_2 (void){

    // Leds amarelos para fogo
    LedMatrix_SetPixel(2, 0, LED_MEDIUM, LED_MEDIUM, LED_OFF);
    LedMatrix_SetPixel(1, 1, LED_MEDIUM, LED_MEDIUM, LED_OFF);
    LedMatrix_SetPixel(2, 1, LED_MEDIUM, LED_MEDIUM, LED_OFF);
    LedMatrix_SetPixel(3, 1, LED_MEDIUM, LED_MEDIUM, LED_OFF);
    LedMatrix_SetPixel(2, 2, LED_MEDIUM, LED_MEDIUM, LED_OFF);

    LedMatrix_SetPixel(1, 0, LED_MEDIUM, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(0, 1, LED_MEDIUM, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(1, 2, LED_MEDIUM, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(2, 3, LED_MEDIUM, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(3, 2, LED_MEDIUM, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(4, 1, LED_MEDIUM, LED_OFF, LED_OFF);
    LedMatrix_SetPixel(3, 0, LED_MEDIUM, LED_OFF, LED_OFF);
    LedMatrix_Update();
}

int main()
{
    stdio_init_all();
    buzzer_init(21);
    gpio_set_dir(8, GPIO_IN); 
    gpio_pull_down(8);
    LedMatrix_Init(&ws2812_config);

    while (true) {

        int botao = gpio_get(8);

        if(botao){
            buzzer_tone(21, 2000);
            fogo_fraco();
            sleep_ms(250);
            fogo_forte_1();
            sleep_ms(250);
            buzzer_off(21);
            fogo_forte_2();
            sleep_ms(250);
            LedMatrix_Clear();
            LedMatrix_Update();
            sleep_ms(100);
        
        } else {
            LedMatrix_Clear();
            LedMatrix_Update();
            buzzer_off(21);
        }
    }
}

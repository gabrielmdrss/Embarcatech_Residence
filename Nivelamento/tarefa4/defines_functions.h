#ifndef DEFINES_FUNCTIONS_H
#define DEFINES_FUNCTIONS_H

#include "hardware/adc.h"
#include "ledMatrix/ledMatrix.h"
#include "ssd1306/ssd1306.h"
#include "ssd1306/ssd1306_fonts.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define SW_BUTTON_PIN 22     // Pino do botão SW do joystick (GPIO 22)
#define LED_RED   13         // Pino do LED vermelho (GPIO 13)
#define LED_GREEN 11         // Pino do LED verde (GPIO 11)
#define LED_PWM_INTENSITY 40 // Intensidade do LED

int gr = 0, ho = 0, di = 0, pt = 0, ct = 0;

const unsigned char bitmap_item_sel_outline_25x25[] = {
    0xFF, 0xFF, 0xFF, 0x80,
    
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x80,
    0xFF, 0xFF, 0xFF, 0x80
};

int cursor = 0;                // Posição do cursor no menu para navegação.
int button_enter_clicked = 0;  // só executa ação quando o botão ENTER é clicado, e espera até outro clique
int left_clicked = 0;          // só executa ação quando o botão é clicado, e espera até outro clique
int right_clicked = 0;         // mesmo que acima

static uint slice_red, chan_red;        // Variáveis para PWM do LED vermelho
static uint slice_green, chan_green;    // Variáveis para PWM do LED verde


void init_pins (void) {

    gpio_init(SW_BUTTON_PIN);                // Inicializa o pino do botão SW do joystick
    gpio_set_dir(SW_BUTTON_PIN, GPIO_IN);    // Define o pino do botão B como entrada
    gpio_pull_up(SW_BUTTON_PIN);             // Habilita o pull-up interno no pino do botão B

    gpio_set_function(LED_RED, GPIO_FUNC_PWM);
    slice_red = pwm_gpio_to_slice_num(LED_RED);
    chan_red = pwm_gpio_to_channel(LED_RED);
    pwm_set_wrap(slice_red, 255);
    pwm_set_chan_level(slice_red, chan_red, 0);
    pwm_set_enabled(slice_red, true);

    // Configura LED_GREEN para PWM
    gpio_set_function(LED_GREEN, GPIO_FUNC_PWM);
    slice_green = pwm_gpio_to_slice_num(LED_GREEN);
    chan_green = pwm_gpio_to_channel(LED_GREEN);
    pwm_set_wrap(slice_green, 255);
    pwm_set_chan_level(slice_green, chan_green, 0);
    pwm_set_enabled(slice_green, true);

}

/* Função de filtro passa-baixa para suavizar a leitura do ADC */
uint low_pass_filter(uint new_value) {
    float alpha = 0.8;              // Fator de suavização (0.0 a 1.0)
    static uint filtered_value = 0; // Valor suavizado (preservado entre as chamadas)

    filtered_value = (alpha * new_value) + ((1 - alpha) * filtered_value);
    return filtered_value;
}

void update_cursor(void){

    if (!(gpio_get(SW_BUTTON_PIN)) && button_enter_clicked == 0) {

        button_enter_clicked = 1;   // Marca o botão ENTER como pressionado

        if (cursor == 0) gr = !gr; // Inverte o estado do botão GR
        else if (cursor == 1) ho = !ho; // Inverte o estado do botão HO
        else if (cursor == 2) di = !di; // Inverte o estado do botão DI
        else if (cursor == 3) pt = !pt; // Inverte o estado do botão PT
        
    }

    // Se o botão ENTER for liberado, a variável auxiliar retorna para baixo, dando chance de clicar novamente
    if ((gpio_get(SW_BUTTON_PIN)) && button_enter_clicked == 1) {
        button_enter_clicked = 0;
    }

    adc_select_input(1);
    uint adc_x_raw = adc_read();
    uint filtered_read = low_pass_filter(adc_x_raw);
    static bool first = true;

    if (first) { 
        // apenas inicializa o filtro e bloqueia click, não move o cursor
        left_clicked = right_clicked = 0;
        first = false;
        return;
    }

    // Verifica o estado do joystick para cima
    if ((filtered_read > 3000) && right_clicked == 0) {
        right_clicked = 1; // Marca como pressionado
        cursor++;
        if (cursor == 4)
            cursor = 0;
    }
    // Libera o botão para cima
    if (filtered_read <= 3000) {
        right_clicked = 0;
    }

    // Verifica o estado do joystick para baixo
    if ((filtered_read < 800) && left_clicked == 0) {
        left_clicked = 1; // Marca como pressionado
        cursor--;
        if (cursor == -1)
            cursor = 3;
    }
    // Libera o botão para baixo
    if (filtered_read >= 800) {
        left_clicked = 0;
    }

    ssd1306_DrawBitmap((cursor*25 + 15), 22, bitmap_item_sel_outline_25x25, 25, 25, White);
    ssd1306_SetCursor(17, 25);
    ssd1306_WriteString("GR", Font_11x18, White);
    ssd1306_SetCursor(42, 25);
    ssd1306_WriteString("HO", Font_11x18, White);
    ssd1306_SetCursor(67, 25);
    ssd1306_WriteString("DI", Font_11x18, White);
    ssd1306_SetCursor(92, 25);
    ssd1306_WriteString("PT", Font_11x18, White);
}

#endif // DEFINES_FUNCTIONS_H
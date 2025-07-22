#ifndef MENU_H
#define MENU_H

#include "ssd1306/ssd1306.h"
#include "ssd1306/ssd1306_fonts.h"
#include "icons.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define SW_BUTTON_PIN 22     // Pino do botão B (GPIO 22)
#define BUTTON_B 6           // Pino do botão B (GPIO 5)
#define BUTTON_A 5           // Pino do botão A (GPIO 10)
#define LED_RED   13         // Pino do LED vermelho (GPIO 13)
#define LED_GREEN 11         // Pino do LED verde (GPIO 11)
#define LED_PWM_INTENSITY 30 // Intensidade do LED

uint8_t output = 0;                     // Variável para armazenar o valor de saída da porta lógica
static uint slice_red, chan_red;        // Variáveis para PWM do LED vermelho
static uint slice_green, chan_green;    // Variáveis para PWM do LED verde


void init_pins (void) {

    gpio_init(SW_BUTTON_PIN);                // Inicializa o pino do botão SW do joystick
    gpio_set_dir(SW_BUTTON_PIN, GPIO_IN);    // Define o pino do botão B como entrada
    gpio_pull_up(SW_BUTTON_PIN);             // Habilita o pull-up interno no pino do botão B

    // Inicializa e configura o botão A
    gpio_init(BUTTON_A);                
    gpio_set_dir(BUTTON_A, GPIO_IN);    
    gpio_pull_up(BUTTON_A);             

    // Inicializa e configura o botão B
    gpio_init(BUTTON_B);                
    gpio_set_dir(BUTTON_B, GPIO_IN);    
    gpio_pull_up(BUTTON_B);

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

    /* Inicializa as variáveis auxiliares do joystick como 1
para evitar que a primeira leitura seja lida como um movimento*/

    up_clicked = 1;      
    down_clicked = 1;    
}

/* Função de filtro passa-baixa para suavizar a leitura do ADC */
uint low_pass_filter(uint new_value) {
    float alpha = 0.5;              // Fator de suavização (0.0 a 1.0)
    static uint filtered_value = 0; // Valor suavizado (preservado entre as chamadas)

    filtered_value = (alpha * new_value) + ((1 - alpha) * filtered_value);
    return filtered_value;
}

/* Função para desenhar o cabeçalho na tela com o título da opção selecionada */
void cabecalho(char *titulo, int x, int y) {
    ssd1306_SetCursor(x, y);
    ssd1306_WriteString(titulo, Font_7x10, White);
    ssd1306_FillRectangle(1, 15, 128, 16, White);
    ssd1306_DrawRectangle(1, 20, 127, 63, White);
}

/* Função para desenhar o esquema de uma porta lógica genérica na tela */
void esquematico (void) {

    // Desenha a letra "A" para indicar a entrada A
    char buffer[8];
    ssd1306_SetCursor(11, 31);
    ssd1306_WriteString("A", Font_6x8, White);
    
    // Desconsiderar a entrada B se o item selecionado for NOT
    if(item_selected != 0){
    ssd1306_SetCursor(12, 46);
    ssd1306_WriteString("B", Font_7x10, White);

    snprintf(buffer, sizeof(buffer), "%d", (gpio_get(BUTTON_B))); // Exibe o valor da entrada B
    ssd1306_SetCursor(42, 41);
    ssd1306_WriteString(buffer, Font_6x8, White);

    ssd1306_DrawRectangle(20, 50, 50, 50, White);       // Reta que simboliza a entrada B da porta lógica
    }

    snprintf(buffer, sizeof(buffer), "%d", (gpio_get(BUTTON_A))); // Armazena o valor da entrada A em uma string
    ssd1306_SetCursor(42, 26);
    ssd1306_WriteString(buffer, Font_6x8, White);   // Exibe o valor da entrada A


    ssd1306_DrawRectangle(20, 35, 50, 35, White);       // Reta que simboliza a entrada A da porta lógica
    ssd1306_FillRectangle(50, 30, 75, 55, White);       // Retângulo que representa a porta lógica
    ssd1306_DrawRectangle(75, 43, 100, 43, White);      // Reta que simboliza a saída da porta lógica
}

void home_screen(void) {
    // Limpa o display
    ssd1306_Fill(Black);

    // Exibe o item de menu anterior
    ssd1306_SetCursor(10, 5);
    ssd1306_WriteString(menu_items[item_sel_previous], Font_7x10, White);

    // Exibe o item de menu atual
    ssd1306_SetCursor(10, 5 + 20 + 2);
    ssd1306_WriteString(menu_items[item_selected], Font_7x10, White);

    // Exibe o item de menu próximo
    ssd1306_SetCursor(10, 5 + 20 + 20 + 2 + 2);
    ssd1306_WriteString(menu_items[item_sel_next], Font_7x10, White);

    // Desenha o contorno de seleção
    ssd1306_DrawBitmap(0, 22, bitmap_item_sel_outline, 128, 21, White);

    // Desenha o fundo da barra de rolagem
    ssd1306_DrawBitmap(128 - 8, 0, bitmap_scrollbar_background, 8, 64, White);

    // Desenha a barra de rolagem indicando a posição atual
    ssd1306_DrawRectangle(125, 64 / NUM_ITEMS * item_selected, 128,
            (64 / NUM_ITEMS * item_selected + (64 / NUM_ITEMS)), White);
    ssd1306_DrawRectangle(126, 64 / NUM_ITEMS * item_selected, 127,
            (64 / NUM_ITEMS * item_selected + (64 / NUM_ITEMS)), White);
}

void update_cursor(void){

    adc_select_input(0);
    uint adc_y_raw = adc_read();
    uint filtered_read = low_pass_filter(adc_y_raw);
    static bool first = true;

    if (first) { 
        // apenas inicializa o filtro e bloqueia click, não move o cursor
        up_clicked = down_clicked = 0;
        first = false;
        return;
    }

    // Verifica o estado do joystick para cima
    if ((filtered_read > 3000) && up_clicked == 0) {
        up_clicked = 1; // Marca como pressionado
        cursor--;
        if (cursor == -1)
            cursor = 3;
        item_selected--;
        if (item_selected < 0)
            item_selected = NUM_ITEMS - 1;
    }
    // Libera o botão para cima
    if (filtered_read <= 3000) {
        up_clicked = 0;
    }

    // Verifica o estado do joystick para baixo
    if ((filtered_read < 1100) && down_clicked == 0) {
        down_clicked = 1; // Marca como pressionado
        cursor++;
        if (cursor == 4)
            cursor = 0;
        item_selected++;
        if (item_selected >= NUM_ITEMS)
            item_selected = 0;
    }
    // Libera o botão para baixo
    if (filtered_read >= 1100) {
        down_clicked = 0;
    }
}

void specific_screen (void) {
    // Limpa o Display
    ssd1306_Fill(Black);
    char buffer[8];

    // Se o item selecionado for 0, exibe a tela de NOT
    if (item_selected == 0) {
        cabecalho("NOT", 55, 0);
        esquematico();
        output = !gpio_get(BUTTON_A); // Porta NOT lógica
        ssd1306_SetCursor(90, 34);
        snprintf(buffer, sizeof(buffer), "%d", output); // Exibe o valor da saída
        ssd1306_WriteString(buffer, Font_6x8, White);

    // Se o item selecionado for 1, exibe a tela de AND
    } else if (item_selected == 1) {
        cabecalho("AND", 55, 0);
        esquematico();
        output = (gpio_get(BUTTON_A)) && (gpio_get(BUTTON_B)); // Porta AND lógica
        ssd1306_SetCursor(90, 34);
        snprintf(buffer, sizeof(buffer), "%d", output); // Exibe o valor da saída
        ssd1306_WriteString(buffer, Font_6x8, White);

    // Se o item selecionado for 2, exibe a tela de OR
    } else if (item_selected == 2) {
        cabecalho("OR", 58, 0);
        esquematico();
        output = (gpio_get(BUTTON_A)) || (gpio_get(BUTTON_B)); // Porta OR lógica
        ssd1306_SetCursor(90, 34);
        snprintf(buffer, sizeof(buffer), "%d", output); // Exibe o valor da saída
        ssd1306_WriteString(buffer, Font_6x8, White);

    // Se o item selecionado for 3, exibe a tela de XOR
    } else if (item_selected == 3) {
        cabecalho("XOR", 55, 0);
        esquematico();
        output = (gpio_get(BUTTON_A)) ^ (gpio_get(BUTTON_B)); // Porta XOR lógica
        ssd1306_SetCursor(90, 34);
        snprintf(buffer, sizeof(buffer), "%d", output); // Exibe o valor da saída
        ssd1306_WriteString(buffer, Font_6x8, White);

    // Se o item selecionado for 4, exibe a tela de NAND
    } else if (item_selected == 4) {
        cabecalho("NAND", 51, 0);
        esquematico();
        output = !(gpio_get(BUTTON_A) && gpio_get(BUTTON_B)); // Porta NAND lógica
        ssd1306_SetCursor(90, 34);
        snprintf(buffer, sizeof(buffer), "%d", output); // Exibe o valor da saída
        ssd1306_WriteString(buffer, Font_6x8, White);

    // Se o item selecionado for 5, exibe a tela de NOR
    } else if (item_selected == 5) {
        cabecalho("NOR", 55, 0);
        esquematico();
        output = !(gpio_get(BUTTON_A) || gpio_get(BUTTON_B)); // Porta NOR lógica
        ssd1306_SetCursor(90, 34);
        snprintf(buffer, sizeof(buffer), "%d", output); // Exibe o valor da saída
        ssd1306_WriteString(buffer, Font_6x8, White);

    // Se o item selecionado for 6, exibe a tela de XNOR
    } else if (item_selected == 6) {
        cabecalho("XNOR", 51, 0);
        esquematico();
        output = !(gpio_get(BUTTON_A)) ^ (gpio_get(BUTTON_B)); // Porta XNOR lógica
        ssd1306_SetCursor(90, 34);
        snprintf(buffer, sizeof(buffer), "%d", output); // Exibe o valor da saída
        ssd1306_WriteString(buffer, Font_6x8, White);
    }
}

void atualiza_LED (void) {
    
// Se estiver na tela inicial, desliga ambos os LEDs (nível de PWM = 0)
    if (current_screen == 0) {
        pwm_set_chan_level(slice_red, chan_red, 0);
        pwm_set_chan_level(slice_green, chan_green, 0);
    } else {
        // Se não estiver na tela inicial, atualiza de acordo com output:
        // Se output == 0, acende LED vermelho com intensidade reduzida
        if (output == 0) {
            pwm_set_chan_level(slice_red, chan_red, LED_PWM_INTENSITY);
            pwm_set_chan_level(slice_green, chan_green, 0);
        }
        // Se output == 1, acende LED verde com intensidade reduzida
        else {
            pwm_set_chan_level(slice_red, chan_red, 0);
            pwm_set_chan_level(slice_green, chan_green, LED_PWM_INTENSITY);
        }
    }
}

void menu (void){

    if(current_screen == 0) {
        home_screen();
        update_cursor();
    
    } else
        specific_screen();

    // Atualiza o LED conforme a tela e a saída
    atualiza_LED();

    // -------------- Verifica o acionamento do botão SW (Enter) -------------- //
    if (!(gpio_get(SW_BUTTON_PIN)) && button_enter_clicked == 0) {

        button_enter_clicked = 1;   // Marca o botão ENTER como pressionado

        // Alterna para o outro tipo de tela
        current_screen = !current_screen;
        
    }

    // Se o botão ENTER for liberado, a variável auxiliar retorna para baixo, dando chance de clicar novamente
    if ((gpio_get(SW_BUTTON_PIN)) && button_enter_clicked == 1) {
        button_enter_clicked = 0;
    }

    // ------------ Atualiza os itens presentes no menu interativo ------------ //
    item_sel_previous = item_selected - 1;  // O item anterior é o item selecionado menos 1

    // Se o item anterior for menor que 0 = O item anterior estaria abaixo do primeiro = torná-lo o último
    if (item_sel_previous < 0) {
        item_sel_previous = NUM_ITEMS - 1;
    } 
    item_sel_next = item_selected + 1;      // O próximo item é o item selecionado mais 1

    // Se o próximo item for maior ou igual ao número total de itens = O próximo item estaria após o último = torná-lo o primeiro
    if (item_sel_next >= NUM_ITEMS) {
        item_sel_next = 0;
    }

    // ------------------------------------------------------------------------ //

    ssd1306_UpdateScreen(); // Atualiza a tela do display OLED
}
#endif /*MENU_H*/
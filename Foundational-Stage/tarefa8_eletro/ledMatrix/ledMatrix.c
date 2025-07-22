#include "ledMatrix.h"

// Global buffer for storing colors of 25 LEDs (5x5)
static uint32_t led_matrix[25] = {0};

PIO pio_ws = pio0;       // PIO instance for WS2812 LED control
uint sm_ws = 0;          // State machine for WS2812 LED control

ws2812_config_t ws2812_config = {
    .pin = WS2812
};


/* Initializes WS2812 LED matrix and configures PIO. */
void LedMatrix_Init(ws2812_config_t *ws2812_config) {
    // Load WS2818b program into the PIO and claim a state machine.
    uint offset = pio_add_program(pio_ws, &ws2818b_program);
    sm_ws = pio_claim_unused_sm(pio_ws, true);
    
    // Initialize the state machine with the chosen PIO program.
    ws2818b_program_init(pio_ws, sm_ws, offset, ws2812_config->pin, 800000.f);

    // Clear the LED buffer by setting all elements to zero.
    for (int i = 0; i < 25; i++) {
        led_matrix[i] = 0;
    }

    // Update the physical LEDs with the cleared buffer.
    LedMatrix_Update();
}


/* Sets the same color for all LEDs in the matrix buffer. */
void LedMatrix_Fill(led_intensity_t red, led_intensity_t green, led_intensity_t blue) {
    // Build the color value once.
    uint32_t color = build_color(red, green, blue);
    // Set every LED in the buffer to this color.
    for (int i = 0; i < 25; i++) {
        led_matrix[i] = color;
    }
}


/* Fills a quadrant of the LED matrix with a specified color. */
void LedMatrix_FillRectangle(int x1, int y1, int x2, int y2, led_intensity_t red, led_intensity_t green, led_intensity_t blue) {
    // Build the color value once.
    uint32_t color = build_color(red, green, blue);
    
    // Clamp the provided coordinates to the valid range [0, 4].
    x1 = clamp(x1, 0, 4);
    y1 = clamp(y1, 0, 4);
    x2 = clamp(x2, 0, 4);
    y2 = clamp(y2, 0, 4);
    
    // Ensure x1/y1 is the top-left and x2/y2 is the bottom-right.
    if(x1 > x2) { int temp = x1; x1 = x2; x2 = temp; }
    if(y1 > y2) { int temp = y1; y1 = y2; y2 = temp; }
    
    // Loop through the specified rows.
    for (int y = y1; y <= y2; y++) {
        // Loop through the specified columns.
        for (int x = x1; x <= x2; x++) {
            int index;
            // Compute the buffer index applying snake wiring.
            if (y % 2 == 0) {
                index = y * 5 + (4 - x); // Reverse order for even rows.
            }
            else {
                index = y * 5 + x;       // Normal order for odd rows.
            }
            // Set the LED color in the buffer.
            led_matrix[index] = color;
        }
    }
}


/* Sets the color of an individual LED at (x,y) in the matrix buffer. */
void LedMatrix_SetPixel(int x, int y, led_intensity_t red, led_intensity_t green, led_intensity_t blue) {
    // Clamp x and y to range [0,4].
    x = clamp(x, 0, 4);
    y = clamp(y, 0, 4);
    int index;
    // Compute buffer index using snake wiring.
    if (y % 2 == 0) {
        index = y * 5 + (4 - x); // Even rows reversed.
    } else {
        index = y * 5 + x;       // Odd rows direct.
    }
    // Store the color.
    led_matrix[index] = build_color(red, green, blue);
}


/* Sends the LED matrix buffer to the physical LEDs via PIO FIFO. */
void LedMatrix_Update(void) {
    // Loop through the buffer.
    for (int i = 0; i < 25; i++) {
        uint32_t c = led_matrix[i];
        uint8_t g = (c >> 16) & 0xFF;  // Extract green.
        uint8_t r = (c >>  8) & 0xFF;  // Extract red.
        uint8_t b = c & 0xFF;          // Extract blue.
        // Transmit color components in GRB order.
        pio_sm_put_blocking(pio_ws, sm_ws, g);
        pio_sm_put_blocking(pio_ws, sm_ws, r);
        pio_sm_put_blocking(pio_ws, sm_ws, b);
    }
    // Delay for latching.
    sleep_us(300);
}


/* Clears the LED matrix buffer by setting all elements to zero and updates the LEDs. */
void LedMatrix_Clear(void) {
    // Loop through each LED in the matrix and set its value to 0.
    for (int i = 0; i < 25; i++) {
        led_matrix[i] = 0;
    }
}


/* Clamps a value between a minimum and maximum. */
static int clamp(int val, int min, int max) {
    if (val < min) return min;    // Return lower bound if value is too low.
    if (val > max) return max;    // Return upper bound if value is too high.
    return val;                   // Otherwise, return the original value.
}


/* Builds a color in 0xGRB format, clamping intensity values to [0, 4]. */
static uint32_t build_color(led_intensity_t red, led_intensity_t green, led_intensity_t blue) {
    // Clamp each intensity value to ensure it is between 0 and 4.
    red   = clamp(red, 0, 4);
    green = clamp(green, 0, 4);
    blue  = clamp(blue, 0, 4);
    // Combine channels into one 32-bit value; green is the most significant byte.
    return ((uint32_t)green << 16) | ((uint32_t)red << 8) | ((uint32_t)blue);
}

void Test_LedMatrix_Wipe(void) {
    // Clear the matrix first.
    LedMatrix_Clear();
    LedMatrix_Update();
    sleep_ms(100);

    // Iterate through each coordinate in the 5x5 matrix.
    for (int index = 0; index < 25; index++) {
        // Compute x and y from the linear index.
        int x = index % 5;
        int y = index / 5;
        
        // Set the current LED to red at maximum intensity.
        LedMatrix_SetPixel(x, y, LED_WEAK, LED_OFF, LED_OFF);
        LedMatrix_Update();
        sleep_ms(200);
    }
    
    sleep_ms(500);
    // Clear the matrix after the wipe.
    LedMatrix_Clear();
    LedMatrix_Update();
    sleep_ms(200);
}
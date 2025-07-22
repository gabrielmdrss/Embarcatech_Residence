/******************************************************************************
 * @file    ledMatrix.h
 * @brief   Header providing general constants, a structure for Led Matrix w281812
 *          data, and function prototypes for handling joystick operations.
 * @authors Gabriel Domingos
 * @date    March 2025
 * @version 1.0.0
 *
 * @note    This header file provides the function prototypes for initializing the
 *          Led Matrix, setting the color of a pixel, setting the color of all
 *          pixels, and setting the color of a specific quadrant.
 ******************************************************************************/

#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include "pico/stdlib.h"
#include "ws2818b.pio.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"

/** 
 * @brief Defines ws2812 LED config value.
 * 
 * These config defines are used to target ws2812 LED pin.
 */
#define WS2812                           7 

/**
 * @brief Structure for WS2812 LED configuration.
 *
 * This structure defines the configuration for controlling WS2812 LEDs.
 */
typedef struct {
    uint8_t pin;
} ws2812_config_t;

/**
 * @brief Enumeration for LED intensity levels.
 *
 * This enumeration defines the intensity levels for controlling the brightness
 * of WS2812 LEDs.
 */
typedef enum {
    LED_OFF = 0,
    LED_WEAK = 4,
    LED_MEDIUM = 2,
    LED_STRONG = 1,
    LED_MAX = 3
} led_intensity_t;

/**
 * @brief External declaration of WS2812 LED configuration.
 *
 * This declares an external variable `ws2812_config` of type `ws2812_config_t`,
 * which holds the configuration settings for controlling WS2812 LEDs.
 * The actual definition of this variable is expected to be provided in another 
 * source file.
 */
extern ws2812_config_t ws2812_config;

/**
 * @brief PIO instance for WS2812 LED control.
 */
extern PIO pio_ws;

/**
 * @brief State machine for WS2812 LED control.
 */
extern uint sm_ws;


/**
 * @brief Initializes WS2812 LED control.
 *
 * This function initializes the GPIO pin specified in the provided `ws2812_config`
 * for controlling WS2812 LEDs. It sets up the GPIO pin direction for output.
 *
 * @param  ws2812_config Pointer to the configuration structure (`ws2812_config_t`)
 *                       containing the GPIO pin used for WS2812 LED control.
 * @return None
 */
void LedMatrix_Init(ws2812_config_t *ws2812_config);


/**
 * @brief Sets the same color for all LEDs in the matrix.
 *
 * This function assigns the discrete intensity levels for the red, green, and blue channels
 * to every LED in the matrix.
 *
 * @param red Intensity for the red channel (LED_OFF, LED_WEAK, LED_MEDIUM, LED_STRONG, LED_MAX).
 * @param green Intensity for the green channel (LED_OFF, LED_WEAK, LED_MEDIUM, LED_STRONG, LED_MAX).
 * @param blue Intensity for the blue channel (LED_OFF, LED_WEAK, LED_MEDIUM, LED_STRONG, LED_MAX).
 */
void LedMatrix_Fill(led_intensity_t red, led_intensity_t green, led_intensity_t blue);


/**
 * @brief Fills a quadrant of the LED matrix with a color.
 *
 * This function sets every LED within the rectangle defined by the top-left corner
 * (x1,y1) and bottom-right corner (x2,y2) to the specified color. Coordinates are clamped
 * to the valid range [0, 4] and the snake wiring order is used for mapping.
 *
 * @param x1 Horizontal coordinate of the top-left corner.
 * @param y1 Vertical coordinate of the top-left corner.
 * @param x2 Horizontal coordinate of the bottom-right corner.
 * @param y2 Vertical coordinate of the bottom-right corner.
 * @param red Intensity for the red channel (LED_OFF, LED_WEAK, LED_MEDIUM, LED_STRONG, LED_MAX).
 * @param green Intensity for the green channel (LED_OFF, LED_WEAK, LED_MEDIUM, LED_STRONG, LED_MAX).
 * @param blue Intensity for the blue channel (LED_OFF, LED_WEAK, LED_MEDIUM, LED_STRONG, LED_MAX).
 */
void LedMatrix_FillRectangle(int x1, int y1, int x2, int y2, led_intensity_t red, led_intensity_t green, led_intensity_t blue);


/**
 * @brief Sets the color of an individual LED in the matrix.
 *
 * This function assigns intensity levels to a single LED at (x,y) using a snake wiring pattern.
 *
 * @param x Horizontal coordinate of the LED (0 to 4).
 * @param y Vertical coordinate of the LED (0 to 4).
 * @param red Intensity for the red channel.
 * @param green Intensity for the green channel.
 * @param blue Intensity for the blue channel.
 */
void LedMatrix_SetPixel(int x, int y, led_intensity_t red, led_intensity_t green, led_intensity_t blue);


/**
 * @brief Transmits the LED matrix data to the physical LEDs.
 *
 * This function sends the current state of the LED matrix buffer to the LEDs.
 *
 * @return None
 */
void LedMatrix_Update(void);


/**
 * @brief Clears all LED matrix pixels (sets them to LED_OFF) and updates the physical LEDs.
 *
 * This function resets the LED matrix buffer by setting each pixel's color to zero, 
 * then transmits the cleared buffer to the physical LEDs.
 *
 * @param ws2812_config Pointer to the WS2812 configuration structure.
 */
void LedMatrix_Clear(void);


/**
 * @brief Limits a value between a specified minimum and maximum.
 *
 * This helper function clamps the provided value within the inclusive range of min to max.
 *
 * @param val The value to be clamped.
 * @param min The minimum allowed value.
 * @param max The maximum allowed value.
 * @return The clamped value.
 */
static int clamp(int val, int min, int max);


/**
 * @brief Constructs a color value in 0xGRB format.
 *
 * This helper function converts LED intensity levels for red, green, and blue channels 
 * (specified as led_intensity_t) into a single 32-bit color value following the 0xRGB format.
 *
 * @param red Intensity of the red channel (allowed values: LED_OFF, LED_WEAK, LED_MEDIUM, LED_STRONG, LED_MAX).
 * @param green Intensity of the green channel (allowed values: LED_OFF, LED_WEAK, LED_MEDIUM, LED_STRONG, LED_MAX).
 * @param blue Intensity of the blue channel (allowed values: LED_OFF, LED_WEAK, LED_MEDIUM, LED_STRONG, LED_MAX).
 * @return A 32-bit color value in 0xGRB format.
 */
static uint32_t build_color(led_intensity_t red, led_intensity_t green, led_intensity_t blue);


void Test_LedMatrix_Wipe(void);

#endif /* LEDMATRIX_H */

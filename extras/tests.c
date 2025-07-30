//
//  PicoCalc Logo
//  Copyright Blair Leduc.
//  See LICENSE for details.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "pico/stdlib.h"
#include "pico/rand.h"

#include "picocalc/screen.h"
#include "picocalc/picocalc.h"
#include "drivers/keyboard.h"

#define M_PI		(3.14159265358979323846)

// Function to apply gamma correction
// value: 0-255, gamma: 0.1 to 2.0
static uint8_t gamma_correct(uint8_t value, float gamma)
{
    return (uint8_t)(powf(value / 255.0f, gamma) * 255.0f + 0.5f);
}

// Function to convert HSV to RGB565
// hue_int: 0-360 degrees, sat_int: 0-255, val_int: 0-255
// Returns a 16-bit RGB565 color
uint16_t hsv_to_rgb565(uint16_t hue_int, uint8_t sat_int, uint8_t val_int)
{
    // Convert integer inputs to floating point
    float hue = (float)hue_int;
    float sat = (float)sat_int / 255.0f;
    float val = (float)val_int / 255.0f;

    float r, g, b;

    // Normalize hue to 0-360 range
    hue = fmodf(hue, 360.0f);

    float c = val * sat; // Chroma
    float x = c * (1.0f - fabsf(fmodf(hue / 60.0f, 2.0f) - 1.0f));
    float m = val - c;

    if (hue < 60.0f)
    {
        r = c;
        g = x;
        b = 0.0f;
    }
    else if (hue < 120.0f)
    {
        r = x;
        g = c;
        b = 0.0f;
    }
    else if (hue < 180.0f)
    {
        r = 0.0f;
        g = c;
        b = x;
    }
    else if (hue < 240.0f)
    {
        r = 0.0f;
        g = x;
        b = c;
    }
    else if (hue < 300.0f)
    {
        r = x;
        g = 0.0f;
        b = c;
    }
    else
    {
        r = c;
        g = 0.0f;
        b = x;
    }

    // Add m and convert to 0-255 range
    uint8_t red = (uint8_t)((r + m) * 255.0f + 0.5f); // +0.5f for rounding
    uint8_t green = (uint8_t)((g + m) * 255.0f + 0.5f);
    uint8_t blue = (uint8_t)((b + m) * 255.0f + 0.5f);

    // Apply gamma correction here (e.g., gamma = 0.8f to boost highlights)
    float gamma = 0.3f;
    red = gamma_correct(red, gamma);
    green = gamma_correct(green, gamma);
    blue = gamma_correct(blue, gamma);
    // Convert to RGB565: 5 bits red, 6 bits green, 5 bits blue
    return ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
}

// Fill the frame buffer with 320 rainbow colors from left to right
void fill_rainbow_framebuffer(void)
{
    uint16_t *screen = screen_gfx_frame();

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        // Calculate hue: 0 to 359 degrees across the width
        uint16_t hue = (x * 360) / SCREEN_WIDTH;

        // Use full saturation and brightness for vivid colors
        uint16_t color = hsv_to_rgb565(hue, 255, 255);

        // Fill the entire column with this color
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            screen[y * SCREEN_WIDTH + x] = color;
        }
    }
}

// Function to test gamma correction and color ramps
// This will create a test pattern with red, green, blue, and white ramps
// and draw quarter circles in each corner with different colors.
void gamma_test(void)
{
    screen_set_mode(SCREEN_MODE_GFX);
    uint16_t *screen = screen_gfx_frame();
    float gamma = 0.3f; // Use the gamma value that works for your display

    // Red ramp
    for (int y = 0; y < 32; y++)
    {
        for (int r = 0; r < 32; r++)
        {
            for (int x = 0; x < 10; x++)
            {
                screen[y * 320 + r * 10 + x] = r << 11;
            }
        }
    }

    // Green ramp
    for (int y = 0; y < 32; y++)
    {
        for (int g = 0; g < 64; g++)
        {
            for (int x = 0; x < 5; x++)
            {
                screen[(y + 64) * 320 + g * 5 + x] = g << 5;
            }
        }
    }

    // Blue ramp
    for (int y = 0; y < 32; y++)
    {
        for (int b = 0; b < 32; b++)
        {
            for (int x = 0; x < 10; x++)
            {
                screen[(y + 128) * 320 + b * 10 + x] = b; // 5 bits for blue
            }
        }
    }

    // White/gray ramp
    for (int y = 0; y < 32; y++)
    {
        for (int w = 0; w < 64; w++)
        {
            for (int x = 0; x < 5; x++)
            {
                screen[(y + 196) * 320 + w * 5 + x] = ((w >> 1) << 11) | (w << 5) | (w >> 1);
            }
        }
    }

    // Parameters for the quarter circles
    int circle_size = 80;
    int center_offset = circle_size - 1;
    int base_y = SCREEN_HEIGHT - circle_size; // Bottom of screen

    // Positions for each color circle
    int centers_x[4] = {0, 80, 160, 240}; // Adjust for spacing
    uint16_t colors[4] = {
        0x01F << 11, // Red
        0x3F << 5,   // Green
        0x1F,        // Blue
        0xFFFF       // White
    };
    // Mask each color to 8 different levels (RBG565)
    // This will create a gradient effect for each color
    uint16_t levels[8];
    for (int i = 0; i < 7; i++)
    {
        // For 8 levels, scale from 0 to max value for each channel
        uint8_t red = (i * 31) / 7;   // 5 bits: 0-31
        uint8_t green = (i * 63) / 7; // 6 bits: 0-63
        uint8_t blue = (i * 31) / 7;  // 5 bits: 0-31

        levels[i] = (red << 11) | (green << 5) | blue;
    }
    levels[7] = 0xFFFFU; // 100% white


    for (int c = 0; c < 4; c++)
    {
        int cx = centers_x[c];
        int cy = base_y + center_offset;

        for (int angle = 0; angle <= 90; angle += 15)
        {
            float rad = angle * (M_PI / 180.0f);
            int x2 = cx + (int)(cosf(rad) * center_offset + 0.5f);
            int y2 = cy - (int)(sinf(rad) * center_offset + 0.5f);

            // Draw line from center to circumference
            screen_gfx_line(cx, cy, x2, y2, colors[c] & levels[angle / 15]);
        }
    }

    screen_gfx_update();

    printf("Welcome to Picocalc Logo!\n");

    while (true)
    {
        tight_loop_contents();
    }
}

// Function to test the rainbow effect
// This will fill the screen with a rainbow gradient from left to right
void rainbow_test(void)
{
    fill_rainbow_framebuffer();
    screen_gfx_update();

    printf("Welcome to Picocalc Logo!\n");

    while (true)
    {
        tight_loop_contents();
    }
}

// Function to test drawing lines
// This will draw random lines on the screen at 50Hz
// and allow switching between text and graphics modes with F1, F2, F3 keys
// Press F1 for text mode, F2 for split mode, F3 for graphics mode
void lines_test()
{
    absolute_time_t start_time = get_absolute_time();
    int counter = 1;
    while (true)
    {
        absolute_time_t now = get_absolute_time();
        if (absolute_time_diff_us(start_time, now) >= 20000)
        {
            start_time = now;
            screen_gfx_update(); // Update the gfx screen at 50Hz
        }

        // Draw a line between two random points with a random colour
        int x1 = get_rand_32() % SCREEN_WIDTH;
        int y1 = get_rand_32() % SCREEN_HEIGHT;
        int x2 = get_rand_32() % SCREEN_WIDTH;
        int y2 = get_rand_32() % SCREEN_HEIGHT;
        uint16_t color = get_rand_32() % 0xFFFF;
        screen_gfx_line(x1, y1, x2, y2, color);

        printf("%d: (%d, %d) to (%d, %d) in colour 0x%04X\n", counter++, x1, y1, x2, y2, color);

        if (keyboard_key_available())
        {
            char ch = keyboard_get_key();
            if (ch == KEY_F1)
            {
                screen_set_mode(SCREEN_MODE_TXT);
            }
            else if (ch == KEY_F2)
            {
                screen_set_mode(SCREEN_MODE_SPLIT);
            }
            else if (ch == KEY_F3)
            {
                screen_set_mode(SCREEN_MODE_GFX);
            }
        }

        tight_loop_contents();
    }
}

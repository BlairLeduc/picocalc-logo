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
#include "drivers/keyboard.h"
#include "picocalc/picocalc.h"

volatile bool user_interrupt = false;


// Function to convert HSV to RGB565 using floating point
// hue: 0.0-360.0 degrees, sat: 0.0-1.0, val: 0.0-1.0
uint16_t hsv_to_rgb565(uint16_t hue_int, uint8_t sat_int, uint8_t val_int)
{
    // Convert integer inputs to floating point
    float hue = (float)hue_int;
    float sat = (float)sat_int / 255.0f;
    float val = (float)val_int / 255.0f;
    
    float r, g, b;
    
    // Normalize hue to 0-360 range
    hue = fmodf(hue, 360.0f);
    
    float c = val * sat;  // Chroma
    float x = c * (1.0f - fabsf(fmodf(hue / 60.0f, 2.0f) - 1.0f));
    float m = val - c;
    
    if (hue < 60.0f) {
        r = c; g = x; b = 0.0f;
    } else if (hue < 120.0f) {
        r = x; g = c; b = 0.0f;
    } else if (hue < 180.0f) {
        r = 0.0f; g = c; b = x;
    } else if (hue < 240.0f) {
        r = 0.0f; g = x; b = c;
    } else if (hue < 300.0f) {
        r = x; g = 0.0f; b = c;
    } else {
        r = c; g = 0.0f; b = x;
    }
    
    // Add m and convert to 0-255 range
    uint8_t red = (uint8_t)((r + m) * 255.0f + 0.5f);    // +0.5f for rounding
    uint8_t green = (uint8_t)((g + m) * 255.0f + 0.5f);
    uint8_t blue = (uint8_t)((b + m) * 255.0f + 0.5f);
    
    // Convert to RGB565: 5 bits red, 6 bits green, 5 bits blue
    return ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
}

// Fill the frame buffer with 320 rainbow colors from left to right
void fill_rainbow_framebuffer(void)
{
    uint16_t *screen = screen_gfx_frame();

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Calculate hue: 0 to 359 degrees across the width
        uint16_t hue = (x * 360) / SCREEN_WIDTH;
        
        // Use full saturation and brightness for vivid colors
        uint16_t color = hsv_to_rgb565(hue, 255, 255);
        
        // Fill the entire column with this color
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            screen[y * SCREEN_WIDTH + x] = color;
        }
    }
}




int main()
{
    char buffer[40];

    picocalc_init();

    //fill_rainbow_framebuffer();
    //screen_gfx_update();

    //printf("Welcome to Picocalc Logo!\n");

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

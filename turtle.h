//
//  PicoCalc Logo
//  Copyright Blair Leduc.
//  See LICENSE for details.
//

#pragma once

#include "pico/stdlib.h"

#include "picocalc/screen.h"

// Turtle definitions
#define TURTLE_HOME_X (SCREEN_WIDTH / 2.0f)  // Home position x coordinate
#define TURTLE_HOME_Y (SCREEN_HEIGHT / 2.0f) // Home position y coordinate
#define TURTLE_DEFAULT_ANGLE (0.0f)          // Default angle for turtle graphics
#define TURTLE_DEFAULT_COLOUR (0xFFFF)       // Default turtle color (white)
#define TURTLE_DEFAULT_VISIBILITY (true)     // Default turtle visibility state
#define TURTLE_DEFAULT_PEN_DOWN (true)       // Default turtle pen state (down)

// Simple colour definitions (Rainbow, plus black and white)
#define COLOUR_BLACK (0x0000)   // Black
#define COLOUR_WHITE (0xFFFF)   // White
#define COLOUR_RED (0xF800)     // Red
#define COLOUR_ORANGE (0xFBE0)  // Orange
#define COLOUR_YELLOW (0xFFE0)  // Yellow
#define COLOUR_GREEN (0x07E0)   // Green
#define COLOUR_BLUE (0x001F)    // Blue
#define COLOUR_CYAN (0x07FF)    // Cyan
#define COLOUR_MAGENTA (0xF81F) // Magenta

// Function prototypes
void turtle_clearscreen(void);
void turtle_draw();
void turtle_move(float distance);
void turtle_home(void);
void turtle_set_position(float x, float y);
void turtle_get_position(float *x, float *y);
void turtle_set_angle(float angle);
float turtle_get_angle(void);
void turtle_set_colour(uint16_t colour);
uint16_t turtle_get_colour(void);
void turtle_set_pen_down(bool down);
bool turtle_get_pen_down(void);
void turtle_set_visibility(bool visible);
bool turtle_get_visibility(void);

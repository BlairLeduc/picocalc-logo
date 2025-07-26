//
//  PicoCalc Logo
//  Copyright Blair Leduc.
//  See LICENSE for details.
//

#pragma once

#include "pico/stdlib.h"
#include "drivers/font.h"

// Screen modes
#define SCREEN_MODE_TXT (0)   // Full-screen text, no graphics
#define SCREEN_MODE_GFX (1)   // Full-screen graphics, no text
#define SCREEN_MODE_SPLIT (2) // Split screen with graphics on top and text on bottom

// Screen dimensions
#define SCREEN_WIDTH (320)                                                            // Width of the screen in pixels
#define SCREEN_HEIGHT (320)                                                           // Height of the screen in pixels
#define SCREEN_COLUMNS (64)                                                           // Max Number of text columns that fit on the screen
#define SCREEN_ROWS (SCREEN_HEIGHT / GLYPH_HEIGHT)                                    // Number of text rows that fit on the screen
#define SCREEN_SPLIT_GFX_HEIGHT (240)                                                 // Height of the graphics area in split mode
#define SCREEN_SPLIT_TXT_HEIGHT (SCREEN_HEIGHT - SCREEN_SPLIT_GFX_HEIGHT)             // Height of the text area in split mode
#define SCREEN_SPLIT_TXT_ROW (SCREEN_HEIGHT - SCREEN_SPLIT_TXT_HEIGHT) / GLYPH_HEIGHT // Star row of text rows in split mode
#define SCREEN_SPLIT_TXT_ROWS (SCREEN_SPLIT_TXT_HEIGHT / GLYPH_HEIGHT)                // Number of text rows in split mode

// Turtle definitions
#define TURTLE_HOME_X (SCREEN_WIDTH / 2.0f)  // Home position x coordinate
#define TURTLE_HOME_Y (SCREEN_HEIGHT / 2.0f) // Home position y coordinate
#define TURTLE_DEFAULT_ANGLE (0.0f)          // Default angle for turtle graphics
#define TURTLE_DEFAULT_COLOR (0xFFFF)        // Default turtle color (white)
#define TURTLE_DEFAULT_VISIBILITY (true)     // Default turtle visibility state
#define TURTLE_DEFAULT_PEN_DOWN (true)       // Default turtle pen state (down)

// Text definitions
#define TEXT_DEFAULT_FONT (&font_5x10)   // Default font for text mode
#define TEXT_DEFAULT_FOREGROUND (0xFFFF) // Default foreground color (white)
#define TEXT_DEFAULT_BACKGROUND (0x0000) // Default background color (black)

// Function prototypes
uint8_t screen_get_mode();
void screen_set_mode(uint8_t mode);

uint16_t *screen_gfx_frame();
uint16_t *screen_txt_frame();

void screen_gfx_update(void);
void screen_txt_update(void);
void screen_txt_clear(void);

// Cursor management functions
void screen_set_cursor(uint8_t column, uint8_t row);
void screen_get_cursor(uint8_t *column, uint8_t *row);
bool screen_txt_putc(uint8_t c);
bool screen_txt_puts(const char *str);

void screen_set_font(const font_t *font);
const font_t *screen_get_font(void);

void screen_gfx_line(float x1, float y1, float x2, float y2, uint16_t colour);

void screen_init(void);

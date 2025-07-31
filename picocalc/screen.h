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

// Text definitions
#define TXT_DEFAULT_FONT (&font_5x10)   // Default font for text mode
#define TXT_DEFAULT_FOREGROUND (0xFFFF) // Default foreground color (white)
#define TXT_DEFAULT_BACKGROUND (0x0000) // Default background color (black)
#define TXT_ATTR_BOLD (0x100)           // Bold attribute bit
#define TXT_ATTR_UNDERLINE (0x200)      // Underline attribute bit
#define TXT_ATTR_REVERSE (0x400)        // Reverse video attribute bit
#define TXT_ATTR_FONT_5x10 (0x800)      // Use 5x10 font
#define TXT_ATTR_FONT_8x10 (0x000)      // Use

// BMP definitions
#define BMP_FILE_HEADER_SIZE (14)                          // Size of the BMP file header in bytes
#define BMP_DIB_HEADER_SIZE (40)                           // Size of the DIB header in bytes
#define BMP_BYTES_PER_PIXEL (2)                            // Bytes per pixel for RGB565 format
#define BMP_ROW_SIZE (SCREEN_WIDTH * BMP_BYTES_PER_PIXEL)  // Size of a row in bytes
#define BMP_PIXEL_DATA_SIZE (BMP_ROW_SIZE * SCREEN_HEIGHT) // Size of the pixel data in bytes
#define BMP_COLOR_MASKS_SIZE (12)                          // Size of the color masks in bytes
#define BMP_COLOR_DEPTH (16)                               // Color depth in bits per pixel
#define BMP_COMPRESSION (3)                                // Compression type (BI_BITFIELDS)
#define BMP_COLOUR_PLANES (1)                              // Number of color planes
#define BMP_PIXELS_PER_METER (2835)                        // Pixels per meter for X and Y (default)
#define BMP_FILE_SIZE (BMP_FILE_HEADER_SIZE + BMP_DIB_HEADER_SIZE + BMP_PIXEL_DATA_SIZE)
#define BMP_PIXEL_DATA_OFFSET (BMP_FILE_HEADER_SIZE + BMP_DIB_HEADER_SIZE + BMP_COLOR_MASKS_SIZE)

// Function prototypes

// Screen mode functions (TXT, GFX, SPLIT)
uint8_t screen_get_mode();
void screen_set_mode(uint8_t mode);

// Graphics functions
uint16_t *screen_gfx_frame();
void screen_gfx_clear(void);
void screen_gfx_point(float x, float y, uint16_t colour, bool xor);
void screen_gfx_line(float x1, float y1, float x2, float y2, uint16_t colour, bool xor);
void screen_gfx_update(void);
int screen_gfx_save(const char *filename);

// Text functions
uint16_t *screen_txt_frame();
void screen_txt_clear(void);
bool screen_txt_putc(uint8_t c);
bool screen_txt_puts(const char *str);
void screen_txt_set_cursor(uint8_t column, uint8_t row);
void screen_txt_get_cursor(uint8_t *column, uint8_t *row);
void screen_txt_enable_cursor(bool cursor_on);
void screen_txt_draw_cursor(void);
void screen_txt_erase_cursor(void);
void screen_txt_set_font(const font_t *font);
const font_t *screen_txt_get_font(void);
void screen_txt_update(void);

// Screen initialization
void screen_init(void);

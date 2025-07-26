//
//  PicoCalc Logo
//  Copyright Blair Leduc.
//  See LICENSE for details.
//

//  PicoCalc screen driver
//
//  This driver provides a simple interface to the LCD display on the PicoCalc.
//  It supports full-screen text mode, full-screen graphics mode, and split screen mode.
//  The screen is a 320x320 pixel display with a 5x10 pixel or 8x10 pixel font.

#include <string.h>
#include <math.h>

#include <pico/stdlib.h>

#include "screen.h"
#include "drivers/font.h"
#include "drivers/lcd.h"

// Frame buffers for the screen
static uint16_t gfx_buffer[SCREEN_WIDTH * SCREEN_HEIGHT] = {0};
static uint16_t txt_buffer[SCREEN_COLUMNS * SCREEN_ROWS] = {0};

// The screen can be in one of three modes:
//
// 1. Full-screen text mode
//    This mode uses the LCD's text capabilities and does not use the frame buffer.
//
// 2. Full-screen graphics mode
//    This mode uses the frame buffer to display graphics on the entire screen.
//
// 3. Split screen mode with graphics on top and text on bottom
//    This mode uses the top 240 lines for graphics and the bottom 80 lines for text.
//
// The default mode is split screen.
//

static uint8_t screen_mode = SCREEN_MODE_SPLIT;

// Graphics state
static float turtle_x = TURTLE_HOME_X;                  // Current x position for graphics
static float turtle_y = TURTLE_HOME_Y;                  // Current y position for graphics
static float turtle_angle = TURTLE_DEFAULT_ANGLE;       // Current angle for graphics
static bool turtle_pen_down = TURTLE_DEFAULT_PEN_DOWN;  // Pen state for graphics
static bool turtle_visible = TURTLE_DEFAULT_VISIBILITY; // Turtle visibility state for graphics
static uint16_t turtle_color = TURTLE_DEFAULT_COLOR;    // Turtle color for graphics

// Text state
static const font_t *screen_font = TEXT_DEFAULT_FONT; // Default font for text mode
static uint16_t text_row = 0;                         // The last row written to in text mode
static uint16_t foreground = TEXT_DEFAULT_FOREGROUND; // Default foreground colour (white)
static uint16_t background = TEXT_DEFAULT_BACKGROUND; // Default background colour (black)
static uint8_t cursor_column = 0;                     // Cursor x position for text mode
static uint8_t cursor_row = 0;                        // Cursor y position for text mode

uint16_t *screen_gfx_frame()
{
    return gfx_buffer;
}

uint16_t *screen_txt_frame()
{
    return txt_buffer;
}

uint8_t screen_get_mode()
{
    return screen_mode;
}

void screen_set_mode(uint8_t mode)
{
    if (mode == SCREEN_MODE_TXT || mode == SCREEN_MODE_GFX || mode == SCREEN_MODE_SPLIT)
    {
        screen_mode = mode;

        if (mode == SCREEN_MODE_TXT)
        {
            // In text mode, we don't use the frame buffer
            lcd_define_scrolling(0, 0); // No scrolling area in full-screen text mode
            screen_txt_update();
        }
        else if (mode == SCREEN_MODE_GFX)
        {
            // In full-screen graphics mode, we clear the screen
            lcd_define_scrolling(0, 0); // No scrolling area in full-screen graphics mode
            screen_gfx_update();
        }
        else if (mode == SCREEN_MODE_SPLIT)
        {
            lcd_define_scrolling(SCREEN_SPLIT_GFX_HEIGHT, 0); // Set scrolling area for text at the bottom
            screen_gfx_update();
            screen_txt_update();
        }
    }
}

// Write the frame buffer to the LCD display
void screen_gfx_update(void)
{
    if (screen_mode == SCREEN_MODE_GFX)
    {
        lcd_blit(gfx_buffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    else if (screen_mode == SCREEN_MODE_SPLIT)
    {
        // Blit the graphics area only
        lcd_blit(gfx_buffer, 0, 0, SCREEN_WIDTH, SCREEN_SPLIT_GFX_HEIGHT);
    }
    // In text mode, we don't update the display
}

// Write the text buffer to the LCD display
void screen_txt_update(void)
{
    if (screen_mode == SCREEN_MODE_TXT)
    {
        // In full-screen text mode, we clear the screen and redraw the text
        for (uint8_t row = 0; row < SCREEN_ROWS; row++)
        {
            for (uint8_t col = 0; col < SCREEN_COLUMNS; col++)
            {
                uint8_t c = txt_buffer[row * SCREEN_COLUMNS + col] & 0xFF;
                lcd_putc(col, row, c > 0 && c < 0x7F ? c : ' ');
            }
        }
    }
    else if (screen_mode == SCREEN_MODE_SPLIT)
    {
        // In split screen mode, we copy:
        // From and including the cursor_row and the preceeding number of rows equal to the text area height
        // If there are not enough rows, we insert empty rows after the cursor_row

        // Clear the text area first
//        lcd_scroll_clear();

        // Calculate the starting row in the buffer to display
        int16_t start_row = text_row - (SCREEN_SPLIT_TXT_ROWS - 1);
        if (start_row < 0)
        {
            start_row = 0;
        }

        // Display the visible rows
        for (uint8_t display_row = 0; display_row < SCREEN_SPLIT_TXT_ROWS; display_row++)
        {
            int16_t buffer_row = start_row + display_row;

            if (buffer_row < SCREEN_ROWS)
            {
                // Copy this row from the buffer to the display
                for (uint8_t col = 0; col < SCREEN_COLUMNS; col++)
                {
                    uint8_t c = txt_buffer[buffer_row * SCREEN_COLUMNS + col] & 0xFF;
                    lcd_putc(col, start_row + display_row, c > 0 && c < 0x7F ? c : ' ');
                }
            }
            // If buffer_row >= SCREEN_ROWS, the row remains empty (already cleared)
        }
    }
    // else for full-screen graphics mode, we do not update the text display
}

void screen_gfx_clear(void)
{
    memset(gfx_buffer, 0, sizeof(gfx_buffer)); // Clear the graphics buffer

    if (screen_mode == SCREEN_MODE_GFX)
    {
        lcd_clear_screen(); // Clear the LCD screen in graphics mode
    }
    else if (screen_mode == SCREEN_MODE_SPLIT)
    {
        // Clear the graphics area in split mode
        lcd_solid_rectangle(background, 0, 0, SCREEN_WIDTH, SCREEN_SPLIT_GFX_HEIGHT);
    }
}

void screen_txt_clear(void)
{
    text_row = 0;                              // Reset the text row to the top
    memset(txt_buffer, 0, sizeof(txt_buffer)); // Clear the text buffer

    if (screen_mode == SCREEN_MODE_TXT)
    {
        lcd_clear_screen(); // Clear the LCD screen in text mode
    }
    else if (screen_mode == SCREEN_MODE_SPLIT)
    {
        // Clear the text area in split mode
        lcd_scroll_clear();
    }
}

void screen_set_font(const font_t *font)
{
    if (font)
    {
        screen_font = font;
        lcd_set_font(font);
    }
}

const font_t *screen_get_font(void)
{
    return screen_font;
}

// Cursor management functions

void screen_set_cursor(uint8_t column, uint8_t row)
{
    uint8_t max_col = SCREEN_WIDTH / screen_font->width - 1; // Maximum column index based on font width

    if (column < max_col && row < SCREEN_ROWS)
    {
        cursor_column = column;
        cursor_row = row;
    }
}

void screen_get_cursor(uint8_t *column, uint8_t *row)
{
    if (column)
    {
        *column = cursor_column;
    }
    if (row)
    {
        *row = cursor_row;
    }
}

// Helper function to scroll the text buffer up one line
static void screen_txt_scroll_up(void)
{
    // Move all rows up by one using memmove (handles overlapping memory correctly)
    memmove(txt_buffer,
            txt_buffer + SCREEN_COLUMNS,
            (SCREEN_ROWS - 1) * SCREEN_COLUMNS * sizeof(uint16_t));

    // Clear the last row using memset
    memset(txt_buffer + (SCREEN_ROWS - 1) * SCREEN_COLUMNS,
           0,
           SCREEN_COLUMNS * sizeof(uint16_t));

    if (screen_mode == SCREEN_MODE_TXT || screen_mode == SCREEN_MODE_SPLIT)
    {
        // Use hardware acceleration to scroll the display
        lcd_scroll_up();
    }
}

// Function to put a character at the current cursor position
// Returns true if the screen scrolled up
bool screen_txt_putc(uint8_t c)
{
    bool scrolled = false;
    if (c == '\n' || c == '\r')
    {
        // Move to next line
        cursor_column = 0;
        cursor_row++;
        if (screen_mode == SCREEN_MODE_TXT || screen_mode == SCREEN_MODE_GFX)
        {
            if (cursor_row >= SCREEN_ROWS)
            {
                // Scroll the text buffer up one line
                screen_txt_scroll_up();
                cursor_row = SCREEN_ROWS - 1;
                scrolled = true;
            }
        }
        else if (screen_mode == SCREEN_MODE_SPLIT)
        {
            // Calculate the starting row in the buffer to display
            int16_t start_row = text_row - (SCREEN_SPLIT_TXT_ROWS - 1);
            if (start_row < 0)
            {
                start_row = 0;
            }

            // In split mode, we need to check the text area height
            if (cursor_row >= start_row + SCREEN_SPLIT_TXT_ROWS)
            {
                // Scroll the text buffer up one line
                screen_txt_scroll_up();
                cursor_row = start_row + SCREEN_SPLIT_TXT_ROWS - 1;
                scrolled = true;
            }
        }
        text_row = cursor_row; // Update the last row written to
    }
    else if (c == '\b') // Backspace
    {
        if (cursor_column > 0)
        {
            cursor_column--;
        }
        else if (cursor_row > 0)
        {
            cursor_row--;
            cursor_column = SCREEN_COLUMNS - 1;
        }
        else
        {
            // At top-left, do nothing
            return false;
        }

        txt_buffer[cursor_row * SCREEN_COLUMNS + cursor_column] = 0; // Clear the character
        if (screen_mode == SCREEN_MODE_TXT || screen_mode == SCREEN_MODE_SPLIT)
        {
            if (screen_mode == SCREEN_MODE_SPLIT)
            {
                // Calculate the starting row in the buffer to display
                int16_t start_row = text_row - (SCREEN_SPLIT_TXT_ROWS - 1);
                if (start_row < 0)
                {
                    start_row = 0;
                }
                if (cursor_row >= start_row && cursor_row < start_row + SCREEN_SPLIT_TXT_ROWS)
                {
                    // Redraw the character at the cursor position
                    lcd_putc(cursor_column, SCREEN_SPLIT_TXT_ROW + cursor_row - start_row, ' ');
                }
            }
            else
            {
                // In text mode, we can simply clear the character
                lcd_putc(cursor_column, cursor_row, ' ');
            }
        }
    }
    else if (c >= 0x20 && c < 0x7F) // Printable characters
    {
        // Store character in buffer
        if (cursor_row < SCREEN_ROWS && cursor_column < SCREEN_COLUMNS)
        {
            txt_buffer[cursor_row * SCREEN_COLUMNS + cursor_column] = c;
            if (screen_mode == SCREEN_MODE_TXT || screen_mode == SCREEN_MODE_SPLIT)
            {
                if (screen_mode == SCREEN_MODE_SPLIT)
                {
                    // Calculate the starting row in the buffer to display
                    int16_t start_row = text_row - (SCREEN_SPLIT_TXT_ROWS - 1);
                    if (start_row < 0)
                    {
                        start_row = 0;
                    }
                    if (cursor_row >= start_row && cursor_row < start_row + SCREEN_SPLIT_TXT_ROWS)
                    {
                        // Redraw the character at the cursor position
                        lcd_putc(cursor_column, SCREEN_SPLIT_TXT_ROW + cursor_row - start_row, c);
                    }
                }
                else
                {
                    // In text mode, we can simply clear the character
                    lcd_putc(cursor_column, cursor_row, c);
                }
            }
            cursor_column++;

            // Wrap to next line if needed
            if (cursor_column >= SCREEN_COLUMNS)
            {
                cursor_column = 0;
                cursor_row++;
                if (cursor_row >= SCREEN_ROWS)
                {
                    // Scroll the text buffer up one line
                    screen_txt_scroll_up();
                    cursor_row = SCREEN_ROWS - 1;
                    scrolled = true;
                }
            }
        }
    }

    return scrolled;
}

// Function to put a string at the current cursor position
// Returns true if the screen scrolled up
bool screen_txt_puts(const char *str)
{
    bool scrolled = false;
    if (!str || !*str) // Check for null or empty string
    {
        return false;
    }

    while (*str)
    {
        if (screen_txt_putc(*str))
        {
            scrolled = true;
        }
        str++;
    }

    return scrolled;
}

static int wrap_and_round(float value, int max)
{
    // Wrap into [0, max)
    value = value - floorf(value / max) * max;
    // Round to nearest integer
    int pixel = (int)(value + 0.5f);
    // Wrap again in case rounding pushed out of bounds
    pixel = ((pixel % max) + max) % max;

    return pixel;
}

static void set_pixel(int x, int y, uint16_t colour)
{
    gfx_buffer[y * SCREEN_WIDTH + x] = colour;
}

void screen_gfx_point(float x, float y, uint16_t colour)
{
    int pixel_x = wrap_and_round(x, SCREEN_WIDTH);
    int pixel_y = wrap_and_round(y, SCREEN_HEIGHT);

    set_pixel(pixel_x, pixel_y, colour);
}

void screen_gfx_line(float x1, float y1, float x2, float y2, uint16_t colour)
{
    // Calculate the number of steps based on the longest axis
    float dx = x2 - x1;
    float dy = y2 - y1;
    int steps = (int)ceilf(fmaxf(fabsf(dx), fabsf(dy)));

    if (steps == 0)
    {
        // Single point
        screen_gfx_point(x1, y1, colour);
        return;
    }

    float x_inc = dx / steps;
    float y_inc = dy / steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; ++i)
    {
        int px = wrap_and_round(x, SCREEN_WIDTH);
        int py = wrap_and_round(y, SCREEN_HEIGHT);

        set_pixel(px, py, colour);

        x += x_inc;
        y += y_inc;
    }
}

void screen_init()
{
    // Initialize the display
    lcd_init();

    // Set for a default of split screen
    screen_set_mode(SCREEN_MODE_TXT);

    // Set the default font
    screen_set_font(&font_5x10);

    // Set foreground and background colors
    lcd_set_foreground(0xFFFF); // White
    lcd_set_background(0x0000); // Black

    // Disable cursor
    lcd_enable_cursor(false);
}

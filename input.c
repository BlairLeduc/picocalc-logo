//
//  PicoCalc Logo
//  Copyright Blair Leduc.
//  See LICENSE for details.
//

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "drivers/keyboard.h"
#include "drivers/lcd.h"
#include "picocalc/screen.h"
#include "input.h"

static char history_buffer[HISTORY_SIZE][HISTORY_LINE_LENGTH] = {0};
static uint8_t history_head = 0;
static uint8_t history_tail = 0;
static uint8_t history_index = 0;

static void history_add(const char *line)
{
	strncpy(history_buffer[history_head], line, HISTORY_LINE_LENGTH - 1);
	history_buffer[history_head][HISTORY_LINE_LENGTH - 1] = '\0';
	history_head = (history_head + 1) % HISTORY_SIZE;
	if (history_head == history_tail)
	{
		history_tail = (history_tail + 1) % HISTORY_SIZE; // Overwrite oldest
	}
}

bool read_line(char *buf, int size)
{
    char key;
    uint8_t start_row = 0, start_col = 0;
    uint8_t end_row = 0, end_col = 0;
    static uint8_t index = 0;
    uint8_t length = 0;

    screen_get_cursor(&start_col, &start_row);
    end_row = start_row;
    end_col = start_col;
    lcd_enable_cursor(true);

    while (1)
    {
        lcd_draw_cursor();
        key = getchar();
        lcd_erase_cursor();

        switch (key)
        {
        case '\b':
            if (index > 0)
            {
                index--;
                length--;

                if (index == length)
                {
                    buf[index] = 0; // Null-terminate the string
                    putchar('\b');  // Move cursor back
                    screen_get_cursor(&end_col, &end_row);
                }
                else
                {
                    uint8_t col, row;
                    putchar('\b'); // Move cursor back
                    screen_get_cursor(&col, &row);
                    memcpy(buf + index, buf + index + 1, length - index + 1);
                    screen_txt_puts(buf + index); // Redisplay the rest of the line
                    screen_get_cursor(&end_col, &end_row);
                    screen_txt_putc(' ');         // Clear the rest of the line
                    screen_set_cursor(col, row);
                }
            }
            break;
        case KEY_DEL: // DEL key
            if (index < length)
            {
                uint8_t col, row;
                screen_get_cursor(&col, &row);
                memcpy(buf + index, buf + index + 1, length - index + 1);
                length--;
                screen_txt_puts(buf + index); // Redisplay the rest of the line
                screen_get_cursor(&end_col, &end_row);
                screen_txt_putc(' ');           // Clear the rest of the line
                screen_set_cursor(col, row);
            }
            break;
        case KEY_ESC: // delete to beginning of line
            if (index > 0)
            {
                screen_set_cursor(start_col, start_row);
                for (int i = index; i < length - 1; i++)
                {
                    screen_txt_putc(' '); // Clear the rest of the line
                }
                index = 0;
                length = 0;
                buf[0] = 0; // Reset buffer
                screen_set_cursor(start_col, start_row);
                end_col = start_col;
                end_row = start_row;
            }
            break;
        case KEY_HOME:
            if (index > 0)
            {
                index = 0;
                screen_set_cursor(start_col, start_row);
            }
            break;
        case KEY_END:
            if (index < length)
            {
                index = length;
                screen_set_cursor(end_col, end_row);
            }
            break;
        case KEY_UP:
            if (history_head != history_tail)
            { // history is not empty
                if (history_index == history_head)
                {
                    // First time pressing up, go to most recent entry
                    history_index = (history_head + HISTORY_SIZE - 1) % HISTORY_SIZE;
                }
                else if (history_index != history_tail)
                {
                    // Move to the previous entry, wrapping if needed
                    history_index = (history_index + HISTORY_SIZE - 1) % HISTORY_SIZE;
                }
                strncpy(buf, history_buffer[history_index], size - 1);
                buf[size - 1] = 0; // Ensure null-termination
                index = strlen(buf);
                screen_set_cursor(start_col, start_row);
                if (screen_txt_puts(buf))
                {
                    start_row--; // Adjust start row if text scrolled
                }
                screen_get_cursor(&end_col, &end_row);
                for (int i = index; i < length - 1; i++)
                {
                    screen_txt_putc(' '); // Clear the rest of the line
                }
                length = index;
                screen_set_cursor(end_col, end_row);
            }
            break;
        case KEY_DOWN:
            if (history_head != history_tail && history_index != history_head)
            {
                // Move to the next entry, wrapping if needed
                history_index = (history_index + 1) % HISTORY_SIZE;
                if (history_index == history_head)
                {
                    // At the newest entry (blank line)
                    buf[0] = 0;
                }
                else
                {
                    strncpy(buf, history_buffer[history_index], size - 1);
                    buf[size - 1] = 0; // Ensure null-termination
                }
                index = strlen(buf);
                screen_set_cursor(start_col, start_row);
                if (screen_txt_puts(buf))
                {
                    start_row--; // Adjust start row if text scrolled
                }
                screen_get_cursor(&end_col, &end_row);
                for (int i = index; i < length - 1; i++)
                {
                    screen_txt_putc(' '); // Clear the rest of the line
                }
                length = index;
                screen_set_cursor(end_col, end_row);
            }
            break;
        case KEY_LEFT:
            if (index > 0)
            {
                index--;
                screen_set_cursor(start_col + index, start_row);
            }
            break;
        case KEY_RIGHT:
            if (index < length)
            {
                index++;
                screen_set_cursor(start_col + index, start_row);
            }
            break;
        default:
            if (key == '\n')
            {
                lcd_erase_cursor();
                lcd_enable_cursor(false);

                history_add((const char *)buf); // Add to history
                return key;
            }
            if (key >= 0x20 && key < 0x7F)
            {
                if (length < size - 1)
                {
                    if (index == length)
                    {
                        buf[index++] = key;
                        buf[index] = 0; // Null-terminate the string
                        length++;
                        if (screen_txt_putc(key))
                        {
                            start_row--; // Adjust start row if text scrolled
                        }
                        screen_get_cursor(&end_col, &end_row);
                    }
                    else
                    {
                        uint8_t col, row;
                        memmove(buf + index + 1, buf + index, length - index + 1);
                        buf[index++] = key;
                        length++;
                        screen_get_cursor(&col, &row);
                        if (screen_txt_puts(buf + index - 1))
                        {
                            start_row--; // Adjust start row if text scrolled
                        }
                        screen_set_cursor(col + index, row);
                    }
                }
                else
                {
                    //os_beep(1); // Beep if buffer is full or width exceeded
                }
            }
            break;
        }
    }

    return key;
}

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

#include "drivers/keyboard.h"

#include "evaluate.h"
#include "input.h"
#include "picocalc/picocalc.h"
#include "picocalc/screen.h"
#include "turtle.h"
#include "version.h"

volatile bool user_interrupt = false;

const char prompt[4] = "??~>";

int main()
{
    char buffer[120];
    int state = EVAL_STATE_COMPLETE;

    // Initialize PicoCalc
    picocalc_init();

    // Initialize the screen and set the mode
    screen_set_mode(SCREEN_MODE_SPLIT);
    screen_txt_set_font(&font_8x10);

    // Boot banner
    printf("Welcome to PicoCalc Logo %s\n", PICOCALC_LOGO_VERSION);
    printf("Copyright Blair Leduc.\n\n");

    turtle_draw(); // Draw the turtle at home position
    screen_gfx_update(); // Update the graphics screen

    // REPL loop
    while(true)
    {
        printf("%c ", prompt[state]);

        buffer[0] = 0; // Reset buffer
        read_line(buffer, sizeof(buffer));
        
        state = evaluate(buffer);
        if (state == EVAL_STATE_ERROR)
        {
            printf("%s\n", last_error);
        }
    }
}

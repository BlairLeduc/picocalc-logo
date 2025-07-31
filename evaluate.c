//
//  PicoCalc Logo
//  Copyright Blair Leduc.
//  See LICENSE for details.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"

#include "turtle.h"
#include "evaluate.h"

char error_message[256] = {0}; // Buffer for error messages
char *last_error = error_message; // Pointer to the last error message

void print_version(void);
void print_license(void);

int evaluate(const char *expr)
{
    char *cmd = strtok((char *)expr, " ");
    char *arg = strtok(NULL, " "); // Get the argument if any

    // This function evaluates the expression and returns the result.
    // For now, it just returns a dummy value.
    if (strcmp(cmd, "version") == 0)
    {
        print_version();
        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "license") == 0)
    {
        print_license();
        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "rt") == 0 || strcmp(cmd, "right") == 0)
    {
         // Turn right by the specified angle
        turtle_set_angle(turtle_get_angle() + atof(arg));

        screen_gfx_update();
        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "lt") == 0 || strcmp(cmd, "left") == 0)
    {
        // Turn left by the specified angle
        turtle_set_angle(turtle_get_angle() - atof(arg));

        screen_gfx_update();
        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "fd") == 0 || strcmp(cmd, "forward") == 0)
    {
         // Move forward by the specified distance
        turtle_move(atof(arg));
        screen_gfx_update();

        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "bk") == 0 || strcmp(cmd, "back") == 0)
    {
        // Move backward by the specified distance
        turtle_move(-atof(arg));
        screen_gfx_update();

        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "setcolor") == 0 || strcmp(cmd, "color") == 0)
    {
        // Convert hex color to uint16_t
        turtle_set_colour((uint16_t)strtol(arg, NULL, 16));

        screen_gfx_update();
        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "home") == 0)
    {
        turtle_home();

        screen_gfx_update();
        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "cs") == 0 || strcmp(cmd, "clearscreen") == 0)
    {
        turtle_clearscreen();
        screen_gfx_update();

        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "penup") == 0 || strcmp(cmd, "pu") == 0)
    {
        // Lift the pen up
        turtle_set_pen_down(false);
        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "pendown") == 0 || strcmp(cmd, "pd") == 0)
    {
        turtle_set_pen_down(true);
        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "showturtle") == 0 || strcmp(cmd, "st") == 0)
    {
        turtle_set_visibility(true);
        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(cmd, "hideturtle") == 0 || strcmp(cmd, "ht") == 0)
    {
        turtle_set_visibility(false);
        return EVAL_STATE_COMPLETE;
    }

    snprintf(error_message, sizeof(error_message), "I don't know how to %s", expr);
    return EVAL_STATE_ERROR;
}
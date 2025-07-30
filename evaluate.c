//
//  PicoCalc Logo
//  Copyright Blair Leduc.
//  See LICENSE for details.
//

#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"

#include "evaluate.h"

char error_message[256] = {0}; // Buffer for error messages
char *last_error = error_message; // Pointer to the last error message

void print_version(void);
void print_license(void);

int evaluate(const char *expr)
{
    // This function evaluates the expression and returns the result.
    // For now, it just returns a dummy value.
    if (strcmp(expr, "version") == 0)
    {
        print_version();
        return EVAL_STATE_COMPLETE;
    }
    else if (strcmp(expr, "license") == 0)
    {
        print_license();
        return EVAL_STATE_COMPLETE;
    }

    snprintf(error_message, sizeof(error_message), "I don't know how to %s", expr);
    return EVAL_STATE_ERROR;
}
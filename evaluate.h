//
//  PicoCalc Logo
//  Copyright Blair Leduc.
//  See LICENSE for details.
//

#pragma once

#include "pico/stdlib.h"

#define EVAL_STATE_COMPLETE (0) // Evaluation is complete
#define EVAL_STATE_ERROR (1)    // Evaluation encountered an error
#define EVAL_STATE_IN_WORD (2)  // Evaluation is in the middle of a word
#define EVAL_STATE_IN_PROC (3)  // Evaluation is in a procedure or function

extern char *last_error; // Pointer to the last error message

// Function Prototypes
int evaluate(const char *expr);
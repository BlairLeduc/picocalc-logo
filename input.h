//
//  PicoCalc Logo
//  Copyright Blair Leduc.
//  See LICENSE for details.
//

#pragma once

#include "pico/stdlib.h"

// History buffer size and line length
#define HISTORY_SIZE 20
#define HISTORY_LINE_LENGTH 120
#define KEY_TIME_OUT 0xFF

// Function prototypes
bool read_line(char *buf, int size);
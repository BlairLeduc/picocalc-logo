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

// Function prototypes
void read_line(char *buf, int size);
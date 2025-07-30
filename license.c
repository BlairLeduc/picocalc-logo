//
//  PicoCalc Logo
//  Copyright Blair Leduc.
//  See LICENSE for details.
//

#include <stdio.h>
#include "pico/stdlib.h"
#include "drivers/font.h"
#include "picocalc/screen.h"
#include "version.h"

void print_version(void)
{
    printf("PicoCalc Logo %s\n", PICOCALC_LOGO_VERSION);
}

void print_license()
{
    const font_t *font = screen_txt_get_font();
    screen_txt_set_font(&font_5x10);
    
    printf("MIT License\n");
    printf("\n");
    printf("Copyright (c) 2025 Blair Leduc\n");
    printf("\n");
    printf("Permission is hereby granted, free of charge, to any person\n");
    printf("obtaining a copy of this software and associated documentation\n");
    printf("files (the \"Software\"), to deal in the Software without\n");
    printf("restriction, including without limitation the rights to use,\n");
    printf("copy, modify, merge, publish, distribute, sublicense, and/or\n");
    printf("sell copies of the Software, and to permit persons to whom the\n");
    printf("Software is furnished to do so, subject to the following\n");
    printf("conditions:\n");
    printf("\n");
    printf("The above copyright notice and this permission notice shall be\n");
    printf("included in all copies or substantial portions of the Software.\n");
    printf("\n");
    printf("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n");
    printf("EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES\n");
    printf("OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND\n");
    printf("NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT\n");
    printf("HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,\n");
    printf("WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING\n");
    printf("FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR\n");
    printf("OTHER DEALINGS IN THE SOFTWARE.\n");

    screen_txt_set_font(font);
}

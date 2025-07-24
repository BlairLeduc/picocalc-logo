#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pico/stdlib.h"

#include "drivers/picocalc.h"
#include "drivers/keyboard.h"
#include "drivers/onboard_led.h"

volatile bool user_interrupt = false;

void set_onboard_led(uint8_t led)
{
    led_set(led & 0x01);
}

int main()
{
    // Initialize the LED driver and set the LED callback
    // If the LED driver fails to initialize, we can still run the text starter
    // without LED support, so we pass NULL to picocalc_init.
    int led_init_result = led_init();

    picocalc_init(led_init_result == 0 ? set_onboard_led : NULL);


}

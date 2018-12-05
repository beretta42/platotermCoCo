/*  Joystick Mouse/Touch Driver
 */
#include <stdint.h>
#include <tgi.h>

#define SCREEN ((uint8_t *)0x400)


uint16_t poll_joy(uint8_t axis);


void do_joy(void) {
    uint8_t x = 0;
    uint8_t y = 0;
    x = poll_joy(0);
    y = poll_joy(1);
    tgi_setpixel(x*4,y*3);
}

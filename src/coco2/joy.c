/*  Joystick Mouse/Touch Driver
    fixme: this code has a fixed idea of screen resolution, 256x192

    fixme: this needs to do a moving average to smooth jitters
 */

#include <stdint.h>
#include <tgi.h>

#define keystrobe (*(volatile unsigned char *)0xff02)
#define keyread   (*(volatile unsigned char *)0xff00)

uint16_t poll_joy(uint8_t axis);

/* we'll store our x and y coords in 9 bits
   so we don't have to know our screen res here */
int mouse_x;
int mouse_y;
int mouse_b;

static int hidden = 1;

void do_joy(void) {
    int x1 = 0;
    int y1 = 0;
    uint8_t nb;
    x1 = poll_joy(0);
    y1 = poll_joy(1);
    if (hidden == 0) {
	if ((x1 != mouse_x) || (y1 != mouse_y)){
	    tgi_unput_mouse(mouse_x,mouse_y);
	    tgi_put_mouse(x1,y1);
	}
    }
    mouse_x = x1;
    mouse_y = y1;
    keystrobe = ~1;
    mouse_b = (~keyread) & 0xf;
}


void mouse_show(void) {
    if (hidden == 1){
	tgi_put_mouse(mouse_x, mouse_y);
	hidden = 0;
    }
}

void mouse_hide(void) {
    if (hidden == 0){
	tgi_unput_mouse(mouse_x, mouse_y);
	hidden = 1;
    }
}

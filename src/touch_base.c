/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * touch.c - Touchscreen functions
 */

#include <stdbool.h>
#include <string.h>
#include <mouse.h>
#include <peekpoke.h>
#include "touch.h"
#include "config.h"

static padBool TouchActive;

static struct mouse_info mouse_data;
static uint16_t screen_w;
static uint16_t screen_h;
static uint8_t mouse_present=false;
static uint8_t mou_res=0;

extern ConfigInfo config;

/**
 * touch_init() - Set up touch screen
 */
void touch_init(void)
{

}

/**
 * touch_allow - Set whether touchpanel is active or not.
 */
void touch_allow(padBool allow)
{
  TouchActive=allow;
}

/**
 * touch_main - Process mouse events and turn into scaled touch events
 */
void touch_main(void)
{
  static uint8_t lastbuttons;
  padPt coord;
  di();
  //if (mouse_present==false) fixme: why not finding mouse?
  //    goto out;
  
  if (mouse_b == lastbuttons)
    goto out; /* debounce */
  else if (mouse_b & MOUSE_BTN_LEFT)
    {
      coord.x = mouse_x;
      coord.y = 511 - mouse_y;
      Touch(&coord);
    }
  lastbuttons = mouse_b;
 out:
  ei();
}

/**
 * touch_hide() - hide the mouse cursor
 */
void touch_hide(void)
{

}

/**
 * touch_done() - Stop the mouse driver
 */
void touch_done(void)
{

}

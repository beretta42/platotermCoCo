/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * plato.c - main program
 */

#include <stdbool.h>
#include <stdio.h>
#include "terminal.h"
#include "screen.h"
#include "touch.h"
#include "keyboard.h"
#include "io.h"
#include "config.h"
#include "prefs.h"
#include "serial.h"

uint8_t already_started=false;

/**
 * greeting(void) - Show terminal greeting
 */
void greeting(void)
{
    static uint8_t b[256];
    int s;
    FILE *f;
    f = fopen("SPLASH","r");
    do {
	s = fread(b,1,256,f);
	ShowPLATO(b,s);
    } while (s);
    fclose(f);
    prefs_show_greeting();
    terminal_initial_position();
}


/* fixme: this should be from the config structure */
void autocon(void)
{
    if (config.init[0] == 0)
	return;
    uint8_t *p = config.init;
    while (*p)
	ser_put(*p++);
    ser_put('\r');
    ser_put('\n');
}

void main(void)
{
  screen_init();
  config_init();
  io_init();
  //touch_init();
  //terminal_init();
  greeting();
  io_echof = 1;
  screen_beep();

  already_started=true;

  if (config.valid == 0)
      prefs_run();
  else
      autocon();

  // And do the terminal
  for (;;)
    {
      io_main();
      keyboard_main();
      touch_main();
    }
  
  screen_done();
  touch_done();
}

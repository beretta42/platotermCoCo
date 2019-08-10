/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * prefs.c - Preferences menu functions
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <conio.h>
#include <tgi.h>
#include <string.h>
#include <serial.h>
#include <mouse.h>
#include <ctype.h>
#include "io.h"
#include "screen.h"
#include "keyboard.h"
#include "prefs.h"
#include "protocol.h"
#include "terminal.h"
#include "config.h"

extern unsigned char font[];
extern unsigned int fontptr[];

extern ConfigInfo config;

uint8_t prefs_running;
static padBool TTYSave;
static padPt TTYLocSave;
static uint8_t CharWideSave;
static uint8_t CharHighSave;
static uint8_t CurMemSave;
static DispMode CurModeSave;
static padBool ModeBoldSave;
static padBool RotateSave;
static padBool ReverseSave;
extern padBool TTY;
extern padPt TTYLoc;
extern uint8_t CharWide;
extern uint8_t CharHigh;
extern DispMode CurMode;
extern padBool ModeBold;
extern padBool Rotate;
extern padBool Reverse;

uint8_t temp_val[8];
uint8_t ch;
uint8_t prefs_need_updating;
uint8_t touch_prefs_updated;
uint8_t io_prefs_updated;

static unsigned char check_up[] = {
    0b11111100,
    0b10000100,
    0b10000100,
    0b10000100,
    0b10000100,
    0b11111100,
};


static unsigned char check_set[] = {
    0b00000110,
    0b00001100,
    0b11011000,
    0b01110000,
    0b00110000,
    0b00000000,
};


static unsigned char check_down[] = {
    0b11111100,
    0b10101100,
    0b11000100,
    0b10001100,
    0b11010100,
    0b11111100,
};

static unsigned char radio_up[] = {
    0b01111000,
    0b11001100,
    0b10000100,
    0b10001100,
    0b11011100,
    0b01111000,
};

static unsigned char radio_down[] = {
    0b01111000,
    0b11101100,
    0b11000100,
    0b10000100,
    0b11001100,
    0b01111000,
};

static unsigned char radio_set[] = {
    0b01111000,
    0b11010100,
    0b10111100,
    0b11110100,
    0b10101100,
    0b01111000,
};

typedef struct widget_s widget;

struct widget_s {
    int x;
    int y;
    int w;
    int h;
    int d;
    char *t;
    void (* doev)(widget *w, int e);
    void (* dowd)(widget *w, int e);
    widget *next;
};

widget *head = NULL;

static tgi_puts(int x, int y, char *text) {
    int todo = strlen(text);
    while (todo--) {
	tgi_char_blit(x, y, &font[fontptr[*text++ - 32]]);
	x += 4;
    }
}

static void docheck(widget *w, int e) {
    tgi_cset(0);
    tgi_bar( w->x, w->y, w->x+8, w->y+6);
    switch (e) {
    case 0:
	tgi_char_blit(w->x+1, w->y+1, check_up);
	break;
    case 1:
	tgi_char_blit(w->x+1, w->y+1, check_down);
	break;
    case 2:
	tgi_char_blit(w->x+1, w->y+1, check_up);
	w->d = !w->d;
	break;
    }
    if (w->d)
        tgi_char_blit(w->x+1, w->y+1, check_set);
    tgi_puts(w->x + 9, w->y + 1, w->t);
}

static void doradio(widget *w, int e) {
    tgi_cset(0);
    tgi_bar( w->x, w->y, w->x+8, w->y+6);	
    switch (e) {
    case 0:
	tgi_char_blit(w->x+1, w->y+1, radio_up);
	tgi_puts(w->x + 9, w->y + 1, w->t);
	break;
    case 1:
	tgi_char_blit(w->x+1, w->y+1, radio_down);
	break;
    case 2:
	tgi_char_blit(w->x+1, w->y+1, radio_up);
	w->d = 1;
	w->dowd(w, 2);
	break;
    }
    if (w->d)
	tgi_char_blit(w->x+1, w->y+1, radio_set);
}

static void drawall(void) {
    widget *w = head;
    while (w) {
	w->doev(w, 0);
	w = w->next;
    }
}

static void widget_add(widget *w) {
    w->next = head;
    head = w;
}

static widget *pcollide(int x, int y) {
    widget *w = head;
    while (w) {
	if (x >= w->x &&
	    x <= w->x + w->w &&
	    y >= w->y &&
	    y <= w->y + w->h)
	    return w;
	w = w->next;
    }
    return NULL;
}


static void doflow(widget *w, int e);
static widget hard  = {50, 100, 40, 10, 1, "CTS/RTS", doradio, doflow};
static widget soft = {50, 110, 45, 10, 0,  "XON/XOFF", doradio, doflow};
static widget none = {50, 120, 30, 10, 0,  "NONE", doradio, doflow};
static void doflow(widget *w, int e) {
    hard.d = 0;
    soft.d = 0;
    none.d = 0;
    w->d = 1;
    hard.doev(&hard,0);
    soft.doev(&soft,0);
    none.doev(&none,0);
}


static void dospeed(widget *w, int e);
static widget S300 = {50, 130, 30, 10, 1, "300", doradio, dospeed};
static widget S600 = {50, 140, 30, 10, 1, "600", doradio, dospeed};
static widget S1200 = {50, 150, 30, 10, 1, "1200", doradio, dospeed};
static widget S2400 = {50, 160, 30, 10, 1, "2400", doradio, dospeed};
static widget S4800 = {100, 130, 30, 10, 1, "4800", doradio, dospeed};
static widget S9600 = {100, 140, 30, 10, 1, "9600", doradio, dospeed};
static widget S19200 = {100, 150, 30, 10, 1, "19200", doradio, dospeed};
static widget S115200 = {100, 160, 30, 10, 1, "115200", doradio, dospeed};
static void dospeed(widget *w, int e) {
    S300.d = S600.d = S1200.d = S2400.d = S4800.d =
	S9600.d = S19200.d = S115200.d = 0;
    w->d = 1;
    S300.doev(&S300, 0);
    S600.doev(&S600, 0);
    S1200.doev(&S1200, 0);
    S2400.doev(&S2400, 0);
    S4800.doev(&S4800, 0);
    S9600.doev(&S9600, 0);
    S19200.doev(&S19200, 0);
    S115200.doev(&S115200, 0);
}

static void doecho(widget *w, int e);
static widget echo = {50,170,30,10,1, "Local Echo", docheck, doecho};
static void doecho(widget *w, int e) {
}

/**
 * Run the preferences menu
 */
void prefs_run(void)
{
    int new;
    int x,y;
    int last;
    widget *w;
    widget_add(&hard);
    widget_add(&soft);
    widget_add(&none);
    widget_add(&S300);
    widget_add(&S600);
    widget_add(&S1200);
    widget_add(&S2400);
    widget_add(&S4800);
    widget_add(&S9600);
    widget_add(&S19200);
    widget_add(&S115200);
    widget_add(&echo);
    drawall();
    while(1) {
	di();
	new = mouse_b;
	x = mouse_x / 2 - 2;
	y = mouse_y * 3 / 8;
	ei();
	if (new != last && (w = pcollide(x,y))) {
	    mouse_hide();
	    if (new)
		w->doev(w, 1);
	    else
		w->doev(w, 2);
	    mouse_show();
	}
	last = new;
    }
}

void prefs_display(const char* text)
{
}

void prefs_clear(void)
{
}

/**
 * close prefs. 
 */
void prefs_done(void)
{
  TTY=TTYSave;
  CharWide=CharWideSave;
  CharHigh=CharHighSave;
  CurMem=CurMemSave;
  ModeBold=ModeBoldSave;
  Rotate=RotateSave;
  Reverse=ReverseSave;
  prefs_clear();
  TTYLoc.x=TTYLocSave.x;
  TTYLoc.y=TTYLocSave.y;
}

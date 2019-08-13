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


static unsigned char combo_up[] = {
    0b00000000,
    0b11111110,
    0b01000100,
    0b00101000,
    0b00010000,
    0b00000000,
};

static unsigned char combo_down[] = {
    0b00000000,
    0b00000000,
    0b11111110,
    0b01111100,
    0b00111000,
    0b00010000,
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
    widget *child;
    widget *parent;
};

static widget *head = NULL;
static widget *focus = NULL;


static tgi_puts(int x, int y, char *text) {
    int todo = strlen(text);
    while (todo--) {
	tgi_char_blit(x, y, &font[fontptr[*text++ - 32]]);
	x += 4;
    }
}


static void bound_list(widget *p) {
    widget *w = p;
    int x1 = w->x;
    int y1 = w->y;
    int x2 = w->x + w->w;
    int y2 = w->y + w->h;
    while (w) {
	x1 = x1 < w->x ? x1 : w->x;
	y1 = y1 < w->y ? y1 : w->y;
	x2 = x2 > (w->x + w->w) ? x2 : (w->x + w->w);
	y2 = y2 > (w->y + w->h) ? y2 : (w->y + w->h);
	w = w->next;
    }
    tgi_cset(1);
    tgi_hline(x1, y1, x2-x1);
    tgi_hline(x1, y2, x2-x1);
    tgi_vline(x1, y1, y2-y1);
    tgi_vline(x2, y1, y2-y1);
}

static void draw_children(widget *p) {
    widget *w = p->child;
    while (w) {
	w->doev(w,0);
	w = w->next;
    }
}


static void docombo(widget *w, int e) {
    tgi_cset(0);
    tgi_bar(w->x, w->y, w->x + w->w, w->y + w->h);
    switch (e) {
    case 0:
	tgi_char_blit(w->x+1, w->y+1, combo_up);
	tgi_puts(w->x+9, w->y+2, w->t);
	break;
    case 1:
	tgi_char_blit(w->x+1, w->y+1, combo_down);
	tgi_puts(w->x+10, w->y+3, w->t);
	break;
    case 2:
	tgi_char_blit(w->x+1, w->y+1, combo_up);
	tgi_puts(w->x+9, w->y+2, w->t);
	if (w->d) {
	    w->d = 0;
	    focus = head;
	}
	else {
	    w->d = 1;
	    draw_children(w);
	    bound_list(w->child);
	    focus = w->child;
	}
	break;
    }
}

static void dolabel(widget *w, int e) {
    if (!e) {
	tgi_cset(0);
	tgi_bar( w->x, w->y, w->x+w->w, w->y+w->h);
	tgi_puts(w->x+1, w->y+1, w->t);
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


/* think about a draw_area(*widget) */

static void draw_all(void) {
    widget *w = head;
    mouse_hide();
    tgi_clear();
    while (w) {
	w->doev(w, 0);
	w = w->next;
    }
    mouse_show();
}



static void docombo_item(widget *w, int e) {
    tgi_cset(0);
    tgi_bar(w->x, w->y, w->x + w->w, w->y + w->h);
    switch(e) {
    case 0:
	tgi_puts(w->x + 2, w->y +2, w->t);
	break;
    case 1:
	tgi_puts(w->x + 3, w->y +3, w->t);
	break;
    case 2:
	focus = head;
	w->parent->t = w->t;
	w->parent->d = 0;
	draw_all();
	break;
    }
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
	if (w->dowd) w->dowd(w, 2);
	break;
    }
    if (w->d)
	tgi_char_blit(w->x+1, w->y+1, radio_set);
}

static void dobutton(widget *w, int e) {
    tgi_cset(0);
    tgi_bar(w->x, w->y, w->x + w->w, w->y + w->h);
    tgi_cset(1);
    tgi_hline(w->x+1, w->y, w->w - 2);
    tgi_hline(w->x+1, w->y + w->h-1, w->w - 2);
    tgi_vline(w->x, w->y+1, w->h - 2);
    tgi_vline(w->x + w->w-1, w->y+1, w->h - 2);
    switch (e) {
    case 0:
	tgi_puts(w->x + 3, w->y + 3, w->t);
	break;
    case 1:
	tgi_puts(w->x + 4, w->y + 4, w->t);
	break;
    case 2:
	tgi_puts(w->x + 3, w->y + 3, w->t);
	break;
    }
}


/* add widget to another or head list */
static void widget_add(widget *p, widget *w) {
    if (!p) {
	w->next = head;
	w->parent = NULL;
	head = w;
    }
    else {
	w->next = p->child;
	w->parent = p;
	p->child = w;
    }
}

static widget *pcollide(int x, int y) {
    widget *w = focus;
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


static void doecho(widget *w, int e);
static widget echo = {50,130,30,10,1, "Local Echo", docheck, doecho};
static void doecho(widget *w, int e) {
}

static widget speedl = {50, 140, 20, 10, 0, "Baud", dolabel};
static widget speed = {70, 140, 30, 10, 0, "1200", docombo};
static widget S115200 = {70,150, 50, 10, 0, "115200", docombo_item};
static widget S9600 = {70,160, 50, 10, 0, "9600", docombo_item};
static widget S4800 = {70,170, 50, 10, 0, "4800", docombo_item};
static widget S2400 = {70,180, 50, 10, 0, "2400", docombo_item};
static widget S1200 = {120,150, 50, 10, 0, "1200", docombo_item};
static widget S600 = {120,160, 50, 10, 0, "600", docombo_item};
static widget S300 = {120,170, 50, 10, 0, "300", docombo_item};


static widget mpil = {50,150, 40, 10, 0, "MPI Slot", dolabel};
static widget mpi = {90,150, 20, 10, 0, "1", docombo};
static widget mpi1 = {90, 160, 10, 10, 0, "1", docombo_item};
static widget mpi2 = {100, 160, 10, 10, 0, "2", docombo_item};
static widget mpi3 = {110, 160, 10, 10, 0, "3", docombo_item};
static widget mpi4 = {120, 160, 10, 10, 0, "4", docombo_item};

static widget ifacel = {50, 90, 45, 10, 0, "Interface", dolabel};
static widget iface  = {95, 90, 45, 10, 0, "DW Becker", docombo};
static widget iface1 = {95, 100, 60, 10, 0, "DW Becker", docombo_item};
static widget iface2 = {95, 110, 60, 10, 0, "DW Bitbanger", docombo_item};
static widget iface3 = {95, 120, 60, 10, 0, "DW RS232", docombo_item};
static widget iface4 = {95, 130, 60, 10, 0, "RS232 Pak", docombo_item};

static widget save = {50, 161, 26, 12, 0, "Save", dobutton};

/**
 * Run the preferences menu
 */
void prefs_run(void)
{
    int new;
    int x,y;
    int last;
    widget *w;
    widget *clicked;
    widget_add(NULL, &ifacel);
    widget_add(NULL, &iface);
    widget_add(&iface, &iface1);
    widget_add(&iface, &iface2);
    widget_add(&iface, &iface3);
    widget_add(&iface, &iface4);
    widget_add(NULL, &hard);
    widget_add(NULL, &soft);
    widget_add(NULL, &none);
    widget_add(NULL, &echo);
    widget_add(NULL, &speedl);
    widget_add(NULL, &speed);
    widget_add(&speed, &S115200);
    widget_add(&speed, &S9600);
    widget_add(&speed, &S4800);
    widget_add(&speed, &S2400);
    widget_add(&speed, &S1200);
    widget_add(&speed, &S600);
    widget_add(&speed, &S300);
    widget_add(NULL, &mpi);
    widget_add(NULL, &mpil);
    widget_add(&mpi, &mpi1);
    widget_add(&mpi, &mpi2);
    widget_add(&mpi, &mpi3);
    widget_add(&mpi, &mpi4);
    widget_add(NULL, &save);
    focus = head;
    draw_all();
    last = mouse_b;
    while(1) {
	di();
	new = mouse_b;
	x = mouse_x / 2 - 2;
	y = mouse_y * 3 / 8;
	ei();
	if (new != last) {
	    w = pcollide(x,y);
	    mouse_hide();
	    if (w) {
		if (new) {
		    w->doev(w, 1);
		    clicked = w;
		}
		else {
		    if (w == clicked) w->doev(w, 2);
		    else {
			clicked->doev(clicked,0);
			clicked = NULL;
		    }
		}
	    }
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

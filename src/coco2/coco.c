#include <stdio.h>
#include <string.h>
#include <serial.h>
#include <stdint.h>
#include <protocol.h>
#include <config.h>
#include <mouse.h>
#include <tgi.h>

extern unsigned char font[];
extern unsigned char fontm23[];
extern unsigned char FONT_SIZE_X;
extern unsigned char FONT_SIZE_Y;
extern unsigned short scalex[];
extern unsigned short scaley[];
extern unsigned int fontptr[];

#define FAST_TEXT 1
#define FAST_PIX  1
#define FAST_CLEAR 1
#define FAST_HLINE 1

extern void (*io_recv_serial_flow_off)(void);
extern void (*io_recv_serial_flow_on)(void);
volatile extern uint16_t timer;

extern padBool FastText;

#define screen ((unsigned char *)0x6000)

void noop(void) {
}

/********************
stdlib.h API
*********************/
int atoi(const char *nptr)
{
}

/********************
stdio.h API
*********************/
int open_ll(uint16_t mode);
int read_ll(void);
int write_ll(uint8_t c);
int close_ll(void);

#define DEVNUM *((uint8_t *)0x6f)
#define NAMBUF (uint8_t *)0x94c

FILE *fopen(const char *pathname, const char *mode)
{
    char m;
    memset(NAMBUF, ' ', 11);
    /* fixme: do something more clever about filenames/extensions here */
    memcpy(NAMBUF, pathname, strlen(pathname));
    if (*mode == 'r')
	m = 'I';
    else
	m = 'O';
    return (FILE *)open_ll( (m<<8) | 1 );
}


size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    int r = 0;
    uint8_t *p = (uint8_t *)ptr;
    size_t num = 0;
    DEVNUM = 1;
    while(nmemb--){
	while(size--){
	    r = read_ll();
	    if (r < 0)
		goto out;
	    *p++ = r;
	}
	num++;
    }
 out:
    return num;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb,
	      FILE *stream)
{
    int r = 0;
    uint8_t *p = (uint8_t *)ptr;
    size_t num = 0;
    size_t s;
    DEVNUM = 1;
    while(nmemb--){
	s = size;
	while(s--){
	    r = write_ll(*p++);
	    if (r < 0)
		goto out;
	}
	num++;
    }
 out:
    return num;
}

int fclose(FILE *stream)
{
    DEVNUM = 1;
    return close_ll();
}


/********************
 string.h API
*********************/
void *memset(void *s, int c, size_t n)
{
    unsigned char *p = (unsigned char *)s;
    while (n--) *p++ = c;
}

void *memcpy(void *d, const void *s, size_t n)
{
    uint8_t *dest = (uint8_t *)d;
    uint8_t *src  = (uint8_t *)s;
    while (n--) *dest++ = *src++;
}

size_t strlen (const char* s)
{
    char *e = (char *)s;
    while (*e++);
    return e-s-1;
}

char *strcpy(char *dest, const char *src)
{
    return memcpy(dest,src,strlen(src));
}

int tolower (int c)
{
    if (c >= 'A' && c <= 'Z')
	return c + 40;
    return c;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1) {
	if (*s1++ != *s2++)
	    return -1;
    }
    return 0;
}

/********************
 PlatoTerm API
*********************/


void config_init_hook(void)
{
}

void config_set_defaults(void)
{
    config.io_mode = IO_MODE_DWBECKER;
}

void screen_update_colors(void)
{
}

void io_init_funcptrs(void)
{
    io_recv_serial_flow_off = noop;
    io_recv_serial_flow_on = noop;
}

void io_send_byte(uint8_t b)
{
    ser_put(b);
}

void prefs_touch(void)
{
}

void screen_beep(void)
{
}

void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count)
{
    int16_t offset; /* due to negative offsets */
    uint16_t x;      /* Current X and Y coordinates */
    uint16_t y;
    uint16_t* px;   /* Pointers to X and Y coordinates used for actual plotting */
    uint16_t* py;
    uint8_t i; /* current character counter */
    uint8_t a; /* current character byte */
    uint8_t j,k; /* loop counters */
    int8_t b; /* current character row bit signed */
    uint8_t width = FONT_SIZE_X;
    uint8_t height = FONT_SIZE_Y;
    uint16_t deltaX = 1;
    uint16_t deltaY = 1;
    uint8_t mainColor = 1;
    uint8_t altColor = 0;
    uint8_t *p;
    uint8_t* curfont;
    switch(CurMem) {
    case M0:
	curfont = font;
	offset = -32;
	break;
    case M1:
	curfont = font;
	offset = 64;
	break;
    case M2:
	curfont = fontm23;
	offset = -32;
	break;
    case M3:
	curfont = fontm23;
	offset = 32;
	break;
    }

    if (CurMode == ModeRewrite)
	altColor = 0;
    else if (CurMode == ModeInverse)
      altColor = 1;

    if (CurMode == ModeErase || CurMode == ModeInverse)
	mainColor = 0;
    else
	mainColor = 1;

    tgi_setcolor(mainColor);

    x = scalex[Coord->x & 0x1FF];
    y = scaley[(Coord->y + 15) & 0x1FF];

    if (FastText == padF)
    	goto chardraw_with_fries;

    /* the diet chardraw routine - fast text output. */
    for (i = 0; i < count; ++i) {
#ifdef FAST_TEXT
	if (CurMode == ModeWrite)
	    tgi_char_blit(x,y,*ch++);
	else
	    tgi_char_blit_erase(x,y,*ch++);
#else
	a = *ch;
	++ch;
	a += offset;
	p = &curfont[fontptr[a]];
	for (j = 0; j < FONT_SIZE_Y; ++j) {
	    b = *p;
	    for (k = 0; k < FONT_SIZE_X; ++k) {
		if (b < 0) { /* check sign bit. */
		    tgi_setcolor(mainColor);
		    tgi_setpixel(x, y);
		}
		++x;
		b <<= 1;
	    }
	    ++y;
	    x -= width;
	    ++p;
	}
	y -= height;
#endif
	x += width;
    }
    return;

 chardraw_with_fries:
    if (Rotate) {
	deltaX = -abs(deltaX);
	width = -abs(width);
	px = &y;
	py = &x;
    }
    else {
	px = &x;
	py = &y;
    }
    if (ModeBold) {
	deltaX = deltaY = 2;
	width <<= 1;
	height <<= 1;
    }
    for (i = 0; i < count; ++i) {
	a = *ch;
	++ch;
	a += offset;
	p = &curfont[fontptr[a]];
	for (j = 0; j < FONT_SIZE_Y; ++j) {
	    b = *p;
	    if (Rotate) {
		px = &y;
		py = &x;
	    }
	    else {
		px = &x;
		py = &y;
	    }
	    for (k = 0; k < FONT_SIZE_X; ++k) {
		if (b < 0) { /* check sign bit. */
		    tgi_setcolor(mainColor);
		    if (ModeBold) {
			tgi_setpixel(*px + 1, *py);
			tgi_setpixel(*px, *py + 1);
			tgi_setpixel(*px + 1, *py + 1);
		    }
		    tgi_setpixel(*px, *py);
		}
		else {
		    if (CurMode == ModeInverse || CurMode == ModeRewrite) {
			tgi_setcolor(altColor);
			if (ModeBold) {
			    tgi_setpixel(*px + 1, *py);
			    tgi_setpixel(*px, *py + 1);
			    tgi_setpixel(*px + 1, *py + 1);
			}
			tgi_setpixel(*px, *py);
		    }
		}
		x += deltaX;
		b <<= 1;
	    }
	    y += deltaY;
	    x -= width;
	    ++p;
	}
	Coord->x += width;
	x += width;
	y -= height;
    }
    return;
}

void prefs_show_greeting(void)
{
    prefs_display("platoterm ready - <BREAK> for setup");
}


void screen_load_driver(void)
{
}

void screen_init_hook(void)
{
    /* 256x192 bw mode green/light green */
    *(unsigned char *)0xff22 = 0xf0;
    *(unsigned char *)0xffc0 = 0x00;
    *(unsigned char *)0xffc3 = 0x00;
    *(unsigned char *)0xffc5 = 0x00;
    /* screen ram address to 0x6000 */
    *(unsigned char *)0xffc6 = 0x00;
    *(unsigned char *)0xffc8 = 0x00;
    *(unsigned char *)0xffca = 0x00;
    *(unsigned char *)0xffcc = 0x00;
    *(unsigned char *)0xffcf = 0x00;
    *(unsigned char *)0xffd1 = 0x00;
    *(unsigned char *)0xffd2 = 0x00;
}

/*
void keyboard_main(void)
{
}

void keyboard_clear(void)
{
}
*/

void terminal_char_load(padWord charnum, charData theChar)
{
}



/**********************
 Misc CC65 libs
***********************/
unsigned char ser_ioctl (unsigned char code, void* data)
{
}

unsigned char ser_uninstall (void)
{
}

unsigned char ser_unload (void)
{
}

unsigned char ser_load_driver (const char* driver)
{
    uint8_t h[5];
    uint8_t *type  = (int8_t *)(h+0);
    uint16_t *size = (uint16_t *)(h+1);
    uint8_t **addr = (uint8_t **)(h+3);

    FILE *f;
    f = fopen(driver,"r");
    while(1){
	fread(h,5,1,f);
	if (*type != 0)
	    break;
	di();
	fread(*addr,*size,1,f);
	ei();
    }
    fclose(f);
    ser_init();
    return 0;
}

void screen_wait(void)
{
    uint16_t t = timer;
    while(timer == t);
}

unsigned char mouse_unload (void)
{
}

unsigned char mouse_load_driver (const struct mouse_callbacks* c,
				 const char* driver)
{
}

void mouse_show (void)
{
}

void mouse_move (int x, int y)
{
}

unsigned char mouse_uninstall (void)
{
}

const struct mouse_callbacks mouse_def_callbacks;
const struct mouse_info mouse_info;

/**********************
 TGI API
***********************/

unsigned int pen = 0xff;


// fixme: do most of this stuff in asm

unsigned char tgi_getcolor (void)
{
}

void tgi_setcolor (unsigned char color)
{
    tgi_cset(color);
    pen = color;
}

void tgi_bar (int x1, int y1, int x2, int y2)
{
    int h;
    int w;
    int x;
    int y;
    w = abs(x1 - x2);
    h = abs(y1 - y2);
    if (!w || !h)
	return;
    x = x1 < x2 ? x1 : x2;
    y = y1 < y2 ? y1 : y2;
    do {
        tgi_hline(x,y++,w);
    } while (h--);   
}

void tgi_init (void)
{
}

#ifndef FAST_CLEAR
void tgi_clear (void)
{
    memset(screen, 0, 32*192);
}
#endif

#ifdef FAST_PIX
void tgi_setpixel (int x, int y)
{
    unsigned char mask;
    int off;

    mask = 0x80 >> (x & 7);
    x >>= 3;
    off = y * 32 + x;
    /* for 1 bpp this works, but should be
       more like: screen = (screen & ~mask) | (color & mask) */
    if (pen){
        screen[off] = screen[off] | mask;
    }
    else
        screen[off] = screen[off] & ~mask;
}
#endif

void tgi_line (int x1, int y1, int x2, int y2)
{
    int dx =  abs(x2-x1), sx = x1<x2 ? 1 : -1;
    int dy = -abs(y2-y1), sy = y1<y2 ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */

    for(;;){  /* loop */
	tgi_setpixel(x1,y1);
	if (x1==x2 && y1==y2) break;
	e2 = 2*err;
	if (e2 >= dy) { err += dy; x1 += sx; } /* e_xy+e_x > 0 */
	if (e2 <= dx) { err += dx; y1 += sy; } /* e_xy+e_y < 0 */
    }
}

#ifndef FAST_HLINE
void tgi_hline (int x1, int y1, int x2)
{
    int t;
    if (x1 > x2){
	t = x1;
	x1 = x2;
	x2 = t;
    }
    while(x1 <= x2)
	tgi_setpixel(x1++, y1);
}
#endif

void tgi_done (void)
{
}

void tgi_uninstall (void)
{
}



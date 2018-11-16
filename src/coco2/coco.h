#include <stdio.h>
#include <string.h>
#include <serial.h>
#include <stdint.h>
#include <protocol.h>
#include <mouse.h>

/********************
stdlib.h API
*********************/
int atoi(const char *nptr);

/******************** 
stdio.h API 
*********************/
FILE *fopen(const char *pathname, const char *mode);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb,
	      FILE *stream);
int fclose(FILE *stream);

/********************
 string.h API 
*********************/
void *memset(void *s, int c, size_t n);
size_t strlen (const char* s);
int tolower (int c);
int strcmp(const char *s1, const char *s2);

/********************
 PlatoTerm API 
*********************/


void config_init_hook(void);
void config_set_defaults(void);
void screen_update_colors(void);
void io_init_funcptrs(void);
void io_send_byte(uint8_t b);
void prefs_show_greeting(void);
void prefs_driver(void);
void prefs_touch(void);
void screen_beep(void);
void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count);
void screen_load_driver(void);
void screen_init_hook(void);
void keyboard_main(void);
void keyboard_clear(void);
void terminal_char_load(padWord charnum, charData theChar);

    
/**********************
 Misc CC65 libs
***********************/
char cgetc (void);
void screen_wait(void);
unsigned char mouse_unload (void);
unsigned char mouse_load_driver (const struct mouse_callbacks* c,
				 const char* driver);
void mouse_show (void);
void mouse_move (int x, int y);
unsigned char mouse_uninstall (void);
const struct mouse_callbacks mouse_def_callbacks;
const struct mouse_info mouse_info;

/**********************
 TGI API
***********************/


unsigned char tgi_getcolor (void);
void tgi_setcolor (unsigned char color);
void tgi_bar (int x1, int y1, int x2, int y2);
void tgi_init (void);
void tgi_clear (void);
void tgi_setpixel (int x, int y);
void tgi_line (int x1, int y1, int x2, int y2);
void tgi_done (void);
void tgi_uninstall (void);


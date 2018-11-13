#include <stdio.h>
#include <string.h>
#include <serial.h>
#include <stdint.h>
#include <protocol.h>
#include <mouse.h>

/********************
stdlib.h API
*********************/
int atoi(const char *nptr)
{
}

/******************** 
stdio.h API 
*********************/
FILE *fopen(const char *pathname, const char *mode)
{
    return NULL;
}


size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return 0;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb,
	      FILE *stream)
{
    return 0;
}

int fclose(FILE *stream)
{
    return -1;
}


/********************
 string.h API 
*********************/
void *memset(void *s, int c, size_t n)
{
    unsigned char *p = (unsigned char *)s;
    while (n--) *p++ = c;
}

char* strupr (char* s)
{
    char *p = s;
    while (*p){
	*p = toupper(*p);
	p++;
    }
    return s;
}

size_t strlen (const char* s)
{
    int n;
    while (*s++) n++;
    return n;
}

int toupper (int c)
{
    if (c >= 'a' || c <= 'z')
	return c - 40;
    return c;
}

int tolower (int c)
{
    if (c >= 'A' || c <= 'Z')
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
}

void screen_update_colors(void)
{
}

void io_init_funcptrs(void)
{
}

void io_send_byte(uint8_t b)
{
}

short splash_size = 1;

padByte splash[1] = {0x42};

void prefs_show_greeting(void)
{
}

void prefs_driver(void)
{
}

void prefs_touch(void)
{
}

void screen_beep(void)
{
}

void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count)
{
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

void keyboard_main(void)
{
}

void keyboard_clear(void)
{
}

void terminal_char_load(padWord charnum, charData theChar)
{
}



/**********************
 Misc CC65 libs
***********************/
unsigned char ser_open (const struct ser_params* params)
{
}

unsigned char ser_ioctl (unsigned char code, void* data)
{
}

unsigned char ser_get (char* b)
{
}

unsigned char ser_close (void)
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
}

char cgetc (void)
{
}

void screen_wait(void)
{
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


unsigned char tgi_getcolor (void)
{
}

void tgi_setcolor (unsigned char color)
{
}

void tgi_bar (int x1, int y1, int x2, int y2)
{
}

void tgi_init (void)
{
}

void tgi_clear (void)
{
}

void tgi_setpixel (int x, int y)
{
}

void tgi_line (int x1, int y1, int x2, int y2)
{
}

void tgi_done (void)
{
}

void tgi_uninstall (void)
{
}

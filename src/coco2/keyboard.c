/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * keyboard.h - Keyboard functions
 */

#include <stdint.h>
#include "../keyboard.h"
#include "../protocol.h"
#include "../screen.h"
#include "../plato_key.h"

#define keystrobe (*(volatile unsigned char *)0xff02)
#define keyread   (*(volatile unsigned char *)0xff00)
unsigned char ktab[8];
unsigned char ktab1[8];
volatile unsigned char key;
volatile unsigned char meta;

/* keycode to ascii table */
unsigned char atab[] = {
    '@', 'h', 'p', 'x', '0', '8', 13 /*enter*/,
    'a', 'i', 'q', 'y', '1', '9', 0 /*clear*/,
    'b', 'j', 'r', 'z', '2', ':', 0 /*break*/,
    'c', 'k', 's', 0/*up*/, '3', ';', 0 /*alt*/,
    'd', 'l', 't', 0/*dn*/, '4', ',', 0 /*cntl*/,
    'e', 'm', 'u', 0x08/*lf*/, '5', '-', 0 /*f1*/,
    'f', 'n', 'v', 0x09/*rt*/, '6', '.', 0 /*f2*/,
    'g', 'o', 'w', ' ', '7', '/', 0 /*shift*/,
};

/* keycode to shifted ascii table */
unsigned char satab[] = {
    '@', 'H', 'P', 'X', '0', '(', 10 /*enter*/,
    'A', 'I', 'Q', 'Y', '!', ')', 0 /*clear*/,
    'B', 'J', 'R', 'Z', '"', '*', 0 /*break*/,
    'C', 'K', 'S', 0/*up*/, '#', '+', 0 /*alt*/,
    'D', 'L', 'T', 0/*dn*/, '$', '<', 0 /*cntl*/,
    'E', 'M', 'U', 0x08/*lf*/, '%', '=', 0 /*f1*/,
    'F', 'N', 'V', 0x09/*rt*/, '&', '>', 0 /*f2*/,
    'G', 'O', 'W', ' ', '\'', '?', 0 /*shift*/,
};

/* keycode to plato normal */
unsigned char knone[] = {
    PKEY_AT, 'H', 'P', 'X', 0x00, 0x08, PKEY_NEXT /*enter*/,
    'A', 'I', 'Q', 'Y', 0x01, 0x09, 0 /*clear*/,
    'B', 'J', 'R', 'Z', 0x02, PKEY_COLON, 0 /*break*/,
    'C', 'K', 'S', 0x0c, 0x03, PKEY_SEMICOLON, 0 /*alt*/,
    'D', 'L', 'T', 0/*dn*/, 0x04, PKEY_COMMA, 0 /*cntl*/,
    'E', 'M', 'U', PKEY_BACKSPACE/*lf*/, 0x05, PKEY_MINUS, 0 /*f1*/,
    'F', 'N', 'V', 0/*rt*/, 0x06, PKEY_PERIOD, 0 /*f2*/,
    'G', 'O', 'W', PKEY_SPACE, 0x07, PKEY_SLASH, 0 /*shift*/,
};

/* keycode to plato shifted */
unsigned char kshift[] = {
    '@', 'h', 'p', 'x', '0', PKEY_PARENTHESIS_LEFT, PKEY_NEXT1 /*enter*/,
    'a', 'i', 'q', 'y', PKEY_EXCLAMATION, PKEY_PARENTHESIS_RIGHT, 0 /*clear*/,
    'b', 'j', 'r', 'z', PKEY_QUOTE, PKEY_ASTERISK, 0 /*break*/,
    'c', 'k', 's', 0/*up*/, PKEY_POUND, PKEY_PLUS, 0 /*alt*/,
    'd', 'l', 't', 0/*dn*/, PKEY_DOLLAR, PKEY_LESS_THAN, 0 /*cntl*/,
    'e', 'm', 'u', PKEY_ERASE/*lf*/, PKEY_PERCENT, PKEY_EQUALS, 0 /*f1*/,
    'f', 'n', 'v', 0/*rt*/, PKEY_AMPERSAND, PKEY_GREATER_THAN, 0 /*f2*/,
    'g', 'o', 'w', 0x60, PKEY_APOSTROPHE, PKEY_QUESTION_MARK, 0 /*shift*/,
};

/* keycode to plato control */
unsigned char kcntl[] = {
    '@', PKEY_HELP, 'p', 'x', '0', '8', 0x33 /*enter*/,
    PKEY_ACCESS, 'i', 'q', 'y', '1', '9', 0 /*clear*/,
    PKEY_BACK, 'j', 'r', 'z', '2', ':', 0 /*break*/,
    'c', 'k', PKEY_STOP, 0/*up*/, '3', ';', 0 /*alt*/,
    PKEY_DATA, PKEY_LAB, 't', 0/*dn*/, '4', ',', 0 /*cntl*/,
    PKEY_EDIT, 'm', 'u', 0/*lf*/, '5', '-', 0 /*f1*/,
    'f', 'n', 'v', 0/*rt*/, '6', '.', 0 /*f2*/,
    'g', 'o', 'w', ' ', '7', '/', 0 /*shift*/,
};

/* keycode to plato shift-cntls */
unsigned char kshcntl[] = {
    '@', 0x35, 'p', 'x', '0', '8', 0x33 /*enter*/,
    0x3c, 'i', 'q', 'y', '1', '9', 0 /*clear*/,
    0x38, 'j', 'r', 'z', '2', ':', 0 /*break*/,
    'c', 'k', 0x3a, 0/*up*/, '3', ';', 0 /*alt*/,
    0x39, 0x3d, 't', 0/*dn*/, '4', ',', 0 /*cntl*/,
    0x37, 'm', 'u', 0/*lf*/, '5', '-', 0 /*f1*/,
    'f', 'n', 'v', 0/*rt*/, '6', '.', 0 /*f2*/,
    'g', 'o', 'w', 0x60, '7', 0x7d, 0 /*shift*/,
};

/* poll the coco keyboard - called from interrupt */
void kpoll(void)
{
    unsigned char m = 0;
    unsigned char b = 0xfe;
    int i,j;
    /* copy existing ktab to prime */
    for(i = 0; i < 8; i++)
	ktab1[i] = ktab[i];
    /* read keys into table */
    for(i = 0; i < 8; i++){
	keystrobe = b;
	ktab[i] = (~keyread) & 0x7f;
	b = (b << 1) + 1;
    }
    /* gather and mask shift keys */
    m += (ktab[7] & 0x40) ? 1 : 0;
    ktab[7] &= ~0x40;
    m <<= 1;
    if ( (ktab[4] & 0x40) || (ktab[1] & 0x40) ){
	m+= 1;
    }
    ktab[4] &= ~0x40;
    ktab[1] &= ~0x40;
    m <<= 1;
    m += (ktab[3] & 0x40) ? 1 : 0;
    ktab[3] &= ~0x40;
    /* find new char code, if any */
    for (i = 0; i < 8; i++) {
	b = (ktab[i] ^ ktab1[i]) & ktab[i];
	for (j = 0; j < 7; j++) {
	    if (b & 1) {
		key = i * 7 + j + 1;
		meta = m;
		return;
	    }
	    b = b >> 1;
	}
    }
}

/**
 * keyboard_out - If platoKey < 0x7f, pass off to protocol
 * directly. Otherwise, platoKey is an access key, and the
 * ACCESS key must be sent, followed by the particular
 * access key from PTAT_ACCESS.
 */
void keyboard_out(uint8_t platoKey){
    if (platoKey == 0xff)
	return;
    if (platoKey > 0x7f) {
	//Key(ACCESS);
	//key(ACCESS_KEYS[platoKey - 0x80]);
	return;
    }
    Key(platoKey);
    return;
};

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void){
    int k;
    if(key){
	k = key;
	key = 0;
	if (TTY) {
	    if (meta & 4)
		k = satab[k-1];
	    else
		k = atab[k-1];
	    ser_put_clean(k);
	    screen_tty_char(k);
	} 
	else {
	    if (meta == 0)
		keyboard_out(knone[k-1]);
	    else if (meta == 4)
		keyboard_out(kshift[k-1]);
	    else if (meta == 2)
		keyboard_out(kcntl[k-1]);
	    else if (meta == 6)
		keyboard_out(kshcntl[k-1]);
	}
    }
    return;
};

/**
 * keyboard_clear() - Clear the keyboard buffer
 */
void keyboard_clear(void){
    key = 0;
};



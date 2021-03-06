#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i=0;
    int w=0;

    if (argc < 2)
	w = 4;
    else if( argv[1][2] == '5')
	w = 5;
    else {
	fprintf(stderr,"usage: mk_font [-4,-5]\n");
	exit(1);
    }


    printf("/* ! Autogenerated by tools/mk_font.c */\n\n"
	   "#ifndef FONT_H\n"
	   "#define FONT_H\n\n");

    printf("unsigned char FONT_SIZE_X = %d;\n",w);
    printf("unsigned char FONT_SIZE_Y = 6;\n");
    printf("unsigned char fontm23[768];\n");

    if (w == 4){
	printf("unsigned char font[]={\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,           /* SPACE 0x20 */\n"
	 "0x80,0x80,0x80,0x00,0x80,0x00,           /* ! 0x21     */\n"
	 "0xA0,0xA0,0x00,0x00,0x00,0x00,           /* \" 0x22     */\n"
	 "0xA0,0xE0,0xA0,0xE0,0xA0,0x00,           /* # 0x23     */\n"
	 "0x40,0xE0,0xC0,0x60,0xE0,0x40,           /* $ 0x24     */\n"
	 "0xA0,0x20,0x40,0x80,0xA0,0x00,           /* \% 0x25     */\n"
	 "0x40,0xA0,0x40,0xA0,0x60,0x00,           /* & 0x26     */\n"
	 "0x80,0x80,0x00,0x00,0x00,0x00,           /* ' 0x27     */\n"
	 "0x20,0x40,0x40,0x40,0x20,0x00,           /* ( 0x28     */\n"
	 "0x40,0x20,0x20,0x20,0x40,0x00,           /* ) 0x29     */\n"
	 "0xa0,0x40,0xe0,0x40,0xa0,0x00,           /* * 0x2a     */\n"
	 "0x00,0x40,0xe0,0x40,0x00,0x00,           /* + 0x2b     */\n"
	 "0x00,0x00,0x00,0x40,0x80,0x00,           /* , 0x2c     */\n"
	 "0x00,0x00,0xE0,0x00,0x00,0x00,           /* - 0x2d     */\n"
	 "0x00,0x00,0x00,0x00,0x80,0x00,           /* . 0x2e     */\n"
	 "0x00,0x20,0x60,0xc0,0x80,0x00,           /* / 0x2f     */\n"
	 "0x60,0xA0,0xA0,0xA0,0xC0,0x00,           /* 0 0x30     */\n"
	 "0x40,0xC0,0x40,0x40,0xE0,0x00,           /* 1 0x31     */\n"
	 "0xC0,0x20,0x40,0x80,0xE0,0x00,           /* 2 0x32     */\n"
	 "0xC0,0x20,0x40,0x20,0xC0,0x00,           /* 3 0x33     */\n"
	 "0x20,0xA0,0xE0,0x20,0x20,0x00,           /* 4 0x34     */\n"
	 "0xE0,0x80,0xC0,0x20,0xC0,0x00,           /* 5 0x35     */\n"
	 "0x40,0x80,0xC0,0xA0,0x40,0x00,           /* 6 0x36     */\n"
	 "0xE0,0x20,0x20,0x40,0x40,0x00,           /* 7 0x37     */\n"
	 "0x40,0xA0,0x40,0xA0,0x40,0x00,           /* 8 0x38     */\n"
	 "0x40,0xA0,0x60,0x20,0x40,0x00,           /* 9 0x39     */\n"
	 "0x80,0x00,0x00,0x80,0x00,0x00,           /* : 0x3A     */\n"
	 "0x40,0x00,0x00,0x40,0x80,0x00,           /* ; 0x3B     */\n"
	 "0x20,0x40,0x80,0x40,0x20,0x00,           /* < 0x3C     */\n"
	 "0x00,0xE0,0x00,0xE0,0x00,0x00,           /* = 0x3D     */\n"
	 "0x80,0x40,0x20,0x40,0x80,0x00,           /* < 0x3E     */\n"
	 "0xC0,0x20,0x40,0x00,0x40,0x00,           /* ? 0x3F     */\n"
	 "0x40,0xA0,0xA0,0xA0,0x80,0x60,           /* @ 0x40     */\n"
	 "0x40,0xA0,0xE0,0xA0,0xA0,0x00,           /* A 0x41     */\n"
	 "0xC0,0xA0,0xC0,0xA0,0xC0,0x00,           /* B 0x42     */\n"
	 "0x60,0x80,0x80,0x80,0x60,0x00,           /* C 0x43     */\n"
	 "0xC0,0xA0,0xA0,0xA0,0xC0,0x00,           /* D 0x44     */\n"
	 "0xE0,0x80,0xC0,0x80,0xE0,0x00,           /* E 0x45     */\n"
	 "0xE0,0x80,0xC0,0x80,0x80,0x00,           /* F 0x46     */\n"
	 "0x60,0x80,0xA0,0xA0,0x60,0x00,           /* G 0x47     */\n"
	 "0xA0,0xA0,0xE0,0xA0,0xA0,0x00,           /* H 0x48     */\n"
	 "0xE0,0x40,0x40,0x40,0xE0,0x00,           /* I 0x49     */\n"
	 "0x20,0x20,0x20,0x20,0xC0,0x00,           /* J 0x4A     */\n"
	 "0xA0,0xA0,0xC0,0xA0,0xA0,0x00,           /* K 0x4B     */\n"
	 "0x80,0x80,0x80,0x80,0xE0,0x00,           /* L 0x4C     */\n"
	 "0xA0,0xE0,0xE0,0xA0,0xA0,0x00,           /* M 0x4D     */\n"
	 "0xC0,0xA0,0xA0,0xA0,0xA0,0x00,           /* N 0x4E     */\n"
	 "0x40,0xA0,0XA0,0xA0,0x40,0x00,           /* O 0x4F     */\n"
	 "0xC0,0xA0,0xC0,0x80,0x80,0x00,           /* P 0x50     */\n"
	 "0x40,0xA0,0xA0,0xA0,0x40,0x20,           /* Q 0x51     */\n"
	 "0xC0,0xA0,0xC0,0xA0,0xA0,0x00,           /* R 0x52     */\n"
	 "0x60,0x80,0x40,0x20,0xC0,0x00,           /* S 0x53     */\n"
	 "0xE0,0x40,0x40,0x40,0x40,0x00,           /* T 0x54     */\n"
	 "0xA0,0xA0,0xA0,0xA0,0x40,0x00,           /* U 0x55     */\n"
	 "0xA0,0xA0,0xA0,0x40,0x40,0x00,           /* V 0x56     */\n"
	 "0xA0,0xA0,0xE0,0xE0,0xA0,0x00,           /* W 0x57     */\n"
	 "0xA0,0xA0,0x40,0xA0,0xA0,0x00,           /* X 0x58     */\n"
	 "0xA0,0xA0,0x40,0x40,0x40,0x00,           /* Y 0x59     */\n"
	 "0xE0,0x20,0x40,0x80,0xE0,0x00,           /* Z 0x5A     */\n"
	 "0xE0,0x80,0x80,0x80,0xE0,0x00,           /* [ 0x5B     */\n"
	 "0x80,0xC0,0x60,0x20,0x00,0x00,           /* \\ 0x5C     */\n"
	 "0xE0,0x20,0x20,0x20,0xE0,0x00,           /* ] 0x5D     */\n"
	 "0x40,0xA0,0x00,0x00,0x00,0x00,           /* ^ 0x5E     */\n"
	 "0x00,0x00,0x00,0x00,0xE0,0x00,           /* _ 0x5F     */\n"
	 "0x80,0x40,0x00,0x00,0x00,0x00,           /* ` 0x60     */\n"
	 "0x00,0x60,0xA0,0xA0,0x60,0x00,           /* a 0x61     */\n"
	 "0x80,0xC0,0xA0,0xA0,0xC0,0x00,           /* b 0x62     */\n"
	 "0x00,0x60,0x80,0x80,0x60,0x00,           /* c 0x63     */\n"
	 "0x20,0x60,0xA0,0xA0,0x60,0x00,           /* d 0x64     */\n"
	 "0x00,0x60,0xA0,0xC0,0x60,0x00,           /* e 0x65     */\n"
	 "0x20,0x40,0xE0,0x40,0x40,0x00,           /* f 0x66     */\n"
	 "0x00,0x60,0xA0,0xA0,0x60,0xC0,           /* g 0x67     */\n"
	 "0x80,0xC0,0xA0,0xA0,0xA0,0x00,           /* h 0x68     */\n"
	 "0x40,0x00,0xC0,0x40,0xE0,0x00,           /* i 0x69     */\n"
	 "0x20,0x00,0x60,0x20,0x20,0xC0,           /* j 0x6a     */\n"
	 "0x80,0xA0,0xC0,0xA0,0xA0,0x00,           /* k 0x6b     */\n"
	 "0xC0,0x40,0x40,0x40,0xE0,0x00,           /* l 0x6c     */\n"
	 "0x00,0xA0,0xE0,0xA0,0xA0,0x00,           /* m 0x6d     */\n"
	 "0x00,0xC0,0xA0,0xA0,0xA0,0x00,           /* n 0x6e     */\n"
	 "0x00,0x40,0xA0,0xA0,0x40,0x00,           /* o 0x6f     */\n"
	 "0x00,0x60,0xA0,0xA0,0xC0,0x80,           /* p 0x70     */\n"
	 "0x00,0x60,0xA0,0xA0,0x60,0x20,           /* q 0x71     */\n"
	 "0x00,0xC0,0xA0,0x80,0x80,0x00,           /* r 0x72     */\n"
	 "0x00,0x20,0x40,0x20,0xC0,0x00,           /* s 0x73     */\n"
	 "0x40,0xE0,0x40,0x40,0x20,0x00,           /* t 0x74     */\n"
	 "0x00,0xA0,0xA0,0xA0,0x60,0x00,           /* u 0x75     */\n"
	 "0x00,0xA0,0xA0,0x40,0x40,0x00,           /* v 0x76     */\n"
	 "0x00,0xA0,0xA0,0xE0,0xA0,0x00,           /* w 0x77     */\n"
	 "0x00,0xA0,0x40,0x40,0xA0,0x00,           /* x 0x78     */\n"
	 "0x00,0xA0,0xA0,0xA0,0x60,0xC0,           /* y 0x79     */\n"
	 "0x00,0xE0,0x40,0x80,0xE0,0x00,           /* z 0x7A     */\n"
	 "0x60,0x40,0x80,0x80,0x40,0x60,           /* { 0x7B     */\n"
	 "0x80,0x80,0x80,0x80,0x80,0x80,           /* | 0x7C     */\n"
	 "0xC0,0x40,0x20,0x20,0x40,0xC0,           /* } 0x7D     */\n"
	 "0x60,0xC0,0x00,0x00,0x00,0x00,           /* ~ 0x7E     */\n"
	 "0x00,0xF0,0x90,0x90,0xF0,0x00,           /* BOX 0x7F   */\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,           /* SPACE 0xA0 */\n"
	 "0x00,0x10,0x20,0x40,0x80,0x00,           /* / 0xA1     */\n"
	 "0xE0,0x00,0xE0,0x00,0xE0,0x00,           /* EQUIV 0xA2 */\n"
	 "0x00,0x00,0x00,0x00,0x60,0xC0,           /* LOW TILDE 0xA3 */\n"
	 "0x20,0x70,0x80,0x70,0x20,0x00,           /* ASSIGN 0xA4 */\n"
	 "0x20,0xE0,0x40,0xE0,0x80,0x00,           /* nEqual 0xA5 */\n"
	 "0x40,0xE0,0x40,0x40,0x00,0x00,           /* up arrow 0xA6 */\n"
	 "0x00,0x20,0xF0,0x20,0x00,0x00,           /* rt arrow 0xA7 */\n"
	 "0x40,0x40,0xE0,0x40,0x00,0x00,           /* dn arrow 0xA8 */\n"
	 "0x00,0x40,0xF0,0x40,0x00,0x00,           /* lf arrow 0xA9 */\n"
	 "0x00,0xA0,0x40,0xA0,0x00,0x00,           /* multiply 0xAA */\n"
	 "0xF0,0x80,0x60,0x80,0xF0,0x00,           /* sigma    0xAB */\n"
	 "0x00,0x00,0x40,0xA0,0xE0,0x00,           /* delta    0xAC */\n"
	 "0x00,0x90,0x90,0x60,0x00,0x00,           /* union    0xAD */\n"
	 "0x00,0x60,0x90,0x90,0x00,0x00,           /* intrsect 0xAE */\n"
	 "0x40,0x00,0xE0,0x00,0x40,0x00,           /* divide   0xAF */\n"
	 "0x00,0x50,0xA0,0x70,0x00,0x00,           /* alpha    0xB0 */\n"
	 "0x20,0x50,0xA0,0x90,0xE0,0x80,           /* beta     0xB1 */\n"
	 "0x40,0x80,0x40,0xA0,0xE0,0x00,           /* delta    0xB2 */\n"
	 "0x80,0x40,0x20,0x50,0x90,0x00,           /* lambda   0xB3 */\n"
	 "0x00,0x00,0x50,0x50,0x60,0x80,           /* mu       0xB4 */\n"
	 "0x00,0x00,0xF0,0x60,0x60,0x00,           /* pi       0xB5 */\n"
	 "0x00,0x20,0x50,0x50,0xA0,0x80,           /* rho      0xB6 */\n"
	 "0x00,0x00,0xE0,0xA0,0x40,0x00,           /* sigma    0xB7 */\n"
	 "0x00,0x00,0x90,0xB0,0x60,0x00,           /* omega    0xB8 */\n"
	 "0x20,0x40,0x80,0x40,0x20,0x70,           /* lt ||=   0xB9 */\n"
	 "0x80,0x40,0x20,0x40,0x80,0xE0,           /* gt ||=   0xBA */\n"
	 "0x40,0xA0,0xE0,0xA0,0x40,0x00,           /* theta    0xBB */\n"
	 "0x30,0x60,0xA0,0x60,0x30,0x00,           /* l-embed  0xBC */\n"
	 "0x00,0x60,0x90,0x60,0x00,0x00,           /* degree   0xBD */\n"
	 "0xC0,0x60,0x50,0x60,0xC0,0x00,           /* r-embed  0xBE */\n"
	 "0xC0,0xA0,0x50,0x50,0xA0,0xC0,           /* arrow    0xBF */\n"
	 "0xF0,0x20,0x40,0x40,0x20,0xF0,           /* copyrig  0xC0 */\n"
	 "0xA0,0x00,0x00,0x00,0x00,0x00,           /* diaeresis 0xC1 */\n"
	 "0x00,0xE0,0xA0,0xE0,0x00,0x00,           /* box      0xC2 */\n"
	 "0x00,0x00,0x40,0x00,0x00,0x00,           /* interpunct 0xC3 */\n"
	 "0x00,0x00,0x60,0xF0,0x60,0x00,           /* diamond  0xC4 */\n"
	 "0x00,0x90,0x60,0x60,0x90,0x00,           /* multiply 0xC5 */\n"
	 "0x20,0x40,0x00,0x00,0x00,0x00,           /* acute ac 0xC6 */\n"
	 "0x00,0x00,0x00,0x10,0x20,0x00,           /* cedilla  0xC7 */\n"
	 "0xA0,0x40,0xA0,0x00,0x00,0x00,           /* hacek    0xC8 */\n"
	 "0x00,0x00,0x60,0xF0,0x60,0x00,           /* diamond  0xC9 */\n"
	 "0x40,0x40,0x40,0x40,0x40,0x40,           /* | 0xca */\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00,\n"
	 "0x00,0x00,0x00,0x00,0x00,0x00\n"
	 "};\n"
	 );
    }

    if (w == 5){

	printf("unsigned char font[]={\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,    /* SPACE 0x20 */\n"
  "0x40,0x40,0x40,0x00,0x40,0x00,    /* ! 0x21 */\n"
  "0x50,0x50,0x00,0x00,0x00,0x00,    /* \" 0x22 */\n"
  "0x50,0xF8,0x50,0xF8,0x50,0x00,    /* # 0x23 */\n"
  "0x40,0xF0,0xC0,0x30,0xF0,0x20,    /* $ 0x24 */\n"
  "0x90,0x20,0x20,0x40,0x90,0x00,    /* %% 0x25 */\n"
  "0x40,0xA0,0x40,0xB0,0x50,0x00,    /* & 0x26 */\n"
  "0x20,0x40,0x00,0x00,0x00,0x00,    /* ' 0x27 */\n"
  "0x20,0x40,0x40,0x40,0x20,0x00,    /* ( 0x28 */\n"
  "0x40,0x20,0x20,0x20,0x40,0x00,    /* ) 0x29 */\n"
  "0x90,0x60,0xF0,0x60,0x90,0x00,    /* * 0x2a */\n"
  "0x20,0x20,0xF8,0x20,0x20,0x00,    /* + 0x2b */\n"
  "0x00,0x00,0x40,0x40,0x80,0x00,    /* , 0x2c */\n"
  "0x00,0x00,0xF0,0x00,0x00,0x00,    /* - 0x2d */\n"
  "0x00,0x00,0x00,0x40,0x40,0x00,    /* . 0x2e */\n"
  "0x00,0x10,0x20,0x40,0x80,0x00,    /* / 0x2f */\n"
  "0x60,0x90,0x90,0x90,0x60,0x00,    /* 0 0x30 */\n"
  "0x20,0x60,0x20,0x20,0x70,0x00,    /* 1 0x31 */\n"
  "0x60,0x90,0x20,0x40,0xF0,0x00,    /* 2 0x32 */\n"
  "0xF0,0x10,0x20,0x10,0xE0,0x00,    /* 3 0x33 */\n"
  "0x20,0x60,0xA0,0xF0,0x20,0x00,    /* 4 0x34 */\n"
  "0xE0,0x80,0xE0,0x10,0xE0,0x00,    /* 5 0x35 */\n"
  "0x60,0x80,0xE0,0x90,0xE0,0x00,    /* 6 0x36 */\n"
  "0xF0,0x10,0x20,0x40,0x40,0x00,    /* 7 0x37 */\n"
  "0x60,0x90,0x60,0x90,0x60,0x00,    /* 8 0x38 */\n"
  "0x60,0x90,0x70,0x10,0xE0,0x00,    /* 9 0x39 */\n"
  "0x00,0x40,0x00,0x40,0x00,0x00,    /* : 0x3a */\n"
  "0x00,0x40,0x00,0x40,0x80,0x00,    /* ; 0x3b */\n"
  "0x20,0x40,0x80,0x40,0x20,0x00,    /* < 0x3c */\n"
  "0x00,0xF0,0x00,0xF0,0x00,0x00,    /* = 0x3d */\n"
  "0x40,0x20,0x10,0x20,0x40,0x00,    /* > 0x3e */\n"
  "0x60,0x90,0x20,0x00,0x40,0x00,    /* ? 0x3f */\n"
  "0x60,0x90,0xB0,0x80,0x60,0x00,    /* @ 0x40 */\n"
  "0x60,0x90,0xF0,0x90,0x90,0x00,    /* A 0x41 */\n"
  "0xE0,0x90,0xE0,0x90,0xE0,0x00,    /* B 0x42 */\n"
  "0x70,0x80,0x80,0x80,0x70,0x00,    /* C 0x43 */\n"
  "0xE0,0x90,0x90,0x90,0xE0,0x00,    /* D 0x44 */\n"
  "0xF0,0x80,0xE0,0x80,0xF0,0x00,    /* E 0x45 */\n"
  "0xF0,0x80,0xE0,0x80,0x80,0x00,    /* F 0x46 */\n"
  "0x60,0x80,0x90,0x90,0x70,0x00,    /* G 0x47 */\n"
  "0x90,0x90,0xF0,0x90,0x90,0x00,    /* H 0x48 */\n"
  "0xE0,0x40,0x40,0x40,0xE0,0x00,    /* I 0x49 */\n"
  "0x10,0x10,0x10,0x90,0x60,0x00,    /* J 0x4a */\n"
  "0x90,0xA0,0xC0,0xA0,0x90,0x00,    /* K 0x4b */\n"
  "0x80,0x80,0x80,0x80,0xF0,0x00,    /* L 0x4c */\n"
  "0x88,0xD8,0xA8,0x88,0x88,0x00,    /* M 0x4d */\n"
  "0x90,0xD0,0xB0,0x90,0x90,0x00,    /* N 0x4e */\n"
  "0xF0,0x90,0x90,0x90,0xF0,0x00,    /* O 0x4f */\n"
  "0xE0,0x90,0xE0,0x80,0x80,0x00,    /* P 0x50 */\n"
  "0xF0,0x90,0x90,0xB0,0xF8,0x00,    /* Q 0x51 */\n"
  "0xE0,0x90,0xE0,0xA0,0x90,0x00,    /* R 0x52 */\n"
  "0x70,0x80,0xE0,0x10,0xE0,0x00,    /* S 0x53 */\n"
  "0xF8,0x20,0x20,0x20,0x20,0x00,    /* T 0x54 */\n"
  "0x90,0x90,0x90,0x90,0xF0,0x00,    /* U 0x55 */\n"
  "0x90,0x90,0x90,0x60,0x60,0x00,    /* V 0x56 */\n"
  "0x88,0x88,0xA8,0xD8,0x88,0x00,    /* W 0x57 */\n"
  "0x90,0x90,0x60,0x90,0x90,0x00,    /* X 0x58 */\n"
  "0x90,0x90,0xE0,0x40,0x40,0x00,    /* Y 0x59 */\n"
  "0xF0,0x10,0x20,0x40,0xF0,0x00,    /* Z 0x5a */\n"
  "0x70,0x40,0x40,0x40,0x70,0x00,    /* [ 0x5b */\n"
  "0x00,0x80,0x40,0x20,0x10,0x00,    /* \\ 0x5c */ \n"
  "0x70,0x10,0x10,0x10,0x70,0x00,    /* ] 0x5d */\n"
  "0x20,0x50,0x00,0x00,0x00,0x00,    /* CIRCUMFLEX ^ 0x5e*/\n"
  "0x00,0x00,0x00,0x00,0x00,0xF0,    /* _ 0x5f */\n"
  "0x40,0x20,0x00,0x00,0x00,0x00,    /* GRAVE ACCENT ` 0x60 */\n"
  "0x00,0xC0,0x20,0xA0,0XF0,0x00,    /* a 0x61 */\n"
  "0x80,0xE0,0x90,0x90,0xE0,0x00,    /* b 0x62 */\n"
  "0x00,0x60,0x80,0x80,0x60,0x00,    /* c 0x63 */\n"
  "0x10,0x70,0x90,0x90,0x70,0x00,    /* d 0x64 */\n"
  "0x00,0x60,0xB0,0x80,0x60,0x00,    /* e 0x65 */\n"
  "0x60,0x80,0xE0,0x80,0x80,0x00,    /* f 0x66 */\n"
  "0x00,0x60,0x90,0xF0,0x10,0x60,    /* g 0x67 */\n"
  "0x80,0xE0,0x90,0x90,0x90,0x00,    /* h 0x68 */\n"
  "0x40,0x00,0x40,0x40,0x40,0x00,    /* i 0x69 */\n"
  "0x20,0x00,0x20,0x20,0x20,0xC0,    /* j 0x6a */\n"
  "0x80,0xA0,0xC0,0xA0,0x90,0x00,    /* k 0x6b */\n"
  "0x40,0x40,0x40,0x40,0x40,0x00,    /* l 0x6c */\n"
  "0x00,0x90,0xF0,0x90,0x90,0x00,    /* m 0x6d */\n"
  "0x00,0xE0,0x90,0x90,0x90,0x00,    /* n 0x6e */\n"
  "0x00,0x60,0x90,0x90,0x60,0x00,    /* o 0x6f */\n"
  "0x00,0xE0,0x90,0x90,0xE0,0x80,    /* p 0x70 */\n"
  "0x00,0x70,0x90,0x90,0x70,0x10,    /* q 0x71 */\n"
  "0x00,0x70,0x40,0x40,0x40,0x00,    /* r 0x72 */\n"
  "0x00,0x70,0xC0,0x30,0xE0,0x00,    /* s 0x73 */\n"
  "0x40,0xE0,0x40,0x40,0x60,0x00,    /* t 0x74 */\n"
  "0x00,0x90,0x90,0x90,0x70,0x00,    /* u 0x75 */\n"
  "0x00,0x90,0xA0,0xA0,0x40,0x00,    /* v 0x76 */\n"
  "0x00,0x90,0x90,0xF0,0x90,0x00,    /* w 0x77 */\n"
  "0x00,0x90,0x60,0x60,0x90,0x00,    /* x 0x78 */\n"
  "0x00,0x90,0x90,0x70,0x10,0x60,    /* y 0x79 */\n"
  "0x00,0xF0,0x20,0x40,0xF0,0x00,    /* z 0x7a */\n"
  "0x60,0x40,0x80,0x40,0x60,0x00,    /* { 0x7b */\n"
  "0x20,0x20,0x20,0x20,0x20,0x20,    /* | 0x7c */\n"
  "0xC0,0x40,0x20,0x40,0xC0,0x00,    /* } 0x7d */\n"
  "0x00,0x00,0x68,0xB0,0x00,0x00,    /* ~ 0x7e */\n"
  "0x00,0x70,0x50,0x70,0x00,0x00,    /* BOX 0x7f */\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,    /* SPACE 0xa0 */\n"
  "0x00,0x10,0x20,0x40,0x80,0x00,    /* / 0xa1 */\n"
  "0x00,0xF0,0x00,0xF0,0x00,0xF0,    /* EQUIVALENT 0xa2 */\n"
  "0x00,0x00,0x00,0x00,0x68,0xB0,    /* LOW TILDE 0xa3 */\n"
  "0x20,0x70,0x80,0x70,0x20,0x00,    /* ASSIGN 0xa4 */\n"
  "0x20,0xF0,0x20,0xF0,0x40,0x00,    /* NOT EQUAL 0xa5 */\n"
  "0x00,0x20,0x70,0x20,0x20,0x00,    /* UP ARROW 0xa6 */\n"
  "0x00,0x00,0x20,0xF0,0x20,0x00,    /* RIGHT ARROW 0xa7 */\n"
  "0x00,0x20,0x20,0x70,0x20,0x00,    /* DOWN ARROW 0xa8 */\n"
  "0x00,0x00,0x40,0xF0,0x40,0x00,    /* LEFT ARROW 0xa9 */\n"
  "0x00,0x50,0x20,0x50,0x00,0x00,    /* MULTIPLY 0xaa */\n"
  "0xF0,0x80,0x60,0x80,0xF0,0x00,    /* SIGMA 0xab */\n"
  "0x00,0x20,0x50,0x88,0xF8,0x00,    /* DELTA 0xac*/\n"
  "0x00,0x90,0x90,0x60,0x00,0x00,    /* UNION 0xad*/\n"
  "0x00,0x60,0x90,0x90,0x00,0x00,    /* INTERSECT 0xae*/\n"
  "0x00,0x60,0x00,0x60,0x00,0x00,    /* DIVIDE 0xaf */\n"
  "0x00,0x50,0xA0,0x70,0x00,0x00,    /* ALPHA 0xb0 */\n"
  "0x20,0x50,0xA0,0x90,0xE0,0x80,    /* BETA 0xb1 */\n"
  "0x20,0x40,0x20,0x50,0x70,0x00,    /* DELTA 0xb2 */\n"
  "0x80,0x40,0x20,0x50,0x90,0x00,    /* LAMBDA 0xb3 */\n"
  "0x00,0x00,0x50,0x50,0x60,0x80,    /* MU 0xb4 */\n"
  "0x08,0x78,0xD0,0x50,0x50,0x00,    /* PI 0xb5 */\n"
  "0x00,0x20,0x50,0x50,0xA0,0x80,    /* RHO 0xb6 */\n"
  "0x30,0x40,0xA0,0xA0,0x40,0x00,    /* SIGMA 0xb7 */\n"
  "0x00,0x00,0x90,0xB0,0x60,0x00,    /* OMEGA 0xb8 */\n"
  "0x20,0x40,0x80,0x40,0x20,0xE0,    /* LESS THAN OR EQUAL 0xb9 */\n"
  "0x80,0x40,0x20,0x40,0x80,0xE0,    /* GREATER THAN OR EQUAL 0xba */\n"
  "0x20,0x50,0x70,0x50,0x20,0x00,    /* THETA 0xbb */\n"
  "0x30,0x60,0xA0,0x60,0x30,0x00,    /* l-embed 0xbc */\n"
  "0x00,0x60,0x90,0x60,0x00,0x00,    /* DEGREE 0xbd */\n"
  "0xC0,0x60,0x50,0x60,0xC0,0x00,    /* r-embed 0xbe */\n"
  "0xC0,0xA0,0x50,0x50,0xA0,0xC0,    /* ANSWER ARROW 0xbf */\n"
  "0xF8,0x20,0x40,0x40,0x20,0xF8,    /* COPYRIGHT 0xc0 */\n"
  "0x50,0x00,0x00,0x00,0x00,0x00,    /* DIERESIS 0xc1 */\n"
  "0x00,0x70,0x50,0x70,0x00,0x00,    /* BOX 0xc2 */\n"
  "0x00,0x00,0x20,0x00,0x00,0x00,    /* INTERPUNCT 0xc3 */\n"
  "0x20,0x70,0xF8,0x70,0x20,0x00,    /* DIAMOND 0xc4 */\n"
  "0x00,0x50,0x20,0x50,0x00,0x00,    /* MULTIPLY 0xc5 */\n"
  "0x10,0x20,0x00,0x00,0x00,0x00,    /* ACUTE ACCENT 0xc6 */\n"
  "0x00,0x00,0x00,0x08,0x10,0x00,    /* CEDILLA 0xc7 */\n"
  "0x10,0x20,0x00,0x00,0x00,0x00,    /* HACEK 0xc8 */\n"
  "0x20,0x70,0xF8,0x70,0x20,0x00,    /* DIAMOND 0xc9 */\n"
  "0x20,0x20,0x20,0x20,0x20,0x20,    /* | 0xca */\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00,\n"
  "0x00,0x00,0x00,0x00,0x00,0x00\n"
    "};\n\n");
    }

    printf("unsigned int fontptr[]={\n");
    for (i=0;i<160;i++) {
	printf("%d,",i*6);
	if (i % 8 == 7 )
	    printf("\n");
    }
    printf("};\n\n"
	   "#endif /* FONT_H */\n");

    return 0;
}

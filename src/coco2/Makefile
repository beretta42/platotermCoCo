CROSS_AS=m6809-unknown-as
CROSS_LD=m6809-unknown-ld
CROSS_CC=m6809-unknown-gcc

all: ../tools/mk_scale plato.bin

../tools/mk_scale:
	$(MAKE) -C ../tools

scale.c: ../tools/mk_scale
	../tools/mk_scale 256 192 > scale.c

plato.bin: scale.c coco.c
	$(CROSS_CC) -nostdlib -I.. -Ifakelib -o plato.bin \
	../config_base.c \
	../io_base.c \
	../keyboard_base.c \
	../plato.c \
	../prefs_base.c \
	../protocol.c \
	../screen_base.c \
	../terminal.c \
	../touch_base.c \
	coco.c \
	scale.c

plato.dsk: plato.bin
	rm -f plato.dsk
	decb dskini plato.dsk
	decb copy -b -2 plato.bin plato.dsk,PLATO.BIN
	decb copy -a -0 AUTOEXEC.BAS plato.dsk,AUTOEXEC.BAS


clean:
	rm -f scale.c plato.bin plato.dsk *~

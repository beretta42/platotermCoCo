CROSS_AS=m6809-unknown-as
CROSS_LD=m6809-unknown-ld
CROSS_CC=m6809-unknown-gcc
CROSS_CC_OPT=-I.. -Ifakelib -fomit-frame-pointer

all: plato.dsk

../tools/mk_scale:
	$(MAKE) -C ../tools

scale.c: ../tools/mk_scale
	../tools/mk_scale 256 192 > scale.c

crt0.o: crt0.s
	$(CROSS_AS) -o crt0.o crt0.s

plato.bin: scale.c coco.c crt0.o
	$(CROSS_CC) $(CROSS_CC_OPT) -c ../config_base.c
	$(CROSS_CC) $(CROSS_CC_OPT) -c	../io_base.c 
	$(CROSS_CC) $(CROSS_CC_OPT) -c	../keyboard_base.c
	$(CROSS_CC) $(CROSS_CC_OPT) -c	../plato.c
	$(CROSS_CC) $(CROSS_CC_OPT) -c	../prefs_base.c
	$(CROSS_CC) $(CROSS_CC_OPT) -c	../protocol.c
	$(CROSS_CC) $(CROSS_CC_OPT) -c	../screen_base.c
	$(CROSS_CC) $(CROSS_CC_OPT) -c	../terminal.c
	$(CROSS_CC) $(CROSS_CC_OPT) -c	../touch_base.c
	$(CROSS_CC) $(CROSS_CC_OPT) -c	coco.c
	$(CROSS_CC) $(CROSS_CC_OPT) -c	scale.c
	$(CROSS_LD) -Map=plato.map --oformat=decb --script=basic.link \
	-o plato.bin crt0.o config_base.o io_base.o \
	keyboard_base.o plato.o prefs_base.o protocol.o screen_base.o \
	terminal.o touch_base.o coco.o scale.o

plato.dsk: plato.bin
	rm -f plato.dsk
	decb dskini plato.dsk
	decb copy -b -2 plato.bin plato.dsk,PLATO.BIN
	decb copy -a -0 -l AUTOEXEC.BAS plato.dsk,AUTOEXEC.BAS


clean:
	rm -f scale.c plato.bin plato.dsk *~ *.o *.map
	cd ../tools; rm -f mk_ascii_key_h mk_font mk_scale

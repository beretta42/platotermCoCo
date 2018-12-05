CROSS_AS=m6809-unknown-as
CROSS_LD=m6809-unknown-ld
CROSS_CC=m6809-unknown-gcc
CROSS_CC_OPT=-I.. -Ifakelib -fomit-frame-pointer -Os

all: plato.dsk

../tools/mk_scale:
	$(MAKE) -C ../tools

../tools/mk_font:
	$(MAKE) -C ../tools

scale.c: ../tools/mk_scale
	../tools/mk_scale 256 192 > scale.c

font.c: ../tools/mk_font
	../tools/mk_font > font.c

crt0.o: crt0.s
	$(CROSS_AS) -o crt0.o crt0.s

splash.dat: splash.hex
	xxd -r -p splash.hex splash.dat

dwbkr.mod: dwmod.s sermod.link
	$(CROSS_AS) --defsym BECKER=1 -o dwbkr.o dwmod.s
	$(CROSS_LD) -Map=dwbkr.map --oformat=decb \
	--script=sermod.link -o dwbkr.mod dwbkr.o

dwbit.mod: dwmod.s sermod.link
	$(CROSS_AS) --defsym BECKER=0 -o dwbit.o dwmod.s
	$(CROSS_LD) -Map=dwbit.map --oformat=decb \
	--script=sermod.link -o dwbit.mod dwbit.o

plato.bin: font.c scale.c coco.c crt0.o
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
	$(CROSS_CC) $(CROSS_CC_OPT) -c	font.c
	$(CROSS_CC) $(CROSS_CC_OPT) -c  keyboard.c
	$(CROSS_CC) $(CROSS_CC_OPT) -c  joy.c
	$(CROSS_AS) -o serial.o serial.s
	$(CROSS_AS) -o tgi.o tgi.s
	$(CROSS_AS) -o file.o file.s
	$(CROSS_AS) -o joyll.o joyll.s
	$(CROSS_LD) -Map=plato.map --oformat=decb --script=basic.link \
	-o plato.bin crt0.o config_base.o io_base.o \
	keyboard_base.o plato.o prefs_base.o protocol.o screen_base.o \
	terminal.o touch_base.o coco.o scale.o font.o keyboard.o \
	serial.o tgi.o file.o joy.o joyll.o

plato.dsk: plato.bin dwbkr.mod dwbit.mod splash.dat
	rm -f plato.dsk
	decb dskini plato.dsk
	decb copy -b -2 plato.bin plato.dsk,PLATO.BIN
	decb copy -a -0 -l AUTOEXEC.BAS plato.dsk,AUTOEXEC.BAS
	decb copy -b -3 dwbkr.mod plato.dsk,DWBKR
	decb copy -b -3 dwbit.mod plato.dsk,DWBIT
	decb copy -b -3 splash.dat plato.dsk,SPLASH

clean:
	rm -f font.c scale.c plato.bin plato.dsk *~ *.o *.map *.mod \
	splash.dat
	cd ../tools; rm -f mk_ascii_key_h mk_font mk_scale

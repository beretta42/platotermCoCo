# PlatoTerm:


### requirements:

* 32K RAM.
* coco2, coco3, Vcc, or Mame.
* A DriveWire4 bitbanger/becker connection to a server with internet access.
* Probably need a Monochrome TV/Monitor.
* a normal coco joystick/mouse on the Right joystick port.
* Disk Extended Color Basic, version 1.1 (or HDBDOS,YADOS,SDCDOS)

### To run:

> LOADM"PLATO.BIN
> EXEC

Usage:

On start, platoterm will automagically dial Thom Cherryhomes'
IRATA.online, which, for reference, is "tcp connect irata.online 8005"
in Drivewire, or "ATDirata.online:8005", in Hayesish.

Special plato keys:

CLEAR-B    BACK
CLEAR-L    LAB
CLEAR-E    EDIT
CLEAR-S	   STOP
CLEAR-D	   DATA
ENTER      NEXT
BREAK      enter the platoterm control menu

All these can be shifted with "SHIFT".

The control menu doesn't do much at this minute, and will be replaced
by a nicer, slightly more modern gui.  But for now, the only real
changeable option is the Interface.  You get (for now) two choices:
1. Drivewire via Becker port, or 2. Drivewire via bitbanger.  option 1
is for emulation, and option two works for the bitbanger.  Normal UART
(drivewire and non) will be forthcoming.  All the rest of the BREAK
menus is boloney left over from the 6502 ports this code is based on.

### possible driver additions:

* CoCo3 320x200 (1bpp or 4bpp) mode.
* 640x480 8bpp (CoCoFPGA)
* A 6551 Uart driver
* A Drivewire *over* a 6551 (slow?)
* other mice: rs232 mice, high-res, other?

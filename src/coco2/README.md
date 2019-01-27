# PlatoTerm:


### requirements:

* 32K RAM.
* coco2, coco3, Vcc, or Mame.
* A DriveWire4 bitbanger/becker connection to a server with internet access,
  or a standard Radio Shack RS232 Pack (untested)
* Probably need a Monochrome TV/Monitor.
* a normal coco joystick/mouse on the Right joystick port.
* Disk Extended Color Basic, version 1.1 (or HDBDOS,YADOS,SDCDOS)

### To run:

> LOADM"PLATO.BIN
> EXEC

Usage:

On first start, platoterm will bring you to the configuration menu to
allow you to set the serial interface.  On subsequent boots, platoterm
will remember this setting.  On boot, platoterm will be in a
more-or-less "regular" terminal mode, allowing the you to type any
connection strings that your host connection software requires.  If
using DriveWire4 this string should be something like this:

"tcp connect irata.online 8005"

And in tcpser IP modem emulator:

"ATDirata.online:8005"


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
changeable option is the Interface.  You get (for now) three choices:
1. Drivewire via Becker port, 2. Drivewire via bitbanger, or 3 a Radio
Shack RS232 pack.  option 1 is for emulation, and option two works for
the bitbanger.  Option 3, the RS232 pack is in testing, and will
connect at 9600 baud.  Using an RS232 pack will require 'tcpser' or in
linux, a clever 'ncat' invocation.

### Future possible driver additions:

* CoCo3 320x200 (1bpp or 4bpp) mode.
* 640x480 8bpp (CoCoFPGA)
* A Drivewire *over* a 6551 (slow?)
* other mice: rs232 mice, high-res, other?

#### references:

* https://www.irata.online/
* https://github.com/n6il/pyDriveWire
* https://sites.google.com/site/drivewire4/
* https://github.com/FozzTexx/tcpser
* http://www.jbrain.com/pub/linux/serial/

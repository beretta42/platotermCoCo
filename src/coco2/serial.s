;;; 
;;;
;;;  call revector to module's routines
;;;
;;;
	.area	.text

	export	_ser_put
	export  _ser_put_clean
	export  _ser_open
	export  _ser_get
	export  _ser_close
	export  _ser_init
	export  poll

BASE	equ	$5C00
	
;;; Open Vport
_ser_open
	jmp	[BASE+2]
;;; Close Vport
_ser_close
	jmp	[BASE+4]
;;; Send a byte to vport
;;;    char ser_put(char c);
_ser_put
	jmp	[BASE+6]
	
_ser_put_clean
	jmp	[BASE+8]
;;; Gets a byte from vport
;;;   char ser_get(char *c);
_ser_get
	jmp	[BASE+10]
;;; poll the drivewire server for bytes
;;;   This routine should be called under the interrupt thread.
;;;   no polling will happen while the main thread (via ser_get)
;;;   is processing the recent received data; this simplifies the
;;;   queue processing.
;;; 
poll
	jmp	[BASE+12]

_ser_init
	jmp	[BASE+14]

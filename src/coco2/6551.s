;;; 
;;;
;;;  6551 ACIA Serial Module
;;;
;;;
;;;   todo:
;;;      hardware base address from config struct
;;; 	 baud rate from config struct
;;;      hardware flow control
;;;	 MPI irq routing

	export	_ser_init
	
	.area	.header

	.dw	$4224		; 0 magic
	.dw	_ser_open	; 2
	.dw	_ser_close	; 4
	.dw	_ser_put	; 6
	.dw	_ser_get	; 8
	.dw	poll		; 10
	.dw	_ser_init	; 12

DCMODEM equ	$ff6c		; direct-connect modem base address
DEFBASE	equ	$ff68		; RS232 pak base address
DATA	equ	0		; data register offset from base
STAT	equ	1		; status reg
CMD	equ	2		; command reg
CNTL	equ	3		; control reg
XOFF	equ	19		; software flow control off
XON	equ	17		; software flow control on
	
PIA	equ	$ff23		; bit 7 is flag, bit 0 is enable
	
	.area	.text

base	.dw	DEFBASE		; our uart port
vect	rmb	2		; BASIC's firq vector
;;; firq input ring buffer
len	.db	0		; size of buffer
buf	rmb	256		; buffer
iin	.db	0		; input index into
oin	.db	0		; output index
flow	.db	0		; flow control state: 0 is on, 1 is off
	
;;; initialize the system
_ser_init
	rts
	
;;; Open Vport
;;; unsigned char ser_open (const struct ser_params* params)
_ser_open
	ldx	$110		; get old firq vector
	stx	vect		; store it
	ldx	#interrupt	; install new firq vector
	stx	$110
	lda	$ff23		; turn on pia firq
	ora	#1
	sta	$ff23
	ldx	base		; configure UART
	lda	#$1c		; 1 stop, 8 bits, 4800 baud
	sta	CNTL,x
	lda	#$09		; no parity, no echo, no tx int, rx int, enable
	sta	CMD,x
	clrb
	rts

;;; Close Vport
;;; unsigned char ser_close (void);
_ser_close
	clrb
	rts


;;; Send a byte to vport
;;;    char ser_put(char c);
_ser_put
	ldx	base
	;; wait till empty
a@	lda	STAT,x
	anda	#$10
	beq	a@
	;; store char in data for xmit
	stb	DATA,x
	clrb
	rts


;;; Gets a byte from vport
;;;   char ser_get(char *c);
_ser_get
	tst	len		; bytes waiting?
	bne	a@		; yup
	ldb	#1		; nope return empty
	rts
a@	pshs	x
	ldx	#buf
	ldb	oin
	abx
	lda	,x
	incb
	stb	oin
	dec	len
	sta	[,s++]
	clrb
	rts


	
;;; This is called from vsync (60 hz).
;;;   This routine checks to see if flow control
;;;   needs toggling
poll	ldb	len
	tst	flow		; test of flow state
	bne	off@		; if off await to turn on
	;; else await full-ish and send an xoff
	cmpb	#48		; 3/4 full?
	blo	out@
	inc	flow
	jmp	flow_off
out@	rts
	;; await empty-ish and send an xon
off@	cmpb	#32
	bhi	out@
	clr	flow
	jmp	flow_on


flow_off
	ldb	#XOFF
	bsr	_ser_put
	rts

flow_on
	ldb	#XON
	bsr	_ser_put
	rts


;;; This is a firq handler
;;;   simple as it gets to keep it small/fast
interrupt
	pshs	d,x
	ldb	PIA-1		; clear pia interrupt
	ldx	base
	ldb	STAT,x		; get and save status
	andb	#$08		; read interrupt?
	beq	out@
	lda	DATA,x		; read a byte from uart put in fast buffer
	ldb	iin		; get input index
	ldx	#buf		; calculate position
	abx
	sta	,x
	incb
	stb	iin
	inc	len
out@	puls	d,x
	rti			; return


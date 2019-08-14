;;;
;;;
;;;  6551 ACIA Serial Module
;;;	the 6551 ties the older meaning to RTS.  Now-a-days, since c.1985,
;;;     The RTS means to the other side "I can receive data".  The older
;;;	meaning of "I'm ready to send you data", is how the 6551 takes it, and
;;;     shuts down the xmitter circuit.
;;;   todo:
;;;      take settings from config settings
;;;	 MPI firq routing (how to coord with disk, if needed?)
;;;      check for race conditions! (AKA think harder)
;;;	 document needed cable wiring (CD must stay low to xmit)
;;;	 do we need TX flow control? (prob not)
;;;      soft flow control
;;;      no flow control (we'll need changable baud rate for sure)

	export	_ser_init

DCMODEM equ	$ff6c		; direct-connect modem base address
DEFBASE	equ	$ff68		; RS232 pak base address
DATA	equ	0		; data register offset from base
STAT	equ	1		; status reg
CMD	equ	2		; command reg
CNTL	equ	3		; control reg
XOFF	equ	19		; software flow control off
XON	equ	17		; software flow control on
PIA     equ	$ff22
PIAC	equ	$ff23		; bit 7 is flag, bit 0 is enable
MPI	equ	$ff7f		; mpi bank register
FVECT	equ	$110		; firq vector

	.area	.header

	.dw	$4224		; 0 magic
	.dw	_ser_open	; 2
	.dw	_ser_close	; 4
	.dw	_ser_put	; 6
	.dw	_ser_get	; 8
	.dw	poll		; 10
	.dw	_ser_init	; 12

	.area	.bss

rbuf	rmb	256
tbuf	rmb	256
vect	rmb	2		; BASIC's firq vector

rin:	rmb	1		; input buffer index
rout:	rmb	1		; output buffer index
rlen:	rmb	1		; length of recv buffer
rflow:	rmb	1		; receive handshake flag

	;; flowb is for software flow control, where we need to
	;; tell the xmiter thread to forget the user buffer and
	;; insert a XOFF/XON into the outgoing data stream.
flowb:	rmb	1		; flag / which char to insert into xmit buffer

tin:	rmb	1		; input buffer index
tout:	rmb	1		; output buffer index
tlen:	rmb	1		; length of xmit buffer

	.area	.data
base	.dw	DEFBASE		; our uart port


	.area	.text

cntl_lu	.db	$16		; 300
	.db	$17		; 600
	.db	$18		; 1200
	.db	$1a		; 2400
	.db	$1c		; 4800
	.db	$1e		; 9600
	.db	$1f		; 19200
	.db	$00		; 115200

mpi_lu	.db	$0
	.db	$11
	.db	$22
	.db	$33

;;; initialize the system
_ser_init:
	rts

;;; Open Vport
;;; unsigned char ser_open (ConfigInfo *params)
_ser_open:
	pshs	cc,x
	orcc	#$50		; off interrupts
	ldx	#.bss_base	; clear out BSS
b@	clr	,x+		;
	cmpx	#.bss_base+.bss_len ;
	bne	b@
	ldx	1,s	        ; get mpi slot from config
	lda	3,x		;
	ldx	#mpi_lu		; lookup in table
	lda	a,x		;
	sta	MPI		; set it
	ldx	FVECT		; save basic's firq vector
	stx	vect		;
	ldx	#fint		; set our firq handler
	stx	FVECT		;
	lda	PIAC		; turn on pia interrupts on trans to low
	anda	#~$2		;
	ora	#$1		;
	sta	PIAC		;
	ldx	base		; reset 6551
	sta	STAT,x		;
	ldx	1,s		; get baud from config
	lda	2,x		;
	ldx	#cntl_lu	; convert to uart
	lda	a,x		;
	ldx	base		;
	sta	CNTL,x		; set control reg
	bsr	xmit_on		; set CMD status
	ldx	base
a@	lda	DATA,x		; keep reading data, and status
	lda	STAT,x		; until spurious interrupts disappear
	bmi	a@		;
	clrb
	puls	cc,x,pc		; restore ints, return


;;; Close Vport
;;; unsigned char ser_close (void);
_ser_close:
	pshs	cc		; turn off interrupts
	orcc	#$50		;
	lda	#$33		; set MPI to slot 4
	sta	MPI		;
	ldx	vect		; reset BASIC's firq vector
	stx	FVECT		;
	lda	PIAC		; turn off PIA interrupts
	anda	#~$1		;
	sta	PIAC		;
	ldx	base		; turn off 6551
	clr	CNTL,x		;
	clr	CMD,x		;
	puls	cc,pc		; return

	;; This turns tx on, and also rx
xmit_on:
	tst	rflow		; 6551 can't do xmit if RTS is not asserted
	bne	out@		; dont try, or we'll ruin rx flow control
	ldx	base
	lda	#5
	sta	CMD,x
out@	rts

	;; This turns off tx off, rx off
xmit_off:
	ldx	base
	lda	#1		; just DTR
	sta	CMD,x
	rts

;;; Send a byte to vport
;;;    void ser_put(char c);
_ser_put:
	tfr	b,a
	ldb	tin		; store byte into xmit buffer
	ldx	#tbuf		;
	abx			;
	sta	,x		;
	inc	tlen		; inc buffer lenght
	inc	tin		; inc input position
	bra	xmit_on		; tail call, then turn on transmitter


;;; Gets a byte from vport
;;;   char ser_get(char *c);
_ser_get:
	pshs	x		; save buffer addr
	tst	rlen		; does RX buffer have a lenght?
	bne	a@		; yup then get a byte
	ldb	#1		; return nope - no bytes
	puls	x,pc
a@	ldb	rout		; get next byte from buffer
	ldx	#rbuf		;
	abx			;
	ldb	,x		;
	stb	[,s++]		; store to buffer
	dec	rlen		; adjust receive length
	inc	rout		; inc output position
	clrb			; return no error - there's a byte
	rts


;;; This is called from vsync (60 hz).
;;;   This routine checks to see if flow control
;;;   needs toggling
poll:   inc	$6001		; fixme: debug spinner
	lda	rlen		; both paths need the length
	ldb	rflow		; rx flow control on?
	beq	isoff@		; rec is on see if we should turn off
	cmpa	#40		; more than 40 charactors in buff?
	bhi	ret@		; yup return
	dec	rflow		; nope - turn on flow
	clr	$6000		; fixme: debug indicator
	bra	xmit_on		; tail call
isoff@	cmpa	#222		; more than 150 charactor in buff?
	blo	ret@		; nope return
	inc	rflow		; mark that we're in a tx flow control state
	ldb	#$ff		; fixme: debug indicator here
	stb	$6000		;
	jmp	xmit_off	; tail call - turn off rx flow control
ret@	rts



;;; This is a firq handler
;;;   simple as it gets to keep it small/fast
fint:	pshs	x,d		; save regs we actually use (the fast part)
	inc	$6003		; fixme: debug indicator
	lda	PIAC		; get PIA status
	bpl	ret@		; skip this sillyness if not a CART interrupt
	lda	PIA		; clear pia interrupt
	ldx	base		; read uart status
	lda	STAT,x		;
*	bpl	ret@		; not us? / broken int flag on some 6551's ?
	;; fixme: save rx errors, DCD and DSR status here?
	bita	#7		; any rx errors?
	beq	b@		; no continue
	ldb	#$55		; yes: fixme: what to do on rx errors?
	stb	$6000		; fixme: debug indicator
	lda	DATA,x		; read the data to clear the errors
b@	bita	#$8		; is recv ?
	bne	rec@
a@	bita	#$10		; is xmit ?
	bne	xmit@
ret@	puls	x,d		; restore regs used
	rti			; return
	;; receive
rec@    pshs	a		; save status byte
	ldx	base		; get data byte from uart
	lda	DATA,x		;
	ldb	rin		; store in rx buffer
	ldx	#rbuf		;
	abx			;
	sta	,x		;
	inc	rlen		; increment buffer's length
	;; fixme: test for buffer overflow here
	inc	rin		; increment input position
	puls	a		; restore status byte
	bra	a@		; go look for xmit empty and do
	;; if transmit
xmit@	ldb	rflow		; is rx flow on?
	bne	ret@		; yes then do nothing
	tst	tlen		; no check xmit buffer length
	bne	c@		; if not empty then send
	ldx	base		; is empty so turn off xmit interrupts
	ldb	#9		; tx int off, DTR on
	stb	CMD,x		;
	bra	ret@		;
c@	ldb	tout		; get next byte from xmit buffer
	ldx	#tbuf		;
	abx			;
	ldb	,x		;
	ldx	base		; store to UART data reg
	stb	DATA,x		;
	dec	tlen		; dec buffer length
	inc	tout		; inc xmit output position
	bra	ret@		; return

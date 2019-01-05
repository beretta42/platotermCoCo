;;; 
;;;
;;;  6551 ACIA Serial Module
;;;
;;;
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
	
	
PIA	equ	$ff23		; bit 7 is flag, bit 0 is enable
	
	.area	.text

base	.dw	DEFBASE		; our uart port
vect	rmb	2		; BASIC's firq vector
stat	.db	0		; mirror of uart status
;;; firq input buffer
len	.db	0		; size of buffer
buf	rmb	256		; buffer
ptr	.dw	buf		; input ptr for firq handler
;;; irq input buffer
len2	.db 	0	
buf2	rmb	256		; secondary buffer
ptr2	.dw	buf2
	
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
	lda	#$1e		; 1 stop, 8 bits, 9600 baud
	sta	CNTL,x
	lda	#$05		; no parity, no echo, tx int, rx int, enable
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
a@	lda	stat
	anda	#$10
	beq	a@
	;; store char in data for xmit
	stb	DATA,x
	clrb
	rts


;;; Gets a byte from vport
;;;   char ser_get(char *c);
_ser_get
	tst	len2		; bytes waiting?
	bne	a@		; yup
	ldb	#1		; nope return empty
	rts
a@	pshs	u
	ldu	ptr2		; get byte from buffer
	ldb	,u+
	stu	ptr2
	stb	,x		; store in *c param
	dec	len2
	bne	b@
	;; reset buffer
	ldx	#buf2
	stx	ptr2
b@	clrb
	puls	u,pc


	
;;; This is called from vsync to
;;;  copy bytes out of the firq buffer and into a bigger irq buffer
poll
	pshs	cc
	orcc	#$50
	lda	len2		; still processing?
	bne	out@		; yup
	lda	len		; bytes waiting in firq buffer?
	beq	out@		; no
	;; copy to 2nd buffer
	ldx	#buf
	ldu	#buf2
	sta	len2
a@	ldb	,x+
	stb	,u+
	deca
	bne	a@
	;; reset firq buffer
	ldx	#buf
	stx	ptr
	clr	len
out@	puls	cc,pc		; restore, return



;;; This is a firq handler
interrupt
	pshs	b,x
	inc	$6000		; fixme: for debugging
	ldb	PIA-1		; clear pia interrupt
	ldx	base
	ldb	STAT,x		; get and save status
	stb	stat
	andb	#$08		; read interrupt?
	beq	out@
	ldb	DATA,x		; read a byte from uart put in fast buffer
	ldx	ptr
	stb	,x+
	stx	ptr
	inc	len
out@	puls	b,x
	rti			; return


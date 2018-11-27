;;; 
;;;
;;;  Drivewire Serial Module
;;;
;;;
	export	_ser_init
	
	.area	.header

	.dw	$4224		; 0 magic
	.dw	_ser_open	; 2
	.dw	_ser_close	; 4
	.dw	_ser_put	; 6
	.dw	_ser_put_clean	; 8
	.dw	_ser_get	; a
	.dw	poll		; c
	.dw	_ser_init	; e
	
	.area	.text


;;; input buffer
len	.db	0		; size of buffer
buf	rmb	256		; buffer break apart
timer	.db	0		; dw poll timer
reset	.db	16		; reset timer


;;; initialize the system
_ser_init
	rts
	
;;; Open Vport
_ser_open
	pshs	y
	ldx	#0x0129
	ldb	#0xc4
	pshs	b,x
	tfr	s,x
	ldy	#3
	jsr	DWWrite
	leas	3,s
	clrb
	puls	y,pc

;;; Close Vport
_ser_close
	pshs	y
	ldx	#0x012a
	ldb	#0xc4
	pshs	b,x
	tfr	s,x
	ldy	#3
	jsr	DWWrite
	leas	3,s
	puls	y,pc


;;; Send a byte to vport
;;;    char ser_put(char c);
_ser_put
	;; make 7E1 parity :) fixme: something better?
	pshs	b
	;; bit 0
	clra
	lsrb
	adca	#0
	;; bit 1
	lsrb
	adca	#0
	;; bit 2
	lsrb
	adca	#0
	;; bit 3
	lsrb
	adca	#0
	;; bit 4
	lsrb
	adca	#0
	;; bit 5 
	lsrb
	adca	#0
	;; bit 6
	lsrb
	adca	#0
	;; done
	rora
	rorb
	orb	,s+
_ser_put_clean
	pshs	y		; save regs
	;; send to DW
	lda	#1		; channel no
	tfr	d,x		;
	ldb	#$c3		; write channel opcode
	pshs	b,x		; push it onto stack
	leax	,s		; load up buffer param
	ldy	#3		; load up no of bytes
	jsr	DWWrite		; write it
	leas	3,s		; remove packet off stack
	clrb			; clear return?
	puls	y,pc		; restore, return


;;; Gets a byte from vport
;;;   char ser_get(char *c);
_ser_get
	tst	len		; bytes waiting?
	bne	a@		; yup
	ldb	#1		; nope return empty
	rts
a@	pshs	u
	ldu	@ptr		; get byte from buffer
	ldb	,u+
	stu	@ptr
	stb	,x		; store in *c param
	dec	len
	bne	b@
	;; reset buffer
	ldx	#buf
	stx	@ptr
b@	clrb
	puls	u,pc
@ptr	.dw	buf

	
;;; poll the drivewire server for bytes
;;;   This routine should be called under the interrupt thread.
;;;   no polling will happen while the main thread (via ser_get)
;;;   is processing the recent received data; this simplifies the
;;;   queue processing.
;;; 
poll
	;; dont poll if there's unprocessed data in buffer
	tst	len
	beq	a@
	rts
	;; dont poll if its not time
a@	dec	timer
	beq	b@
	rts
b@	ldb	reset
	stb	timer
	;; send poll op
	lda	#$43
	pshs	d
	leax	,s
	ldy	#1
	jsr	DWWrite
	;; get response
	ldy	#2
	leax	,s
	jsr	DWRead
	;; translate reponse
	ldb	,s		; check response byte 1
	beq	back@		;
	lda	#1		; one datum kicks polling to high
	sta	reset
	sta	timer
	cmpb	#18		; is multi byte for port 1?
	beq	multi@
	cmpb	#16		; closed?
	beq	closed@
	;; single byte
	lda	1,s		; else is a single byte
	sta	buf
	inc	len
	puls	x,pc		; fix stack return
	;; back off poll timer
back@	lda	reset
	cmpa	#16
	bhs	c@
	lsl	reset
c@	puls	x,pc
	;; get multiple bytes from server
multi@	ldb	1,s		; number of bytes
	stb	len		; save in len
	stb	smc2@+2		; save in read request
	stb	smc@+3		; save in buffer size
	ldb	#$63
smc2@	ldx	#$0100
	pshs	b,x		; push ser read
	leax	,s
	ldy	#3
	jsr	DWWrite
	ldx	#buf
smc@	ldy	#0
	jsr	DWRead
	leas	5,s
	rts
closed@	jsr	_ser_close
	jsr	_ser_open
	puls	x,pc

; Hardcode these for now so that we can use below files unmodified
H6309    equ 0
*BECKER   equ 1
BECKERTO equ 0
ARDUINO  equ 0
JMCPBCK  equ 0
BAUD38400 equ 0
SY6551N equ 0

NOINTMASK equ 0
IntMasks equ $50

; These files are copied almost as-is from HDB-DOS
        *PRAGMA nonewsource
         include "dw.def"
         include "dwread.s"
         include "dwwrite.s"

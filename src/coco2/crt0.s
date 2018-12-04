	export start
	export _di
	export _ei
	export _timer
	export _ashrhi3
	export _enable_poll
	import _main
	import _kpoll
	import poll

	section	.text

_timer	.dw	0
_enable_poll
	.db	0

start:	orcc	#$50		; off interrupts
	ldx	#noop
	stx	$5c0c
	ldx	#interrupt	; set interrupt vector
	stx	$10d		;
	lds	#$8000		; set stack
	ldx	#.bss_base	; clear bss
	ldy	#.bss_len	;
a@	clr	,x+		;
	leay	-1,y		;
	bne	a@		;
	andcc	#~$50		; on interrupts
	jmp	do_joy
	jmp	_main		; jump to C's main

noop	rts
	
interrupt:
	ldd	_timer
	addd	#1
	std	_timer
        lda     $ff02           ; clear vsync pia
        jsr     _kpoll          ; go poll keyboard
	tst	_enable_poll	; is call serial polling ok
	beq	a@		; nope - skip
        jsr     poll            ; go poll serial device
a@      rti                     ; return from interrupt

_di:
        orcc    #$50            ; stop interrupts
        rts

_ei:
        andcc   #~$50           ; start interrupts
        rts

_ashrhi3:
        pshs    x
1$:
        leax    -1,x
        cmpx    #-1
        beq     2$
        asra
        rorb
        bra     1$
2$:
        puls    x,pc

	export start
	export _di
	export _ei
	import _main
	import _kpoll
	import poll

	section	.text

start:	orcc	#$50		; off interrupts
	ldx	#interrupt	; set interrupt vector
	stx	$10d		;
	lds	#$6000		; set stack
	ldx	#.bss_base	; clear bss
	ldy	#.bss_len	;
a@	clr	,x+		;
	leay	-1,y		;
	bne	a@		;
	andcc	#~$50		; on interrupts
	jmp	_main		; jump to C's main
	
interrupt:
        lda     $ff02           ; clear vsync pia
        jsr     _kpoll          ; go poll keyboard
        jsr     poll            ; go poll and add drivewire
a@      rti                     ; return from interrupt

_di:
        orcc    #$50            ; stop interrupts
        rts

_ei:
        andcc   #~$50           ; start interrupts
        rts

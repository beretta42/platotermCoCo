;;; Joystick/Mouse Touch Driver
;;;   via normal low-res joystick interface
	
	export do_joy
	export debug
	
	section .text

data:	rmb	1
half:	rmb	1

do_joy:	bsr	read
	lda	data
	sta	$400
	bra	do_joy
read:	;; select Left Horizontal	
	ldb	$ff01
	andb	#~$8
	stb	$ff01
	ldb	$ff03
	andb	#~$8
	stb	$ff03
	;; initialize vars for loop
	ldb	#$1f
	stb	data
	ldb	#$10
	stb	half
a@	;; the loop
debug	lda	data
	lsla
	lsla
	ora	#2
	sta	$ff20
	exg	a,a		; fixme: how much delay is needed?
	exg	a,a
	tst	$ff00
	bmi	higher
lower:	lda	data
	suba	half
	sta	data
	lsr	half
	beq	out
	bra	a@
higher:	lda	data
	adda	half
	sta	data
	lsr	half
	beq	out
	bra	a@
out:	rts

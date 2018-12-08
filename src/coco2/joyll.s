;;; Joystick/Mouse Touch Driver
;;;   via normal low-res joystick interface
	
	export _poll_joy
	export debug
	
	section .text

	
_poll_joy:
	select: lslb
	lslb
	lslb
	pshs	b
	;; lsb
	andb	#8
	pshs	b
	ldb	$ff01
	andb	#~$8
	orb	,s+
	stb	$ff01
	;; msb
	puls	b
	lsrb
	andb	#8
	pshs	b
	ldb	$ff03
	andb	#~$8
	orb	,s+
	stb	$ff03
read:	;; initialize vars for loop
	ldd	#$4080
a@	;; the loop
debug	orb	#2
	stb	$ff20
	andb	#~2
	exg	a,a		; fixme: how much delay is needed?
	exg	a,a
	tst	$ff00
	pshs	a
	bmi	higher
lower:	subb	,s+
	bra	c@
higher:	addb	,s+
c@	lsra
	cmpa	#1
	bne	a@
	clra
	lslb
	rola
	tfr	d,x
	rts

	export start
	import _main

	section	.text

start:	lds	#$6000
	ldx	#.bss_base
	ldy	#.bss_len
a@	clr	,x+
	leay	-1,y
	bne	a@
	jmp	_main



	export	_open_ll
	export  _read_ll
	export  _write_ll
	export	_close_ll

	section .data
save	rmb	2
	
	section .text
	
	;; open a decb file
_open_ll:
	pshs	y,u		; save abused regs
	sts	save		; save stack
	ldy	#open_err	; usurp BASIC error vector
	sty	$192
	tfr	x,d		; glue
	;; this DECB vectors should be grokked from the BASIC
	;; RAM vectors in DP. Don't assume decb1.1 locations!
	jsr	$c48d		; call 1.1 DECB's open
	ldx	#1		; return no error
	puls	y,u,pc 
open_err:
	lds	save		; reload stack
	ldx	#0		; return NULL
	puls	y,u,pc

	;; read byte from file
_read_ll:
	jsr	$c5c4
	tst	<$70		; read BASIC's EOF flag
	bne	eof
	tfr	a,b
	clra
	tfr	d,x
	rts
eof:	ldx	#-1
	rts

	;; write byte to file
_write_ll:
	exg	a,b
	jsr	$cc24
	ldx	#0
	rts


	;;
_close_ll:	
	pshs	y,u
	sts	save
	ldy	#close_err
	sty	$192
	jsr	$caf9
	ldx	#0
	puls	y,u,pc
close_err:
	lds	save
	ldx	#-1
	puls	y,u,pc

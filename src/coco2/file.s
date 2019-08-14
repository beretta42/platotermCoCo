

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
	;; fixme: this DECB vectors should be grokked from the BASIC
	;; RAM vectors in DP. Don't assume decb1.1 locations!
	jsr	$c48d		; call 1.1 DECB's open
open_ok:
	ldx	#1		; return no error
	puls	y,u,pc 
open_err:
	lds	save		; reload stack
	ldx	#0		; return NULL
	puls	y,u,pc

	;; read byte from file
_read_ll:
	sts	save
	ldx	#read_err
	stx	$192
	ldx	#read_ok
	pshs	x
	jsr	$16a
read_ok:
	tst	<$70		; read BASIC's EOF flag
	bne	read_eof
	tfr	a,b
	clra
	tfr	d,x
	rts
read_err:
	lds	save
	ldx	#-2
	rts
read_eof:
	ldx	#-1
	rts

	;; write byte to file
_write_ll:
	sts	save
	ldx	#write_err
	stx	$192
	ldx	#write_ok
	pshs	x
	exg	a,b
	jsr	$167
write_ok:
	ldx	#0
	rts
write_err:
	lds	save
	ldx	#-1
	rts


	;;
_close_ll:	
	pshs	y,u
	sts	save		; remember stack frame
	ldy	#close_err	; usurp any BASIC errors
	sty	$192
	ldx	#close_ok
	pshs	x		; push return address twice BASIC will gobble it
	jsr	$176		; call BASIC close ram hook
close_ok:
	ldx	#0		; closed ok!
	puls	y,u,pc
close_err:
	lds	save		; reset stack frame
	ldx	#-1		; closed bad!
	puls	y,u,pc

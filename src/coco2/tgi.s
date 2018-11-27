	export _tgi_clear
	export _tgi_setpixel
	export _tgi_cset
	export _tgi_char_blit
	export _tgi_char_blit_erase
	export _tgi_hline

	import _font

	section .text

;;; clear the screen
;;; fixme: should clear to pen color
_tgi_clear
	ldb	#128
	pshs	b,y,u
	ldu	#$6000+(32*192)
	ldd	#0
	ldy	#0
	ldx	#0
a@	pshu	d,x,y
	pshu	d,x,y
	pshu	d,x,y
	pshu	d,x,y
	pshu	d,x,y
	pshu	d,x,y
	pshu	d,x,y
	pshu	d,x,y
	dec	,s
	bne	a@
	puls	b,y,u,pc


;;; put a pixel on screen  x y r y
_tgi_setpixel
	pshs	x,y
	ldb	1,s
	andb	#$7
smc1	ldy	#tab
	leay	b,y
	ldd	,s
	lsrb
	lsrb
	lsrb
	pshs	d
	ldb	9,s
	lda	#32
	mul
	addd	,s++
	addd	#$6000
	tfr	d,x
	ldb	,x
smc2	orb	,y
	stb	,x
	puls	x,y,pc


_tgi_cset
	tstb
	beq	a@
	ldx	#tab
	stx	smc1+2
	stx	smc3+1
	ldb	#$ea
	stb	smc2
	stb	smc5
	ldb	#$80
	stb	smc4+1
	ldb	#$24
	stb	smc6
	rts
a@
	ldx	#tabi
	stx	smc1+2
	stx	smc3+1
	ldb	#$e4
	stb	smc2
	stb	smc5
	ldb	#~$80
	stb	smc4+1
	ldb	#$25
	stb	smc6
	rts

	;; table of shifted bit masks
tab
	.db	$80
	.db	$40
	.db	$20
	.db	$10
	.db	$08
	.db	$04
	.db	$02
	.db	$01
tabi
	.db	~$80
	.db	~$40
	.db	~$20
	.db	~$10
	.db	~$08
	.db	~$04
	.db	~$02
	.db	~$01


;;; put a char on screen
;;;   b x y, u, r y
_tgi_char_blit
	pshs	b,x,y,u
	;; u = find ptr to glyph
	subb	#32
	lda	#6
	mul
	addd	#_font
	tfr	d,u
	;; y = find screen ptr
	tfr	x,d
	lsrb
	lsrb
	lsrb
	pshs	d
	ldb	12,s
	lda	#32
	mul
	addd	,s++
	addd	#$6000
	tfr	d,y
	;; tos = find rotation
	ldb	2,s
	andb	#7
	pshs	b
	;; blit
	ldb	,u+
	bsr	foo
	ldb	,u+
	bsr	foo
	ldb	,u+
	bsr	foo
	ldb	,u+
	bsr	foo
	ldb	,u+
	bsr	foo
	ldb	,u+
	bsr	foo
	;; return
	puls	d,x,y,u,pc

foo
	lda	2,s
	pshs	a
	beq	b@
	clra
a@	lsrb
	rora
	dec	,s
	bne	a@
	ora	1,y
	sta	1,y
b@	orb	,y
	stb	,y
	leay	32,y
	puls	b,pc


;;; put a char on screen
;;;   b x y, u, r y
_tgi_char_blit_erase
	pshs	b,x,y,u
	;; u = find ptr to glyph
	subb	#32
	lda	#6
	mul
	addd	#_font
	tfr	d,u
	;; y = find screen ptr
	tfr	x,d
	lsrb
	lsrb
	lsrb
	pshs	d
	ldb	12,s
	lda	#32
	mul
	addd	,s++
	addd	#$6000
	tfr	d,y
	;; tos = find rotation
	ldb	2,s
	andb	#7
	pshs	b
	;; blit
	ldb	,u+
	bsr	foo1
	ldb	,u+
	bsr	foo1
	ldb	,u+
	bsr	foo1
	ldb	,u+
	bsr	foo1
	ldb	,u+
	bsr	foo1
	ldb	,u+
	bsr	foo1
	;; return
	puls	d,x,y,u,pc

foo1	lda	2,s
	pshs	a
	beq	b@
	clra
a@	lsrb
	rora
	dec	,s
	bne	a@
	coma
	anda	1,y
	sta	1,y
b@	comb
	andb	,y
	stb	,y
	leay	32,y
	puls	b,pc


;;; draw a horizontal line
;;;   X y r Y W
;;;  fixme: more speedups possible, do we bother?
_tgi_hline
	pshs	x,y
	;; y = find screen ptr
	tfr	x,d
	lsrb
	lsrb
	lsrb
	pshs	d
	ldb	9,s
	lda	#32
	mul
	addd	,s++
	addd	#$6000
	tfr	d,y
	;; tos = bit mask
	ldb	1,s
	andb	#7
smc3	ldx	#tab
	ldb	b,x
	pshs	b
	;; loop
	lda	10,s
a@	ldb	,y
smc5	orb	,s
	stb	,y
	lsr	,s
smc6	bcc	b@
	leay	1,y
smc4	ldb	#$80
	stb	,s
b@	deca
	bne	a@
	puls	b,x,y,pc

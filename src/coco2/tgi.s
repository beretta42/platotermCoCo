	export _tgi_clear
	export _tgi_setpixel
	export _tgi_cset
	export _tgi_char_blit
	export _tgi_char_blit_erase
	export _tgi_char_blit_rewrite
	export _tgi_hline
	export _tgi_vline
	export _tgi_put_mouse
	export _tgi_unput_mouse

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
	stx	smc11+1
	ldb	#$ea
	stb	smc2
	stb	smc5
	stb	smc12
	ldb	#$80
	stb	smc4+1
	ldb	#$24
	stb	smc6
	rts
a@
	ldx	#tabi
	stx	smc1+2
	stx	smc3+1
	stx	smc11+1
	ldb	#$e4
	stb	smc2
	stb	smc5
	stb	smc12
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

;;; put a char on screen - rewrite mode
;;;   b x y, u, r y
_tgi_char_blit_rewrite
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
	;; find masks
	pshs	b
	ldd	#$f000
	tst	,s
	beq	a@
b@	lsra
	rorb
	dec	,s
	bne	b@
a@	leas	1,s
	coma
	comb
	sta	smc10+1
	stb	smc10+3
	;; blit
	lda	,u+
	bsr	foo2
	lda	,u+
	bsr	foo2
	lda	,u+
	bsr	foo2
	lda	,u+
	bsr	foo2
	lda	,u+
	bsr	foo2
	lda	,u+
	bsr	foo2
	;; return
	puls	d,x,y,u,pc

foo2	ldb	2,s
	pshs	b
	clrb
	tst	,s
	beq	b@
a@	lsra
	rorb
	dec	,s
	bne	a@
b@	pshs	d
	ldd	,y
smc10	anda	#0
	andb	#0
	ora	,s+
	orb	,s+
	std	,y
	leay	32,y
	puls	b,pc


;;; draw a vertial line
;;; t X y r Y H
_tgi_vline:
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
	;; b = mask
	ldb	1,s
	andb	#7
smc11	ldx	#tab
	ldb	b,x
	stb	smc12+1
	lda	9,s
	pshs	a
a@	ldb	,y
smc12	orb	#0
	stb	,y
	dec	,s
	beq	b@
	leay	32,y
	bra	a@
b@	puls	b,x,y,pc

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

mouse	.dw	0x8000, 0xe000, 0xf800, 0xfc00, 0xf000, 0x9800, 0x0c00, 0x0600
	.dw	0x0800, 0x0e00, 0x0f80, 0x0fc0, 0x0f00, 0x0980, 0x00c0, 0x0060
	.dw	0x8000, 0xe000, 0xf800, 0xfc00, 0xf000, 0x9800, 0x0c00, 0x0600
	.dw	0x4000, 0x7000, 0x7c00, 0x7e00, 0x7800, 0x4c00, 0x0600, 0x0300
	.dw	0x2000, 0x3800, 0x3e00, 0x3f00, 0x3c00, 0x2600, 0x0300, 0x0180
	.dw	0x1000, 0x1c00, 0x1f00, 0x1f80, 0x1e00, 0x1300, 0x0180, 0x00c0
	.dw	0x0800, 0x0e00, 0x0f80, 0x0fc0, 0x0f00, 0x0980, 0x00c0, 0x0060
	.dw	0x0400, 0x0700, 0x07c0, 0x07e0, 0x0780, 0x04c0, 0x0060, 0x0030
	.dw	0x0200, 0x0380, 0x03e0, 0x03f0, 0x03c0, 0x0260, 0x0030, 0x0018
	.dw	0x0100, 0x01c0, 0x01f0, 0x01f8, 0x01e0, 0x0130, 0x0018, 0x000c
_tgi_put_mouse:
_tgi_unput_mouse:
	;; X y u r Y
	pshs	x,y,u
	tfr	x,d
	andb	#7
	pshs	b		; push modulus 8
	tfr	x,d
	lsrb
	lsrb
	lsrb
	pshs	b		; push row offset
	ldb	11,s		; get Y
	lda	#32
	mul
	addb	,s+		; add offset
	adca	#0		; 16-bitify add
	addd	#$6000		; add screen base
	tfr	d,x		; X = screen bytes
	ldb	,s+		; pull modulus (bit shifts)
	lda	#16
	mul
	addd	#mouse
	tfr	d,y
	ldb	#4
	pshs	b
	;; begin loop
a@
	ldd	,y++		; get first row
	eora	,x		; xor to screen data
	eorb	1,x		;
	std	,x		; and save back to screen
	leax	32,x		; next row
	ldd	,y++		; get first row
	eora	,x		; xor to screen data
	eorb	1,x		;
	std	,x		; and save back to screen
	leax	32,x		; next row
	;; loop inc
	dec	,s
	bne	a@
	puls	b,x,y,u,pc	; pull counter, retore, return

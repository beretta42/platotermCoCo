*******************************************************
*
* DWRead
*    Receive a response from the DriveWire server.
*    Times out if serial port goes idle for more than 1.4 (0.7) seconds.
*    Serial data format:  1-8-N-1
*    4/12/2009 by Darren Atkinson
*
* Entry:
*    X  = starting address where data is to be stored
*    Y  = number of bytes expected
*
* Exit:
*    CC = carry set on framing error, Z set if all bytes received
*    X  = starting address of data received
*    Y  = checksum
*    U is preserved.  All accumulators are clobbered
*

          IFNE ARDUINO
* Note: this is an optimistic routine. It presumes that the server will always be there, and
* has NO timeout fallback. It is also very short and quick.
DWRead    clra                          ; clear Carry (no framing error)
          pshs   u,x,cc              ; preserve registers
          leau   ,x
          ldx    #$0000
loop@     tst    $FF51                  ; check for CA1 bit (1=Arduino has byte ready)
          bpl    loop@                  ; loop if not set
          ldb    $FF50                  ; clear CA1 bit in status register
          stb    ,u+                    ; save off acquired byte
          abx                           ; update checksum
          leay   ,-y
          bne    loop@

          leay      ,x                  ; return checksum in Y
          puls      cc,x,u,pc        ; restore registers and return

          ELSE

          IFNE SY6551N
          IFNDEF    SY6551B
SY6551B   EQU       $FF68            ; Set base address for future use
          ENDC
          IFNDEF    SYDATA
SYDATA    EQU       SY6551B
          ENDC
          IFNDEF    SYCONT
SYCONT    EQU       SY6551B+3
          ENDC
          IFNDEF    SYCOMM
SYCOMM    EQU       SY6551B+2
          ENDC
          IFNDEF    SYSTAT
SYSTAT    EQU       SY6551B+1
          ENDC
* NOTE: There is no timeout currently on here...
DWRead    clra                       ; clear Carry (no framing error)
          deca                       ; clear Z flag, A = timeout msb ($ff)
          tfr    cc,b
          pshs   u,x,dp,b,a          ; preserve registers, push timeout msb
          leau   ,x
          ldx    #$0000
          IFEQ   NOINTMASK
          orcc   #IntMasks
          ENDC
loop@     ldb    SYSTAT
          andb   #$08
          beq    loop@
          ldb    SYDATA
          stb    ,u+
          abx
          leay   ,-y
          bne    loop@

          tfr    x,y
          ldb    #0
          lda    #3
          leas   1,s                 ; remove timeout msb from stack
          inca                       ; A = status to be returned in C and Z
          ora    ,s                  ; place status information into the..
          sta    ,s                  ; ..C and Z bits of the preserved CC
          leay   ,x                  ; return checksum in Y
          puls   cc,dp,x,u,pc        ; restore registers and return
          ELSE
          IFNE JMCPBCK
* NOTE: There is no timeout currently on here...
DWRead    clra                          ; clear Carry (no framing error)
          deca                          ; clear Z flag, A = timeout msb ($ff)
          tfr       cc,b
          pshs      u,x,dp,b,a          ; preserve registers, push timeout msb
          leau   ,x
          ldx    #$0000
          IFEQ   NOINTMASK
          orcc   #IntMasks
          ENDC
loop@     ldb    $FF4C
          bitb   #$02
          beq    loop@
          ldb    $FF44
          stb    ,u+
          abx
          leay   ,-y
          bne    loop@

          tfr    x,y
          ldb    #0
          lda    #3
          leas      1,s                 ; remove timeout msb from stack
          inca                          ; A = status to be returned in C and Z
          ora       ,s                  ; place status information into the..
          sta       ,s                  ; ..C and Z bits of the preserved CC
          leay      ,x                  ; return checksum in Y
          puls      cc,dp,x,u,pc        ; restore registers and return
          ELSE
          IFNE BECKER
          IFNDEF    BECKBASE
BECKBASE  EQU       $FF41               ; Set base address for future use
          ENDC
          IFNDEF    BECKDATA
BECKDATA  EQU       BECKBASE+1          ; Set Becker Port Data Address Location
          ENDC
          IFNDEF    BECKSTAT
BECKSTAT  EQU       BECKBASE            ; Sete Becker Port Status Register Address Location
          ENDC
* NOTE: There is no timeout currently on here...
DWRead    clra                          ; clear Carry (no framing error)
          deca                          ; clear Z flag, A = timeout msb ($ff)
          tfr       cc,b
          pshs      u,x,dp,b,a          ; preserve registers, push timeout msb
          leau   ,x
          ldx    #$0000
          IFEQ   NOINTMASK
          orcc   #IntMasks
          ENDC
loop@     ldb    BECKSTAT
          bitb   #$02
          beq    loop@
          ldb    BECKDATA
          stb    ,u+
          abx
          leay   ,-y
          bne    loop@
          tfr    x,y
          ldb    #0
          lda    #3
          leas      1,s                 ; remove timeout msb from stack
          inca                          ; A = status to be returned in C and Z
          ora       ,s                  ; place status information into the..
          sta       ,s                  ; ..C and Z bits of the preserved CC
          leay      ,x                  ; return checksum in Y
          puls      cc,dp,x,u,pc        ; restore registers and return
          ELSE
          IFNE BECKERTO
          IFNDEF    BECKBASE
BECKBASE  EQU       $FF41               ; Set base address for future use
          ENDC
          IFNDEF    BECKDATA
BECKDATA  EQU       BECKBASE+1          ; Set Becker Port Data Address Location
          ENDC
          IFNDEF    BECKSTAT
BECKSTAT  EQU       BECKBASE            ; Sete Becker Port Status Register Address Location
          ENDC
;;; I added a Vsync/IRQ timeout for 2 seconds here.  This should
;;; make the timeout work regardless of speed of emulated systems
;;; esp. Vcc and CoCoFPGA.  Vcc over 89 Mhz is known to be wonky
;;; and the IRQ appearantly slows down.  There is a good chance
;;; that MESS will not work with this method, as it's "unthrottled"
;;; mode, also increases the speed of IRQ (oops!)
DWRead	  clra                  ; clear Carry, Set Z
	  pshs	 cc,x,u         ; save regs
          leau   ,x		; U is data buffer
          orcc	 #0x50	        ; turn off interrupts
	  ldd	 0x10d		; save current irq handler
	  pshs	 d		; on stack
	  ldx	 #irq		; load X with pointer to irq handler
	  stx	 0x10d		; and put it in the jmp table
	  andcc	 #0xef		; turn on irq
	  ldx    #$0000		; X is reset check sum
ini@	  lda	 #120		; set RDYTMR to 120 jiffies = 2 seconds
	  sta	 RDYTMR		;
loop@
	  ldb    BECKSTAT		; test for data ready flag
          bitb   #$02
          bne    rdy@		; byte is ready
	  tst	 RDYTMR		; test timer
	  bne    loop@          ; not timed out, try again
        ;; timed out!
	  bsr	 reset@
	  puls   cc		; pull CC
	  comb                  ; reset Z (timeout error)
	  puls	 x,u,pc	        ; restore registers and return
	;; a byte is ready
rdy@      ldb    BECKDATA       ; get byte from port
          stb    ,u+            ; store in data buffer
          abx                   ; add received byte to checksum
          leay   ,-y            ; decrement byte counter
          bne    ini@           ; go get another byte if not done
	;; done reading bytes return
          tfr    x,y		; put checksum in y
	  bsr	 reset@		; reset to old irq handler
          puls   cc,x,u,pc      ; restore registers and return
	;; reset irq handler
reset@	  orcc	 #0x50		; turn off interrupts
	  puls	 x,u		; pull ret, old handler
	  stu	 0x10d		; put old handler back in int table
	  jmp	 ,x		; return

irq
	lda 	PIA0+3     	; is a proper 60hz timeout?
	bpl 	o@              ; nop return
	lda 	PIA0+2          ; reset the interrupt flag
	dec 	RDYTMR          ; get timer
o@	rti			; return from interrupt

          ENDC
          ENDC
          ENDC
          ENDC
          ENDC

          IFEQ BECKER+JMCPBCK+ARDUINO+BECKERTO+SY6551N
          IFNE BAUD38400
*******************************************************
* 38400 bps using 6809 code and timimg
*******************************************************

DWRead    clra                          ; clear Carry (no framing error)
          deca                          ; clear Z flag, A = timeout msb ($ff)
          tfr       cc,b
          pshs      u,x,dp,b,a          ; preserve registers, push timeout msb
          IFEQ      NOINTMASK
          orcc      #IntMasks           ; mask interrupts
          ENDC
          tfr       a,dp                ; set direct page to $FFxx
          setdp     $ff
          leau      ,x                  ; U = storage ptr
          ldx       #0                  ; initialize checksum
          adda      #2                  ; A = $01 (serial in mask), set Carry

* Wait for a start bit or timeout
rx0010    bcc       rxExit              ; exit if timeout expired
          ldb       #$ff                ; init timeout lsb
rx0020    bita      <BBIN               ; check for start bit
          beq       rxByte              ; branch if start bit detected
          subb      #1                  ; decrement timeout lsb
          bita      <BBIN
          beq       rxByte
          bcc       rx0020              ; loop until timeout lsb rolls under
          bita      <BBIN
          beq       rxByte
          addb      ,s                  ; B = timeout msb - 1
          bita      <BBIN
          beq       rxByte
          stb       ,s                  ; store decremented timeout msb
          bita      <BBIN
          bne       rx0010              ; loop if still no start bit

* Read a byte
rxByte    leay      ,-y                 ; decrement request count
          ldd       #$ff80              ; A = timeout msb, B = shift counter
          sta       ,s                  ; reset timeout msb for next byte
rx0030    exg       a,a
          nop
          lda       <BBIN               ; read data bit
          lsra                          ; shift into carry
          rorb                          ; rotate into byte accumulator
          lda       #$01                ; prep stop bit mask
          bcc       rx0030              ; loop until all 8 bits read

          stb       ,u+                 ; store received byte to memory
          abx                           ; update checksum
          ldb       #$ff                ; set timeout lsb for next byte
          anda      <BBIN               ; read stop bit
          beq       rxExit              ; exit if framing error
          leay      ,y                  ; test request count
          bne       rx0020              ; loop if another byte wanted
          lda       #$03                ; setup to return SUCCESS

* Clean up, set status and return
rxExit    leas      1,s                 ; remove timeout msb from stack
          inca                          ; A = status to be returned in C and Z
          ora       ,s                  ; place status information into the..
          sta       ,s                  ; ..C and Z bits of the preserved CC
          leay      ,x                  ; return checksum in Y
          puls      cc,dp,x,u,pc        ; restore registers and return
          setdp     $00


          ELSE
          IFNE H6309
*******************************************************
* 57600 (115200) bps using 6309 native mode
*******************************************************

DWRead    clrb                          ; clear Carry (no framing error)
          decb                          ; clear Z flag, B = $FF
          pshs      u,x,dp,cc           ; preserve registers
          IFEQ      NOINTMASK
          orcc      #IntMasks           ; mask interrupts
          ENDC
*         ldmd      #1                  ; requires 6309 native mode
          tfr       b,dp                ; set direct page to $FFxx
          setdp     $ff
          leay      -1,y                ; adjust request count
          leau      ,x                  ; U = storage ptr
          tfr       0,x                 ; initialize checksum
          lda       #$01                ; A = serial in mask
          bra       rx0030              ; go wait for start bit

* Read a byte
rxByte    sexw                          ; 4 cycle delay
          ldw       #$006a              ; shift counter and timing flags
          clra                          ; clear carry so next will branch
rx0010    bcc       rx0020              ; branch if even bit number (15 cycles)
          nop                           ; extra (16th) cycle
rx0020    lda       <BBIN               ; read bit
          lsra                          ; move bit into carry
          rorb                          ; rotate bit into byte accumulator
          lda       #0                  ; prep A for 8th data bit
          lsrw                          ; bump shift count, timing bit to carry
          bne       rx0010              ; loop until 7th data bit has been read
          incw                          ; W = 1 for subtraction from Y
          inca                          ; A = 1 for reading bit 7
          anda      <BBIN               ; read bit 7
          lsra                          ; move bit 7 into carry, A = 0
          rorb                          ; byte is now complete
          stb       ,u+                 ; store received byte to memory
          abx                           ; update checksum
          subr      w,y                 ; decrement request count
          inca                          ; A = 1 for reading stop bit
          anda      <BBIN               ; read stop bit
          bls       rxExit              ; exit if completed or framing error

* Wait for a start bit or timeout
rx0030    clrw                          ; initialize timeout counter
rx0040    bita      <BBIN               ; check for start bit
          beq       rxByte              ; branch if start bit detected
          addw      #1                  ; bump timeout counter
          bita      <BBIN
          beq       rxByte
          bcc       rx0040              ; loop until timeout rolls over
          lda       #$03                ; setup to return TIMEOUT status

* Clean up, set status and return
rxExit    beq       rx0050              ; branch if framing error
          eora      #$02                ; toggle SUCCESS flag
rx0050    inca                          ; A = status to be returned in C and Z
          ora       ,s                  ; place status information into the..
          sta       ,s                  ; ..C and Z bits of the preserved CC
          leay      ,x                  ; return checksum in Y
          puls      cc,dp,x,u,pc        ; restore registers and return
          setdp     $00


          ELSE
*******************************************************
* 57600 (115200) bps using 6809 code and timimg
*******************************************************

DWRead    clra                          ; clear Carry (no framing error)
          deca                          ; clear Z flag, A = timeout msb ($ff)
          tfr       cc,b
          pshs      u,x,dp,b,a          ; preserve registers, push timeout msb
          IFEQ      NOINTMASK
          orcc      #IntMasks           ; mask interrupts
          ENDC
          tfr       a,dp                ; set direct page to $FFxx
          setdp     $ff
          leau      ,x                  ; U = storage ptr
          ldx       #0                  ; initialize checksum
          lda       #$01                ; A = serial in mask
          bra       rx0030              ; go wait for start bit

* Read a byte
rxByte    leau      1,u                 ; bump storage ptr
          leay      ,-y                 ; decrement request count
          lda       <BBIN               ; read bit 0
          lsra                          ; move bit 0 into Carry
          ldd       #$ff20              ; A = timeout msb, B = shift counter
          sta       ,s                  ; reset timeout msb for next byte
          rorb                          ; rotate bit 0 into byte accumulator
rx0010    lda       <BBIN               ; read bit (d1, d3, d5)
          lsra
          rorb
          bita      1,s                 ; 5 cycle delay
          bcs       rx0020              ; exit loop after reading bit 5
          lda       <BBIN               ; read bit (d2, d4)
          lsra
          rorb
          leau      ,u
          bra       rx0010

rx0020    lda       <BBIN               ; read bit 6
          lsra
          rorb
          leay      ,y                  ; test request count
          beq       rx0050              ; branch if final byte of request
          lda       <BBIN               ; read bit 7
          lsra
          rorb                          ; byte is now complete
          stb       -1,u                ; store received byte to memory
          abx                           ; update checksum
          lda       <BBIN               ; read stop bit
          anda      #$01                ; mask out other bits
          beq       rxExit              ; exit if framing error

* Wait for a start bit or timeout
rx0030    bita      <BBIN               ; check for start bit
          beq       rxByte              ; branch if start bit detected
          bita      <BBIN               ; again
          beq       rxByte
          ldb       #$ff                ; init timeout lsb
rx0040    bita      <BBIN
          beq       rxByte
          subb      #1                  ; decrement timeout lsb
          bita      <BBIN
          beq       rxByte
          bcc       rx0040              ; loop until timeout lsb rolls under
          bita      <BBIN
          beq       rxByte
          addb      ,s                  ; B = timeout msb - 1
          bita      <BBIN
          beq       rxByte
          stb       ,s                  ; store decremented timeout msb
          bita      <BBIN
          beq       rxByte
          bcs       rx0030              ; loop if timeout hasn't expired
          bra       rxExit              ; exit due to timeout

rx0050    lda       <BBIN               ; read bit 7 of final byte
          lsra
          rorb                          ; byte is now complete
          stb       -1,u                ; store received byte to memory
          abx                           ; calculate final checksum
          lda       <BBIN               ; read stop bit
          anda      #$01                ; mask out other bits
          ora       #$02                ; return SUCCESS if no framing error

* Clean up, set status and return
rxExit    leas      1,s                 ; remove timeout msb from stack
          inca                          ; A = status to be returned in C and Z
          ora       ,s                  ; place status information into the..
          sta       ,s                  ; ..C and Z bits of the preserved CC
          leay      ,x                  ; return checksum in Y
          puls      cc,dp,x,u,pc        ; restore registers and return
          setdp     $00

          ENDC
          ENDC
          ENDC


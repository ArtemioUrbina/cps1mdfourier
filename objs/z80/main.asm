;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.2.0 #13081 (Linux)
;--------------------------------------------------------
	.module main
	.optsdcc -mz80
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _requestInterrupt
	.globl _interrupt
	.globl _lastLatch
	.globl _latch
	.globl _LATCH1
	.globl _OKI
	.globl _YM2151_DAT
	.globl _YM2151_ADR
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _DATA
_YM2151_ADR	=	0xf000
_YM2151_DAT	=	0xf001
_OKI	=	0xf002
_LATCH1	=	0xf008
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _INITIALIZED
_latch::
	.ds 1
_lastLatch::
	.ds 1
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area _DABS (ABS)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area _HOME
	.area _GSINIT
	.area _GSFINAL
	.area _GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area _HOME
	.area _HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area _CODE
;src/z80/main.c:15: void interrupt() {
;	---------------------------------
; Function interrupt
; ---------------------------------
_interrupt::
;src/z80/main.c:16: latch = LATCH1;
	ld	a, (_LATCH1+0)
	ld	(_latch+0), a
;src/z80/main.c:18: if (lastLatch == latch) {
	ld	a, (_lastLatch+0)
	ld	iy, #_latch
	sub	a, 0 (iy)
	ret	Z
;src/z80/main.c:19: return;
	jr	00102$
00102$:
;src/z80/main.c:22: lastLatch = latch;
	ld	a, (_latch+0)
	ld	(_lastLatch+0), a
;src/z80/main.c:24: if (latch == NO_OP) {
	ld	a, (_latch+0)
	inc	a
	ret	Z
;src/z80/main.c:25: return;
	jr	00104$
00104$:
;src/z80/main.c:28: OKI = 0x80 | latch; // First bit must be 1 then sound ID.
	ld	a, (_latch+0)
	or	a, #0x80
	ld	(_OKI+0), a
;src/z80/main.c:29: OKI = 0x80; // 0x80 = Channel 4  !   0x00 = No sound reduction.
	ld	hl, #_OKI
	ld	(hl), #0x80
;src/z80/main.c:30: }
	ret
;src/z80/main.c:32: void requestInterrupt() {
;	---------------------------------
; Function requestInterrupt
; ---------------------------------
_requestInterrupt::
;src/z80/main.c:40: YM2151_ADR = YM2151_REG_CLKA1;
	ld	hl, #_YM2151_ADR
	ld	(hl), #0x10
;src/z80/main.c:41: YM2151_DAT = 0xC8;
	ld	hl, #_YM2151_DAT
	ld	(hl), #0xc8
;src/z80/main.c:44: YM2151_ADR = YM2151_REG_CLKA2;
	ld	hl, #_YM2151_ADR
	ld	(hl), #0x11
;src/z80/main.c:45: YM2151_DAT = 0x00;
	ld	hl, #_YM2151_DAT
	ld	(hl), #0x00
;src/z80/main.c:46: }
	ret
;src/z80/main.c:48: void main() {
;	---------------------------------
; Function main
; ---------------------------------
_main::
;src/z80/main.c:50: YM2151_ADR = YM2151_REG_CTRL;
	ld	hl, #_YM2151_ADR
	ld	(hl), #0x14
;src/z80/main.c:51: YM2151_DAT = 0x15;
	ld	hl, #_YM2151_DAT
	ld	(hl), #0x15
;src/z80/main.c:55: requestInterrupt();
	call	_requestInterrupt
;src/z80/main.c:57: while(1) {
00102$:
;src/z80/main.c:58: interrupt();
	call	_interrupt
;src/z80/main.c:60: }
	jr	00102$
	.area _CODE
	.area _INITIALIZER
__xinit__latch:
	.db #0x00	; 0
__xinit__lastLatch:
	.db #0xff	; 255
	.area _CABS (ABS)

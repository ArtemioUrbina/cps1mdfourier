    .globl  _start
    .globl  atexit

    .extern hardwareInit
    .extern onVSync
    .extern run
    .extern _end

    dc.l    0xFFF000, _start, Default, Default, Default, Default, Default, Default
    dc.l    Default, Default, Default, Default, Default, Default, Default, Default
    dc.l    Default, Default, Default, Default, Default, Default, Default, Default
    dc.l    Default, Default, VBlank,  Default, Default, Default, Default, Default
    dc.l    Default, Default, Default, Default, Default, Default, Default, Default
    dc.l    Default, Default, Default, Default, Default, Default, Default, Default
    dc.l    Default, Default, Default, Default, Default, Default, Default, Default
    dc.l    Default, Default, Default, Default, Default, Default, Default, Default

.align  4
Default:
    rte

.align  4
* Dummy atexit (does nothing for now)
atexit:
    moveq   #0, d0
    rts

.align  4
VBlank:
    jsr onVSync
    rte

.align  4
_start:
    * Reset coin control
    move.b  #0x80, 0x800030
    nop
    nop
    nop
    move.b  #0x00, 0x800030

    lea     0x900000, A0
    lea     0x903fff, A1
loopinit:
    move.w  #0x0, (A0)+
    cmpa.l  A1, A0
    bls     loopinit

    * Set stack pointer
    move.l  #0xFFF000, sp

    * Initialize BSS section
    move.l  #_end, d0
    sub.l   #__bss_start, d0
    move.l  d0, -(sp)
    clr.l   -(sp)
    pea     __bss_start

* Call Board initialization for CPSA & CPSB registers and Z80 data
    jsr     hardwareInit

    * Enable interrupts
    move.w  #0x2000, sr

    * Jump to mainloop
    jbsr    run

EndLoop:
    bra.s   EndLoop

ClearRAM:
* Clear RAM in the range 0xFF0000 to 0xFFFFFF
    move.l  #0xFF0000, a0
    move.l  #0x00010000, d1
    move.l  #0, d2

ClearLoop:
    move.l  d2, (a0)+
    dbra    d1, ClearLoop
    rts

* GFX RAM in the range 0x900000 0x92FFFF 192 KiB GFXRAM

* Clear all 3 scrolls
ClearScrolls:
    lea     0x908000, a0
    move.l  #10000/4-1, d0

* Clear GFX RAM
ClearHardwareSprites:
    lea     0x900000, a0
    move.l  #255, d0

ClearHardwareSpritesLoop:
    clr.l   (a0)+
    clr.l   (a0)+

    dbra    d0, ClearHardwareSpritesLoop

    rts

ClearScrollsLoop:
    clr.l   (a0)+
    dbra    d0, ClearScrollsLoop
    
    rts



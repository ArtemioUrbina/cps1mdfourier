#include <stdint.h>
#include "z80commands.h"

#define REG_YM2151_ADR *((volatile char*)0xF000)
#define REG_YM2151_DAT *((volatile char*)0xF001)
#define REG_OKI *((volatile char*)0xF002)
#define REG_BANK_SWITCH *((volatile char*)0xF004)
#define REG_OKI_QUALITY_SWITCH *((volatile char*)0xF006)
#define REG_LATCH1 *((volatile char*)0xF008)
#define REG_LATCH2 *((volatile char*)0xF00A)

#define YM2151_REG_CLKA1 0x10
#define YM2151_REG_CLKA2 0x11
#define YM2151_REG_CTRL  0x14

#define STEREO_RIGHT    0x80
#define STEREO_LEFT     0x40
#define STEREO_BOTH     0xC0

int8_t latch = 0;
int8_t latchData = 0;
int8_t chann = 0;
int8_t note = 0;
int8_t octave = 0;
int8_t quality = 1;
int8_t reduction = 0;
volatile int8_t c = 0;

volatile int8_t lastLatch = Z80_NO_OP;

/* YM21521 functions */

void YM2151_writeReg(int8_t adr, int8_t dat) {
    while(REG_YM2151_DAT == 0x80); // Wait until YM2151 is ready for write
    REG_YM2151_ADR = adr;
    REG_YM2151_DAT = dat;
}

void ym2151_keyoff(int8_t channel) {   
    YM2151_writeReg(0x08, channel &0x07);
}

void ym2151_keyoffAll() {
    for(c = 0; c < 8; c ++)
        ym2151_keyoff(c);
}

void ymloadchannel(int8_t channel) {
//0x38 Phase & amplitude modulation sensitivity PMS: 0x00 AMS: 0x00
    YM2151_writeReg(0x38+channel, 0x00);
//0x40 Detune & phase multiply Slot: 0000 Decay: 0x00 Multi: 0x04
    YM2151_writeReg(0x40+channel, 0x04);
//0x48 Detune & phase multiply Slot: 0001 Decay: 0x00 Multi: 0x04
    YM2151_writeReg(0x48+channel, 0x04);
//0x50 Detune & phase multiply Slot: 0010 Decay: 0x00 Multi: 0x04
    YM2151_writeReg(0x50+channel, 0x04);
//0x58 Detune & phase multiply Slot: 0011 Decay: 0x00 Multi: 0x04
    YM2151_writeReg(0x58+channel, 0x04);
//0x60 Total level Slot: 0000 Volume: 0x0000
    YM2151_writeReg(0x60+channel, 0x00);
//0x68 Total level Slot: 0001 Volume: 0x0000
    YM2151_writeReg(0x68+channel, 0x00);
//0x70 Total level Slot: 0010 Volume: 0x0000
    YM2151_writeReg(0x70+channel, 0x00);
//0x78 Total level Slot: 0011 Volume: 0x0000
    YM2151_writeReg(0x78+channel, 0x00);
//0x80 EG Attack Slot: 0000 Keyscale: 01 Attack rate: 0x01F
    YM2151_writeReg(0x80+channel, 0x5F);
//0x88 EG Attack Slot: 0001 Keyscale: 00 Attack rate: 0x01F
    YM2151_writeReg(0x88+channel, 0x1F);
//0x90 EG Attack Slot: 0010 Keyscale: 00 Attack rate: 0x01F
    YM2151_writeReg(0x90+channel, 0x1F);
//0x98 EG Attack Slot: 0011 Keyscale: 00 Attack rate: 0x01F
    YM2151_writeReg(0x98+channel, 0x1F);
//0xA0 EG Decay 1 Slot: 0000 Amplitude modulation Sensitivity: N First decay rate: 0x01F
    YM2151_writeReg(0xA0+channel, 0x1F);
//0xA8 EG Decay 1 Slot: 0001 Amplitude modulation Sensitivity: N First decay rate: 0x01F
    YM2151_writeReg(0xA8+channel, 0x1F);
//0xB0 EG Decay 1 Slot: 0010 Amplitude modulation Sensitivity: N First decay rate: 0x01F
    YM2151_writeReg(0xB0+channel, 0x1F);
//0xB8 EG Decay 1 Slot: 0011 Amplitude modulation Sensitivity: N First decay rate: 0x01F
    YM2151_writeReg(0xB8+channel, 0x1F);
//0xC0 EG Decay 2 Slot: 0000 Detune(2): 0x0 Second decay rate: 0x00
    YM2151_writeReg(0xC0+channel, 0x00);
//0xC8 EG Decay 2 Slot: 0001 Detune(2): 0x0 Second decay rate: 0x00
    YM2151_writeReg(0xC8+channel, 0x00);
//0xD0 EG Decay 2 Slot: 0010 Detune(2): 0x0 Second decay rate: 0x00
    YM2151_writeReg(0xD0+channel, 0x00);
//0xD8 EG Decay 2 Slot: 0011 Detune(2): 0x0 Second decay rate: 0x00
    YM2151_writeReg(0xD8+channel, 0x00);
//0xE0 EG Decay level, Release rate Slot: 0000 First Decay: 0x00 Release rate: 0x0F
    YM2151_writeReg(0xE0+channel, 0x0F);
//0xE8 EG Decay level, Release rate Slot: 0001 First Decay: 0x00 Release rate: 0x0F
    YM2151_writeReg(0xE8+channel, 0x0F);
//0xF0 EG Decay level, Release rate Slot: 0010 First Decay: 0x00 Release rate: 0x0F
    YM2151_writeReg(0xF0+channel, 0x0F);
//0xF8 EG Decay level, Release rate Slot: 0011 First Decay: 0x00 Release rate: 0x0F
    YM2151_writeReg(0xF8+channel, 0x0F);
//0x20 L: Y R: Y Feedback: 000 Connection: 111
    YM2151_writeReg(0x20+channel, 0xC7);
//0x28 Octave:0x06 Note: 0x0C
    YM2151_writeReg(0x28+channel, 0x6C);
}

void yminit() {
//0x01 Test LFO Reset
    YM2151_writeReg(0x01, 0x02);
//0x0F Noise disable Freq: 0x000
    YM2151_writeReg(0x0F, 0x00);
//0x14 Clock Functions CSM OFF F-Reset: 00 IRQEN: 00 LOAD: 00
    YM2151_writeReg(0x14, 0x00);
//0x18 LOW OSCILLATION FREQUENCY 0x0000
    YM2151_writeReg(0x18, 0x00);
//0x19 PHS OR AMP MODULATION DEPTH 0x0000
    YM2151_writeReg(0x19, 0x00);
//0x1B Disk State OFF CT: 00 (4mhz) Waveform: 01 (square)
    YM2151_writeReg(0x1B, 0x01);

    for(c = 0; c < 8; c++)
        ymloadchannel(c);
}

void ymPlay(int8_t channel, int8_t note, int8_t octave, int8_t pan) {
    ym2151_keyoff(channel);

    // set stereo balance Feedback: 000 Connection: 111
    YM2151_writeReg(0x20+channel, pan|0x07);

    //Register 0x28 - frequency
    YM2151_writeReg(0x28+channel,(octave << 4) | note);
    
    // Register 0x08 - Key on
    YM2151_writeReg(0x08, 0x78+channel);
}

void handleCommand(int8_t command, int8_t param) {
    switch(command) {
        case Z80_INIT:
            chann = 0;
            note = 0;
            octave = 0;
            quality = 1;
            reduction = 0;
            yminit();
            break;
        case Z80_PULSE_ON:
            YM2151_writeReg(0x08, 0x78);
            break;
        case Z80_PULSE_OFF:
            ym2151_keyoff(0);
            break;
        case Z80_RESET_PULSE:
            YM2151_writeReg(0x20, 0xC7);
            YM2151_writeReg(0x28, 0x6C);
            break;
        case Z80_SILENCE:
            ym2151_keyoffAll();
            break;
        case Z80_PLAY_NOTE:
            ymPlay(chann, note, octave, STEREO_LEFT);
            ymPlay(chann+4, note, octave, STEREO_RIGHT);

            note ++;
            if(note >= 16) {
                note = 0;
                octave ++;
                if(octave >= 8)
                    octave = 0;
            }
            break;
        case Z80_STOP_NOTE:
            ym2151_keyoff(chann);
            ym2151_keyoff(chann+4);
            chann ++;
            if(chann > 3)
                chann = 0;
            break;
        case Z80_ADPCM_QLOW:
            quality = 0;
            REG_OKI = 0x78; // 0111 1000
            break;
        case Z80_ADPCM_QHI:
            quality = 1;
            REG_OKI = 0x78; // 0111 1000
            break;
        case Z80_ADPCM_PLAY:
            REG_OKI_QUALITY_SWITCH = quality;
            REG_OKI = 0x80 | (0x0F & param); // First bit must be 1 then sound ID, first sample the sweep
            REG_OKI = 0x80; // 0x80 = Channel 4  !   0x00 = No sound reduction.
            break;
        case Z80_ADPCM_REDCT:
            REG_OKI_QUALITY_SWITCH = quality;
            REG_OKI = 0x80 | (0x0F & param); // First bit must be 1 then sound ID, second sample the 1khz tone
            REG_OKI = 0x80 | (0x0F & reduction); // 0x10 = Channel 1  !   0x00 = No sound reduction.
            reduction ++;
            if(reduction >= 8)
                reduction = 0;
            break;
        case Z80_ADPCM_STOP:
            REG_OKI = 0x78; // 0111 1000
            break;
        default:
            break;
    }
}

void interrupt() {
}

void requestInterrupt() {
}

void processCommands() {
    latch = REG_LATCH1;
        
    if (lastLatch == latch)
        return;
    
    lastLatch = latch;
    
    if (latch == Z80_NO_OP)
        return;
    
    latchData = REG_LATCH2;
    handleCommand(latch, latchData);
}

void main() {
    // We don't use interrupts to handle everything as fast as possible
    // otherwise we get slight timing differences
    while(1) {
        processCommands();
    }
}
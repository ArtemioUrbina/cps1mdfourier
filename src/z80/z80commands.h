
/* Communication with 68K functions */

#define Z80_INIT        0x00
#define Z80_PULSE_ON    0x01
#define Z80_PULSE_OFF   0x02
#define Z80_RESET_PULSE 0x03
#define Z80_SILENCE     0x04
#define Z80_PLAY_NOTE   0x05
#define Z80_STOP_NOTE   0x06
#define Z80_ADPCM_QLOW  0x07
#define Z80_ADPCM_QHI   0x08
#define Z80_ADPCM_PLAY  0x09
#define Z80_ADPCM_REDCT 0x10
#define Z80_ADPCM_STOP  0x11
#define Z80_NO_OP       0xFF


/* ADPCM Params */
#define PZ80_ADPCM_NORM 0x01
#define PZ80_ADPCM_LONG 0x02
#define PZ80_ADPCM_1KHI 0x03
#define PZ80_ADPCM_1KLO 0x04

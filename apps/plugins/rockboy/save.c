#include "rockmacros.h"

#include "defs.h"
#include "cpu-gb.h"
#include "cpuregs.h"
#include "hw.h"
#include "regs.h"
#include "lcd-gb.h"
#include "rtc-gb.h"
#include "mem.h"
#include "sound.h"
#include "save.h"

#ifdef ROCKBOX_LITTLE_ENDIAN
#define LIL(x) (x)
#else
#define LIL(x) ((x<<24)|((x&0xff00)<<8)|((x>>8)&0xff00)|(x>>24))
#endif

#define I1(s, p) { 1, { s }, p }
#define I2(s, p) { 2, { s }, p }
#define I4(s, p) { 4, { s }, p }
#define R(r) I1(#r, &R_##r)
#define NOSAVE { -1, { "\0\0\0\0" }, 0 }
#define END { 0, { "\0\0\0\0" }, 0 }

struct svar
{
    int len;
    union
    {
        char key_[4];
        un32 key;
    } k;
    void *ptr;
};

static int ver;
static int sramblock, iramblock, vramblock;
static int hramofs, hiofs, palofs, oamofs, wavofs;

struct svar svars[] = 
{
    I4("GbSs", &ver),
    
    I2("PC  ", &PC),
    I2("SP  ", &SP),
    I2("HL  ", &HL),
#ifdef DYNAREC
    I1("A   ", &A),
    I1("B   ", &A),
    I1("C   ", &A),
    I1("D   ", &A),
    I1("E   ", &A),
    I1("F   ", &A),
#else
    I2("BC  ", &BC),
    I2("DE  ", &DE),
    I2("AF  ", &AF),
#endif    
    I4("IME ", &cpu.ime),
    I4("ima ", &cpu.ima),
    I4("spd ", &cpu.speed),
    I4("halt", &cpu.halt),
    I4("div ", &cpu.div),
    I4("tim ", &cpu.tim),
    I4("lcdc", &cpu.lcdc),
    I4("snd ", &cpu.snd),
    
    I1("ints", &hw.ilines),
    I1("pad ", &hw.pad),
    I4("cgb ", &hw.cgb),
    
    I4("mbcm", &mbc.model),
    I4("romb", &mbc.rombank),
    I4("ramb", &mbc.rambank),
    I4("enab", &mbc.enableram),
    I4("batt", &mbc.batt),
    
    I4("rtcR", &rtc.sel),
    I4("rtcL", &rtc.latch),
    I4("rtcC", &rtc.carry),
    I4("rtcS", &rtc.stop),
    I4("rtcd", &rtc.d),
    I4("rtch", &rtc.h),
    I4("rtcm", &rtc.m),
    I4("rtcs", &rtc.s),
    I4("rtct", &rtc.t),
    I1("rtR8", &rtc.regs[0]),
    I1("rtR9", &rtc.regs[1]),
    I1("rtRA", &rtc.regs[2]),
    I1("rtRB", &rtc.regs[3]),
    I1("rtRC", &rtc.regs[4]),

    I4("S1on", &snd.ch[0].on),
    I4("S1p ", &snd.ch[0].pos),
    I4("S1c ", &snd.ch[0].len),
    I4("S1ec", &snd.ch[0].enlen),
    I4("S1sc", &snd.ch[0].swlen),

    I4("S2on", &snd.ch[1].on),
    I4("S2p ", &snd.ch[1].pos),
    I4("S2c ", &snd.ch[1].len),
    I4("S2ec", &snd.ch[1].enlen),

    I4("S3on", &snd.ch[2].on),
    I4("S3p ", &snd.ch[2].pos),
    I4("S3c ", &snd.ch[2].len),

    I4("S4on", &snd.ch[3].on),
    I4("S4p ", &snd.ch[3].pos),
    I4("S4c ", &snd.ch[3].len),
    I4("S4ec", &snd.ch[3].enlen),

    I4("hdma", &hw.hdma),
    
    I4("sram", &sramblock),
    I4("iram", &iramblock),
    I4("vram", &vramblock),
    I4("hi  ", &hiofs),
    I4("pal ", &palofs),
    I4("oam ", &oamofs),
    
    /* NOSAVE is a special code to prevent the rest of the table
     * from being saved, used to support old stuff for backwards
     * compatibility... */
    NOSAVE,

    /* the following are obsolete as of 0x104 */
    
    I4("hram", &hramofs),
    /* I4("gba ", &hw.gba), */
    /* I4("S1sf", &snd.ch[0].swfreq), */
    I4("wav ", &wavofs),
    
    R(P1), R(SB), R(SC),
    R(DIV), R(TIMA), R(TMA), R(TAC),
    R(IE), R(IF),
    R(LCDC), R(STAT), R(LY), R(LYC),
    R(SCX), R(SCY), R(WX), R(WY),
    R(BGP), R(OBP0), R(OBP1),
    R(DMA),

    R(VBK), R(SVBK), R(KEY1),
    R(BCPS), R(BCPD), R(OCPS), R(OCPD),

    R(NR10), R(NR11), R(NR12), R(NR13), R(NR14),
    R(NR21), R(NR22), R(NR23), R(NR24),
    R(NR30), R(NR31), R(NR32), R(NR33), R(NR34),
    R(NR41), R(NR42), R(NR43), R(NR44),
    R(NR50), R(NR51), R(NR52),

    I1("DMA1", &R_HDMA1),
    I1("DMA2", &R_HDMA2),
    I1("DMA3", &R_HDMA3),
    I1("DMA4", &R_HDMA4),
    I1("DMA5", &R_HDMA5),
    
    END
};


void loadstate(int fd)
{
    int i, j;
    byte buf[4096];
    un32 (*header)[2] = (un32 (*)[2])buf;
    un32 d;
    int irl = hw.cgb ? 8 : 2;
    int vrl = hw.cgb ? 4 : 2;
    int srl = mbc.ramsize << 1;
    size_t base_offset;

    ver = hramofs = hiofs = palofs = oamofs = wavofs = 0;

    base_offset = lseek(fd, 0, SEEK_CUR);
  
    read(fd,buf, 4096);
    
    for (j = 0; header[j][0]; j++)
    {
        for (i = 0; svars[i].ptr; i++)
        {
            if (header[j][0] != svars[i].k.key)
                continue;
            d = LIL(header[j][1]);
            switch (svars[i].len)
            {
            case 1:
                *(byte *)svars[i].ptr = d;
                break;
            case 2:
                *(un16 *)svars[i].ptr = d;
                break;
            case 4:
                *(un32 *)svars[i].ptr = d;
                break;
            }
            break;
        }
    }

    /* obsolete as of version 0x104 */
    if (hramofs) memcpy(ram.hi+128, buf+hramofs, 127);
    if (wavofs) memcpy(ram.hi+48, buf+wavofs, 16);
    
    if (hiofs) memcpy(ram.hi, buf+hiofs, sizeof ram.hi);
    if (palofs) memcpy(lcd.pal, buf+palofs, sizeof lcd.pal);
    if (oamofs) memcpy(lcd.oam.mem, buf+oamofs, sizeof lcd.oam);

    lseek(fd, base_offset + (iramblock << 12), SEEK_SET);
    read(fd,ram.ibank, 4096*irl);
    
    lseek(fd, base_offset + (vramblock << 12), SEEK_SET);
    read(fd,lcd.vbank, 4096*vrl);
    
    lseek(fd, base_offset + (sramblock << 12), SEEK_SET);
    read(fd,ram.sbank, 4096*srl);
    vram_dirty();
    pal_dirty();
    sound_dirty();
    mem_updatemap();            
}

void savestate(int fd)
{
    int i;
    byte buf[4096];
    un32 (*header)[2] = (un32 (*)[2])buf;
    un32 d = 0;
    int irl = hw.cgb ? 8 : 2;
    int vrl = hw.cgb ? 4 : 2;
    int srl = mbc.ramsize << 1;
    size_t base_offset;

    ver = 0x104;
    iramblock = 1;
    vramblock = 1+irl;
    sramblock = 1+irl+vrl;
    hiofs = 4096 - 768;
    palofs = 4096 - 512;
    oamofs = 4096 - 256;
    memset(buf, 0, sizeof buf);

    for (i = 0; svars[i].len > 0; i++)
    {
        header[i][0] = svars[i].k.key;
        switch (svars[i].len)
        {
        case 1:
            d = *(byte *)svars[i].ptr;
            break;
        case 2:
            d = *(un16 *)svars[i].ptr;
            break;
        case 4:
            d = *(un32 *)svars[i].ptr;
            break;
        }
        header[i][1] = LIL(d);
    }
    header[i][0] = header[i][1] = 0;

    memcpy(buf+hiofs, ram.hi, sizeof ram.hi);
    memcpy(buf+palofs, lcd.pal, sizeof lcd.pal);
    memcpy(buf+oamofs, lcd.oam.mem, sizeof lcd.oam);

    /* calculate base offset for output file */
    /* (we'll seek relative to that from now on) */
    base_offset = lseek(fd, 0, SEEK_CUR);
    write(fd,buf, 4096);
    
    lseek(fd, base_offset + (iramblock << 12), SEEK_SET);
    write(fd,ram.ibank, 4096*irl);
    
    lseek(fd, base_offset + (vramblock << 12), SEEK_SET);
    write(fd,lcd.vbank, 4096*vrl);
    
    lseek(fd, base_offset + (sramblock << 12), SEEK_SET);
    write(fd,ram.sbank, 4096*srl);
}

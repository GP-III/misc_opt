/* private object for gameboy
 * 
 * Copyright (C) 2018 moecmks
 * This file is part of KS3578.
 * 
 * do What The Fuck you want to Public License
 * 
 * Version 1.0, March 2000
 * Copyright (C) 2000 Banlu Kemiyatorn (]d).
 * 136 Nives 7 Jangwattana 14 Laksi Bangkok
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 * 
 * Ok, the purpose of this license is simple
 * and you just
 * 
 * DO WHAT THE FUCK YOU WANT TO.
 */

#ifndef _INTERNAL_H
#define _INTERNAL_H 1

#include "conf.h"
#include "gameboy.h"

struct gameboy;
struct controller;
struct divider;
struct timer;
struct cpu;
struct ppu;
struct cartidge;
struct serial;
struct apu;
struct machine_setup;
struct romheader;
/*-\
\-*/
static const 
struct machine_setup {
  ks_double cpu_freq;  
  ks_double vsync_freq;
  ks_double hsync_freq;
  ks_double clk_ms;
  ks_double clk_ns;
  ks_double frame_cycles; 
  ks_double line_cycles;
  ks_double oam_cycles;
  ks_double oambg_b_cycles;
  ks_double oambg_hbl_cycles; 
  ks_int32 line_total;
  ks_int32 line_vbl_st;
  ks_double vbl_clk_last;
  ks_double oam_clk_st;
  ks_double oambg_clk_st;
  ks_double hbl_clk_st_b; 
  ks_double vbl_clk_st; 
  ks_double oam_clk_pick_per;
  ks_double oam_clk_add_hbl_per;
  ks_double tim0507_vclk;
  ks_double cgb_gbp_p;
  ks_double gbp_cgb_p;
  ks_double div04_clks256;
  ks_double oamdma_clks;
  ks_double gdma_clks_b;
  ks_double gdma_clks_per16; // always 7.63 us (DMG/CGB)
  ks_double hdma_clks_per16; // always 8 us (DMG/CGB)
  ks_double joypad_gen_freq; 
} std_machine = {
  4194304.0, /* cpu_freq*/  
  59.73, /* vsync_freq*/
  9198.42, /* hsync_freq*/
  4194304.0/1000.0, /* clk_ms*/
  4194304.0/1000000.0, /* clk_ns*/
  4194304.0/59.73, /* frame_cycles*/ 
  4194304.0/59.73/154.0, /* line_cycles*/
  80.992010239999985, /* oam_cycles*/
  173.51835647999999, /* oambg_b_cycles*/
  (70221.061443161/154.0) - 80.992010239999985, /* oambg_hbl_cycles*/ 
  154, /* line_total*/
  144, /* line_vbl_st*/
  4559.809184621, /* vbl_clk_last*/
  0.0, /* oam_clk_st*/
  80.99201023999998, /* oambg_clk_st*/
  173.51835647999999 + 80.992010239999985, /* hbl_clk_st_b*/ 
  70221.061443161 - 4559.809184621, /* vbl_clk_st*/ 
  80.992010239999985 / 10.0, /* oam_clk_pick_per*/
  12.549357568000001, /* oam_clk_add_hbl_per*/
  4096.0 / 4194304.0, /* tim0507_vclk*/
  8400000.0/ 4194304.0, /* cgb_gbp_p*/
  4194304.0/ 8400000.0, /* gbp_cgb_p*/
  16384.0/ 4194304.0, /* div04_clks256*/
  671.08864000000005, /* oamdma_clks*/
  (4194304.0*220.0)/1000000.0, /* gdma_clks_b*/
  (4194304.0*7.63)/1000000.0,/* gdma_clks_per16*/
  (4194304.0*8.00)/1000000.0,/* hdma_clks_per16*/
  (4194304.0)/JOYPAD_FREQ_IN_DMG /* joypad_gen_freq*/ 
}, adv_machine = {	
  8400000.0, /* cpu_freq*/  
  59.73, /* vsync_freq*/
  9198.42, /* hsync_freq*/
  8400000.0/1000.0, /* clk_ms*/
  8400000.0/1000000.0, /* clk_ns*/
  8400000.0/59.73, /* frame_cycles*/ 
  8400000.0/59.73/154.0, /* line_cycles*/
  162.20400000000001, /* oam_cycles*/
  347.50799999999998, /* oambg_b_cycles*/
  (8400000.0/59.73/154.0) - 162.20400000000001, /* oambg_hbl_cycles*/ 
  154, /* line_total*/
  144, /* line_vbl_st*/
  9132.0031048810633, /* vbl_clk_last*/
  0.0, /* oam_clk_st*/
  162.20400000000001, /* oambg_clk_st*/
  347.50799999999998 + 162.20400000000001, /* hbl_clk_st_b*/ 
  (8400000.0/59.73/154.0) - 9132.0031048810633, /* vbl_clk_st*/ 
  162.20400000000001 / 10.0, /* oam_clk_pick_per*/
  25.132800000000003, /* oam_clk_add_hbl_per*/
#ifdef TIMER_DIVIDER_SAME_FREQ_BOTH_CBG_DMG_BASE_DMG
  4096.0 / 8400000.0, /* tim0507_vclk, the clks obtained here may have greater errors.*/
#else 
  8192.0 / 8400000.0, /* tim0507_vclk, the clks obtained here may have greater errors.*/
#endif 
  8400000.0/ 4194304.0, /* cgb_gbp_p*/
  4194304.0/ 8400000.0, /* gbp_cgb_p*/
#ifdef TIMER_DIVIDER_SAME_FREQ_BOTH_CBG_DMG_BASE_DMG
  16384.0 / 8400000.0, /* div04_clks256, the clks obtained here may have greater errors.*/
#else 
  32768.0 / 8400000.0,,/* div04_clks256, the clks obtained here may have greater errors.*/
#endif
  671.08864000000005, /* oamdma_clks*/
  (8400000.0*110.0)/1000000.0, /* gdma_clks_b*/
  (8400000.0*7.63)/1000000.0,/* gdma_clks_per16*/
  (8400000.0*8.00)/1000000.0,/* hdma_clks_per16*/
  (8400000.0)/JOYPAD_FREQ_IN_CGB /* joypad_gen_freq*/ 
};

struct romheader {
  ks_uint8 title[16];
  ks_uint16 curlic;
  ks_uint8 sgb;
  ks_uint8 ctype;
  ks_uint8 promsize;
  ks_uint8 ramsize;
  ks_uint8 targetcode;
  ks_uint8 anclic;
  ks_uint8 maskver;
  ks_uint8 hdcrc;
  ks_uint16 gcrc;
};
struct controller  { 
  ks_uint8 reg00_P1;

  /* for gamebot update host keybuf and IRQ */
  void (*clks) (struct controller *);

  void *obj;
  void *ubdata_user;
  /* pad infos for gameboy*.*/
  struct controller_pad gb_pad;
  struct gameboy *gb;
};
struct cpu {
  /*  XXX:memory order dep. don't set struct align!*/
  union { struct { ks_uint8 F; ks_uint8 A; }; ks_uint16 AF; };
  union { struct { ks_uint8 C; ks_uint8 B; }; ks_uint16 BC; };
  union { struct { ks_uint8 E; ks_uint8 D; }; ks_uint16 DE; };
  union { struct { ks_uint8 L; ks_uint8 H; }; ks_uint16 HL; };
  union { struct { ks_uint8 SL; ks_uint8 SH; }; ks_uint16 SP; };
  union { struct { ks_uint8 PL; ks_uint8 PH; }; ks_uint16 PC; };

   /* Interrupt Master Enable*/
  ks_uint8 IME;

   /* for Halt */
  ks_bool halt;  

   /* for stop */
  ks_bool stop; 

   /* for halt bug */
  ks_int _backup;

   /* for speed mode */
  ks_uint8 reg4D_key1;

   /* gameboy object  */
  struct gameboy *gb;              
};
struct oam {
  union {
    struct {
      ks_uint8 y;
      ks_uint8 x;
      ks_uint8 id;
      ks_uint8 attr;
    };
    ks_uint8 blk[4];
  };
};
struct pal {
  union {
    struct {
      ks_uint8 _lo;
      ks_uint8 _hi;
    };
    ks_uint16 rgb15;
    ks_uint8 blk[2];
  };
};

/* for ppu:: spi_ca */
#define PIXEL_SPRITE_NOTRANS 1 
#define PIXEL_SPRITE_BACK 2

/* LCDC Status MASK */
#define LCDCS_MODE_FLAG_HBLANK 0
#define LCDCS_MODE_FLAG_VLANK 1
#define LCDCS_MODE_FLAG_SERACH_OAM 2
#define LCDCS_MODE_FLAG_SERACH_OAMVRAM 3
#define LCDCS_MODE_FLAG_ALL_MASK 3

struct ppu {
  ks_uint16 *bufb; /* for alignmem */
  ks_uint8 ram[0x4000];  /* 8K for DMG, 16 for CGB */
  ks_uint16 spb_cac[176]; /* sprite buffer cache *.*/
  ks_uint16 spi_cac[176];

  struct gameboy *gb;
  struct oam sp[40]; 
  struct pal bg_pal[8][4]; /* pal for CGB */
  struct pal sp_pal[8][4]; /* pal for CGB */
  struct ppu_framebuffer fmebuf;

  ks_uint16 spl_dmg[40];
  ks_uint16 bg_pal_dmg[4]; /* pal for DMG */
  ks_uint16 sp_pal_dmg[2][4]; /* pal for DMG */

  ks_uint16 bg_pal_dmgT[4]; /* pal for DMG */
  ks_uint16 sp_pal_dmgT[2][4]; /* pal for DMG */

  /* ks_bool gameboy_mode;  */
  ks_bool s890623_mode; /* TODO: see http://fms.komkon.org/GameBoy/Tech/LCD.gif*/
 
  /* register cache for scanline  */
  ks_uint8 reg43_SCX_cac;
  ks_uint8 reg42_SCY_cac;
  ks_uint8 reg4A_WY_cac;
  ks_uint8 reg4B_WX_cac;
  
  ks_uint8 reg40_LCDC; 
  ks_uint8 reg41_LCDS;                               
  ks_uint8 reg42_SCY;
  ks_uint8 reg43_SCX;        
  ks_uint8 reg44_LY;
  ks_uint8 reg45_LYC;
  ks_uint8 reg46_DMA;
  ks_uint8 reg47_BGP;
  ks_uint8 reg48_OBP0;
  ks_uint8 reg49_OBP1;
  ks_uint8 reg4A_WY;
  ks_uint8 reg4B_WX;
  ks_uint8 reg4F_VBK;
  ks_uint8 reg51_HDMA1;
  ks_uint8 reg52_HDMA2;
  ks_uint8 reg53_HDMA3;
  ks_uint8 reg54_HDMA4;
  ks_uint8 reg55_HDMA5;
  ks_uint8 reg68_BCPS;
  ks_uint8 reg69_BCPD;
  ks_uint8 reg6A_OCPS;
  ks_uint8 reg6B_OCPD;
  
  ks_int32 vscan; 
  ks_int32 vscanR;
  ks_int32 vscan10;
  ks_int32 vscan40;
  ks_int32 xscan;
  ks_int32 xscanR;
  ks_int32 uscan; 
  ks_int32 uscanR;
  ks_int32 uscan168; 
  ks_int32 vscan_cac; 

  ks_bool oam_flop;
  ks_bool vbl_flop;
  ks_bool hbl_flop;

  ks_bool interrupt45_strike;
  ks_bool interrupt45_flop;
  
  ks_int32 line_cac; /* old scanline*/
  ks_int32 lcdm_cac; /* old lcd mode*/

  ks_bool win_stuff; /* window done assert */

  ks_bool hdma_gen;

  ks_uint16 hdma_src;
  ks_uint16 hdma_dst;
  ks_uint16 hdma_r16;

  ks_double hdma_clk;
  ks_double hbl_clks_st; 
  ks_double oambg_clks_divider21;
  ks_double oambg_clks_total;

  void (*bgwin_render) (struct ppu *_5028, ks_int16 scanline);
  void (*sprite_render) (struct ppu *_5028, ks_int16 scanline);
  void (*device_blit) (struct ppu *_5028, void *obj, struct ppu_framebuffer *fbuf); /*blit for host devcice */

  void *obj;
  void (*clks) (struct ppu *);
};
struct gameboy {
  struct controller *joypad;
  struct cpu *lr35902;
  struct ppu *lh5028;
  struct apu *apu;
  struct cartridge *cart;
  struct apu *apu_;
  struct divider *divider;
  struct timer *timer;
  struct serial *serial;
  struct machine_setup *mach_tools;

  ks_double cpu_clks;
  ks_double cpu_clks_timer; // cur full - block 
  ks_double cpu_clks_joypad; // cur full - block 
  ks_double cpu_clks_divider; // cur full - block 
  ks_double cpu_clks_ppu; // cur full - block 
  ks_double cpu_clks_apu; // cur full - block 
  ks_double cpu_clks_cart; // cur full - block 
  ks_double cpu_clks_serial; // RS232 Serial Port for CGB/ DMG.
  ks_double cpu_clks_dma;
  ks_double deflect_ms;

  ks_uint8 reg01_SB;
  ks_uint8 reg02_SC;

  ks_uint8 reg0F_IF; /* interrupt flags register */
  ks_uint8 reg56_IC;
  ks_uint8 reg70_SVBK;
  ks_uint8 regFF_IE; /* Interrupt enable register */

  /* work ram. */
  ks_uint8 wram[0x8000];
  /* high ram. */
  ks_uint8 hram[0x200];
  /* unknow mem, io read/write */
  ks_uint8 unknow_ram[0x10000];

  /* Assert HDMA Gen */
  ks_bool hdma_gen;

  /* controller drv */
  void (*controller_hostdrv) 
             (struct gameboy *, void *controller_drvobj,
                struct controller_pad *, /* gb-self for recv joypadbuffer */ 
             struct controller_pad * /* host-edge 1?pulse gen:nodone */);
  void *controller_drvobj;
  
  /* display drv */
  void (*display_hostdrv) 
             (struct gameboy *, void *display_drvobj,
                struct ppu_framebuffer *fmebuf);
  void *display_drvobj;
  /* sound drv */


};
struct timer {
  ks_uint8 reg05_TIMA;
  ks_uint8 reg06_TMA;
  ks_uint8 reg07_TAC;
  ks_double freq_tab[4];

  void (*clks) (struct timer *);
  ks_double timestamp;

  struct gameboy *gb;
};
struct divider {
  ks_uint8 reg04_DIV;
  ks_double freq;

  void (*clks) (struct divider *);
  ks_double timestamp;
  struct gameboy *gb;
};
struct apu {
  void (*clks) (struct apu *);
  struct gameboy *gb;
};
struct serial {
  void (*clks) (struct serial *);
  struct gameboy *gb;
};

/* cartridge device type */
#define MBC_0 0
#define MBC_1 1
#define MBC_2 2
#define MBC_3 3
#define MBC_4 4
#define MBC_5 5 
#define MBC_6 6
#define MBC_7 7
#define TAMA5 8
#define HUCL1 9
#define HUCL3 10
#define MMM0 11
#define POCKER_CAM 12

struct cartridge {
  struct romheader header;
  ks_bool s_latch;
  ks_bool battery;
  ks_uint8 *promworks;
  ks_uint8 *sramworks;
  ks_uint8 promsize;
  ks_uint8 sramsize; 

  void (*clks) (struct cartridge *);
  ks_uint8 (*read) (struct cartridge *cartridge, ks_uint16 address);
  ks_void (*write) (struct cartridge *cartridge, ks_uint16 address, ks_uint8 value);

  void *ubdata_user;
  struct gameboy *gb;    
};

#define IRQ_1 0x01  /*  VBLANK (NMI)  */
#define IRQ_2 0x02  /*  LCDC  */
#define IRQ_3 0x04  /*  Programmable timer */
#define IRQ_4 0x08  /*  Serial port switching */
#define IRQ_5 0x10  /*  P14-15 Descent edge acknowledge */
#define IRQ_NIL 0xFF  /*  P14-15 Descent edge acknowledge */

#define IRQ_1_ADDRESS 0x40 /*  VBLANK (NMI)  */
#define IRQ_2_ADDRESS 0x48 /*  LCDC  */
#define IRQ_3_ADDRESS 0x50 /*  Programmable timer */
#define IRQ_4_ADDRESS 0x58 /*  Serial port switching */
#define IRQ_5_ADDRESS 0x60 /*  P14-15 Descent edge acknowledge */

/* gameboy internal done **/
int gameboy_resume_ifstop (struct gameboy *gb);
double gameboy_getcycles (struct gameboy *gb); 
double gameboy_get_ppuclks (struct gameboy *gb, ks_bool *stride_frame); 
void gameboy_setcycles (struct gameboy *gb, double cycles);
ks_uint8 ks_callstd gameboy_mmu_read (struct gameboy *gb, ks_uint16 address);
ks_uint16 ks_callstd gameboy_mmu_read_w (struct gameboy *gb, ks_uint16 address);
ks_void ks_callstd gameboy_mmu_write (struct gameboy *gb, ks_uint16 address, ks_uint8 value);
ks_void ks_callstd gameboy_mmu_write_w (struct gameboy *gb, ks_uint16 address, ks_uint16 value);
int gameboy_resume_ifstop (struct gameboy *gb);
int  gameboy_hdma_copy (struct gameboy *gb);

#endif 
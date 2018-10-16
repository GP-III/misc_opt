/* Game boy's GPU and LCD Screen
 * LCD is Sharp LH5028 http://www.datasheetarchive.com/pdf/download.php?id=c615e5d8551c6b559c3db61b709b3234af856c&type=O&term=LH5028
 *
 * This part of the source code is actually not very good.
 * It looks terrible.
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

#include "gameboy.h"
#include "internal.h"

static 
void default_update (struct ppu *_5028, void *obj, struct ppu_framebuffer *fbuf) {
  printf ("%s:%s please set ppu_update callback( controller_setupdate function)\n", __FILE__, __LINE__);
  assert (0);
}

void ppu_setupdate_ (struct ppu *X5028, void (*update) 
            (struct ppu *, 
            void *, 
              struct ppu_framebuffer *), void *obj)
{
  X5028->device_blit = update;
  X5028->obj = obj;
}

int ppu_reset (struct ppu *X5028) {
  
  /* init register http://gbdev.gg8.se/files/docs/mirrors/pandocs.html#powerupsequence */
  X5028->reg40_LCDC = 0x91;
  X5028->reg42_SCY = 0x00;
  X5028->reg43_SCX = 0x00;
  X5028->reg45_LYC = 0x00;
  X5028->reg47_BGP = 0xFC;
  X5028->reg48_OBP0 = 0xFF;
  X5028->reg49_OBP1 = 0xFF;
  X5028->reg4A_WY = 0x00;
  X5028->reg4B_WX = 0x00;
  /* reset cache */
  X5028->reg42_SCY_cac= X5028->reg42_SCY;
  X5028->reg43_SCX_cac= X5028->reg43_SCX;
  X5028->reg4A_WY_cac= X5028->reg4A_WY;
  X5028->reg4B_WX_cac= X5028->reg4B_WX;

  X5028->hbl_clks_st = 9999.8;
  return 0;
}

void ppu_uninit (struct ppu **X5028);

void ppu_write (struct ppu *X5028, ks_uint16 address, ks_uint8 value) {

  ks_uint8 c, s;
  switch (address) {
  case 0xFF40: /* FF40 - LCDC - LCD Control (R/W) **/
    /*Bit 7 - LCD Display Enable             (0=Off, 1=On)
      Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
      Bit 5 - Window Display Enable          (0=Off, 1=On)
      Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
      Bit 3 - BG Tile Map Display Select     (0=9800-9BFF, 1=9C00-9FFF)
      Bit 2 - OBJ (Sprite) Size              (0=8x8, 1=8x16)
      Bit 1 - OBJ (Sprite) Display Enable    (0=Off, 1=On)
      Bit 0 - BG/Window Display/Priority     (0=Off, 1=On)
     */
    X5028->reg40_LCDC = value;
    break;
  case 0xFF41: /* FF41 - STAT - LCDC Status (R/W) */
    /*Bit 6 - LYC=LY Coincidence Interrupt (1=Enable) (Read/Write)
      Bit 5 - Mode 2 OAM Interrupt         (1=Enable) (Read/Write)
      Bit 4 - Mode 1 V-Blank Interrupt     (1=Enable) (Read/Write)
      Bit 3 - Mode 0 H-Blank Interrupt     (1=Enable) (Read/Write)
      Bit 2 - Coincidence Flag  (0:LYC<>LY, 1:LYC=LY) (Read Only)
      Bit 1-0 - Mode Flag       (Mode 0-3, see below) (Read Only)
              0: During H-Blank
              1: During V-Blank
              2: During Searching OAM
              3: During Transferring Data to LCD Driver
    */
    X5028->reg41_LCDS = value;
    break;
  case 0xFF42: /* FF42 - SCY - Scroll Y (R/W) */
    X5028->reg42_SCY = value;
    break;
  case 0xFF43: /* FF43 - SCX - Scroll X (R/W) */
    X5028->reg43_SCX = value;
    break;
  case 0xFF44: /* FF44 - LY - LCDC Y-Coordinate (R) */
    _DEBUG_BREAK();
    break;
  case 0xFF45: /* FF45 - LYC - LY Compare (R/W) */
    X5028->reg45_LYC = value;
    break;
  case 0xFF4A: /* FF4A - WY - Window Y Position (R/W) */
    X5028->reg4A_WY = value;
    break;
  case 0xFF4B: /* FF4B - WX - Window X Position minus 7 (R/W) */
    X5028->reg4B_WX = value;
    break; 
  case 0xFF47: /* FF47 - BGP - BG Palette Data (R/W) - Non CGB Mode Only */
    X5028->bg_pal_dmg[0] = X5028->bg_pal_dmgT[(value & 0x03) >> 0];
    X5028->bg_pal_dmg[1] = X5028->bg_pal_dmgT[(value & 0x0C) >> 2];
    X5028->bg_pal_dmg[2] = X5028->bg_pal_dmgT[(value & 0x30) >> 4];
    X5028->bg_pal_dmg[3] = X5028->bg_pal_dmgT[(value & 0xC0) >> 6];
    X5028->reg47_BGP = value;
    break ;
  case 0xFF48: /* FF48 - OBP0 - Object Palette 0 Data (R/W) - Non CGB Mode Only */
    X5028->sp_pal_dmg[0][0] = X5028->sp_pal_dmgT[0][(value & 0x03) >> 0];
    X5028->sp_pal_dmg[0][1] = X5028->sp_pal_dmgT[0][(value & 0x0C) >> 2];
    X5028->sp_pal_dmg[0][2] = X5028->sp_pal_dmgT[0][(value & 0x30) >> 4];
    X5028->sp_pal_dmg[0][3] = X5028->sp_pal_dmgT[0][(value & 0xC0) >> 6];
    X5028->reg48_OBP0 = value;
    break ;
  case 0xFF49: /* FF49 - OBP1 - Object Palette 1 Data (R/W) - Non CGB Mode Only */
    X5028->sp_pal_dmg[1][0] = X5028->sp_pal_dmgT[1][(value & 0x03) >> 0];
    X5028->sp_pal_dmg[1][1] = X5028->sp_pal_dmgT[1][(value & 0x0C) >> 2];
    X5028->sp_pal_dmg[1][2] = X5028->sp_pal_dmgT[1][(value & 0x30) >> 4];
    X5028->sp_pal_dmg[1][3] = X5028->sp_pal_dmgT[1][(value & 0xC0) >> 6];
    X5028->reg49_OBP1 = value;
    break ;
  case 0xFF68: /* FF68 - BCPS/BGPI - CGB Mode Only - Background Palette Index */
    X5028->reg68_BCPS = value;
    break ;
  case 0xFF69: /* FF69 - BCPD/BGPD - CGB Mode Only - Background Palette Data */
    s = X5028->reg68_BCPS;
    c = s & 0x3F;
    if (c & 1)
      X5028->bg_pal[c >> 3][(c & 7) >> 1]._hi = value;
    else 
      X5028->bg_pal[c >> 3][(c & 7) >> 1]._lo = value;
    if (s & 0x80) {
      c = (c + 1) & 0x3F;
      X5028->reg68_BCPS &= ~0x3F;
      X5028->reg68_BCPS |= c;
    }
    X5028->reg69_BCPD = value;
    break;
  case 0xFF6A: /* FF6A - OCPS/OBPI - CGB Mode Only - Sprite Palette Index */
    X5028->reg6A_OCPS = value;
    break;
  case 0xFF6B: /* FF6B - OCPD/OBPD - CGB Mode Only - Sprite Palette Data */
    s = X5028->reg6A_OCPS;
    c = s & 0x3F;
    if (c & 1)
      X5028->sp_pal[c >> 3][(c & 7) >> 1]._hi = value;
    else 
      X5028->sp_pal[c >> 3][(c & 7) >> 1]._lo = value;
    if (s & 0x80) {
      c = (c + 1) & 0x3F;
      X5028->reg6A_OCPS &= ~0x3F;
      X5028->reg6A_OCPS |= c;
    }
    X5028->reg6B_OCPD = value;
    break;
  case 0xFF46: /* FF46 - DMA - DMA Transfer and Start Address (R/W) */
    for (c = 0; c != 160; c++) {
      s = gameboy_mmu_read (X5028->gb, value * 256 + c);
      ((ks_int8 *)& X5028->sp[0])[c] = s;
    }
    /* OAMDMA ~ 160 us 
       OAMDMA is a parallel DMA */
    /* X5028->gb->cpu_clks_dma += X5028->gb->mach_tools->oamdma_clks; */
    X5028->reg46_DMA = value;
    break ;
  case 0xFF51: /* FF51 - HDMA1 - CGB Mode Only - New DMA Source, High */
    X5028->reg51_HDMA1 = value;
    break;
  case 0xFF52: /* FF52 - HDMA2 - CGB Mode Only - New DMA Source, Low*/
    X5028->reg52_HDMA2 = value;
    break;
  case 0xFF53: /* FF53 - HDMA3 - CGB Mode Only - New DMA Destination, High */
    X5028->reg53_HDMA3 = value;
    break;
  case 0xFF54: /* FF54 - HDMA4 - CGB Mode Only - New DMA Destination, Low*/
    X5028->reg54_HDMA4 = value;
    break;
  case 0xFF55: /* FF55 - HDMA5 - CGB Mode Only - New DMA Length/Mode/Start */
    if (value & 0x80) {
      /* H-Blank DMA. **/
      X5028->hdma_src = X5028->reg51_HDMA1 * 256 + X5028->reg52_HDMA2;
      X5028->hdma_dst = X5028->reg53_HDMA3 * 256 + X5028->reg54_HDMA4;
      X5028->hdma_src &= 0xFFF0;
      X5028->hdma_dst &= 0xFFF0;
      X5028->hdma_r16 = value & 0x7F;
      X5028->hdma_r16 ++;
      X5028->hdma_gen = ks_true;
      /* set HDMA uncompelete/active */
      X5028->reg55_HDMA5 &= 0x7F;
    } else {
      /* GDMA **/
      ks_uint16 src = X5028->reg51_HDMA1 * 256 + X5028->reg52_HDMA2;
      ks_uint16 dst = X5028->reg53_HDMA3 * 256 + X5028->reg54_HDMA4;
      ks_uint16 id;
      ks_uint16 K;

      src &= 0xFFF0;
      dst &= 0xFFF0;
      K = value & 0x7F;
      K+= 1;
      /* copy it*/
      for (; id = K; id++) {
        ks_uint16 c;
        for (c =0; c != 16; c++) {
          s = gameboy_mmu_read (X5028->gb, src + id *16 + c);
          gameboy_mmu_write (X5028->gb, dst + id *16 + c, s);
        }
      }
      /* burning OAM clks */
      /* It takes (220 + (n * 7.63)) microseconds in single speed 
         and (110 + (n * 7.63)) microseconds in double speed mode */
      X5028->gb->cpu_clks_dma += (X5028->gb->mach_tools->gdma_clks_b 
        + X5028->gb->mach_tools->gdma_clks_per16 * (ks_double) K);
      /* X5028->reg55_HDMA5 |= 0x80;/* set GDMA compelete/unactive */
    }
    X5028->reg55_HDMA5 = value;
    break;
  case 0xFF4F: /* FF4F - VBK - CGB Mode Only - VRAM Bank (R/W) */
    X5028->reg4F_VBK = value;
    break;
  default:
    return;
  }
}

ks_uint8 ppu_read (struct ppu *X5028, ks_uint16 address) {

  switch (address) {
  case 0xFF40: /* FF40 - LCDC - LCD Control (R/W) **/
    /*Bit 7 - LCD Display Enable             (0=Off, 1=On)
      Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
      Bit 5 - Window Display Enable          (0=Off, 1=On)
      Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
      Bit 3 - BG Tile Map Display Select     (0=9800-9BFF, 1=9C00-9FFF)
      Bit 2 - OBJ (Sprite) Size              (0=8x8, 1=8x16)
      Bit 1 - OBJ (Sprite) Display Enable    (0=Off, 1=On)
      Bit 0 - BG/Window Display/Priority     (0=Off, 1=On)
     */
    return X5028->reg40_LCDC;
  case 0xFF41: /* FF41 - STAT - LCDC Status (R/W) */
    /*Bit 6 - LYC=LY Coincidence Interrupt (1=Enable) (Read/Write)
      Bit 5 - Mode 2 OAM Interrupt         (1=Enable) (Read/Write)
      Bit 4 - Mode 1 V-Blank Interrupt     (1=Enable) (Read/Write)
      Bit 3 - Mode 0 H-Blank Interrupt     (1=Enable) (Read/Write)
      Bit 2 - Coincidence Flag  (0:LYC<>LY, 1:LYC=LY) (Read Only)
      Bit 1-0 - Mode Flag       (Mode 0-3, see below) (Read Only)
              0: During H-Blank
              1: During V-Blank
              2: During Searching OAM
              3: During Transferring Data to LCD Driver
    */
    return X5028->reg41_LCDS;
  case 0xFF42: /* FF42 - SCY - Scroll Y (R/W) */
    return X5028->reg42_SCY;
  case 0xFF43: /* FF43 - SCX - Scroll X (R/W) */
    return X5028->reg43_SCX;
  case 0xFF44: /* FF44 - LY - LCDC Y-Coordinate (R) */
    return X5028->reg44_LY;
  case 0xFF45: /* FF45 - LYC - LY Compare (R/W) */
    return X5028->reg45_LYC;
  case 0xFF4A: /* FF4A - WY - Window Y Position (R/W) */
    return X5028->reg4A_WY;
  case 0xFF4B: /* FF4B - WX - Window X Position minus 7 (R/W) */
    return X5028->reg4B_WX;
  case 0xFF47: /* FF47 - BGP - BG Palette Data (R/W) - Non CGB Mode Only */
    return X5028->reg47_BGP;
  case 0xFF48: /* FF48 - OBP0 - Object Palette 0 Data (R/W) - Non CGB Mode Only */
    return X5028->reg48_OBP0;
  case 0xFF49: /* FF49 - OBP1 - Object Palette 1 Data (R/W) - Non CGB Mode Only */
    return X5028->reg49_OBP1;
  case 0xFF68: /* FF68 - BCPS/BGPI - CGB Mode Only - Background Palette Index */
    return X5028->reg68_BCPS;
  case 0xFF69: /* FF69 - BCPD/BGPD - CGB Mode Only - Background Palette Data */
    return X5028->reg69_BCPD;
  case 0xFF6A: /* FF6A - OCPS/OBPI - CGB Mode Only - Sprite Palette Index */
    return X5028->reg6A_OCPS;
  case 0xFF6B: /* FF6B - OCPD/OBPD - CGB Mode Only - Sprite Palette Data */
    return X5028->reg6B_OCPD;
  case 0xFF46: /* FF46 - DMA - DMA Transfer and Start Address (R/W) */
    return X5028->reg46_DMA;
  case 0xFF51: /* FF51 - HDMA1 - CGB Mode Only - New DMA Source, High */
    return X5028->reg51_HDMA1;
  case 0xFF52: /* FF52 - HDMA2 - CGB Mode Only - New DMA Source, Low*/
    return X5028->reg52_HDMA2;
  case 0xFF53: /* FF53 - HDMA3 - CGB Mode Only - New DMA Destination, High */
    return X5028->reg53_HDMA3;
  case 0xFF54: /* FF54 - HDMA4 - CGB Mode Only - New DMA Destination, Low*/
    return X5028->reg54_HDMA4;
  case 0xFF55: /* FF55 - HDMA5 - CGB Mode Only - New DMA Length/Mode/Start */
    return X5028->reg55_HDMA5;
  case 0xFF4F: /* FF4F - VBK - CGB Mode Only - VRAM Bank (R/W) */
    return X5028->reg4F_VBK;
    break;
  default:
    return 0xFF;
  }
}
static /* this method for gameboy color */
void bgwin_render_cgb (struct ppu *X5028, ks_int16 scanline) {
  struct {
    union { ks_uint16 blk;
      struct { ks_uint8 lo; ks_uint8 hi; }; };
  } chrdat,  chrcac;
  /* always scan 168 pixel in every line (21 tiles), 
    evenif omfx is ZERO .
      fit buffer offset, so that every time we can scan a complete tile, 
          no matter how much omfx is.
    */
  ks_int32 omfx;
  ks_int32 ofx; 
  ks_int32 obx; 
  ks_int32 omfy; 
  ks_int32 ofy; 
  ks_int32 vsc;
  ks_uint8 tid;
  ks_int8 tat;
  ks_int32 tidaddr;
  ks_uint16 pixcac;
  ks_uint8 *tdat;
  ks_int32 rxpos;
  ks_uint32 c, q, c2, s;
  ks_int32 c3;
  ks_uint16 *vptrWinDrawStart;
  ks_uint16 *vptrScrollStart;
  /* check current scan region in BG or WINDOW (if WINDOW enable)*/
  if ( X5028->win_stuff != ks_false && (X5028->reg40_LCDC & 0x20)) {
    /* draw window  */
    goto windraw;
  } else {
    /* draw background */
    vsc = X5028->uscanR;
    omfx = X5028->reg43_SCX_cac & 7;
    ofx = X5028->reg43_SCX_cac >> 3; 
    omfy = X5028->reg42_SCY_cac & 7; 
    ofy = X5028->reg42_SCY_cac >> 3;
    ofx = ofx + vsc;
    ofy = ofy + scanline;
    omfy = ofy & 7;
    ofy = ofy >> 3;
    ofx = ofx - (ofx & 32); 
    ofy = ofy - (ofy & 32);  
    obx = vsc << 3;
    vptrScrollStart = & X5028->fmebuf.buf[scanline *(X5028->fmebuf.pitch/sizeof (X5028->fmebuf.buf[0]))-omfx];
    /* pick tileid and attr from ::Bit 3 - BG Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF) */
    tidaddr = 0x9800 + (((X5028->reg40_LCDC & 0x08) >> 3) << 10);
    tidaddr = (tidaddr-0x8000)+(ofy<< 5)+ofx;
    tid = X5028->ram[tidaddr]; // fetch tileid 
    tat = X5028->ram[tidaddr+0x2000]; // fetch tileattr. 
    tdat = & X5028->ram[((tat & 0x08)>>3)<<13]; // bank select.
#   if  1
    if (X5028->reg40_LCDC & 0x10) // 0x8000 unsigned address 
      tdat = & tdat[tid<<4];
    else // 
      tdat = & tdat[0x1000+(((ks_int8)tid)<<4)]; // TODO: done.
#   else 
    cti16 = (X5028->reg40_LCDC & 0x10) << 8;
    cti16^= 0x1000;
    tdat = & tdat[cti16+((ks_int8)(cti16 >>5)) & (((ks_int8)tid) << 4)];
#   endif 
#   if  1
    if (tat & 0x40) // check vflip ? 
      tdat = & tdat[(7-omfy)*2];
    else 
      tdat = & tdat[omfy*2];
#   else 
    ctu8 = (tat & 0x40) >> 3; // 8
    ctu8 = ctu8 - (ctu8 >> 3);// 0 or 7
    tdat = & tdat[(ctu8^omfy)<<1];
#   endif 
    chrdat.blk = *(ks_uint16 *)tdat;
    /* check x flip  */
    if (tat & 0x20) {
      chrcac.blk = chrdat.blk & 0x8080;
      pixcac = pixcac | (chrcac.lo << 7) | (chrcac.hi << 8);
      chrcac.blk = chrdat.blk & 0x4040;
      pixcac = pixcac | (chrcac.lo << 6) | (chrcac.hi << 7);
      chrcac.blk = chrdat.blk & 0x2020;
      pixcac = pixcac | (chrcac.lo << 5) | (chrcac.hi << 6);
      chrcac.blk = chrdat.blk & 0x1010;
      pixcac = pixcac | (chrcac.lo << 4) | (chrcac.hi << 5);
      chrcac.blk = chrdat.blk & 0x0808;
      pixcac = pixcac | (chrcac.lo << 3) | (chrcac.hi << 4);
      chrcac.blk = chrdat.blk & 0x0404;
      pixcac = pixcac | (chrcac.lo << 2) | (chrcac.hi << 3);
      chrcac.blk = chrdat.blk & 0x0202;
      pixcac = pixcac | (chrcac.lo << 1) | (chrcac.hi << 2);
      chrcac.blk = chrdat.blk & 0x0101;
      pixcac = pixcac | (chrcac.lo << 0) | (chrcac.hi << 1);        
    } else {
      chrcac.blk = chrdat.blk & 0x8080;
      pixcac = pixcac | (chrcac.lo >> 7) | (chrcac.hi >> 6);
      chrcac.blk = chrdat.blk & 0x4040;
      pixcac = pixcac | (chrcac.lo >> 4) | (chrcac.hi >> 3);
      chrcac.blk = chrdat.blk & 0x2020;
      pixcac = pixcac | (chrcac.lo >> 1) | (chrcac.hi >> 0);
      chrcac.blk = chrdat.blk & 0x1010;
      pixcac = pixcac | (chrcac.lo << 2) | (chrcac.hi << 3);
      chrcac.blk = chrdat.blk & 0x0808;
      pixcac = pixcac | (chrcac.lo << 5) | (chrcac.hi << 6);
      chrcac.blk = chrdat.blk & 0x0404;
      pixcac = pixcac | (chrcac.lo << 8) | (chrcac.hi << 9);
      chrcac.blk = chrdat.blk & 0x0202;
      pixcac = pixcac | (chrcac.lo <<11) | (chrcac.hi <<12);
      chrcac.blk = chrdat.blk & 0x0101;
      pixcac = pixcac | (chrcac.lo <<14) | (chrcac.hi <<15);
    }
    rxpos = obx - omfx;
    q = tat&7;
    if (!(X5028->reg40_LCDC & 0x01)) {
      vptrScrollStart = & vptrScrollStart[obx];
      /* When Bit 0 is cleared, the background and window lose their priority,
        the sprites will be always displayed on top of background and window, 
        independently of the priority flags in OAM and BG Map attributes.
        */ 
      for (c = 0; c != 8; c++) {
        s = rxpos+c;
        if (X5028->spi_cac[s] & PIXEL_SPRITE_NOTRANS) // spline ptr to +8
          vptrScrollStart[c] = X5028->spb_cac[s];
        else 
          vptrScrollStart[c] = X5028->bg_pal[q][pixcac & 3].rgb15;
        pixcac >>= 2;
      }
    } else if (tat & 0x80) { // BG pri. 
      vptrScrollStart = & vptrScrollStart[obx];
      for (c = 0; c != 8; c++) {
        vptrScrollStart[c] = X5028->bg_pal[q][pixcac & 3].rgb15;
        pixcac >>= 2;
      }
    } else  {
      for (c = 0; c != 8; c++) {
        s = rxpos+c;
        c2 = pixcac & 3;
        if (X5028->spi_cac[s] & PIXEL_SPRITE_NOTRANS)
          if (!(X5028->spi_cac[s] & PIXEL_SPRITE_BACK))
            vptrScrollStart[c] = X5028->spb_cac[s];
          else if ( c2 == 0)
            vptrScrollStart[c] = X5028->spb_cac[s];
          else 
            vptrScrollStart[c] = X5028->bg_pal[q][c2].rgb15;
        else 
          vptrScrollStart[c] = X5028->bg_pal[q][c2].rgb15;
        pixcac >>= 2;
      }
    }
    rxpos = obx - omfx; // -7 | 25
    rxpos+= 8; // 1 | 33
    rxpos+= 7; // 8 | 40 
    if ( X5028->win_stuff == ks_false && (X5028->reg40_LCDC & 0x20)
        && (X5028->reg4B_WX_cac <= 166 && X5028->reg4B_WX_cac < rxpos) /* check X**/
        && (X5028->reg4A_WY_cac <= scanline && (X5028->reg4A_WY_cac <= 143))) 
    {
      X5028->win_stuff = ks_true;
      X5028->xscanR = 0;
      q = 15 - omfx; // 8 | 16-> 9 
      // 7->0
      // 8->1 
      while (X5028->reg4B_WX_cac >= q) // 15 >= q / 16
        {
          X5028->xscanR ++; //  1 or 2 
          q += 8;
        }
      goto windraw;
    }
  }
  return ;
windraw:
  ofx = X5028->uscanR - X5028->xscanR; // espl x 
  c = scanline - X5028->reg4A_WY_cac; // total - y 
  omfx = 0;
  omfy = c & 7; 
  ofy = c >> 3;
  c3 = ((ks_int8)X5028->reg4B_WX_cac)-7;
  c3 = c3 + (ofx<<3);
  vptrWinDrawStart = & X5028->fmebuf.buf[scanline *(X5028->fmebuf.pitch/sizeof (X5028->fmebuf.buf[0]))+c3];
  /* pick tileid and attr from ::Bit 6 - Tile Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF) */
  tidaddr = 0x9800 + (((X5028->reg40_LCDC & 0x40) >> 6) << 10);
  tidaddr = (tidaddr-0x8000)+(ofy<< 5)+ofx;
  tid = X5028->ram[tidaddr]; // fetch tileid 
  tat = X5028->ram[tidaddr+0x2000]; // fetch tileattr. 
  tdat = & X5028->ram[((tat & 0x08)>>3)<<13]; // bank select.
#   if  1
  if (X5028->reg40_LCDC & 0x10) // 0x8000 unsigned address 
    tdat = & tdat[tid<<4];
  else // 
    tdat = & tdat[0x1000+(((ks_int8)tid)<<4)]; // TODO: done.
#   else 
  cti16 = (X5028->reg40_LCDC & 0x10) << 8;
  cti16^= 0x1000;
  tdat = & tdat[cti16+((ks_int8)(cti16 >>5)) & (((ks_int8)tid) << 4)];
#   endif 
#   if  1
  if (tat & 0x40) // check vflip ? 
    tdat = & tdat[(7-omfy)*2];
  else 
    tdat = & tdat[omfy*2];
#   else 
  ctu8 = (tat & 0x40) >> 3; // 8
  ctu8 = ctu8 - (ctu8 >> 3);// 0 or 7
  tdat = & tdat[(ctu8^omfy)<<1];
#   endif 
  chrdat.blk = *(ks_uint16 *)tdat;
  /* check x flip  */
  if (tat & 0x20) {
    chrcac.blk = chrdat.blk & 0x8080;
    pixcac = pixcac | (chrcac.lo << 7) | (chrcac.hi << 8);
    chrcac.blk = chrdat.blk & 0x4040;
    pixcac = pixcac | (chrcac.lo << 6) | (chrcac.hi << 7);
    chrcac.blk = chrdat.blk & 0x2020;
    pixcac = pixcac | (chrcac.lo << 5) | (chrcac.hi << 6);
    chrcac.blk = chrdat.blk & 0x1010;
    pixcac = pixcac | (chrcac.lo << 4) | (chrcac.hi << 5);
    chrcac.blk = chrdat.blk & 0x0808;
    pixcac = pixcac | (chrcac.lo << 3) | (chrcac.hi << 4);
    chrcac.blk = chrdat.blk & 0x0404;
    pixcac = pixcac | (chrcac.lo << 2) | (chrcac.hi << 3);
    chrcac.blk = chrdat.blk & 0x0202;
    pixcac = pixcac | (chrcac.lo << 1) | (chrcac.hi << 2);
    chrcac.blk = chrdat.blk & 0x0101;
    pixcac = pixcac | (chrcac.lo << 0) | (chrcac.hi << 1);        
  } else {
    chrcac.blk = chrdat.blk & 0x8080;
    pixcac = pixcac | (chrcac.lo >> 7) | (chrcac.hi >> 6);
    chrcac.blk = chrdat.blk & 0x4040;
    pixcac = pixcac | (chrcac.lo >> 4) | (chrcac.hi >> 3);
    chrcac.blk = chrdat.blk & 0x2020;
    pixcac = pixcac | (chrcac.lo >> 1) | (chrcac.hi >> 0);
    chrcac.blk = chrdat.blk & 0x1010;
    pixcac = pixcac | (chrcac.lo << 2) | (chrcac.hi << 3);
    chrcac.blk = chrdat.blk & 0x0808;
    pixcac = pixcac | (chrcac.lo << 5) | (chrcac.hi << 6);
    chrcac.blk = chrdat.blk & 0x0404;
    pixcac = pixcac | (chrcac.lo << 8) | (chrcac.hi << 9);
    chrcac.blk = chrdat.blk & 0x0202;
    pixcac = pixcac | (chrcac.lo <<11) | (chrcac.hi <<12);
    chrcac.blk = chrdat.blk & 0x0101;
    pixcac = pixcac | (chrcac.lo <<14) | (chrcac.hi <<15);
  }
  q = tat&7;
  if (!(X5028->reg40_LCDC & 0x01)) {
    for (c = 0; c != 8; c++) {
      s = c3+c;
      if (X5028->spi_cac[s] & PIXEL_SPRITE_NOTRANS) // spline ptr to +8
        vptrWinDrawStart[c] = X5028->spb_cac[s];
      else 
        vptrWinDrawStart[c] = X5028->bg_pal[q][pixcac & 3].rgb15;
      pixcac >>= 2;
    }
  } else if (tat & 0x80) { // BG pri. 
    for (c = 0; c != 8; c++) {
      vptrWinDrawStart[c] = X5028->bg_pal[q][pixcac & 3].rgb15;
      pixcac >>= 2;
    }
  } else  {
    for (c = 0; c != 8; c++) {
      s = c3+c;
      c2 = pixcac & 3;
      if (X5028->spi_cac[s] & PIXEL_SPRITE_NOTRANS)
        if (!(X5028->spi_cac[s] & PIXEL_SPRITE_BACK))
          vptrWinDrawStart[c] = X5028->spb_cac[s];
        else if ( c2 == 0)
          vptrWinDrawStart[c] = X5028->spb_cac[s];
        else 
          vptrWinDrawStart[c] = X5028->bg_pal[q][c2].rgb15;
      else 
        vptrWinDrawStart[c] = X5028->bg_pal[q][c2].rgb15;
      pixcac >>= 2;
    }
  }
}
static /* this method for gameboy */
void bgwin_render_dmg (struct ppu *X5028, ks_int16 scanline) {
  struct {
    union { ks_uint16 blk;
      struct { ks_uint8 lo; ks_uint8 hi; }; };
  } chrdat,  chrcac;
  /* always scan 168 pixel in every line (21 tiles), 
    evenif omfx is ZERO .
      fit buffer offset, so that every time we can scan a complete tile, 
          no matter how much omfx is.
    */
  ks_int32 omfx;
  ks_int32 ofx; 
  ks_int32 obx; 
  ks_int32 omfy; 
  ks_int32 ofy; 
  ks_int32 vsc;
  ks_uint8 tid;
  ks_int32 tidaddr;
  ks_uint16 pixcac;
  ks_uint8 *tdat;
  ks_int32 rxpos;
  ks_uint32 c, q, c2, s;
  ks_int32 c3;
  ks_uint16 *vptrWinDrawStart;
  ks_uint16 *vptrScrollStart;
  /* check current scan region in BG or WINDOW (if WINDOW enable)*/
  if ( X5028->win_stuff != ks_false && (X5028->reg40_LCDC & 0x20)) {
    /* draw window  */
    goto windraw;
  } else {
    /* draw background */
    vsc = X5028->uscanR;
    omfx = X5028->reg43_SCX_cac & 7;
    ofx = X5028->reg43_SCX_cac >> 3; 
    omfy = X5028->reg42_SCY_cac & 7; 
    ofy = X5028->reg42_SCY_cac >> 0;
    ofx = ofx + vsc;
    ofy = ofy + scanline;
    omfy = ofy & 7;
    ofy = ofy >> 3;
    ofx = ofx - (ofx & 32); 
    ofy = ofy - (ofy & 32);  
    obx = vsc << 3;
    vptrScrollStart = & X5028->fmebuf.buf[scanline *(X5028->fmebuf.pitch/sizeof (X5028->fmebuf.buf[0]))-omfx];
    /* pick tileid and attr from ::Bit 3 - BG Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF) */
    tidaddr = 0x9800 + (((X5028->reg40_LCDC & 0x08) >> 3) << 10);
    tidaddr = (tidaddr-0x8000)+(ofy<< 5)+ofx;
    tid = X5028->ram[tidaddr]; // fetch tileid 
    tdat = & X5028->ram[0]; 
#   if  1
    if (X5028->reg40_LCDC & 0x10) // 0x8000 unsigned address 
      tdat = & tdat[tid<<4];
    else // 
      tdat = & tdat[0x1000+(((ks_int8)tid)<<4)]; // TODO: done.
#   else 
    c2 = (X5028->reg40_LCDC & 0x10) << 8;
    c2^= 0x1000;
    tdat = & tdat[c2+((ks_int8)(c2 >>5)) & (((ks_int8)tid) << 4)];
#   endif 
    tdat = & tdat[omfy*2];
    chrdat.blk = *(ks_uint16 *)tdat;
    chrcac.blk = chrdat.blk & 0x8080;
    pixcac =          (chrcac.lo >> 7) | (chrcac.hi >> 6);
    chrcac.blk = chrdat.blk & 0x4040;
    pixcac = pixcac | (chrcac.lo >> 4) | (chrcac.hi >> 3);
    chrcac.blk = chrdat.blk & 0x2020;
    pixcac = pixcac | (chrcac.lo >> 1) | (chrcac.hi >> 0);
    chrcac.blk = chrdat.blk & 0x1010;
    pixcac = pixcac | (chrcac.lo << 2) | (chrcac.hi << 3);
    chrcac.blk = chrdat.blk & 0x0808;
    pixcac = pixcac | (chrcac.lo << 5) | (chrcac.hi << 6);
    chrcac.blk = chrdat.blk & 0x0404;
    pixcac = pixcac | (chrcac.lo << 8) | (chrcac.hi << 9);
    chrcac.blk = chrdat.blk & 0x0202;
    pixcac = pixcac | (chrcac.lo <<11) | (chrcac.hi <<12);
    chrcac.blk = chrdat.blk & 0x0101;
    pixcac = pixcac | (chrcac.lo <<14) | (chrcac.hi <<15);
    rxpos = obx - omfx;
    
    if (!(X5028->reg40_LCDC & 0x01)) {
      vptrScrollStart = & vptrScrollStart[obx];
      /* When Bit 0 is cleared, both background and window become blank (white), 
        ie. the Window Display Bit (Bit 5) is ignored in that case. 
          Only Sprites may still be displayed (if enabled in Bit 1).
        */ 
      for (c = 0; c != 8; c++) {
        s = rxpos+c;
        if (X5028->spi_cac[s] & PIXEL_SPRITE_NOTRANS) // spline ptr to +8
          vptrScrollStart[c] = X5028->spb_cac[s];
        else 
          vptrScrollStart[c] = X5028->bg_pal_dmg[0];
        pixcac >>= 2;
      }
    } else  {
      vptrScrollStart = & vptrScrollStart[obx];
      for (c = 0; c != 8; c++) {
        s = rxpos+c;
        c2 = pixcac & 3;
        if (X5028->spi_cac[s] & PIXEL_SPRITE_NOTRANS)
          if (!(X5028->spi_cac[s] & PIXEL_SPRITE_BACK))
            vptrScrollStart[c] = X5028->spb_cac[s];
          else if ( c2 == 0)
            vptrScrollStart[c] = X5028->spb_cac[s];
          else 
            vptrScrollStart[c] = X5028->bg_pal_dmg[pixcac & 3];
        else 
          vptrScrollStart[c] = X5028->bg_pal_dmg[pixcac & 3];
        pixcac >>= 2;
      }
    }
    rxpos = obx - omfx; // -7 | 25
    rxpos+= 8; // 1 | 33
    rxpos+= 7; // 8 | 40 
    if ( X5028->win_stuff == ks_false && (X5028->reg40_LCDC & 0x20)
        && (X5028->reg4B_WX_cac <= 166 && X5028->reg4B_WX_cac < rxpos) /* check X**/
        && (X5028->reg4A_WY_cac <= scanline && (X5028->reg4A_WY_cac <= 143))) 
    {
      X5028->win_stuff = ks_true;
      X5028->xscanR = 0;
      q = 15 - omfx; // 8 | 16-> 9 
      // 7->0
      // 8->1 
      while (X5028->reg4B_WX_cac >= q) // 15 >= q / 16
        {
          X5028->xscanR ++; //  1 or 2 
          q += 8;
        }
      goto windraw;
    }
  }
  return ;
windraw:
  ofx = X5028->uscanR - X5028->xscanR; // espl x 
  c = scanline - X5028->reg4A_WY_cac; // total - y 
  omfx = 0;
  omfy = c & 7; 
  ofy = c >> 3;
  c3 = ((ks_int8)X5028->reg4B_WX_cac)-7;
  c3 = c3 + (ofx<<3);
  vptrWinDrawStart = & X5028->fmebuf.buf[scanline *(X5028->fmebuf.pitch/sizeof (X5028->fmebuf.buf[0]))+c3];
  /* pick tileid and attr from ::Bit 6 - Tile Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF) */
  tidaddr = 0x9800 + (((X5028->reg40_LCDC & 0x40) >> 6) << 10);
  tidaddr = (tidaddr-0x8000)+(ofy<< 5)+ofx;
  tid = X5028->ram[tidaddr]; // fetch tileid 
  tdat = & X5028->ram[0]; // bank select.
#   if  1
  if (X5028->reg40_LCDC & 0x10) // 0x8000 unsigned address 
    tdat = & tdat[tid<<4];
  else // 
    tdat = & tdat[0x1000+(((ks_int8)tid)<<4)]; // TODO: done.
#   else 
  cti16 = (X5028->reg40_LCDC & 0x10) << 8;
  cti16^= 0x1000;
  tdat = & tdat[cti16+((ks_int8)(cti16 >>5)) & (((ks_int8)tid) << 4)];
#   endif 
  tdat = & tdat[omfy*2];
  chrdat.blk = *(ks_uint16 *)tdat;
  chrcac.blk = chrdat.blk & 0x8080;
  pixcac = pixcac | (chrcac.lo >> 7) | (chrcac.hi >> 6);
  chrcac.blk = chrdat.blk & 0x4040;
  pixcac = pixcac | (chrcac.lo >> 4) | (chrcac.hi >> 3);
  chrcac.blk = chrdat.blk & 0x2020;
  pixcac = pixcac | (chrcac.lo >> 1) | (chrcac.hi >> 0);
  chrcac.blk = chrdat.blk & 0x1010;
  pixcac = pixcac | (chrcac.lo << 2) | (chrcac.hi << 3);
  chrcac.blk = chrdat.blk & 0x0808;
  pixcac = pixcac | (chrcac.lo << 5) | (chrcac.hi << 6);
  chrcac.blk = chrdat.blk & 0x0404;
  pixcac = pixcac | (chrcac.lo << 8) | (chrcac.hi << 9);
  chrcac.blk = chrdat.blk & 0x0202;
  pixcac = pixcac | (chrcac.lo <<11) | (chrcac.hi <<12);
  chrcac.blk = chrdat.blk & 0x0101;
  pixcac = pixcac | (chrcac.lo <<14) | (chrcac.hi <<15);

  if (!(X5028->reg40_LCDC & 0x01)) {
    for (c = 0; c != 8; c++) {
      s = c3+c;
      if (X5028->spi_cac[s] & PIXEL_SPRITE_NOTRANS) // spline ptr to +8
        vptrWinDrawStart[c] = X5028->spb_cac[s];
      else 
        vptrWinDrawStart[c] = X5028->bg_pal_dmg[0];
    }
  } else  {
    for (c = 0; c != 8; c++) {
      s = c3+c;
      c2 = pixcac & 3;
      if (X5028->spi_cac[s] & PIXEL_SPRITE_NOTRANS)
        if (!(X5028->spi_cac[s] & PIXEL_SPRITE_BACK))
          vptrWinDrawStart[c] = X5028->spb_cac[s];
        else if ( c2 == 0)
          vptrWinDrawStart[c] = X5028->spb_cac[s];
        else 
          vptrWinDrawStart[c] = X5028->bg_pal_dmg[pixcac & 3];
      else 
        vptrWinDrawStart[c] = X5028->bg_pal_dmg[pixcac & 3];
      pixcac >>= 2;
    }
  }
}
static /* this method for gameboy color */
void sprite_render_cgb (struct ppu *X5028, ks_int16 scanline) {

  ks_int16 size_y; /* ?8*16:8*8 */
  /* ks_bool touch;*/

  size_y = (X5028->reg40_LCDC & 0x04)? 16 : 8;
  /* touch = ks_false; */

  /* check sprite in visual scanline */
  for (; X5028->vscan40 < 40; X5028->vscan40++) {
    ks_int16 x;
    ks_int16 y;
    ks_int16 id;
    struct oam *ptr = & X5028->sp[X5028->vscan40];
    y = ptr->y;
    x = ptr->x;

    if (ptr->y == 0 || ptr->y >= 160)
      continue ;
    y -= 16;
    if (scanline >= y && (scanline < (y + size_y))
      && (x != 0 && x < 168)) {

        struct nca {
          union {
            ks_uint16 blk;
            struct {
              ks_uint8 lo;
              ks_uint8 hi;
            };
          };
        } chrdat,  chrcac;

        ks_uint16 pixcac ;
        ks_uint8 *chrb;
        /* sprite inc */
        X5028->vscanR++;
        /* pick tile data by tile id and atrr's D3*/
        chrb = & X5028->ram[ptr->attr & 0x08 ? 0x2000 : 0x0000];
        if (size_y != 16)
          chrb = & chrb[ptr->id *16]; // a tile data include 16 bytes 
        else 
          chrb = & chrb[(ptr->id & 0xFE) *16]; // always pointer tile16's high part in init state .
        /* check Y mapper */
        if (ptr->attr & 0x40) {
          if (size_y == 16) {
            /* sprite16 flip */
            /* get rev offset */
            int calct = scanline - y;
            calct = 7 -calct; /* flip Y in one tile */
            calct *= 2;
            /* get oppo base tile byte8 (^source pos[high or low])*/
            if (calct <= 7) /* high, switch to low */ {
              calct += 16;
            } else { /* low switch to high, nodone, already in high (see line 114).*/    
            }
            chrdat.blk = *(ks_int16 *)& chrb[calct];
          } else {
            /* sprite8 flip */
            int calct = scanline - y;
            calct = 7 -calct; /* flip Y */
            calct *= 2;
            chrdat.blk = *(ks_int16 *)& chrb[calct];
          }
        } else {
          int calct = scanline - y;
          calct *= 2;
          chrdat.blk = *(ks_int16 *)& chrb[calct];
        }
        /* mix gbc's pixel (d1d0) */
        /* see GBCPUman.pdf:: 2.8.1. Tiles */
        pixcac = 0; // reset all 
        /* check x flip  */
        if (ptr->attr & 0x20) {
          chrcac.blk = chrdat.blk & 0x8080;
          pixcac = pixcac | (chrcac.lo << 7) | (chrcac.hi << 8);
          chrcac.blk = chrdat.blk & 0x4040;
          pixcac = pixcac | (chrcac.lo << 6) | (chrcac.hi << 7);
          chrcac.blk = chrdat.blk & 0x2020;
          pixcac = pixcac | (chrcac.lo << 5) | (chrcac.hi << 6);
          chrcac.blk = chrdat.blk & 0x1010;
          pixcac = pixcac | (chrcac.lo << 4) | (chrcac.hi << 5);
          chrcac.blk = chrdat.blk & 0x0808;
          pixcac = pixcac | (chrcac.lo << 3) | (chrcac.hi << 4);
          chrcac.blk = chrdat.blk & 0x0404;
          pixcac = pixcac | (chrcac.lo << 2) | (chrcac.hi << 3);
          chrcac.blk = chrdat.blk & 0x0202;
          pixcac = pixcac | (chrcac.lo << 1) | (chrcac.hi << 2);
          chrcac.blk = chrdat.blk & 0x0101;
          pixcac = pixcac | (chrcac.lo << 0) | (chrcac.hi << 1);        
        } else {
          chrcac.blk = chrdat.blk & 0x8080;
          pixcac = pixcac | (chrcac.lo >> 7) | (chrcac.hi >> 6);
          chrcac.blk = chrdat.blk & 0x4040;
          pixcac = pixcac | (chrcac.lo >> 4) | (chrcac.hi >> 3);
          chrcac.blk = chrdat.blk & 0x2020;
          pixcac = pixcac | (chrcac.lo >> 1) | (chrcac.hi >> 0);
          chrcac.blk = chrdat.blk & 0x1010;
          pixcac = pixcac | (chrcac.lo << 2) | (chrcac.hi << 3);
          chrcac.blk = chrdat.blk & 0x0808;
          pixcac = pixcac | (chrcac.lo << 5) | (chrcac.hi << 6);
          chrcac.blk = chrdat.blk & 0x0404;
          pixcac = pixcac | (chrcac.lo << 8) | (chrcac.hi << 9);
          chrcac.blk = chrdat.blk & 0x0202;
          pixcac = pixcac | (chrcac.lo <<11) | (chrcac.hi <<12);
          chrcac.blk = chrdat.blk & 0x0101;
          pixcac = pixcac | (chrcac.lo <<14) | (chrcac.hi <<15);
        }
        x -= 8;
        for (id = 0; id != 8; id++) {
          ks_uint8 idxpal; /* palette group index, max is8, see OAM struct */
          idxpal = pixcac & 3;
          if (x >= 0 && x <= 159) {
            /* check trans or */
            if (X5028->spi_cac[x+8] & PIXEL_SPRITE_NOTRANS) {
              /* notrans */
              if (X5028->spi_cac[x+8] & PIXEL_SPRITE_BACK) {
                if (!(ptr->attr & 0x80) && idxpal != 0) {
#               if  1 /* maybe error, it's not a big problem. */
                  /* cur pick oam is foreground sprite */
                  ks_uint16 idxpg = ptr->attr & 7; 
                  X5028->spb_cac[x+8] = X5028->sp_pal[idxpg][idxpal].rgb15;
                  X5028->spi_cac[x+8] = PIXEL_SPRITE_NOTRANS;            
#               endif 
                }
              } else {
                /* nodone in foreground sprite */
              }
            } else {
              /* trans, write directly */
              ks_uint16 idxpg = ptr->attr & 7; 
              X5028->spb_cac[x+8] = X5028->sp_pal[idxpg][idxpal].rgb15;
              X5028->spi_cac[x+8] = 0;
              if (ptr->attr & 0x80) {
                /* background sprite */
                X5028->spi_cac[x+8] |= PIXEL_SPRITE_BACK;
              }
              if (idxpal != 0) {
                /* idxpal is trans color */
                X5028->spi_cac[x+8] |= PIXEL_SPRITE_NOTRANS;
              }
            }
          }
          x ++;
          pixcac >>= 2;
        }
        /* scan one sprite tile compete */
        return ;
    }
  }
}
static /* this method for gameboy */
void sprite_render_dmg (struct ppu *X5028, ks_int16 scanline) {

  ks_int16 size_y; /* ?8*16:8*8 */
  /* ks_bool touch;*/
  
  size_y = (X5028->reg40_LCDC & 0x04)? 16 : 8;
  /* touch = ks_false; */

  /* check sprite in visual scanline */
  for (; X5028->vscan40 < 40; X5028->vscan40++) {
    ks_int16 x;
    ks_int16 y;
    ks_int16 id;
    struct oam *ptr = & X5028->sp[X5028->vscan40];
    y = ptr->y;
    x = ptr->x; 

    if (ptr->y == 0 || ptr->y >= 160)
      continue ;
    y -= 16;
    if (scanline >= y && (scanline < (y + size_y))
      && (x != 0 && x < 168)) {

        struct nca {
          union {
            ks_uint16 blk;
            struct {
              ks_uint8 lo;
              ks_uint8 hi;
            };
          };
        } chrdat,  chrcac;

        ks_uint16 pixcac ;
        ks_uint8 *chrb;
        /* sprite inc */
        X5028->vscanR++;
        X5028->vscan40++;
        /* pick tile data by tile id and atrr's D3*/
        chrb = & X5028->ram[ptr->attr & 0x08 ? 0x2000 : 0x0000];
        if (size_y != 16)
          chrb = & chrb[ptr->id *16]; // a tile data include 16 bytes 
        else 
          chrb = & chrb[(ptr->id & 0xFE) *16]; // always pointer tile16's high part in init state .
        /* check Y mapper */
        if (ptr->attr & 0x40) {
          if (size_y == 16) {
            /* sprite16 flip */
            /* get rev offset */
            int calct = scanline - y;
            calct = 7 -calct; /* flip Y in one tile */
            calct *= 2;
            /* get oppo base tile byte8 (^source pos[high or low])*/
            if (calct <= 7) /* high, switch to low */ {
              calct += 16;
            } else { /* low switch to high, nodone, already in high (see line 114).*/    
            }
            chrdat.blk = *(ks_int16 *)& chrb[calct];
          } else {
            /* sprite8 flip */
            int calct = scanline - y;
            calct = 7 -calct; /* flip Y */
            calct *= 2;
            chrdat.blk = *(ks_int16 *)& chrb[calct];
          }
        } else {
          int calct = scanline - y;
          calct *= 2;
          chrdat.blk = *(ks_int16 *)& chrb[calct];
        }
        /* mix gbc's pixel (d1d0) */
        /* see GBCPUman.pdf:: 2.8.1. Tiles */
        pixcac = 0; // reset all 
        /* check x flip  */
        if (ptr->attr & 0x20) {
          chrcac.blk = chrdat.blk & 0x8080;
          pixcac = pixcac | (chrcac.lo << 7) | (chrcac.hi << 8);
          chrcac.blk = chrdat.blk & 0x4040;
          pixcac = pixcac | (chrcac.lo << 6) | (chrcac.hi << 7);
          chrcac.blk = chrdat.blk & 0x2020;
          pixcac = pixcac | (chrcac.lo << 5) | (chrcac.hi << 6);
          chrcac.blk = chrdat.blk & 0x1010;
          pixcac = pixcac | (chrcac.lo << 4) | (chrcac.hi << 5);
          chrcac.blk = chrdat.blk & 0x0808;
          pixcac = pixcac | (chrcac.lo << 3) | (chrcac.hi << 4);
          chrcac.blk = chrdat.blk & 0x0404;
          pixcac = pixcac | (chrcac.lo << 2) | (chrcac.hi << 3);
          chrcac.blk = chrdat.blk & 0x0202;
          pixcac = pixcac | (chrcac.lo << 1) | (chrcac.hi << 2);
          chrcac.blk = chrdat.blk & 0x0101;
          pixcac = pixcac | (chrcac.lo << 0) | (chrcac.hi << 1);        
        } else {
          chrcac.blk = chrdat.blk & 0x8080;
          pixcac = pixcac | (chrcac.lo >> 7) | (chrcac.hi >> 6);
          chrcac.blk = chrdat.blk & 0x4040;
          pixcac = pixcac | (chrcac.lo >> 4) | (chrcac.hi >> 3);
          chrcac.blk = chrdat.blk & 0x2020;
          pixcac = pixcac | (chrcac.lo >> 1) | (chrcac.hi >> 0);
          chrcac.blk = chrdat.blk & 0x1010;
          pixcac = pixcac | (chrcac.lo << 2) | (chrcac.hi << 3);
          chrcac.blk = chrdat.blk & 0x0808;
          pixcac = pixcac | (chrcac.lo << 5) | (chrcac.hi << 6);
          chrcac.blk = chrdat.blk & 0x0404;
          pixcac = pixcac | (chrcac.lo << 8) | (chrcac.hi << 9);
          chrcac.blk = chrdat.blk & 0x0202;
          pixcac = pixcac | (chrcac.lo <<11) | (chrcac.hi <<12);
          chrcac.blk = chrdat.blk & 0x0101;
          pixcac = pixcac | (chrcac.lo <<14) | (chrcac.hi <<15);
        }
        x -= 8;
        x -= 8;
        for (id = 0; id != 8; id++) {
          ks_uint8 idxpal; /* palette group index, max is8, see OAM struct */
          idxpal = pixcac & 3;
          if (x >= 0 && x <= 159) {
            /* check trans or */
            if (X5028->spi_cac[x+8] & PIXEL_SPRITE_NOTRANS) {
              /* notrans */
              if (X5028->spi_cac[x+8] & PIXEL_SPRITE_BACK) {
                if (!(ptr->attr & 0x80) && idxpal != 0) {
#               if  1 /* maybe error, it's not a big problem. */
                  /* cur pick oam is foreground sprite */
                  ks_uint16 idxpg = (ptr->attr & 0x10) >> 4; 
                  X5028->spb_cac[x+8] = X5028->sp_pal_dmg[idxpg][idxpal];
                  X5028->spi_cac[x+8] = PIXEL_SPRITE_NOTRANS;            
#               endif 
                }
              } else {
                /* nodone in foreground sprite */
              }
            } else {
              /* trans, write directly */
              ks_uint16 idxpg = (ptr->attr & 0x10) >> 4; 
              X5028->spb_cac[x+8] = X5028->sp_pal_dmg[idxpg][idxpal];
              X5028->spi_cac[x+8] = 0;
              if (ptr->attr & 0x80) {
                /* background sprite */
                X5028->spi_cac[x+8] |= PIXEL_SPRITE_BACK;
              }
              if (idxpal != 0) {
                /* idxpal is trans color */
                X5028->spi_cac[x+8] |= PIXEL_SPRITE_NOTRANS;
              }
            }
          }
          x ++;
          pixcac >>= 2;
        }
        /* scan one sprite tile compete */
        return ; // TODO: gb sprite priority.
    }
  }
}
/* XXX:so bad */
void ppu_run (struct ppu *X5028) {
  
  ks_int32 scanline;
  ks_int16 interv;
  ks_int16 _nums;
  ks_double clkline;
  
  if (X5028->gb->cpu_clks_ppu > X5028->gb->mach_tools->frame_cycles) {
    X5028->gb->cpu_clks_ppu -= X5028->gb->mach_tools->frame_cycles; // sub a frame block 
    /* at this time and in the scanning of the next frame of the device. 
              reset ppu device context  */
    
    /* vblank Interrupt, check touch */
    if (X5028->vbl_flop == ks_false)
      if (X5028->reg41_LCDS & 0x10)
        X5028->gb->reg0F_IF |= IRQ_1;
      else {}
    else {}
    
    /* reset context */
    X5028->vbl_flop = ks_false;
    X5028->oam_flop = ks_false;
    X5028->hbl_flop = ks_false;
    X5028->interrupt45_flop = ks_false;
    X5028->interrupt45_strike = ks_false;
    X5028->vscan = -1;
    X5028->vscanR = 0;
    X5028->vscan40 = 0;
    X5028->uscan = -1;
    X5028->uscanR = 0;
    X5028->uscan168 = 0; 
    X5028->win_stuff = ks_false;
  }
  scanline = (ks_int32) (X5028->gb->cpu_clks_ppu/ X5028->gb->mach_tools->line_cycles);
  clkline = X5028->gb->cpu_clks_ppu - X5028->gb->mach_tools->line_cycles * (ks_double) scanline;
  /* clear mask */
  X5028->reg41_LCDS &= ~LCDCS_MODE_FLAG_ALL_MASK;
  X5028->reg44_LY = scanline;

  /*
    PPU time series

    Mode 0: The LCD controller is in the H-Blank period and
         the CPU can access both the display RAM (8000h-9FFFh)
         and OAM (FE00h-FE9Fh)

    Mode 1: The LCD controller is in the V-Blank period (or the
         display is disabled) and the CPU can access both the
         display RAM (8000h-9FFFh) and OAM (FE00h-FE9Fh)

    Mode 2: The LCD controller is reading from OAM memory.
         The CPU <cannot> access OAM memory (FE00h-FE9Fh)
         during this period.

    Mode 3: The LCD controller is reading from both OAM and VRAM,
         The CPU <cannot> access OAM and VRAM during this period.
         CGB Mode: Cannot access Palette Data (FF69,FF6B) either.
    The following are typical when the display is enabled:

    Mode 2  2_____2_____2_____2_____2_____2___________________2____
    Mode 3  _33____33____33____33____33____33__________________3___
    Mode 0  ___000___000___000___000___000___000________________000
    Mode 1  ____________________________________11111111111111_____

    The Mode Flag goes through the values 0, 2, and 3 at a cycle of about 109uS. 
    0 is present about 48.6uS, 2 about 19uS, 
    and 3 about 41uS. This is interrupted every 16.6ms by the VBlank (1). 
    The mode flag stays set at 1 for about 1.08 ms.

    Mode 0 is present between 201-207 clks, 2 about 77-83 clks, and 3 about 169-175 clks.
    A complete cycle through these states takes 456 clks. 
    VBlank lasts 4560 clks. A complete screen refresh occurs every 70224 clks.)
 */
  
  /* check scanline and LCDY interrupt */
  if (X5028->line_cac != scanline 
       || (X5028->interrupt45_strike != ks_false)) {
    /* setting onrush effective */
    if (X5028->line_cac != scanline) {
      X5028->interrupt45_flop = ks_false; /* reset gate */
      X5028->interrupt45_strike = ks_false; /* close strike function*/
    } /*  scanline next, (low to high) for each scanline, 
                   multiple LCDY interruptions can not be triggered at the same time
           during each vertical synchronization  */
    if (X5028->reg45_LYC == scanline) /*  check enable ?*/
         /* Bit 6 - LYC=LY Coincidence Interrupt (1=Enable) (Read/Write) */
      if (X5028->reg41_LCDS & 0x40 && !X5028->interrupt45_flop) {      
        X5028->interrupt45_strike = ks_false; /* close strike function*/
        X5028->interrupt45_flop = ks_true;
        X5028->gb->reg0F_IF |= IRQ_2;
      } else {}
    else {}
  }
  /* check FF41::Bit 2 Coincidence Flag  (0:LYC<>LY, 1:LYC=LY) (Read Only)*/
  if (scanline == X5028->reg45_LYC) 
    X5028->reg41_LCDS |= 0x04;
  else X5028->reg41_LCDS &= ~0x04;
  
  /* check mode */
  if (X5028->gb->cpu_clks_ppu < X5028->gb->mach_tools->vbl_clk_st) {
    if (clkline > X5028->hbl_clks_st) {
      /*  LCD MODE0 -  hblank period */
      X5028->reg41_LCDS |= LCDCS_MODE_FLAG_HBLANK;
      /*  check edge */
      if (X5028->lcdm_cac != LCDCS_MODE_FLAG_HBLANK) {
        /* oamvram to current, (low to high) */
        while (X5028->uscanR < 21) {
          X5028->bgwin_render (X5028, scanline); 
          X5028->uscanR ++;
        }
        /* reset next mode-2 | mode- 3*/ 
        X5028->oam_flop = ks_false;
        X5028->vscan = -1;
        X5028->vscan40 = 0;
        X5028->uscan = -1;
        X5028->vscanR = 0;
        X5028->uscanR = 0;
        /* check HDMA. **/
        if (scanline >= 0 && scanline <= 143 && X5028->hdma_gen && X5028->hdma_r16)  {
          /* copy 16 bytes  */
          ks_uint16 c;
          for (c =0; c != 16; c++) {
            ks_uint8 s;
            s = gameboy_mmu_read (X5028->gb, X5028->hdma_src + c);
            gameboy_mmu_write (X5028->gb, X5028->hdma_dst + c, s);
          }
          X5028->hdma_src += 16;
          X5028->hdma_dst += 16;
          X5028->hdma_r16 --;
          /* add hdma cycles, ~ 8us */
          X5028->gb->cpu_clks_dma += (X5028->gb->mach_tools->clk_ns * 8.0);
        }
        /* in HDMA, no hblank perido */
        X5028->hbl_flop = ks_true;
      }
      /* check hblank interrupt  */
      if ((X5028->reg41_LCDS & 0x08) && X5028->hbl_flop == ks_false) {
        X5028->hbl_flop = ks_true;
        X5028->gb->reg0F_IF |= IRQ_2;
      }
    } 
    else if (clkline > X5028->gb->mach_tools->oambg_clk_st) {
      /* LCD MODE3  - oamvram */
      X5028->reg42_SCY_cac = X5028->reg42_SCY;
      X5028->reg43_SCX_cac = X5028->reg43_SCX;
      clkline -= X5028->gb->mach_tools->oambg_clk_st; // sub, get start clcks epls.
      X5028->reg41_LCDS |= LCDCS_MODE_FLAG_SERACH_OAMVRAM;
      /* check edge */
      if (X5028->lcdm_cac != LCDCS_MODE_FLAG_SERACH_OAMVRAM) {
        /*  check stride sprite render  */
        /* get render pos */
        interv = 9;

        /* check render oam *.*/
        if ( X5028->vscan != interv
             &&   X5028->vscanR < 10
               && X5028->vscan40   < 40) {
          _nums = interv - X5028->vscan;
          X5028->vscan += _nums;
          /* render sprite */
          do 
          {
                   X5028->sprite_render (X5028, scanline); } 
             while (--_nums);
        }
        /*  check sprite interrupt  */
        if ((X5028->reg41_LCDS & 0x20) && !X5028->oam_flop) {      
          X5028->oam_flop = ks_true;
          X5028->gb->reg0F_IF |= IRQ_2;
        }
        X5028->oam_flop = ks_false;

        /* adjust BGOAM clk */

        X5028->hbl_clks_st = (X5028->gb->mach_tools->oambg_b_cycles + 
              X5028->vscanR * X5028->gb->mach_tools->oam_clk_add_hbl_per);
        X5028->oambg_clks_divider21 = 
            X5028->hbl_clks_st / 21.0;
        X5028->hbl_clks_st += X5028->gb->mach_tools->oam_cycles;
      }
      /* get render pos */
      interv = KS_INT16_CAST (clkline/ X5028->oambg_clks_divider21);

      /* check render bg *.*/
      if (interv != X5028->uscan) {
        _nums = interv - X5028->uscan;
        X5028->uscan += _nums;
        do { 
          X5028->bgwin_render (X5028, scanline); 
          X5028->uscanR ++;
        }  while (--_nums);
      } 
    } else   {
      /* LCD MODE2 - oam */
      X5028->reg41_LCDS |= LCDCS_MODE_FLAG_SERACH_OAM;
      X5028->hbl_clks_st = 571583.0;
      /* check edge */
      if (X5028->lcdm_cac == LCDCS_MODE_FLAG_HBLANK) {
        /* hblank to current, (low to high), check stride interrupt */
        if (X5028->hbl_flop == ks_false)      
          if (X5028->reg41_LCDS & 0x08)
            X5028->gb->reg0F_IF |= IRQ_2;
          else {}
        else {}
        X5028->hbl_flop = ks_false;
        /* clear sprite ctx */
        memset (X5028->spi_cac, 0, sizeof (X5028->spi_cac));
      }
      /* get render pos */
      interv = KS_INT16_CAST (clkline/ X5028->gb->mach_tools->oam_clk_pick_per);

      /* check render oam *.*/
      if ( X5028->vscan != interv
           &&   X5028->vscanR < 10
             && X5028->vscan40   < 40) {
        _nums = interv - X5028->vscan;
        X5028->vscan += _nums;
        /* render sprite */
        do 
        {
                 X5028->sprite_render (X5028, scanline); } 
           while (--_nums);
      }
      /*  check sprite interrupt  */
      if ((X5028->reg41_LCDS & 0x20) && !X5028->oam_flop) {      
        X5028->oam_flop = ks_true;
        X5028->gb->reg0F_IF |= IRQ_2;
      }
    }
  } else {
    /* LCD MODE1 - vblank */
    X5028->reg41_LCDS |= LCDCS_MODE_FLAG_VLANK;
    /* check edge */
    if (X5028->lcdm_cac != LCDCS_MODE_FLAG_VLANK) {
      /* post device render */
      X5028->device_blit (X5028, X5028->obj, & X5028->fmebuf);
      X5028->gb->reg0F_IF |= IRQ_1;
      /* check hblank lack */
    }
  }
  X5028->line_cac = scanline;
  X5028->lcdm_cac = X5028->reg41_LCDS & LCDCS_MODE_FLAG_ALL_MASK;
}

int ppu_cgb_mode (struct ppu *X5028) {
  X5028->bgwin_render = bgwin_render_cgb;
  X5028->sprite_render = sprite_render_cgb;
  return 0;
}
int ppu_dmg_mode (struct ppu *X5028) {
  X5028->bgwin_render = bgwin_render_dmg;
  X5028->sprite_render = sprite_render_dmg;
  return 0;
}

int ppu_init (struct ppu **X5028) {

  struct ppu *sys = ks_null;
  assert (X5028 != ks_null);

  sys = (struct ppu *)calloc (sizeof (struct ppu), 1);
  assert (sys != ks_null);
  *X5028 = sys;

  if (1) {
    /* init dmg palette 
     0  White
     1  Light gray
     2  Dark gray
     3  Black
     */
    ks_uint16 color[4] = {
#   if 0
                 (0x1F << 0) | (0x1F << 5) | (0x1F << 10), /* color index 0*/
                 (0x15 << 0) | (0x15 << 5) | (0x15 << 10),  /* color index 1*/
                 (0x0A << 0) | (0x0A << 5) | (0x0A << 10),  /* color index 2*/
                 (0x00 << 0) | (0x00 << 5) | (0x00 << 10)  /* color index 3*/
#   else 
      // fce88c:0
      // dcb45c:1
      // 987c3c:2
      // 4c3c1c:0
      // ((0x) >> 3)
                 (((0x8c) >> 3) << 0) | (((0xe8) >> 3) << 5) | (((0xfc) >> 3) << 10), /* color index 0*/
                 (((0x5c) >> 3) << 0) | (((0xb4) >> 3) << 5) | (((0xdc) >> 3) << 10),  /* color index 1*/
                 (((0x3c) >> 3) << 0) | (((0x7c) >> 3) << 5) | (((0x98) >> 3) << 10),  /* color index 2*/
                 (((0x1c) >> 3) << 0) | (((0x3c) >> 3) << 5) | (((0x4c) >> 3) << 10)  /* color index 3*/
#   endif 
              };
    ks_uint16 Q;
    
    for (Q = 0; Q != 4; Q++) {
      sys->bg_pal_dmgT[Q] = color[Q];
      sys->sp_pal_dmgT[0][Q] = color[Q];   
      sys->sp_pal_dmgT[1][Q] = color[Q];     
    }
  }
  sys->fmebuf.w = 160;
  sys->fmebuf.h = 144;
  sys->fmebuf.pitch = 512;
  sys->bufb = (ks_uint16 *)malloc ( (sys->fmebuf.w + 96) * (sys->fmebuf.h + 16) * 2 + 128);
  sys->fmebuf.buf = (ks_uint16 *)(((((intptr_t)& sys->bufb[0]) + 31) & -32) + 16);
  sys->device_blit = default_update;
  sys->clks = ppu_run;
  sys->vscan = -1;
  return 0;
}
#include <stdio.h>
#include <stdlib.h>

// ----------------------------------------------
//	purpose:built ppu look up table 
//	command:cmd->tcc ppu_table.c 
// ----------------------------------------------

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed int boolean;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

int main (void) {
	
	FILE *fp;
	
	uint32_t i;
	uint32_t t;
	uint32_t revTile[256];
	uint32_t bpMaskL[256];
	uint32_t bpMaskH[256];

	/* --- ready data --- */
	for (i = 0; i != 256; i ++)
	{
		t = (i & 0xaa) >> 1 | (i & 0x55) << 1;
		t = (t & 0xcc) >> 2 | (t & 0x33) << 2;
		revTile[i] = (t & 0xf0) >> 4 | (t & 0x0f) << 4;
		
		bpMaskL[i] = ((i & 0x80) >> 7)
				   | ((i & 0x40) >> 4)
				   | ((i & 0x20) >> 1)
				   | ((i & 0x10) << 2)
				   | ((i & 0x08) << 5)
				   | ((i & 0x04) << 8)
				   | ((i & 0x02) << 11)
				   | ((i & 0x01) << 14);
				  
		bpMaskH[i] = bpMaskL[i] << 1;
	}
	fp
	= fopen ("PPUTbl.lcs", "wt+");
	
	/* --- out revTile --- */
	fprintf (fp, "static const uint8_t revTile[256] = {\n\n\t");
	for (i = 0; i != 256; i ++) 
	{
		fprintf (fp, "0x%02X",
				revTile[i]
			);
				
		i == 255 
			? fprintf (fp, "\n};\n")
			: i % 8 == 7 
			? fprintf (fp, ","), fprintf (fp, "\n\t")
			: fprintf (fp, ",");
	}
	
	/* --- out bitmap_low --- */ 
	fprintf (fp, "static const uint16_t bpMaskL[256] = {\n\n\t");
	for (i = 0; i != 256; i ++)
	{			
		fprintf (fp, "0x%04X",
				bpMaskL[i]
			);
			
		i == 255 
			? fprintf (fp, "\n};\n")
			: i % 8 == 7 
			? fprintf (fp, ","), fprintf (fp, "\n\t")
			: fprintf (fp, ",");
	}
	
	/* --- out bitmap_high --- */ 
	fprintf (fp, "static const uint16_t bpMaskH[256] = {\n\n\t");
	for (i = 0; i != 256; i ++)
	{
		fprintf (fp, "0x%04X",
				bpMaskH[i]
			);
			
		i == 255 
			? fprintf (fp, "\n};\n")
			: i % 8 == 7 
			? fprintf (fp, ","), fprintf (fp, "\n\t")
			: fprintf (fp, ",");
	}
	
	fclose (fp);
	return 0;
}
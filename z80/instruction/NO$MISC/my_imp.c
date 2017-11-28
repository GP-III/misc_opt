#define BCD_ADJUST \
\
	if (CPUs->F & N_FLAG) { \
		if (((CPUs->A & 0x0F) > 0x09) || (CPUs->F & H_FLAG)) { \
			CPUs->F |= H_FLAG | (CPUs->A < 0x06 ? C_FLAG : 0); \
			CPUs->A += 0xFA;  \
		} else { \
			CPUs->F &= H_FLAG;\
		} \
		if ((CPUs->A > 0x9F) || (CPUs->F & C_FLAG)) { \
			CPUs->A += 0xA0;  \
			CPUs->F |= C_FLAG;\
		} else { \
			CPUs->F &= C_FLAG;\
		} \
	} else { \
		if (((CPUs->A & 0x0F) > 0x09) || (CPUs->F & H_FLAG)) { \
			CPUs->F |= H_FLAG | (CPUs->A > 0x99 ? C_FLAG : 0); \
			CPUs->A += 0x06;  \
		} else { \
			CPUs->F &= H_FLAG;\
		} \
		if (((CPUs->A & 0xF0) > 0x90) || (CPUs->F & C_FLAG)) { \
			CPUs->A += 0x60;  \
			CPUs->F |= C_FLAG;\
		} else { \
			CPUs->F &= C_FLAG;\
		} \
	} \
\
	CPUs->F |= ((CPUs->F & (C_FLAG | N_FLAG | H_FLAG)) | PZST[CPUs->A])
#ifndef _VBE_MACROS_H_
#define _VBE_MACROS_H_

#define BIT(n) 		(0x01 << (n))
#define PB2BASE(x)	(((x) >> 4) &  0x0F000)

#define SET_VBE						0x4F02
#define LINEAR_FB					0x105
#define VBE_INT						0x10 /* BIOS video services */
#define VBE_INFO					0x4F00
#define VBE_MODE_INFO				0x4F01


#define MODE105 					0x105
#define MODE110 					0x110
#define MODE115 					0x115
#define MODE11A 					0x11A
#define MODE14C						0x14C
#define MODE117						0x117
#define MASK_115_14C				0x00FF
#define MASK_110_11A				0x001F
#define GREEN_MASK_11A				0x003F

#define DELAY_TOS 					1000000

//Bitmap

#define BITMAPERROR 0x4D42

#define BACKGROUNDCOLOR 0
#endif

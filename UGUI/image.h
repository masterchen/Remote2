#include "ugui.h"
#ifndef _BMP
#define _BMP

extern uint8_t logo_bmp[21632];

const UG_BMP logo =
{
   (void*)logo_bmp,
   104,
   104,
   BMP_BPP_16,
   BMP_RGB565
};

#endif



// ----------------------------------------------------------------------------
// DDScrollBuffer.h
// ----------------------------------------------------------------------------

#ifndef DDSCROLLBUFFER_H
#define DDSCROLLBUFFER_H

#include "vb_types.h"
#include "vb_defs.h"
#include "vb_vars.h"
#include "vb_lib.h"

#include "global.h"

extern void DDScrollBuffer_Blt();
extern void DDScrollBuffer_Cls(int BackColor);
extern long DDScrollBuffer_CreateAtSize(long Width, long Height, long hWndViewPort);
extern long DDScrollBuffer_Get_DestXOff();
extern long DDScrollBuffer_Get_DestYOff();
extern int DDScrollBuffer_Get_Height();
extern DirectDrawSurface7 DDScrollBuffer_Get_Surface();
extern long DDScrollBuffer_Get_Width();
extern void DDScrollBuffer_Let_DestXOff(long NewVal);
extern void DDScrollBuffer_Let_DestYOff(long NewVal);
extern void DDScrollBuffer_ScrollTo(int X, int Y);
extern void DDScrollBuffer_ScrollTowards(int X, int Y, double Step);
extern void DDScrollBuffer_SoftScrollTo(int X, int Y, long TimeMS, int FPS);

#endif /* DDSCROLLBUFFER_H */
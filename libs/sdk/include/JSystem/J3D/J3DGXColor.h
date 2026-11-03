#ifndef _JSYSTEM_J3D_J3DGXCOLOR_H
#define _JSYSTEM_J3D_J3DGXCOLOR_H

#include "types.h"
#include <dolphin/gx.h>
#include <dolphin/gx.h>

struct J3DGXColor : public GXColor {
	J3DGXColor() { }

	J3DGXColor(u8 _r, u8 _g, u8 _b, u8 _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	J3DGXColor(const J3DGXColor& other) { __memcpy(this, &other, sizeof(J3DGXColor)); }

	J3DGXColor(const GXColor color)
	    : GXColor(color)
	{
	}

	J3DGXColor(const u32& other)
	{
		GXColor* otherBytes = (GXColor*)&other;
		r                   = otherBytes->r;
		g                   = otherBytes->g;
		b                   = otherBytes->b;
		a                   = otherBytes->a;
	}


	J3DGXColor& operator=(const GXColor color)
	{
		*(GXColor*)this = color;
		return *this;
	}

	const u8* asBytes() const { return (u8*)this; }
};

struct J3DGXColorS10 : public GXColorS10 {
	J3DGXColorS10() { }

	J3DGXColorS10(u16 _r, u16 _g, u16 _b, u16 _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	J3DGXColorS10(f32 _r, f32 _g, f32 _b, u16 _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	J3DGXColorS10(const J3DGXColorS10& other) { __memcpy(this, &other, sizeof(J3DGXColorS10)); }

	J3DGXColorS10(const GXColorS10& color)
	    : GXColorS10(color)
	{
	}

	J3DGXColorS10(const u64& other)
	{
		GXColorS10* otherBytes = (GXColorS10*)&other;
		r                      = otherBytes->r;
		g                      = otherBytes->g;
		b                      = otherBytes->b;
		a                      = otherBytes->a;
	}

	// __memcpy like tp's copy ctor (J3DMatBlock.h:27). tp's operator= (J3DMatBlock.h:32-38) is a u32 cast
	// it labels "Fakematch?"; banned here (docs/COORDINATOR.md).
	J3DGXColorS10& operator=(const GXColorS10& color)
	{
		__memcpy(this, &color, sizeof(GXColorS10));
		return *this;
	}
};

#endif

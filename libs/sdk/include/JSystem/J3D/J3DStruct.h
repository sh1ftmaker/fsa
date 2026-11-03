#ifndef _JSYSTEM_J3D_J3DSTRUCT_H
#define _JSYSTEM_J3D_J3DSTRUCT_H

#include "types.h"

// From zeldaret/tp's J3DGraphBase/J3DStruct.h. FSA's J3DTexMtxInfo::operator=
// (J3DStruct.cpp) copies the SRT info with this paired-single copy.
struct J3DTextureSRTInfo {
	inline void operator=(const J3DTextureSRTInfo& other)
	{
		register const f32* src = &other.mScaleX;
		register f32* dst       = &mScaleX;
		register f32 xy;
		asm {
			// clang-format off
			psq_l xy, 0(src), 0, 0
			psq_st xy, 0(dst), 0, 0
			// clang-format on
		};

		// FIXME: FSA copies mRotation and the 2 padding bytes after it as one word
		// (lwz/stw at 0x18), as tp's *(u32*)&mRotation does; that pointer-cast word
		// copy is banned here (docs/COORDINATOR.md), and the s16 copy gives lha/sth.
		mRotation = other.mRotation;
		src               = &other.mTranslationX;
		dst               = &mTranslationX;

		asm {
			// clang-format off
			psq_l xy, 0(src), 0, 0
			psq_st xy, 0(dst), 0, 0
			// clang-format on
		};
	}

	f32 mScaleX;       // _00
	f32 mScaleY;       // _04
	s16 mRotation;     // _08
	f32 mTranslationX; // _0C
	f32 mTranslationY; // _10
}; // Size: 0x14

#endif

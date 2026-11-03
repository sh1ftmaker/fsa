#ifndef _J3DFRAMECTRL_H
#define _J3DFRAMECTRL_H

#include "types.h"

// mkdd's J3DFrameCtrl with tww's enums and inline accessors
// (zeldaret/tww include/JSystem/J3DGraphAnimator/J3DAnimation.h). FSA's game
// code (game/tu_801D1C18.cpp, SpriteAnim_801D267C) matches with the
// accessors. The fields stay public as in mkdd.
struct J3DFrameCtrl {
	enum Attribute_e {
		/*  -1 */ EMode_NULL = -1,
		/* 0x0 */ EMode_NONE,
		/* 0x1 */ EMode_RESET,
		/* 0x2 */ EMode_LOOP,
		/* 0x3 */ EMode_REVERSE,
		/* 0x4 */ EMode_LOOP_REVERSE,
	};

	enum State_e {
		/* 0x1 */ STATE_STOP_E = 0x1,
		/* 0x2 */ STATE_LOOP_E = 0x2,
	};

	J3DFrameCtrl() { init(0); }

	virtual ~J3DFrameCtrl() { } // _08 (weak)

	void init(s16);
	BOOL checkPass(f32);
	void update();

	u8 getAttribute() const { return mAttribute; }
	void setAttribute(u8 attr) { mAttribute = attr; }
	u8 getState() const { return mState; }
	bool checkState(u8 state) const { return mState & state ? true : false; }
	s16 getStart() const { return mStart; }
	void setStart(s16 start) {
		mStart = start;
		mFrame = start;
	}
	s16 getEnd() const { return mEnd; }
	void setEnd(s16 end) { mEnd = end; }
	s32 getLoop() const { return mLoop; }
	void setLoop(s16 loop) { mLoop = loop; }
	f32 getRate() const { return mRate; }
	void setRate(f32 rate) { mRate = rate; }
	f32 getFrame() const { return mFrame; }
	void setFrame(f32 frame) { mFrame = frame; }

	// _00 VTBL
	u8 mAttribute; // _04
	u8 mState;     // _05
	s16 mStart;    // _06
	s16 mEnd;      // _08
	s16 mLoop;     // _0A
	f32 mRate;     // _0C
	f32 mFrame;    // _10
};

#endif

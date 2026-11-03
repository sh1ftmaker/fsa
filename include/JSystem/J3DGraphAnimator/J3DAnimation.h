#ifndef J3DANIMATION_H
#define J3DANIMATION_H

// J3DFrameCtrl only, from tww's include/JSystem/J3DGraphAnimator/J3DAnimation.h
// (zeldaret/tww). FSA's J3DAnimation.cpp has init (0x800C3608), checkPass
// (0x800C3638) and update (0x800C3BD8); the weak vtable and destructor were
// kept in act_cloud (__vt__12J3DFrameCtrl 0x8049CC34, __dt__12J3DFrameCtrlFv).
// Add the unit's other classes when it is decompiled.

#include "types.h"

class J3DFrameCtrl {
public:
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

    J3DFrameCtrl() { this->init(0); }
    void init(s16 end);
    BOOL checkPass(f32);
    void update();
    virtual ~J3DFrameCtrl() {}

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

private:
    /* 0x04 */ u8 mAttribute;
    /* 0x05 */ u8 mState;
    /* 0x06 */ s16 mStart;
    /* 0x08 */ s16 mEnd;
    /* 0x0A */ s16 mLoop;
    /* 0x0C */ f32 mRate;
    /* 0x10 */ f32 mFrame;
}; // Size: 0x14

#endif /* J3DANIMATION_H */

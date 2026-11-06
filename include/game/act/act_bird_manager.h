#ifndef GAME_ACT_ACT_BIRD_MANAGER_H
#define GAME_ACT_ACT_BIRD_MANAGER_H

#include "game/act/tu_80207548.h"

class cls_804A5C88 : public cls_804A5B28 {
public:
    struct Path {
        void fn_80209640(JGeometry::TVec2f* delta);
        void getPos(JGeometry::TVec2f* out) const {
            if (out != NULL) {
                out->set(mPos);
            }
        }

        /* 0x00 */ s32 mState;
        /* 0x04 */ JGeometry::TVec2f mPos;
        /* 0x0C */ s32 mDirection;
        /* 0x10 */ s16 mAngle;
        /* 0x12 */ s16 mAngleStep;
        /* 0x14 */ JGeometry::TVec2f mStart;
        /* 0x1C */ JGeometry::TVec2f mTarget;
    }; // Size: 0x24

    cls_804A5C88();
    /* 0x08 */ virtual ~cls_804A5C88();
    /* 0x0C */ virtual void init();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0xA4 */ virtual void vfunc_0xA4();
    /* 0xA8 */ virtual void vfunc_0xA8();

    /* 0x340 */ s32 field_0x340;
    /* 0x344 */ s32 field_0x344;
    /* 0x348 */ s32 field_0x348;
    /* 0x34C */ Path mPath;
}; // Size: 0x370

#endif

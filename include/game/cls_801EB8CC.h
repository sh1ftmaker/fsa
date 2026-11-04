#ifndef GAME_CLS_801EB8CC_H
#define GAME_CLS_801EB8CC_H

#include "types.h"
#include "JSystem/JGeometry.h"
#include "game/cls_801EBA14.h"

class JPABaseEmitter;
class cls_801EB8CC;

// A class known by its destructor, __dt__12cls_801EB8CCFv (0x801EB8CC, defined in
// game/tu_801EB00C.cpp), which runs fn_801EE434 on the pointers at 0x34
// (seven) and 0x50, then deletes itself when asked to. It is the member at
// 0x230 of VaatiTimeUp_804ACCC0 and of act_vaati_boss (0x464). Named after
// its destructor, the first function seen. fn_801EB710 (game/tu_801EB00C.cpp)
// is its method, called on the member by VaatiTimeUp_804ACCC0::init().
// Its constructor, __ct__12cls_801EB8CCFv (game/tu_801EB00C.cpp, was
// fn_801EB950), zeroes the floats and pointers and builds field_0xc.
class cls_801EB8CC {
public:
    cls_801EB8CC();
    ~cls_801EB8CC();
    // fn_801EB710 (game/tu_801EB00C.cpp), not yet understood: stores *pos in
    // field_0x0 and keeps arg (r4) for a loop over sincosTable_.
    void fn_801EB710(s32 arg, const JGeometry::TVec3f* pos);
    // fn_801EB648 (game/tu_801EB00C.cpp): 1 while field_0x54 (as a byte) is set
    // and one of the seven emitters at 0x34 is still there.
    s32 fn_801EB648() const;
    // fn_801EB00C (game/tu_801EB00C.cpp, not written yet; 0x1E8 bytes). Called
    // by Vaati (tu_801E4D54 fn_801E5754) on its member at 0x464 with a
    // player's position and a stack TVec3f: the vector it leaves in *out has
    // a z the caller compares with 0.
    void fn_801EB00C(const JGeometry::TVec3f* pos, JGeometry::TVec3f* out);

    /* 0x00 */ JGeometry::TVec3f field_0x0;
    /* 0x0C */ cls_801EBA14 field_0xc;
    /* 0x34 */ JPABaseEmitter* field_0x34[7];
    /* 0x50 */ JPABaseEmitter* field_0x50;
    /* 0x54 */ f32 field_0x54;
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ u8 field_0x5c;
    /* 0x5E */ s16 field_0x5e;
}; // Size: 0x60

#endif

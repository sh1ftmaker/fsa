#ifndef GAME_CLS_801EB8CC_H
#define GAME_CLS_801EB8CC_H

#include "types.h"
#include "game/cls_801EBA14.h"

// A class known by its destructor, __dt__12cls_801EB8CCFv (0x801EB8CC, defined in
// game/tu_801EB00C.cpp), which runs fn_801EE434 on the pointers at 0x34
// (seven) and 0x50, then deletes itself when asked to. It is the member at
// 0x230 of VaatiTimeUp_804ACCC0 and of act_vaati_boss (0x464). Named after
// its destructor, the first function seen. fn_801EB710 (game/tu_801EB00C.cpp)
// is its method, called on the member by VaatiTimeUp_804ACCC0::init().
class cls_801EB8CC {
public:
    ~cls_801EB8CC();
    // fn_801EB710 (game/tu_801EB00C.cpp), not yet understood.
    void fn_801EB710();

    /* 0x00 */ u8 field_0x0[0xC];
    /* 0x0C */ cls_801EBA14 field_0xc;
    /* 0x34 */ u8 field_0x34[0x54 - 0x34]; // seven pointers at 0x34, one at 0x50
}; // Size: at least 0x54

#endif

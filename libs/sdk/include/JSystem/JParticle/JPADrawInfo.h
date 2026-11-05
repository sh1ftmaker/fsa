#ifndef _JSYSTEM_JPA_JPADRAWINFO_H
#define _JSYSTEM_JPA_JPADRAWINFO_H

// Split out of JPAEmitter.h as tp has it (JSystem/JParticle/JPADrawInfo.h),
// so game/Camera.h can hold one without the emitter headers.

#include <dolphin/mtx.h>
#include "types.h"

class JPADrawInfo {
public:
    // FSA's version also has a default constructor setting both matrices to
    // identity: Title_804AF820::fn_803A44D8 (game/tu_803A3484.cpp, 0x803A48D4)
    // and the Camera_804AA9E4 constructor (0x8030E738, its 0xE4 / 0x114 pair)
    // call PSMTXIdentity on the two in a row, then copy the view and
    // C_MTXLightOrtho into them later.
    JPADrawInfo() {
        PSMTXIdentity(mCamMtx);
        PSMTXIdentity(mPrjMtx);
    }

    JPADrawInfo(Mtx src, f32 fovY, f32 aspect) {
        PSMTXCopy(src, mCamMtx);
        C_MTXLightPerspective(mPrjMtx, fovY, aspect, 0.5f, -0.5f, 0.5f, 0.5f);
    }

    JPADrawInfo(Mtx src, f32 top, f32 bottom, f32 left, f32 right) {
        PSMTXCopy(src, mCamMtx);
        C_MTXLightOrtho(mPrjMtx, top, bottom, left, right, 0.5f, 0.5f, 0.5f, 0.5f);
    }

    Mtx mCamMtx;
    Mtx mPrjMtx;

    void getCamMtx(Mtx dst) const { PSMTXCopy(mCamMtx, dst); }
    void getPrjMtx(Mtx dst) const { PSMTXCopy(mPrjMtx, dst); }
};

#endif

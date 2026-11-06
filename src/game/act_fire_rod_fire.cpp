#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/ViewLayer.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "game/DrawShadow.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/tu_801F9484.h"

extern const s32 lbl_8046D388[4];

// FIXME: 99.45%. scheduling + reloc-name. Identical codegen; pending update's float literals in .sdata2.
void FireRodFire_804A89F0::init() {
    field_0x284 = (field_0x90 >> 11) & 1;
    field_0x98 = (field_0x90 >> 4) & 3;
    field_0x260 = field_0x90 & 3;
    field_0x292 = (field_0x90 >> 16) & 1;
    field_0x293 = 0;
    field_0x28c = 1.0f;
    if (field_0x260 == 0xF) {
        field_0x260 = -1;
    }
    field_0x264 = (field_0x90 >> 6) & 0xF;
    field_0x268 = (field_0x90 >> 12) & 0xF;

    if (field_0x268 < 2) {
        if (viewLayerFlag(field_0x4) != 0) {
            if (field_0x98 < 2) {
                field_0x80.i.x = -6.0f;
                field_0x80.i.y = -4.0f;
                field_0x80.f.x = 6.0f;
                field_0x80.f.y = 4.0f;
            } else {
                field_0x80.i.x = -4.0f;
                field_0x80.i.y = -6.0f;
                field_0x80.f.x = 4.0f;
                field_0x80.f.y = 6.0f;
            }
        } else {
            field_0x80.i.x = -6.0f;
            field_0x80.i.y = -6.0f;
            field_0x80.f.x = 6.0f;
            field_0x80.f.y = 6.0f;
        }

        mBox.i.x = -6.0f;
        mBox.i.y = -6.0f;
        mBox.f.x = 6.0f;
        mBox.f.y = 6.0f;
    } else {
        mBox.i.x = -12.0f;
        mBox.i.y = -12.0f;
        mBox.f.x = 12.0f;
        mBox.f.y = 12.0f;
        field_0x80.i.x = -12.0f;
        field_0x80.i.y = -12.0f;
        field_0x80.f.x = 12.0f;
        field_0x80.f.y = 12.0f;
    }

    switch (field_0x98) {
    case 0:
        if (field_0x264 == 3) {
            field_0x23c.set(1.5f, 0.0f, 0.0f);
        } else if (field_0x264 == 4) {
            field_0x23c.set(0.7f, 0.0f, 0.0f);
        } else {
            field_0x23c.set(2.0f, 0.0f, 0.0f);
        }
        if (field_0x264 == 0 || field_0x264 == 3 || field_0x264 == 4) {
            field_0x26c = 0x10;
        } else {
            field_0x26c = 0;
        }
        break;
    case 1:
        if (field_0x264 == 3) {
            field_0x23c.set(-1.5f, 0.0f, 0.0f);
        } else if (field_0x264 == 4) {
            field_0x23c.set(-0.7f, 0.0f, 0.0f);
        } else {
            field_0x23c.set(-2.0f, 0.0f, 0.0f);
        }
        if (field_0x264 == 0 || field_0x264 == 3 || field_0x264 == 4) {
            field_0x26c = 0x14;
        } else {
            field_0x26c = 6;
        }
        break;
    case 2:
        if (field_0x264 == 3) {
            field_0x23c.set(0.0f, 1.5f, 0.0f);
        } else if (field_0x264 == 4) {
            field_0x23c.set(0.0f, 0.7f, 0.0f);
        } else {
            field_0x23c.set(0.0f, 2.0f, 0.0f);
        }
        if (field_0x264 == 0 || field_0x264 == 3 || field_0x264 == 4) {
            field_0x26c = 0x18;
        } else {
            field_0x26c = 0xC;
        }
        break;
    case 3:
        if (field_0x264 == 3) {
            field_0x23c.set(0.0f, -1.5f, 0.0f);
        } else if (field_0x264 == 4) {
            field_0x23c.set(0.0f, -0.7f, 0.0f);
        } else {
            field_0x23c.set(0.0f, -2.0f, 0.0f);
        }
        if (field_0x264 == 0 || field_0x264 == 3 || field_0x264 == 4) {
            field_0x26c = 0x1C;
        } else {
            field_0x26c = 0x12;
        }
        break;
    }

    if (field_0x264 == 5) {
        field_0x26c = 0x20;
    }
    field_0x254 = 0;
    if (field_0x264 == 3 || field_0x264 == 4) {
        field_0x254 = 0x28;
    }
    mEmitter230 = NULL;
    mEmitter234 = NULL;
    field_0x25c = 0;
    field_0x280 = 0;

    if (field_0x264 == 5) {
        field_0x280 = 5;
        field_0x23c.set(0.0f, 0.0f, 0.0f);
        switch (field_0x98) {
        case 0:
            mPos.x += 16.0f;
            break;
        case 1:
            mPos.x -= 16.0f;
            break;
        case 2:
            mPos.y += 16.0f;
            break;
        case 3:
            mPos.y -= 16.0f;
            break;
        }
    }

    if (field_0x264 == 0 || field_0x264 == 3 || field_0x264 == 4 || field_0x264 == 5) {
        if (field_0x4 < 8) {
            JGeometry::TVec3f pos = mPos;
            f32 scale = 1.0f;
            if (field_0x264 != 5) {
                switch (field_0x98) {
                case 0:
                    pos.x += 8.0f;
                    break;
                case 1:
                    pos.x -= 8.0f;
                    break;
                case 2:
                    pos.y += 8.0f;
                    break;
                case 3:
                    pos.y -= 8.0f;
                    break;
                }
            }

            if (field_0x268 == 2) {
                scale = 2.0f;
                switch (field_0x98) {
                case 0:
                    pos.x += 8.0f;
                    field_0x23c.set(4.0f, 0.0f, 0.0f);
                    break;
                case 1:
                    pos.x -= 8.0f;
                    field_0x23c.set(-4.0f, 0.0f, 0.0f);
                    break;
                case 2:
                    pos.y += 8.0f;
                    field_0x23c.set(0.0f, 4.0f, 0.0f);
                    break;
                case 3:
                    pos.y -= 8.0f;
                    field_0x23c.set(0.0f, -4.0f, 0.0f);
                    break;
                }
            }

            if (field_0x264 != 5) {
                mEmitter230 = spawnEmitter_8013CC50(field_0x4, &pos, 0x19B, 0, 0, NULL, NULL, true, scale);
                if (mEmitter230 != NULL) {
                    JPAGetXYZRotateMtx(0, lbl_8046D388[field_0x98], 0, mEmitter230->mGlobalRot);
                }
                mEmitter234 = spawnEmitter_8013CC50(field_0x4, &pos, 0x19C, 0, 0, NULL, NULL, true, scale);
                if (mEmitter234 != NULL) {
                    JPAGetXYZRotateMtx(0, lbl_8046D388[field_0x98], 0, mEmitter234->mGlobalRot);
                }
            }
        }

        if (field_0x264 == 3 || field_0x264 == 4) {
            field_0x260 = -1;
        }
    } else if (field_0x264 == 2) {
        field_0x260 = -1;
    }

    field_0x274 = 2;
    field_0x270 = 0;
    field_0x278 = field_0x4;
    field_0x288 = 0;
    field_0x290 = 0;
    field_0x291 = 0;
    field_0x238 = 0;
    startSound_801F0E34(0x30034, 0);
}

FireRodFire_804A89F0::~FireRodFire_804A89F0() {
    if (mEmitter230 != NULL) {
        releaseEmitter_801EE434(mEmitter230);
    }
    if (mEmitter234 != NULL) {
        releaseEmitter_801EE434(mEmitter234);
    }
}

// FIXME: 87.64%. missing/extra + reloc-name. fn_803075AC parameter 5 type in shared header; switch codegen.
void FireRodFire_804A89F0::draw() {
    if (field_0x264 == 5 || field_0x264 == 6) {
        return;
    }

    JGeometry::TVec3f pos = mPos;
    s32 isTwo = (field_0x268 == 2);
    if (field_0x4 >= 8) {
        switch (field_0x264) {
        case 0:
        case 3:
        case 4:
        case 5:
            fn_803075AC(mSerial, field_0x4, &mPos, field_0x26c + field_0x270 + 0x109, isTwo != 0, 2, -1, 1, 0, 0);
            return;
        case 1:
            if (fn_8022CCAC(field_0x260) || fn_80230188(field_0x260)) {
                return;
            }
            break;
        }

        switch (field_0x98) {
        case 0:
            pos.x -= 16.0f;
            break;
        case 1:
            pos.x += 16.0f;
            break;
        case 2:
            pos.y -= 16.0f;
            break;
        case 3:
            pos.y += 16.0f;
            break;
        }

        fn_803075AC(mSerial, field_0x4, &pos, field_0x26c + field_0x270 + 0xF1, isTwo != 0, 2, -1, 1, 0, 0);
    } else {
        switch (field_0x98) {
        case 0:
            pos.x -= 6.0f;
            pos.y -= 8.0f;
            break;
        case 1:
            pos.x += 6.0f;
            pos.y -= 8.0f;
            break;
        case 2:
            pos.y -= 8.0f;
            break;
        case 3:
            break;
        }

        drawShadow(field_0x4, &pos, JUtility::TColor(0, 0, 0, 0x80), 32.0f);
    }
}

// FIXME: 94.66%. missing/extra + reloc-name. JPABaseEmitter missing offset 0x24 in shared header; float literal placement.
void FireRodFire_804A89F0::vfunc_0x24() {
    if (field_0x264 != 3 && field_0x264 != 4) {
        if (field_0x264 == 0) {
            mPos.x = (s32)((u32)mPos.x & ~0xF);
            mPos.y = (s32)((u32)mPos.y & ~0xF);
            switch (field_0x98) {
            case 0:
                mPos.x += 24.0f;
                mPos.y += 8.0f;
                break;
            case 1:
                mPos.x -= 8.0f;
                mPos.y += 8.0f;
                break;
            case 2:
                mPos.x += 8.0f;
                mPos.y += 16.0f;
                break;
            case 3:
                mPos.x += 8.0f;
                mPos.y -= 8.0f;
                break;
            }

            if (field_0x4 < 8) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x19D, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (field_0x4 < 8) {
                mAlive = false;
            } else {
                field_0x238 = 4;
                field_0x26c = 0;
                field_0x270 = 0;
            }
        } else {
            mPos = field_0x248;
            mAlive = false;
        }

        if (fn_802BACE4() == true) {
            return;
        }

        s32 param;
        s32 r29 = 0;
        if (field_0x264 == 5) {
            r29 = 4;
        }
        if (field_0x264 == 5 && field_0x291 == 0) {
            param = field_0x4;
            if (param < 8) {
                spawnEmitter_8013CC50(param, &mPos, 0x19D, 0, 0, NULL, NULL, true, 1.0f);
            } else {
                getActorManager_801F856C()->spawnActor_801F9484('KMRI', param, &mPos, 6, -1, -1);
            }
        } else {
            param = field_0x260;
            if (param < 0) {
                getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, &mPos, (r29 << 6) | (field_0x98 << 4) | 0xF, -1, -1);
            } else {
                getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, &mPos, (r29 << 6) | (field_0x98 << 4) | param, -1, field_0x110);
            }
        }
    } else {
        if (field_0x4 < 8) {
            mAlive = false;
        } else {
            field_0x238 = 4;
            field_0x26c = 0;
            field_0x270 = 0;
        }
    }

    if (mEmitter230 != NULL) {
        mEmitter230->stopCreateParticle();
        mEmitter230 = NULL;
    }
    if (mEmitter234 != NULL) {
        mEmitter234->stopCreateParticle();
        mEmitter234 = NULL;
    }
}

// FIXME: 98.02%. regalloc-only. Register allocation.
bool FireRodFire_804A89F0::fn_802BACE4() {
    if (field_0x264 != 0) {
        return false;
    }

    f32 boxMinX = mBox.i.x + mPos.x;
    f32 boxMaxX = mBox.f.x + mPos.x;
    f32 boxMinY = mBox.i.y + mPos.y;
    f32 boxMaxY = mBox.f.y + mPos.y;

    s32 serial = -1;
    for (;;) {
        serial = getActorManager_801F856C()->fn_801F78F4('ICEB', serial);
        if (serial < 0) {
            return false;
        }

        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            continue;
        }
        if (field_0x4 != actor->field_0x4) {
            continue;
        }

        f32 aMinX = actor->mBox.i.x + actor->mPos.x;
        f32 aMaxX = actor->mBox.f.x + actor->mPos.x;
        f32 aMinY = actor->mBox.i.y + actor->mPos.y;
        f32 aMaxY = actor->mBox.f.y + actor->mPos.y;

        if (aMinX <= boxMinX) {
            aMinX = boxMinX;
        }
        if (aMinY <= boxMinY) {
            aMinY = boxMinY;
        }
        if (aMaxX >= boxMaxX) {
            aMaxX = boxMaxX;
        }
        if (aMaxY >= boxMaxY) {
            aMaxY = boxMaxY;
        }

        bool hit = false;
        if (aMaxX >= aMinX && aMaxY >= aMinY) {
            hit = true;
        }
        if (hit) {
            return true;
        }
    }
    return false;
}

void FireRodFire_804A89F0::vfunc_0x54(const JGeometry::TVec3f* pos, bool arg2) {
    if (!arg2) {
        if (field_0x264 == 0 || field_0x264 == 1 || field_0x264 == 5) {
            s32 r31 = field_0x260;
            if (r31 < 0) {
                getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, pos, (field_0x98 << 4) | 0xF, -1, -1);
            } else {
                getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, pos, (field_0x98 << 4) | r31, -1, field_0x110);
            }
        }
    } else {
        getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, pos, (field_0x98 << 4) | 0x18F, -1, -1);
    }
}

void FireRodFire_804A89F0::vfunc_0x58() {
    if (field_0x291 == 1) {
        mPos = field_0x248;
    } else {
        startSound_801F0E34(0x3002E, 0);
        vfunc_0x24();
    }
}

// FIXME: 91.67%. missing/extra + reloc-name. Float literal pool ordering.
void FireRodFire_804A89F0::fn_802BAA0C(const JGeometry::TBox2f* box) {
    f32 cx = (box->i.x - box->f.x) * 0.5f + box->f.x;
    f32 cy = (box->i.y - box->f.y) * 0.5f + box->f.y;
    f32 negX = -cx;
    f32 negY = -cy;

    mPos.x = cx;
    mPos.y = cy;
    mPos.z = 0.0f;

    f32 bx0 = box->i.x;
    f32 by0 = box->i.y;
    f32 bx1 = box->f.x;
    f32 by1 = box->f.y;

    mBox.i.x = bx0;
    mBox.i.y = by0;
    mBox.f.x = bx1;
    mBox.f.y = by1;

    mBox.i.x += negX;
    mBox.i.y += negY;
    mBox.f.x += negX;
    mBox.f.y += negY;

    field_0x80.i.x = bx0;
    field_0x80.i.y = by0;
    field_0x80.f.x = bx1;
    field_0x80.f.y = by1;

    field_0x80.i.x += negX;
    field_0x80.i.y += negY;
    field_0x80.f.x += negX;
    field_0x80.f.y += negY;

    if (field_0x268 == 1 || field_0x268 == 2) {
        if (viewLayerFlag(field_0x4)) {
            if (field_0x98 < 2) {
                field_0x80.i.y += 2.0f;
                field_0x80.f.y -= 2.0f;
            } else {
                field_0x80.i.x += 2.0f;
                field_0x80.f.x -= 2.0f;
            }
        }
    }
}

void ActorBase_804A5770::fn_802BAA00() {
    mAlive = false;
}

s32 FireRodFire_804A89F0::fn_802BA9F8() {
    return field_0x264;
}

s32 FireRodFire_804A89F0::fn_802BA9F0() {
    return field_0x260;
}

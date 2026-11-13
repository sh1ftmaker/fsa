#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/ViewLayer.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/GameGlobals.h"
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

// FIXME(shotgun): 70.32 %, missing/extra
void FireRodFire_804A89F0::update() {
    switch (field_0x238) {
    case 0: {
        if (field_0x278 != field_0x4) {
            if (field_0x264 == 0 || field_0x264 == 3 || field_0x264 == 4 || field_0x264 == 5) {
                if (field_0x4 < 8) {
                    JGeometry::TVec3f pos = mPos;
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
                    mEmitter230 = spawnEmitter_8013CC50(field_0x4, &pos, 0x19B, 0, 0, NULL, NULL, true, 1.0f);
                    if (mEmitter230 != NULL) {
                        JPAGetXYZRotateMtx(0, lbl_8046D388[field_0x98], 0, mEmitter230->mGlobalRot);
                    }
                    mEmitter234 = spawnEmitter_8013CC50(field_0x4, &pos, 0x19C, 0, 0, NULL, NULL, true, 1.0f);
                    if (mEmitter234 != NULL) {
                        JPAGetXYZRotateMtx(0, lbl_8046D388[field_0x98], 0, mEmitter234->mGlobalRot);
                    }
                } else {
                    if (mEmitter230 != NULL) {
                        mEmitter230->becomeInvalidEmitter();
                        mEmitter230 = NULL;
                    }
                    if (mEmitter234 != NULL) {
                        mEmitter234->becomeInvalidEmitter();
                        mEmitter234 = NULL;
                    }
                }
                field_0x278 = field_0x4;
            }
        }
        if (field_0x264 == 3 || field_0x264 == 4) {
            if (vfunc_0x34()) {
                if (field_0x292 != 1) {
                    return;
                }
                field_0x293 = 1;
            }
        }
        if ((field_0x264 == 0 || field_0x264 == 3 || field_0x264 == 4) && mEmitter230 != NULL) {
            JGeometry::TVec3f pos = mPos;
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
            mEmitter230->mGlobalTrs.set(pos);
            mEmitter234->mGlobalTrs.set(pos);
            if (field_0x293 == 1) {
                field_0x28c -= 0.03f;
                if (field_0x28c < 0.05f) {
                    fn_801F3668();
                    return;
                }
                mEmitter230->mGlobalPScl.set(field_0x28c, field_0x28c);
                mEmitter234->mGlobalPScl.set(field_0x28c, field_0x28c);
            }
        }
        JGeometry::TBox2f box = mBox;
        box.addPos(mPos.x, mPos.y);
        if (field_0x264 == 0 || field_0x264 == 3 || field_0x264 == 4) {
            if (field_0x293 == 0) {
                addCalc2_801F71A4(&field_0x3c.x, field_0x23c.x, 0.3f, 1.0f);
                addCalc2_801F71A4(&field_0x3c.y, field_0x23c.y, 0.3f, 1.0f);
            } else {
                addCalc2_801F71A4(&field_0x3c.x, 0.0f, 0.045f, 0.3f);
                addCalc2_801F71A4(&field_0x3c.y, 0.0f, 0.045f, 0.3f);
            }
            mPos += field_0x3c;
        }
        field_0x248 = mPos;
        s32 dmg = field_0x114;
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            dmg = 0;
        }
        if (field_0x264 == 2 || (field_0x264 >= 3 && field_0x264 <= 5)) {
            s32 hitPlayer = -1;
            if (field_0x264 == 3 || field_0x264 == 4) {
                hitPlayer = fn_80239A74(&box, field_0x4, field_0x98, false, false);
            }
            if ((field_0x264 == 3 || field_0x264 == 4) && hitPlayer >= 0) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x19D, 0, 0, NULL, NULL, true, field_0x28c);
                fn_80230FD0(hitPlayer, 0x20050);
                field_0x25c = 5;
                field_0x238 = 5;
            } else if (field_0x264 != 5 && field_0x293 == 0) {
                fn_801F2B7C(-100, -1, true);
            }
            if (fn_801F2618(NULL, false)) {
                if (field_0x264 == 3 || field_0x264 == 4) {
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x19D, 0, 0, NULL, NULL, true, field_0x28c);
                    startSound_801F0E34(0x3002E, 0);
                    vfunc_0x24();
                } else {
                    field_0x25c = 5;
                    field_0x238 = 5;
                }
            }
        } else {
            bool skip = false;
            if (field_0x264 == 1) {
                if (fn_8022CCAC(field_0x260) || fn_80230188(field_0x260)) {
                    skip = true;
                }
            }
            if (!skip) {
                if (fn_801F2B7C(dmg, field_0x260, 0x16) || fn_801F2B20(5)) {
                    if (field_0x264 == 0) {
                        if (field_0x4 < 8) {
                            mAlive = false;
                        } else {
                            field_0x238 = 4;
                            field_0x26c = 0;
                            field_0x270 = 0;
                        }
                    } else {
                        if (fn_801F2618(NULL, false)) {
                            vfunc_0x58();
                        }
                    }
                } else {
                    if (fn_801F2618(NULL, false)) {
                        vfunc_0x58();
                    }
                    if (mAlive && field_0x264 == 0 && field_0x268 == 2 && field_0x290 == 0) {
                        field_0x248 = mPos;
                        s32 saved = field_0x254;
                        field_0x254 = 0;
                        switch (field_0x98) {
                        case 0:
                        case 1:
                            mPos.y += 16.0f;
                            break;
                        case 2:
                        case 3:
                            mPos.x += 16.0f;
                            break;
                        }
                        if (fn_801F2618(NULL, false)) {
                            vfunc_0x58();
                        }
                        if (field_0x254 == 0) {
                            field_0x254 = saved;
                        }
                        mPos = field_0x248;
                    }
                }
            }
        }
        if (field_0x4 >= 8) {
            if (field_0x264 == 0 || field_0x264 == 3 || field_0x264 == 4) {
                field_0x274--;
                if (field_0x238 == 0 && field_0x274 == 0) {
                    field_0x274 = 2;
                    field_0x270++;
                    if (field_0x270 >= 4) {
                        field_0x270 = 0;
                    }
                }
            } else {
                field_0x270++;
                if (field_0x270 >= 6) {
                    field_0x270 = 3;
                }
            }
        }
        if (field_0x264 == 5 && mAlive) {
            field_0x270 = 0;
            if (field_0x280 == 0) {
                if (fn_801F2618(NULL, false)) {
                    vfunc_0x58();
                }
                vfunc_0x24();
            } else {
                field_0x280--;
            }
        }
        break;
    }
    case 4:
        field_0x270++;
        if (field_0x270 >= 16) {
            mAlive = false;
        }
        break;
    case 5:
        if (field_0x25c == 0) {
            startSound_801F0E34(0x3002E, 0);
            vfunc_0x24();
        } else {
            field_0x25c--;
        }
        break;
    }
    if (field_0x264 == 0) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
    if (field_0x254 != 0) {
        field_0x254--;
    }
}

// FIXME: (field_0x268 == 2) is built as extrwi 8,19 here; the original keeps cntlzw + srwi 5 for it.
void FireRodFire_804A89F0::draw() {
    if (field_0x264 != 5 && field_0x264 != 6) {
        JGeometry::TVec3f pos = mPos;
        u32 isTwo = (field_0x268 == 2);
        if (field_0x4 >= 8) {
            if (field_0x264 == 0 || field_0x264 == 3 || field_0x264 == 4 || field_0x264 == 5) {
                fn_803075AC(mSerial, field_0x4, &mPos, field_0x26c + field_0x270 + 0x109, isTwo != 0, 2, -1, 1, 0, 0);
                return;
            }
            if (field_0x264 == 1) {
                if (fn_8022CCAC(field_0x260)) {
                    return;
                }
                if (fn_80230188(field_0x260)) {
                    return;
                }
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
}

// FIXME: 98.15%. missing/extra + reloc-name. The original's second `field_0x264 == 0` test has an else that only clears mAlive (3 instructions); float literal placement.
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
        }

        if (field_0x264 == 0) {
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

        s32 kind = 0;
        if (field_0x264 == 5) {
            kind = 4;
        }
        if (field_0x264 == 5 && field_0x291 == 0) {
            s32 layer = field_0x4;
            if (layer < 8) {
                spawnEmitter_8013CC50(layer, &mPos, 0x19D, 0, 0, NULL, NULL, true, 1.0f);
            } else {
                getActorManager_801F856C()->spawnActor_801F9484('KMRI', layer, &mPos, 6, -1, -1);
            }
        } else {
            s32 id = field_0x260;
            if (id < 0) {
                getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, &mPos, (kind << 6) | (field_0x98 << 4) | 0xF, -1, -1);
            } else {
                getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, &mPos, (kind << 6) | (field_0x98 << 4) | id, -1, field_0x110);
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
        mEmitter230->becomeInvalidEmitter();
        mEmitter230 = NULL;
    }
    if (mEmitter234 != NULL) {
        mEmitter234->becomeInvalidEmitter();
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

// FIXME: 91.67%. missing/extra + reloc-name. The original loads box->i.x/i.y/f.x/f.y once, after the mPos stores, and stores them to both boxes; ours lacks four of those loads. Float literal pool ordering.
void FireRodFire_804A89F0::fn_802BAA0C(const JGeometry::TBox2f* box) {
    f32 cx = (box->i.x - box->f.x) * 0.5f + box->f.x;
    f32 cy = (box->i.y - box->f.y) * 0.5f + box->f.y;
    f32 negX = -cx;
    f32 negY = -cy;

    mPos.x = cx;
    mPos.y = cy;
    mPos.z = 0.0f;

    mBox.i.x = box->i.x;
    mBox.i.y = box->i.y;
    mBox.f.x = box->f.x;
    mBox.f.y = box->f.y;

    mBox.i.x += negX;
    mBox.i.y += negY;
    mBox.f.x += negX;
    mBox.f.y += negY;

    field_0x80.i.x = box->i.x;
    field_0x80.i.y = box->i.y;
    field_0x80.f.x = box->f.x;
    field_0x80.f.y = box->f.y;

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

// FIXME(shotgun): 45.65 %, missing/extra + reloc-name
bool FireRodFire_804A89F0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    u32 ignoreIds[5] = {'TOMI', 'IPOT', 'WDRY', 'ICEB', 'NULL'};
    if (field_0x264 != 1) {
        ignoreIds[3] = 'NULL';
    }
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, ignoreIds) == true) {
        return true;
    }
    if (field_0x264 == 3 || field_0x264 == 4) {
        if (field_0x254 > 0) {
            return false;
        }
        JGeometry::TVec3f p(pos->x, pos->y, pos->z);
        u16 code = fn_8022461C(&p, layer);
        if (code == 0) {
            return false;
        }
        if (code >= 0x28 && code <= 0x2B) {
            return false;
        }
        switch (field_0x98) {
        case 0:
            return code != 0x42;
        case 1:
            return code != 0x43;
        case 2:
            return code != 0x40;
        case 3:
            return code != 0x41;
        }
        return false;
    }
    if (field_0x254 != 0) {
        return false;
    }
    s32 cell = fn_80226AE0(pos, field_0x4);
    field_0x288 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(field_0x4, cell);
    if (fn_801EE3C0(field_0x288) == true) {
        JGeometry::TVec3f tmp;
        fn_80226CA8(cell, field_0x4, &tmp);
        tmp.x += 8.0f;
        tmp.y += 8.0f;
        JGeometry::TVec3f spawnPos(tmp);
        vfunc_0x54(&spawnPos, false);
        field_0x254 = 5;
        field_0x291 = 1;
        return true;
    }
    field_0x291 = 0;
    if (field_0x288 >= 0x140 && field_0x288 <= 0x15F) {
        field_0x254 = 0x10;
        if (field_0x290 == 0) {
            field_0x290 = 1;
        } else {
            field_0x290 = 0;
        }
        return false;
    }
    if (field_0x288 == 0x3EB || field_0x288 == 0x34 || field_0x288 == 0x33) {
        JGeometry::TVec3f tmp;
        fn_80226CA8(cell, field_0x4, &tmp);
        tmp.x += 8.0f;
        tmp.y += 8.0f;
        JGeometry::TVec3f spawnPos(tmp);
        vfunc_0x54(&spawnPos, true);
        return false;
    }
    field_0x27c = lbl_8046CB88[field_0x288];
    s32 v = field_0x27c;
    if (v == 0 || v == 0x61 || (v >= 2 && v <= 7) || v == 0x50 || (v >= 0x27 && v <= 0x2F) || v == 0x20 ||
        (v >= 0xC && v <= 0xE) || (v >= 0x39 && v <= 0x3B) || (v >= 0x23 && v <= 0x24) || (v >= 4 && v <= 7) ||
        (v >= 0x18 && v <= 0x1F)) {
        return false;
    }
    if ((v >= 0x10 && v <= 0x16) || v == 0x17) {
        field_0x254 = 0x10;
        if (field_0x290 == 0) {
            field_0x290 = 1;
        } else {
            field_0x290 = 0;
        }
        return false;
    }
    u16 tile = field_0x288;
    if (tile == 0x3A || (tile >= 0x8B && tile <= 0x8E) || tile == 0x3D) {
        return false;
    }
    if (v == 0x25) {
        return getGameView_802D6A9C()->field_0x171 == 0;
    }
    if (v == 0x26) {
        return getGameView_802D6A9C()->field_0x171 == 1;
    }
    if (v == 0x22) {
        if (getGameView_802D6A9C()->field_0x170 != 1) {
            return false;
        }
    }
    return field_0x290 == 0;
}

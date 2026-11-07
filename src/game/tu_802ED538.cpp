#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/SpriteParams.h"
#include "game/act/tu_802ED538.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

// The TIMG resources by the spawn parameter (bits 17-18).
const char* lbl_804AA330[4] = {"jen_blue.bti", "jen_green.bti", "jen_red.bti", "jen_purple.bti"};
const char* lbl_804AA340[4] = {"jen_blue_tsume.bti", "jen_green_tsume.bti", "jen_red_tsume.bti", "jen_purple_tsume.bti"};

// Emitter ids by the spawn parameter (bits 17-18): three each in the first two tables, five in the third.
static const u16 lbl_8046F544[12] = {234, 235, 236, 237, 238, 239, 243, 244, 245, 240, 241, 242};
static const u16 lbl_8046F55C[12] = {960, 961, 962, 963, 964, 965, 969, 970, 971, 966, 967, 968};
static const u16 lbl_8046F5C8[4][4] = {{2, 3, 4, 5}, {6, 7, 8, 9}, {14, 15, 16, 17}, {10, 11, 12, 13}};
static const u16 lbl_8046F5E8[20] = {1206, 1207, 1208, 1209, 1210, 1211, 1212, 1213, 1214, 1215,
                                     1221, 1222, 1223, 1224, 1225, 1216, 1217, 1218, 1219, 1220};

void fn_802EEA24(u8 value) {
    lbl_8053AB60 = value;
}

// FIXME(shotgun): 99.9 %, reloc-name
void cls_804AA350::init() {
    mEmitter230 = NULL;
    mEmitter234 = NULL;
    mEmitter238 = NULL;
    mEmitter23c = NULL;
    mEmitter240 = NULL;
    mEmitter244 = NULL;
    mEmitter248 = NULL;
    mPos.x = (f32)(((s32)mPos.x / 16) * 16 + 8);
    field_0x24c = 0;
    field_0x254 = (field_0x90 >> 17) & 3;
    field_0x250 = 0;
    field_0x258 = (field_0x90 >> 12) & 0x1F;
    field_0x25c = (field_0x90 >> 7) & 0x1F;
    field_0x276 = 0;
    field_0x277 = 0;
    field_0x278 = 0;
    field_0x268.x = mPos.x;
    field_0x268.y = mPos.y;
    field_0x268.z = mPos.z;
    field_0x268.y -= 32.0f;
    field_0x260[0] = -1;
    field_0x260[1] = -1;
    field_0x275 = 0xFF;
    field_0x279 = 0;
    if (vfunc_0x34() == true) {
        mAlive = false;
        field_0x2cc = NULL;
        field_0x24c = 16;
    } else {
        field_0x2cc = fn_804032E8('TIMG', lbl_804AA330[field_0x254]);
        mEmitter230 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x4E, 0, 0, NULL, NULL, true, 1.0f);
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x27c[i].mX = 0.0f;
        field_0x27c[i].mY = 0.0f;
        field_0x27c[i].mZ = 0.0f;
        field_0x27c[i].mDist = 0.0f;
        field_0x27c[i].mAngle = 0;
        field_0x27c[i].mActive = 0;
    }
    field_0x11f = 1;
}

// FIXME(shotgun): 83.7 %, missing/extra
void cls_804AA350::vfunc_0x10() {
    s32 count = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < count; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_WARP && field_0x25c == actor->vfunc_0x3C()) {
            JGeometry::TVec3f pos = actor->mPos;
            pos.x = mPos.x;
            pos.y = 32.0f + mPos.y;
            actor->mPos = pos;
        }
    }
    s32 found = 0;
    for (s32 i = 0; i < count; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == ACTOR_SLR4) {
            field_0x260[found] = i;
            found++;
            fn_802EEA24(1);
            if (found >= 2) {
                break;
            }
        }
    }
}

cls_804AA350::~cls_804AA350() {
    if (mEmitter230 != NULL) {
        releaseEmitter_801EE434(mEmitter230);
    }
    if (mEmitter234 != NULL) {
        releaseEmitter_801EE434(mEmitter234);
    }
    if (mEmitter238 != NULL) {
        releaseEmitter_801EE434(mEmitter238);
    }
    if (mEmitter23c != NULL) {
        releaseEmitter_801EE434(mEmitter23c);
    }
    if (mEmitter240 != NULL) {
        releaseEmitter_801EE434(mEmitter240);
    }
    if (mEmitter244 != NULL) {
        releaseEmitter_801EE434(mEmitter244);
    }
    if (mEmitter248 != NULL) {
        releaseEmitter_801EE434(mEmitter248);
    }
}

static inline void setCameraScript(const JGeometry::TVec3f& pos) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x7f5 == 0) {
        camera->field_0x7f4 = 1;
        camera->field_0x7f8 = 4;
        camera->field_0x7fc.x = pos.x;
        camera->field_0x7fc.y = pos.y;
        camera->field_0x7fc.z = pos.z;
        camera->field_0x7f6 = 0;
    }
}

static inline void armFlag() {
    cls_804A03A8* obj = gGameRoot2_8053AB10->field_0x30;
    if (obj->field_0x284 == 0) {
        obj->field_0x284 = 1;
        obj->field_0x28c = 0.05f;
    }
}

static inline void stopEmitter(JPABaseEmitter*& emitter) {
    if (emitter != NULL) {
        emitter->stopCreateParticle();
        emitter = NULL;
    }
}

// Whether the actor in slot `slot` of the manager is an SLR4.
static inline bool isSlr4(s32 slot) {
    if (slot >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[slot];
        if (actor != NULL && actor->mActorId == ACTOR_SLR4) {
            return true;
        }
    }
    return false;
}

// FIXME(shotgun): 79.8 %, missing/extra + reloc-name
void cls_804AA350::update() {
    switch (field_0x24c) {
    case 0:
        if (!getGameView_802D6A9C()->fn_802DCD0C()) {
            setCameraScript(mPos);
            if (getActorManager_801F856C()->fn_801F68E0() != 0) {
                field_0x250 = 20;
                field_0x24c++;
                gAudioMgr_8053AF80->fn_80458FF0(0x10000D, false, 0);
                fn_8022ADE4();
            }
        }
        break;
    case 1:
        setCameraScript(mPos);
        field_0x250--;
        if (field_0x250 <= 0) {
            field_0x250 = 60;
            field_0x24c++;
        }
        break;
    case 2:
        setCameraScript(mPos);
        armFlag();
        field_0x250--;
        if (field_0x250 <= 0) {
            getGameView_802D6A9C()->fn_802D8050(field_0x90 & 0x1F, true, 0xFF);
            field_0x250 = 180;
            field_0x24c++;
        }
        break;
    case 3:
        setCameraScript(mPos);
        armFlag();
        field_0x250--;
        if (field_0x250 <= 0) {
            getActorManager_801F856C()->fn_801F6884();
            if (isSlr4(field_0x260[0])) {
                fn_802EEA24(0);
            }
            if (isSlr4(field_0x260[1])) {
                fn_802EEA24(0);
            }
            field_0x24c++;
        }
        break;
    case 4:
        if (getGameView_802D6A9C()->fn_802D800C(field_0x258)) {
            if (getActorManager_801F856C()->fn_801F68E0() != 0) {
                if (isSlr4(field_0x260[0])) {
                    fn_802EEA24(1);
                }
                if (isSlr4(field_0x260[1])) {
                    fn_802EEA24(1);
                }
                field_0x250 = 60;
                field_0x24c = 5;
            }
        }
        break;
    case 5:
        setCameraScript(mPos);
        armFlag();
        field_0x250--;
        if (field_0x250 <= 0) {
            stopEmitter(mEmitter230);
            mEmitter230 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F5C8[field_0x254][0], 0, -1, NULL, NULL, true, 1.0f);
            mEmitter234 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F5C8[field_0x254][1], 0, -1, NULL, NULL, true, 1.0f);
            mEmitter238 = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F5C8[field_0x254][2], 0, -1, NULL, NULL, true, 1.0f);
            mEmitter23c = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F5C8[field_0x254][3], 0, -1, NULL, NULL, true, 1.0f);
            field_0x250 = 180;
            field_0x24c++;
            startSound_801F0E34(0x5005A, 0);
        }
        break;
    case 6:
        setCameraScript(mPos);
        armFlag();
        field_0x250--;
        if (field_0x250 <= 0) {
            getGameView_802D6A9C()->fn_802D8050(field_0x25c, true, 0xFF);
            getActorManager_801F856C()->fn_801F6884();
            for (s32 p = 0; p < 4; p++) {
                fn_8022CA6C(p, 5, false);
            }
            if (isSlr4(field_0x260[0])) {
                fn_802EEA24(0);
            }
            if (isSlr4(field_0x260[1])) {
                fn_802EEA24(0);
            }
            field_0x24c++;
            startSound_801F0E34(0x5005B, 0);
        }
        break;
    case 7:
        if (field_0x279) {
            if (field_0x276) {
                getGameView_802D6A9C()->fn_802D8050(field_0x25c, false, 0xFF);
                field_0x250 = 0x7C;
                field_0x24c++;
                startSound_801F0E34(0x5005C, 0);
            }
        } else if (fn_802ED538()) {
            stopEmitter(mEmitter23c);
            if (gGameRoot_8053AAF8->field_0x7e == 1) {
                gAudioMgr_8053AF80->fn_8045815C(true);
                field_0x279 = 1;
            } else {
                startSound_801F0E34(0x5005B, 0);
            }
        }
        break;
    case 8:
        field_0x275 -= 2;
        mEmitter234->mGlobalPrmClr.a = field_0x275;
        mEmitter238->mGlobalPrmClr.a = field_0x275;
        field_0x250--;
        if (field_0x250 <= 0) {
            deleteEmitter(&mEmitter234);
            deleteEmitter(&mEmitter238);
            field_0x275 = 0xFF;
            field_0x24c++;
        }
        break;
    case 9:
        if (field_0x277) {
            spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F544[field_0x254 * 3], 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F544[field_0x254 * 3 + 1], 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F544[field_0x254 * 3 + 2], 0, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x50058, 0);
            field_0x250 = 0x78;
            field_0x24c = 10;
        }
        break;
    case 10:
        field_0x250--;
        if (field_0x250 <= 0) {
            mEmitter240 = spawnEmitter_8013CC50(field_0x4, &field_0x268, lbl_8046F55C[field_0x254 * 3], 0, 0, NULL, NULL, true, 1.0f);
            mEmitter244 = spawnEmitter_8013CC50(field_0x4, &field_0x268, lbl_8046F55C[field_0x254 * 3 + 1], 0, 0, NULL, NULL, true, 1.0f);
            field_0x2cc = fn_804032E8('TIMG', lbl_804AA340[field_0x254]);
            field_0x3c.z = 0.3f;
            field_0x24c++;
        }
        break;
    case 11:
        setEmitterPos_8013CB44(mEmitter240, &field_0x268, 0, -1, false);
        setEmitterPos_8013CB44(mEmitter244, &field_0x268, 0, -1, false);
        if (field_0x275 > 2) {
            field_0x275 -= 2;
        } else {
            field_0x275 = 0;
        }
        mEmitter230->mGlobalPrmClr.a = field_0x275;
        if (field_0x268.z >= 40.0f) {
            field_0x268.z = 40.0f;
            deleteEmitter(&mEmitter230);
            field_0x24c = 12;
        }
        break;
    case 12:
        if (field_0x278) {
            spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F5E8[field_0x254 * 5], 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F5E8[field_0x254 * 5 + 1], 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F5E8[field_0x254 * 5 + 2], 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F5E8[field_0x254 * 5 + 3], 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8046F5E8[field_0x254 * 5 + 4], 0, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x5006F, 0);
            field_0x24c = 13;
            field_0x250 = 180;
        }
        break;
    case 13:
        field_0x250--;
        if (field_0x250 <= 0) {
            getGameView_802D6A9C()->fn_802D8050(field_0x90 >> 27, true, 0xFF);
            field_0x2cc = NULL;
            field_0x24c++;
        }
        break;
    case 14:
        setEmitterPos_8013CB44(mEmitter240, &field_0x268, 0, -1, false);
        setEmitterPos_8013CB44(mEmitter244, &field_0x268, 0, -1, false);
        break;
    case 15:
        if (field_0x275 > 8) {
            field_0x275 -= 8;
        } else {
            field_0x275 = 0;
        }
        if (field_0x275 != 0) {
            mEmitter240->mGlobalPrmClr.a = field_0x275;
            mEmitter244->mGlobalPrmClr.a = field_0x275;
            setEmitterPos_8013CB44(mEmitter240, &field_0x268, 0, -1, false);
            setEmitterPos_8013CB44(mEmitter244, &field_0x268, 0, -1, false);
        } else {
            deleteEmitter(&mEmitter240);
            deleteEmitter(&mEmitter244);
            field_0x24c = 16;
        }
        break;
    }
}

// FIXME(shotgun): 97.6 %, scheduling
void cls_804AA350::draw() {
    if (field_0x2cc != NULL) {
        SpriteParams_801F5AE0 params;
        params.field_0x1c = 0;
        params.field_0x1d = -3;
        params.field_0x24 = 1;
        params.field_0x23 = 1;
        params.field_0x21 = 1;
        if (field_0x4 >= 8) {
            return;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, field_0x2cc, &params);
    }
}

u8 cls_804AA350::fn_802ED7F0(f32 value) {
    if (field_0x24c < 11) {
        return 0;
    }
    if (field_0x274) {
        field_0x268.z = value;
    } else if (value == 0.0f) {
        field_0x274 = 1;
    }
    return field_0x274;
}

void cls_804AA350::fn_802ED7D8() {
    field_0x275 = 0xFF;
    field_0x24c++;
}

// FIXME(shotgun): 42.9 %, missing/extra + reloc-name
bool cls_804AA350::fn_802ED538() {
    bool result = true;
    for (s32 p = 0; p < 4; p++) {
        Elem_80200544& e = field_0x27c[p];
        if (fn_8023F264(p) && getPlayerLayer_8023DE58(p) == field_0x4) {
            if (!fn_80236B50(p) && !fn_8023EA28(p) && !fn_80230188(p) && !fn_8022D534(p) && !e.mActive &&
                fn_802300EC(p)) {
                JGeometry::TVec3f* pos = getPlayerPos_8023E724(p);
                if (pos->z == 0.0f) {
                    f32 dx = mPos.x - pos->x;
                    f32 dy = (32.0f + mPos.y) - pos->y;
                    e.mX = getPlayerPos_8023E724(p)->x;
                    e.mY = getPlayerPos_8023E724(p)->y;
                    e.mZ = getPlayerPos_8023E724(p)->z;
                    f32 len = dx * dx + dy * dy;
                    if (len > 0.0f) {
                        len = len * (1.0f / std::sqrtf(len));
                    }
                    e.mDist = len;
                    e.mAngle = (s16)(s32)(JMath::atanTable_.atan2_(-dx, -dy) * 10430.378f);
                    e.mActive = 1;
                    fn_80230AD4(p);
                    fn_8022CA6C(p, 5, false);
                }
            }
        }
        if (e.mActive && fn_80230188(p) && !fn_8022D75C(p)) {
            if (e.mDist > 4.0f) {
                e.mDist = e.mDist - 4.0f;
            } else {
                e.mDist = 0.0f;
            }
            const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
            e.mX = mPos.x + e.mDist * sincos.sinShort(e.mAngle);
            e.mY = (32.0f + mPos.y) + e.mDist * sincos.cosShort(e.mAngle);
            JGeometry::TVec3f tmp(e.mX, e.mY, e.mZ);
            fn_802303FC(p, &tmp, true);
            if (e.mDist < 1.0f) {
                fn_80230224(p, 0);
            }
        }
        if (getPlayerLayer_8023DE58(p) == field_0x4) {
            result = false;
        }
    }
    return result;
}

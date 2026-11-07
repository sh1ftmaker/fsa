// The SLR1 actor (sword soldier, docs/names/actors.tsv; vtable 0x8049FCA0,
// constructor 0x80203CA4 in the spawner). Only the small helpers (and the
// actor-manager wrappers) are written so far.

#include "game/GameAudio.h" // its JAudio pools give the unit its __sinit (0x8015478C)
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/ViewLayer.h"
#include "game/act/act_chief_soldier.h"
#include "game/act/act_sword_soldier.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

extern const char lbl_80465DBC[]; // "enemy_both2_sob.bin"
extern const char lbl_80465DD0[]; // "enemy_gc_sob.bin"
extern s32 lbl_80529DEC[];
extern s32 lbl_8049FA78[];
extern s32 lbl_804BF850[4]; // serials of the soldiers that took a hit
extern s32 lbl_8053AB40;
extern s32 lbl_8053AB44;
extern s32 lbl_8053AB48;
extern s32 lbl_80539F68;

// Life and sprite mode per kind.
static struct {
    s32 life;
    s32 mode;
} lbl_8049FA40[7] = {{12, 2}, {12, 3}, {1, 4}, {12, 3}, {16, 6}, {8, 2}, {20, 8}};

// A random float in [0, 1) from the game's RNG. Called from 10 units.
// The instructions match; its 1.0f only pairs with lbl_8053D25C once the
// unit's earlier functions put 0.0f (lbl_8053D258) first in .sdata2.
f32 fn_80154744() {
    return gGameRoot_8053AAF8->mRandom.get_ufloat_1();
}

// Keeps pos in the camera's view (3 units call it; they ignore r3).
void fn_80154718(const JGeometry::TVec3f* pos) {
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(pos);
}

void fn_801546F4(JGeometry::TBox2f* dst, const JGeometry::TBox2f* src) {
    dst->set(*src);
}

JGeometry::TVec2f* fn_801546E8(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
    return v;
}

void fn_801546D0(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    *dst = *src;
}

void fn_801546BC(JGeometry::TVec3f* v) {
    v->zero();
}

u8 fn_80154698() {
    return getGameView_802D6A9C()->fn_802DCD0C();
}

// FIXME: 99.67. The instructions match; only the 10430.378f relocation differs
// (lbl_8053D310 in the original; the unit's .sdata2 isn't laid out yet).
s32 fn_8015465C(f32 y, f32 x) {
    return JMath::atanTable_.atan2_(y, x) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
}

u8 fn_80154624(s32 layer) {
    return viewLayerFlag(layer);
}

s32 ActorBase_804A5770::fn_8015461C() const {
    return field_0x4;
}

void ActorBase_804A5770::fn_80154614(s32 value) {
    field_0xb8 = value;
}

void fn_801545F0(JGeometry::TVec3f* a, const JGeometry::TVec3f* b) {
    a->add(*b);
}

s32 ActorBase_804A5770::fn_801545E8() const {
    return mSerial;
}

s32 ActorBase_804A5770::fn_801545E0() const {
    return field_0x9c;
}

u8 ActorBase_804A5770::fn_801545C0(JGeometry::TVec3f* out, bool flag) {
    return fn_801F2618(out, flag);
}

void fn_801545A4(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

void fn_80154590(JGeometry::TBox2f* box, f32 x0, f32 y0, f32 x1, f32 y1) {
    box->set(x0, y0, x1, y1);
}

void fn_8015457C(JGeometry::TVec2f* dst, const JGeometry::TVec2f* src) {
    dst->set(*src);
}

void fn_80154558(JGeometry::TVec2f* a, const JGeometry::TVec2f* b) {
    a->add(*b);
}

// FIXME: 99.58. The instructions match; only the 0.5f relocation differs
// (lbl_8053D2B0 in the original; the unit's .sdata2 isn't laid out yet).
void fn_80154528(const JGeometry::TBox2f* box, JGeometry::TVec2f* out) {
    out->x = box->f.x + (box->i.x - box->f.x) * 0.5f;
    out->y = box->f.y + (box->i.y - box->f.y) * 0.5f;
}

void fn_80154518(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

// The EnemyParam "atcp" damage (ActorBase's 0x114), passed to fn_801F2B7C.
s32 ActorBase_804A5770::fn_80154510() const {
    return field_0x114;
}

f32 fn_801544FC(s16 angle) {
    return JMASinShort(angle);
}

f32 fn_801544E4(s16 angle) {
    return JMACosShort(angle);
}

// An actor's position (its mPos). Out of line in the original, called from
// 4 other units on actors they got from findActorBySerial_801F666C / fn_8015445C.
JGeometry::TVec3f* ActorBase_804A5770::fn_801544DC() {
    return &mPos;
}

void ActorBase_804A5770::fn_801544D0(s32 value) {
    field_0xd8 = value;
    field_0xdc = value;
}

u32 ActorBase_804A5770::fn_801544C8() const {
    return field_0x90;
}

s32 ChiefSoldier_804A0B90::fn_801544C0() {
    return field_0x288;
}

void ChiefSoldier_804A0B90::fn_801544B4() {
    field_0x290 = -1;
}

s32 ChiefSoldier_804A0B90::fn_801544AC() const {
    return field_0x28c;
}

void ChiefSoldier_804A0B90::fn_8015449C() {
    field_0x290 = field_0x290 + 1;
}

// fn_80154490 to fn_801544C0: the sword soldier (SLR1) calls them on the
// SLR4 (Chief Soldier) actor that fn_801542AC finds, so they are that class's
// members; they sit here because this is the first unit that needs them.
void ChiefSoldier_804A0B90::fn_80154490() {
    field_0x2a5 = 1;
}

ActorBase_804A5770* fn_8015445C(s32 serial) {
    return getActorManager_801F856C()->findActorBySerial_801F666C(serial);
}

// FIXME: the instructions match; its four constants (0.0f, .5, 3.0, 0.0)
// only pair with lbl_8053D258 / lbl_8053D2F8-lbl_8053D308 once the unit's
// .sdata2 is laid out in the original order. std::sqrtf out of line; 13
// calls from 7 units.
f32 fn_80154378(f32 x) {
    return std::sqrtf(x);
}

// fabs narrowed to f32, out of line.
f32 fn_8015436C(f32 x) {
    return fabs(x);
}

s32 fn_80154310(u32 id, s32 layer, const JGeometry::TVec3f* pos, u32 arg, s32 a, s32 b) {
    return getActorManager_801F856C()->spawnActor_801F9484(id, layer, pos, arg, a, b);
}

s32 fn_801542AC(u32 id, s32 layer, ActorBase_804A5770** out, s32 excludeSerial) {
    return getActorManager_801F856C()->fn_801F7818(id, layer, out, excludeSerial);
}

s32 fn_80154248(u32 id, s32 layer, const JGeometry::TVec3f* pos, u32 arg) {
    return getActorManager_801F856C()->fn_801F8544(id, layer, pos, arg);
}

// FIXME(shotgun): 99.15 %, missing/extra + reloc-name
void SwordSoldier_8049FCA0::init() {
    mBox.set(-8.0f, -4.0f, 8.0f, 8.0f);
    mBox2.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x94 = (field_0x90 >> 16) & 0xF;
    field_0x288 = (field_0x90 >> 20) & 0xF;
    field_0x280 = 0;
    if (field_0x90 & 0x100) {
        field_0x280 = 1;
    }
    lbl_804BF850[0] = -1;
    lbl_80539F68 = 200;
    lbl_804BF850[1] = -1;
    lbl_804BF850[2] = -1;
    lbl_804BF850[3] = -1;
    lbl_8053AB48 = 0;
    field_0x284 = 0;
    if (!(field_0x90 & 0x1000)) {
        field_0x284 = 1;
    }
    field_0x2a4 = field_0x90 >> 24;
    field_0x2a8 = 0;
    if (field_0x2a4 == 0) {
        field_0x2a4 = -1;
    }
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x234 = 5;
    field_0x274 = 0;
    field_0x294 = 0.0f;
    field_0x298 = 1.0f;
    field_0x290 = 0.7f;
    field_0x27c = 0;
    field_0x29e = 0;
    if (field_0x94 == 1) {
        field_0x290 = 1.2f;
    }
    field_0x28c = 1.5f * field_0x290 + 0.2f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
    field_0x2ac.zero();
    field_0x98 = (s32)(3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) & 3;
    if (viewLayerFlag(field_0x4)) {
        field_0x98 &= 1;
    }
    field_0x238 = 8;
    field_0x278 = lbl_8053AB40;
    lbl_8053AB40++;
    field_0x29c = -1;
    field_0xb0 = 0x12205;
    field_0xbc = 2;
    field_0x90 &= 7;
    field_0x2a0 = 0;
    field_0x190 = 4;
    mLife = lbl_8049FA40[field_0x90].life;
    field_0x234 = lbl_8049FA40[field_0x90].mode;
    field_0xb8 = 1;
    if (field_0x94 == 2 || field_0x94 == 5 || field_0x94 == 3) {
        field_0xb0 = 0;
        field_0xb8 = 0;
        field_0x98 = 2;
        field_0x274 = lbl_8049FA78[field_0x98 * 8];
    }
    if (field_0x90 == 2 || field_0x90 == 3 || field_0x90 == 1) {
        field_0x190 = 5;
    }
    switch (field_0x90) {
    case 6:
        field_0x28c = 0.2f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + field_0x290;
        field_0xbc = 3;
        field_0xc0 = 3;
        field_0x284 = 0;
        field_0x54.set(1.5f, 1.5f, 1.5f);
        field_0x190 = 6;
        field_0xb0 = 0x62209;
        mBox2.set(-10.0f, -8.0f, 10.0f, 10.0f);
        mState = 11;
        break;
    case 1:
        field_0xb8 = 0;
        field_0x98 = 2;
        field_0x274 = 12;
        lbl_8053AB44 = 0;
        mState = 12;
        break;
    case 0:
    case 2:
    case 3:
        field_0xb0 = 0x62209;
        // fall through
    default:
        mState = 1;
        if (lbl_80529DEC[0x46] == 0 && lbl_80529DEC[0x47] == 0 && getGameView_802D6A9C()->field_0x18 == 2 &&
            getGameView_802D6A9C()->field_0x14 == 2) {
            if ((s32)(0.1f * mPos.x) == 0x68 && (s32)(0.1f * mPos.y) == 0x57) {
                field_0x23c = (s32)(40.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x6E;
                field_0x290 = 1.4f;
                field_0x98 = 0;
                switch (field_0x98) {
                case 0:
                    field_0x2ac.set(field_0x290, 0.0f, 0.0f);
                    break;
                case 1:
                    field_0x2ac.set(-field_0x290, 0.0f, 0.0f);
                    break;
                case 2:
                    field_0x2ac.set(0.0f, field_0x290, 0.0f);
                    break;
                case 3:
                    field_0x2ac.set(0.0f, -field_0x290, 0.0f);
                    break;
                }
                mPos.y -= 32.0f;
                mState = 0;
            } else {
                field_0x270 = 1;
            }
        }
        break;
    }
    if (field_0x288 == 1 || field_0x288 == 3) {
        mBox2.set(-8.0f, -8.0f, 8.0f, 8.0f);
        if (field_0x288 == 3) {
            mBox2.set(-10.0f, -8.0f, 10.0f, 10.0f);
            field_0x54.set(1.5f, 1.5f, 1.5f);
        }
        field_0xb0 = 0xA042004;
        field_0x234 = 2;
        field_0xb8 = 0;
        field_0x18c = 0;
        field_0x238 = (s32)(60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x3C;
        field_0x274 = 0x10;
        mState = 0x11;
    }
    if (field_0x2a4 >= 0) {
        if (field_0x94 >= 6 && field_0x94 <= 9) {
            field_0xb0 = 0xA040004;
            field_0x98 = field_0x94 - 6;
            field_0x25c = field_0x98;
            field_0x274 = lbl_8049FA78[field_0x98 * 8 + field_0x258];
            if (field_0x288 == 1 || field_0x288 == 3) {
                field_0x274 = field_0x98 * 4 + (field_0x258 & 3);
            }
            field_0x80.set(-6.0f, -6.0f, 6.0f, 6.0f);
            field_0xb8 = 0;
        }
        mState = 0xF;
    }
}

// FIXME(shotgun): 99.53 %, regalloc-only
void SwordSoldier_8049FCA0::vfunc_0x44() {
    lbl_8053AB44 = 1;
    field_0xb8 = 1;
    if (mLife <= 0) {
        s32 serial = field_0x2a4;
        if (serial >= 0) {
            ChiefSoldier_804A0B90* leader =
                (ChiefSoldier_804A0B90*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (leader != NULL) {
                leader->field_0x290 = -1;
            }
        }
    }
    if (field_0x94 >= 6 && field_0x94 <= 9) {
        s32 serial = field_0x2a4;
        if (serial >= 0) {
            ChiefSoldier_804A0B90* leader =
                (ChiefSoldier_804A0B90*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (leader != NULL) {
                leader->field_0x290 = -1;
            }
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (lbl_804BF850[i] == mSerial) {
            lbl_804BF850[i] = -1;
            field_0xbc = 2;
        }
    }
    if (field_0x284 != 0) {
        if (lbl_8053AB48 == 0) {
            lbl_80539F68 = 0;
        }
        lbl_8053AB48 += 2;
        if (lbl_8053AB48 > 200) {
            lbl_8053AB48 = 200;
        }
    }
}

// FIXME(shotgun): 93.16 %, missing/extra + reloc-name
SwordSoldier_8049FCA0::~SwordSoldier_8049FCA0() {
    if (mLife <= 0) {
        if (field_0x288 == 1 || field_0x288 == 3) {
            if (field_0x288 == 1) {
                getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, 4);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D9, 0, 1, NULL, NULL, true, 1.0f);
            } else {
                getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, 8);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D9, 0, 1, NULL, NULL, true, 1.5f);
            }
        }
    }
    s32 serial = field_0x2a4;
    if (serial >= 0) {
        ChiefSoldier_804A0B90* leader =
            (ChiefSoldier_804A0B90*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (leader != NULL) {
            leader->field_0x290 = -1;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (lbl_804BF850[i] == mSerial) {
            lbl_804BF850[i] = -1;
        }
    }
}

// FIXME(shotgun): 92.59 %, missing/extra + reloc-name
bool SwordSoldier_8049FCA0::vfunc_0x54() {
    if (viewLayerFlag(field_0x4)) {
        return false;
    }
    if (mState == 0xE) {
        return true;
    }
    if (mPos.z > 2.0f) {
        return false;
    }
    field_0x2b8.x = mPos.x;
    field_0x2b8.y = mPos.y;
    field_0x2b8.z = mPos.z;
    field_0x2b8.x = (f32)((s32)field_0x2b8.x & 0xFFF8);
    field_0x2b8.y = (f32)((s32)field_0x2b8.y & 0xFFF8);
    field_0x2b8.x += -12.0f * JMath::sincosTable_.sinShort((s16)fn_801F6874(field_0x98));
    field_0x2b8.y += -12.0f * JMath::sincosTable_.cosShort((s16)fn_801F6874(field_0x98));
    u16 tile = fn_8022461C(&field_0x2b8, field_0x4);
    if ((u16)(tile - 2) <= 1 || tile == 0xE) {
        field_0x250 = 10000;
        mLife = 200;
        field_0x280 = 0;
        if (tile == 0xE && fn_802DE9A0() == 1) {
            field_0x280 = 1;
        }
        field_0x3c.zero();
        field_0x2ac.zero();
        field_0xd8 = 0;
        field_0xdc = 0;
        mState = 0xD;
        return true;
    }
    return false;
}

// FIXME(shotgun): 92.50 %, missing/extra + reloc-name
void SwordSoldier_8049FCA0::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    s32 flags = 0;
    if (field_0x94 == 2) {
        return;
    }
    params.field_0x0[2] = field_0x234;
    if (field_0x288 != 1 && field_0x288 != 3) {
        if (field_0x90 == 2 || field_0x90 == 3 || field_0x90 == 1) {
            params.field_0x0[8] = field_0x234;
        }
        if (field_0x94 >= 6 && field_0x94 <= 9) {
            params.field_0x0[2] = 11;
        }
        if (mState == 0xE) {
            f32 yaw = field_0x294;
            f32 scale = field_0x298;
            f32 sx = sin(0.0f);
            f32 sy = sin(yaw);
            f32 sz = sin(0.0f);
            f32 cx = cos(0.0f);
            f32 cy = cos(yaw);
            f32 cz = cos(0.0f);
            mtx[0][0] = scale * (cy * cz);
            mtx[0][1] = scale * (sx * sy * cz - cx * sz);
            mtx[0][2] = scale * (cx * cz * sy + sx * sz);
            mtx[1][0] = scale * (cy * sz);
            mtx[1][1] = scale * (sx * sy * sz + cx * cz);
            mtx[1][2] = scale * (cx * sz * sy - sx * cz);
            mtx[2][0] = scale * -sy;
            mtx[2][1] = scale * (sx * cy);
            mtx[2][2] = scale * (cx * cy);
            params.mMtx = mtx;
            if (field_0x90 == 2 || field_0x90 == 3 || field_0x90 == 1) {
                fn_801F02BC(lbl_80465DBC, field_0x274 + 0x1F4, &params, 0, 0, false, 1.0f, 1.0f);
            } else {
                fn_801F02BC(lbl_80465DD0, field_0x274 + 0x8F, &params, 0, 0, false, 1.0f, 1.0f);
            }
            return;
        }
    }
    if (field_0x90 == 6 || field_0x288 == 3) {
        mtx[0][0] = field_0x54.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x54.y;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x54.z;
        params.mMtx = mtx;
        flags = 3;
    }
    if (mState == 8) {
        bool blink = false;
        s32 t = field_0x248;
        if (t < 100) {
            if (t >= 50) {
                blink = !(t & 8);
            } else {
                blink = !(t & 4);
            }
        }
        if (blink) {
            params.field_0x14.r = 0x50;
        }
    }
    if (mState == 9) {
        mtx[0][0] = field_0x54.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x54.y;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x54.z;
        params.mMtx = mtx;
    }
    bool far = false;
    if (field_0x4 >= 8 && (field_0x90 == 6 || field_0x288 == 3)) {
        far = true;
    }
    if (field_0x288 == 1 || field_0x288 == 3) {
        fn_801F02BC(lbl_80465DD0, field_0x274 + 0xA8, &params, flags, far, false, 1.0f, 1.0f);
    } else if (field_0x90 == 2 || field_0x90 == 3 || field_0x90 == 1) {
        fn_801F02BC(lbl_80465DBC, field_0x274 + 0x1F4, &params, flags, far, false, 1.0f, 1.0f);
    } else {
        fn_801F02BC(lbl_80465DD0, field_0x274 + 0x8F, &params, flags, far, false, 1.0f, 1.0f);
    }
}


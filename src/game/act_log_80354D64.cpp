#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/BgRewrite.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/SpriteParams.h"
#include "game/act/act_log_80354D64.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

// Unit-local rodata. Pairs / triples per direction (0 +x, 1 -x, 2 +y, 3 -y).
static const f32 lbl_804722B0[4][2] = {{16.0f, 0.0f}, {-16.0f, 0.0f}, {0.0f, 16.0f}, {0.0f, -16.0f}};
static const f32 lbl_804722D0[4][2] = {{0.25f, 0.0f}, {-0.25f, 0.0f}, {0.0f, 0.25f}, {0.0f, -0.25f}};
static const f32 lbl_804722F0[4][3] = {
    {24.0f, 0.0f, 0.0f}, {-24.0f, 0.0f, 0.0f}, {0.0f, 24.0f, 0.0f}, {0.0f, -24.0f, 0.0f}};
static const f32 lbl_80472320[4][3] = {
    {16.0f, 0.0f, 0.0f}, {-16.0f, 0.0f, 0.0f}, {0.0f, 16.0f, 0.0f}, {0.0f, -16.0f, 0.0f}};

static inline bool contains(const JGeometry::TBox2f& box, f32 x, f32 y) {
    return x >= box.i.x && y >= box.i.y && x < box.f.x && y < box.f.y;
}

Log_804ACD70::~Log_804ACD70() {
    if (mEmitter2e0 != NULL) {
        releaseEmitter_801EE434(mEmitter2e0);
    }
}

void Log_804ACD70::init() {
    field_0x2c0 = mSpawnParam_0x90 & 1;
    field_0x2c4 = (mSpawnParam_0x90 >> 4) & 1;
    field_0x2c8 = (mSpawnParam_0x90 >> 8) & 1;
    field_0x1a4 = 2;
    switch (field_0x2c8) {
    case 0:
        switch (field_0x2c0) {
        case 0:
            switch (field_0x2c4) {
            case 0:
                mBox.set(-46.0f, -7.0f, 46.0f, 7.0f);
                mMapBox_0x80.set(-47.0f, -7.0f, 47.0f, 7.0f);
                break;
            case 1:
                mBox.set(-46.0f, -14.0f, 46.0f, 14.0f);
                mMapBox_0x80.set(-47.0f, -15.0f, 47.0f, 15.0f);
                break;
            }
            break;
        case 1:
            switch (field_0x2c4) {
            case 0:
                mBox.set(-7.0f, -46.0f, 7.0f, 46.0f);
                mMapBox_0x80.set(-7.0f, -47.0f, 7.0f, 47.0f);
                break;
            case 1:
                mBox.set(-14.0f, -46.0f, 14.0f, 46.0f);
                mMapBox_0x80.set(-15.0f, -47.0f, 15.0f, 47.0f);
                break;
            }
            break;
        }
        break;
    case 1:
        mBox.set(-15.0f, -23.0f, 15.0f, 7.0f);
        mMapBox_0x80.set(-15.0f, -23.0f, 15.0f, 7.0f);
        mBox2.set(-15.0f, -23.0f, 15.0f, 7.0f);
        break;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x234[i] = -1;
        field_0x264[i] = 0;
        field_0x268[i].field_0x0 = 1;
        field_0x268[i].field_0x4.zero();
        field_0x268[i].field_0x10 = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x254[i] = -1;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x244[i] = 0;
    }
    field_0xb8 = 2;
    field_0x230 = 0;
    field_0x2b8 = 0;
    field_0x2bc = 0;
    field_0x2d0 = -1;
    field_0x2d4.zero();
    field_0x2cc = 0;
    mEmitter2e0 = NULL;
}

// FIXME(shotgun): 97.58 %, scheduling+regalloc; behaviour clean.
void Log_804ACD70::update() {
    JGeometry::TBox2f box(mMapBox_0x80);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    switch (field_0x230) {
    case 0: {
        fn_80239914(&box, mLayer_0x4, mSerial);
        fn_8020D58C()->addBox(&box, mLayer_0x4, mSerial);
        fn_80354FE8();
        if (isMotionless() == false) {
            mPos.add(mVel_0x3c);
            field_0x2d4.sub(mVel_0x3c);
            if (isMotionless() == true) {
                mVel_0x3c.zero();
                if (mEmitter2e0 != NULL) {
                    mEmitter2e0->becomeInvalidEmitter();
                    mEmitter2e0 = NULL;
                }
                field_0x2cc = 1;
            } else if (mEmitter2e0 != NULL) {
                setEmitterPos_8013CB44(mEmitter2e0, &mPos, 0, -2, true);
            }
        }
        if (isMotionless() != false && field_0x2c8 == 0) {
            for (s32 i = 0; i < 4; i++) {
                field_0x234[i] = -1;
            }
            for (s32 i = 0; i < 4; i++) {
                field_0x244[i] = 0;
            }
            field_0x2d0 = -1;
            for (s32 i = 0; i < 4; i++) {
                if (isPlayerActive_8023F264(i)) {
                    JGeometry::TVec3f* ppos = getPlayerPos_8023E724(i);
                    f32 px = ppos->x;
                    f32 py = ppos->y;
                    s32 dir = getPlayerFacing_80230CFC(i);
                    px += lbl_804722B0[dir][0];
                    py += lbl_804722B0[dir][1];
                    field_0x264[i] = fn_8022C004(i);
                    if (field_0x264[i] == 1 && fn_8023E8F0(i) == 1 && fn_8023188C(i) == 1 &&
                        fn_802319B4(i, 0x11) == 1 && !fn_80234D94(i) && contains(box, px, py) == true) {
                        field_0x234[i] = dir;
                        field_0x244[field_0x234[i]]++;
                    } else {
                        field_0x264[i] = 0;
                    }
                    if (field_0x264[i] == 1 && field_0x2d0 == -1) {
                        if (field_0x2c0 == 0) {
                            if (field_0x234[i] == 0 || field_0x234[i] == 1) {
                                break;
                            }
                        } else if (field_0x234[i] == 2 || field_0x234[i] == 3) {
                            break;
                        }
                        if (field_0x244[field_0x234[i]] >= 4) {
                            field_0x2d0 = field_0x234[i];
                        }
                    }
                }
            }
            if (field_0x2d0 != -1) {
                s32 count = 0;
                if (field_0x244[field_0x2d0] >= 4) {
                    for (s32 i = 0; i < 4; i++) {
                        if (field_0x234[i] == field_0x2d0 && !fn_8023E8F0(i)) {
                            field_0x234[i] = -1;
                            field_0x244[field_0x2d0]--;
                        }
                    }
                }
                if (field_0x244[field_0x2d0] >= 4) {
                    for (s32 i = 0; i < 4; i++) {
                        field_0x268[i].field_0x4 = *getPlayerPos_8023E724(i);
                        field_0x268[i].field_0x10 = fn_8022CAF8(i);
                        if (field_0x234[i] == field_0x2d0 && count < 4) {
                            if (fn_8022F9E4(i, mSerial, true, false)) {
                                field_0x254[count] = i;
                                count++;
                                if (count >= 4) {
                                    if (getScreenFlagBit_0x3C() > 0) {
                                        setScreenFlag_0x30(0xFF, true);
                                    }
                                    field_0x230 = 1;
                                    field_0xb8 = 0;
                                }
                            }
                        } else {
                            field_0x234[i] = -1;
                        }
                    }
                }
                if (field_0x230 == 1) {
                    for (s32 i = 0; i < 4; i++) {
                        if (arePlayersInSameGroup_8022E438(field_0x254[0], i) == 1 || arePlayersInSameGroup_8022E438(field_0x254[1], i) == 1) {
                            field_0x268[i].field_0x0 = 0;
                        }
                    }
                }
            }
        }
        break;
    }
    case 1:
        if (field_0xc4) {
            JGeometry::TVec3f pushers[4];
            JGeometry::TVec3f* out = pushers;
            for (s32 i = 0; i < 4; i++) {
                if (field_0x234[i] == field_0x2d0 && field_0x268[i].field_0x0 == 0 &&
                    field_0x268[i].field_0x10 != fn_8022CAF8(i)) {
                    fn_80230C6C(i);
                    return;
                }
                JGeometry::TVec3f cur(*getPlayerPos_8023E724(i));
                JGeometry::TVec3f prev(field_0x268[i].field_0x4);
                if (field_0x268[i].field_0x0 == 0) {
                    if (JGeometry::TUtil<f32>::sqrt((cur.x - prev.x) * (cur.x - prev.x) +
                                                    (cur.y - prev.y) * (cur.y - prev.y)) > 2.0f) {
                        const JGeometry::TVec3f& d = prev - cur;
                        fn_8023DFF4(i, &d);
                    }
                }
                if (field_0x234[i] == field_0x2d0) {
                    *out = *fn_8023E7B0(i);
                    out++;
                }
            }
            pushers[0].add(pushers[1]);
            pushers[0].add(pushers[2]);
            pushers[0].add(pushers[3]);
            pushers[0].scale(0.25f);
            if (pushers[0].z > 0.0f) {
                mPos.set(pushers[0]);
                mLayer_0x4 = getPlayerLayer_8023DE58(field_0xc8);
            }
        } else {
            mVel_0x3c.z += -0.15f;
            mPos.add(mVel_0x3c);
            if (mPos.z < 0.0f || sweepMapBox_801F2618(NULL, false)) {
                if (mPos.z < 0.0f) {
                    if (getRawTile_80226850(&mPos, mLayer_0x4) == 0x90) {
                        gBgRewrite_8053AC98->writeTile_802A381C(&mPos, mLayer_0x4, 0x91);
                    } else if (getRawTile_80226850(&mPos, mLayer_0x4) == 0x91) {
                        gBgRewrite_8053AC98->writeTile_802A381C(&mPos, mLayer_0x4, 0x92);
                    }
                }
                field_0x230 = 2;
            } else {
                s32 mask = getPlayersInBox_801F2740(-1);
                bool hurt = false;
                if (mask > 0) {
                    for (s32 i = 0; i < 4; i++) {
                        if (field_0x234[i] != field_0x2d0 && (mask & (1 << i)) > 0 &&
                            !arePlayersInSameGroup_8022E438(field_0x254[0], i) && !arePlayersInSameGroup_8022E438(field_0x254[1], i)) {
                            hitPlayerGroup_80238548(i, 0, (const JGeometry::TVec2f*)&mPos, 5);
                            hurt = true;
                        }
                    }
                }
                if (hurt == true || fn_801F2B20(0)) {
                    field_0x230 = 2;
                }
            }
        }
        break;
    case 2:
        die_0x24();
        break;
    }
}

void Log_804ACD70::draw() {
    const char* name;
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    switch (field_0x2c8) {
    case 0:
        params.field_0x1d = -1;
        params.field_0x1c = 0;
        switch (field_0x2c4) {
        case 0:
            name = "yy_1_1maruta1.bti";
            break;
        case 1:
            name = "yy_1_1maruta2.bti";
            break;
        }
        if (field_0xc4 == 1) {
            params.field_0x20 = 0;
            params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
            params.field_0x1c = fn_8023CA88(field_0xc8);
        }
        PSMTXIdentity(mtx);
        if (field_0x2c0 == 1) {
            f32 c = JMath::sincosTable_.cosShort(0x4000);
            f32 s = JMath::sincosTable_.sinShort(0x4000);
            mtx[1][1] = 1.0f;
            mtx[0][0] = c;
            mtx[0][2] = s;
            mtx[2][0] = -s;
            mtx[2][2] = c;
            mtx[0][1] = mtx[1][0] = mtx[1][2] = mtx[2][1] = 0.0f;
        }
        params.mMtx = mtx;
        {
            JGeometry::TVec3f saved(mPos);
            mPos.y += 1.0f;
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', name);
            drawSpriteRes_801EFD34(res, &params, 7, 0.0f, -1.0f, 0.0f);
            mPos = saved;
        }
        break;
    case 1:
        drawSprite_801F06F0("enemy_gc_sob.bin", 0x445, &params, 3, 1.0f, 1.0f);
        break;
    }
}

void Log_804ACD70::die_0x24() {
    u16 a;
    u16 b;
    u16 c;
    switch (field_0x2c0) {
    case 0:
        switch (field_0x2c4) {
        case 0:
            a = 0xA1;
            b = 0xA2;
            c = 0xA3;
            break;
        case 1:
            a = 0x9B;
            b = 0x9C;
            c = 0x9D;
            break;
        }
        break;
    case 1:
        switch (field_0x2c4) {
        case 0:
            a = 0xA4;
            b = 0xA5;
            c = 0xA6;
            break;
        case 1:
            a = 0x9E;
            b = 0x9F;
            c = 0xA0;
            break;
        }
        break;
    }
    spawnEmitter_8013CC50(mLayer_0x4, &mPos, a, 1, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(mLayer_0x4, &mPos, b, 1, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(mLayer_0x4, &mPos, c, 1, 0, NULL, NULL, true, 1.0f);
    startSound_801F0E34(0x30003, 0);
    markDead_801F3668();
}

bool Log_804ACD70::isBlocked_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    if (field_0x230 == 0) {
        s32 kind = getTileCollisionCode_8022461C(pos, layer);
        if (kind == 0x27 || kind == 3) {
            return false;
        }
        if (kind == 4 || kind == 0x28 || kind == 0x2C) {
            if (((s32)pos->y & 15) >= 8) {
                return false;
            }
        } else if (kind == 5 || kind == 0x29 || kind == 0x2D) {
            if (((s32)pos->y & 15) < 8) {
                return false;
            }
        } else if (kind == 6 || kind == 0x2A || kind == 0x2E) {
            if (((s32)pos->x & 15) >= 8) {
                return false;
            }
        } else if (kind == 7 || kind == 0x2B || kind == 0x2F) {
            if (((s32)pos->x & 15) < 8) {
                return false;
            }
        }
        return ActorBase_804A5770::isBlocked_0x4C(layer, pos);
    }
    return isBlockedCommon_801F132C(layer, pos, &field_0x2b8);
}

static inline s32 getPullDir(const Log_804ACD70* self) {
    JGeometry::TBox2f box = self->mMapBox_0x80;
    box.i.x += self->mPos.x;
    box.i.y += self->mPos.y;
    box.f.x += self->mPos.x;
    box.f.y += self->mPos.y;
    if (fn_80239470(&box, self->mLayer_0x4, self->mSerial) >= 0 && fn_80238A68(self->mSerial) >= 4) {
        s32 d = fn_8023885C(self->mSerial);
        if (d >= 0 && d < 4) {
            if (d == 0) {
                d = 1;
            } else if (d == 1) {
                d = 0;
            } else if (d == 2) {
                d = 3;
            } else if (d == 3) {
                d = 2;
            }
            return d;
        }
    }
    return -1;
}

static inline s32 getPushDir(const Log_804ACD70* self) {
    if (fn_80239108(self->mSerial) >= 4) {
        s32 d = fn_80238BC4(self->mSerial);
        if (d >= 0 && d < 4) {
            return d;
        }
    }
    return -1;
}

// FIXME: 98.85 %, scheduling+regalloc; behaviour clean.
void Log_804ACD70::fn_80354FE8() {
    if (isMotionless()) {
        if (field_0x2cc == 1) {
            field_0x2cc = 0;
        } else {
            s32 dirA = getPullDir(this);
            s32 dirB = getPushDir(this);
            s32 dir;
            if (dirA != -1) {
                dir = dirA;
            } else if (dirB != -1) {
                dir = dirB;
            } else {
                return;
            }
            JGeometry::TVec3f saved(mPos);
            mPos += JGeometry::TVec3f(lbl_804722B0[dir][0], lbl_804722B0[dir][1], 0.0f);
            u8 hit = sweepMapBox_801F2618(NULL, false);
            mPos = saved;
            if (hit != true && fn_80354D64(dir)) {
                if (dirA == -1 && dirB != -1) {
                    fn_80238E08(mSerial);
                }
                mVel_0x3c.zero();
                mVel_0x3c.set(lbl_804722D0[dir][0], lbl_804722D0[dir][1], 0.0f);
                startSound_801F0E34(0x30004, 0);
                field_0x2d4.x = lbl_804722B0[dir][0];
                field_0x2d4.y = lbl_804722B0[dir][1];
                field_0x2d4.z = 0.0f;
                if (field_0x2c8 == 0) {
                    u16 id;
                    switch (field_0x2c0) {
                    case 0:
                        switch (field_0x2c4) {
                        case 0:
                            id = 0x327;
                            break;
                        case 1:
                            id = 0x325;
                            break;
                        }
                        break;
                    case 1:
                        switch (field_0x2c4) {
                        case 0:
                            id = 0x328;
                            break;
                        case 1:
                            id = 0x326;
                            break;
                        }
                        break;
                    }
                    if (mEmitter2e0 == NULL) {
                        mEmitter2e0 = spawnEmitter_8013CC50(mLayer_0x4, &mPos, id, 0, -2, NULL, NULL, true, 1.0f);
                    }
                }
            }
        }
    }
}

// FIXME(shotgun): 99.75 %, regalloc-only; behaviour clean.
bool Log_804ACD70::fn_80354D64(u8 dir) {
    bool ok = false;
    JGeometry::TVec3f saved(mPos);
    mVel_0x3c.set(lbl_804722F0[dir][0], lbl_804722F0[dir][1], lbl_804722F0[dir][2]);
    mPos += mVel_0x3c;
    JGeometry::TVec3f out(0.0f, 0.0f, 0.0f);
    if (sweepMapBox_801F2618(&out, false)) {
        switch (dir) {
        case 0:
            if (out.x >= 0.0f) {
                ok = true;
            }
            break;
        case 1:
            if (out.x <= 0.0f) {
                ok = true;
            }
            break;
        case 2:
            if (out.y >= 0.0f) {
                ok = true;
            }
            break;
        case 3:
            if (out.y <= 0.0f) {
                ok = true;
            }
            break;
        }
    } else {
        ok = true;
    }
    if (!ok) {
        ok = true;
        JGeometry::TBox2f box(mBox);
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023B0F4(i, &box, mLayer_0x4, false, 0.0f)) {
                JGeometry::TVec3f p(*getPlayerPos_8023E724(i));
                JGeometry::TVec3f off(lbl_80472320[dir][0], lbl_80472320[dir][1], lbl_80472320[dir][2]);
                p += off;
                if (isBlocked_0x4C(mLayer_0x4, &p) == 1) {
                    ok = false;
                }
            }
        }
    }
    mPos = saved;
    mVel_0x3c.set(0.0f, 0.0f, 0.0f);
    return ok;
}

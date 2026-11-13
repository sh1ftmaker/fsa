#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/BgRewrite.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/SpriteParams.h"
#include "game/act/tu_80354D64.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

// Unit-local rodata. Pairs / triples per direction (0 +x, 1 -x, 2 +y, 3 -y).
static const f32 lbl_804722B0[4][2] = {{16.0f, 0.0f}, {-16.0f, 0.0f}, {0.0f, 16.0f}, {0.0f, -16.0f}};
static const f32 lbl_804722D0[4][2] = {{0.25f, 0.0f}, {-0.25f, 0.0f}, {0.0f, 0.25f}, {0.0f, -0.25f}};
static const f32 lbl_804722F0[4][3] = {
    {24.0f, 0.0f, 0.0f}, {-24.0f, 0.0f, 0.0f}, {0.0f, 24.0f, 0.0f}, {0.0f, -24.0f, 0.0f}};
static const f32 lbl_80472320[4][3] = {
    {16.0f, 0.0f, 0.0f}, {-16.0f, 0.0f, 0.0f}, {0.0f, 16.0f, 0.0f}, {0.0f, -16.0f, 0.0f}};

Log_804ACD70::~Log_804ACD70() {
    if (mEmitter2e0 != NULL) {
        releaseEmitter_801EE434(mEmitter2e0);
    }
}

// FIXME(shotgun): 89.65 %, scheduling
void Log_804ACD70::init() {
    field_0x2c0 = field_0x90 & 1;
    field_0x2c4 = (field_0x90 >> 4) & 1;
    field_0x2c8 = (field_0x90 >> 8) & 1;
    field_0x1a4 = 2;
    switch (field_0x2c8) {
    case 0:
        switch (field_0x2c0) {
        case 0:
            switch (field_0x2c4) {
            case 0:
                mBox.set(-46.0f, -7.0f, 46.0f, 7.0f);
                field_0x80.set(-47.0f, -7.0f, 47.0f, 7.0f);
                break;
            case 1:
                mBox.set(-46.0f, -14.0f, 46.0f, 14.0f);
                field_0x80.set(-47.0f, -15.0f, 47.0f, 15.0f);
                break;
            }
            break;
        case 1:
            switch (field_0x2c4) {
            case 0:
                mBox.set(-7.0f, -46.0f, 7.0f, 46.0f);
                field_0x80.set(-7.0f, -47.0f, 7.0f, 47.0f);
                break;
            case 1:
                mBox.set(-14.0f, -46.0f, 14.0f, 46.0f);
                field_0x80.set(-15.0f, -47.0f, 15.0f, 47.0f);
                break;
            }
            break;
        }
        break;
    case 1:
        mBox.set(-15.0f, -23.0f, 15.0f, 7.0f);
        field_0x80.set(-15.0f, -23.0f, 15.0f, 7.0f);
        mBox2.set(-15.0f, -23.0f, 15.0f, 7.0f);
        break;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x234[i] = -1;
        field_0x244[i] = 0;
        field_0x254[i] = -1;
        field_0x264[i] = 0;
        field_0x268[i].field_0x0 = 1;
        field_0x268[i].x = 0.0f;
        field_0x268[i].y = 0.0f;
        field_0x268[i].z = 0.0f;
        field_0x268[i].field_0x10 = 0;
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

// FIXME(shotgun): 67.6 %, missing/extra
void Log_804ACD70::update() {
    JGeometry::TBox2f box;
    box.set(field_0x80.i.x + mPos.x, field_0x80.i.y + mPos.y, field_0x80.f.x + mPos.x,
            field_0x80.f.y + mPos.y);
    switch (field_0x230) {
    case 0: {
        fn_80239914(&box, field_0x4, mSerial);
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        boxes->mLists[boxes->field_0x0].fn_8020D8C0(&box, field_0x4, mSerial);
        fn_80354FE8();
        if (!(field_0x2d4.squared() <= 32.0f * FLT_EPSILON)) {
            mPos.add(field_0x3c);
            field_0x2d4.sub(field_0x3c);
            if (field_0x2d4.squared() <= 32.0f * FLT_EPSILON) {
                field_0x3c.zero();
                if (mEmitter2e0 != NULL) {
                    releaseEmitter_801EE434(mEmitter2e0);
                    mEmitter2e0 = NULL;
                }
                field_0x2cc = 1;
            } else if (mEmitter2e0 != NULL) {
                setEmitterPos_8013CB44(mEmitter2e0, &mPos, 0, -2, true);
            }
        }
        if (field_0x2d4.squared() <= 32.0f * FLT_EPSILON && field_0x2c8 == 0) {
            for (s32 i = 0; i < 4; i++) {
                field_0x234[i] = -1;
                field_0x244[i] = 0;
            }
            field_0x2d0 = -1;
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i)) {
                    JGeometry::TVec3f* ppos = getPlayerPos_8023E724(i);
                    f32 px = ppos->x;
                    f32 py = ppos->y;
                    s32 dir = fn_80230CFC(i);
                    px += lbl_804722B0[dir][0];
                    py += lbl_804722B0[dir][1];
                    field_0x264[i] = fn_8022C004(i);
                    if (field_0x264[i] == 1 && fn_8023E8F0(i) == 1 && fn_8023188C(i) == 1 &&
                        fn_802319B4(i, 0x11) == 1 && !fn_80234D94(i)) {
                        bool inside = false;
                        if (px >= box.i.x && py >= box.i.y && px < box.f.x && py < box.f.y) {
                            inside = true;
                        }
                        if (inside == true) {
                            field_0x234[i] = dir;
                            field_0x244[field_0x234[i]]++;
                        } else {
                            field_0x264[i] = 0;
                        }
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
                        JGeometry::TVec3f* pp = getPlayerPos_8023E724(i);
                        field_0x268[i].x = pp->x;
                        field_0x268[i].y = pp->y;
                        field_0x268[i].z = pp->z;
                        field_0x268[i].field_0x10 = fn_8022CAF8(i);
                        if (field_0x234[i] == field_0x2d0 && count < 4) {
                            if (fn_8022F9E4(i, mSerial, true, false)) {
                                field_0x254[count] = i;
                                count++;
                                if (count >= 4) {
                                    if (vfunc_0x3C() > 0) {
                                        vfunc_0x30(0xFF, true);
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
                        if (fn_8022E438(field_0x254[0], i) == 1 || fn_8022E438(field_0x254[1], i) == 1) {
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
                JGeometry::TVec3f prev(field_0x268[i].x, field_0x268[i].y, field_0x268[i].z);
                if (field_0x268[i].field_0x0 == 0) {
                    f32 dx = cur.x - prev.x;
                    f32 dy = cur.y - prev.y;
                    f32 dist = std::sqrtf(dx * dx + dy * dy);
                    if (dist > 2.0f) {
                        JGeometry::TVec3f d(prev.x - cur.x, prev.y - cur.y, prev.z - cur.z);
                        fn_8023DFF4(i, &d);
                    }
                }
                if (field_0x234[i] == field_0x2d0) {
                    *out = *fn_8023E7B0(i);
                    out++;
                }
            }
            JGeometry::TVec3f sum(pushers[0]);
            sum.add(pushers[1]);
            sum.add(pushers[2]);
            sum.add(pushers[3]);
            sum.scale(0.25f);
            if (sum.z > 0.0f) {
                mPos = sum;
                field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
            }
        } else {
            field_0x3c.z += -0.15f;
            mPos.add(field_0x3c);
            if (mPos.z >= 0.0f && !fn_801F2618(NULL, false)) {
                u8 mask = fn_801F2740(-1);
                bool hurt = false;
                if (mask > 0) {
                    for (s32 i = 0; i < 4; i++) {
                        if (field_0x234[i] != field_0x2d0 && (mask & (1 << i)) > 0 &&
                            !fn_8022E438(field_0x254[0], i) && !fn_8022E438(field_0x254[1], i)) {
                            fn_80238548(i, 0, (const JGeometry::TVec2f*)&mPos, 5);
                            hurt = true;
                        }
                    }
                }
                if (hurt == true || fn_801F2B20(0)) {
                    field_0x230 = 2;
                }
            } else {
                if (mPos.z < 0.0f) {
                    if (fn_80226850(&mPos, field_0x4) == 0x90) {
                        gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x91);
                    } else if (fn_80226850(&mPos, field_0x4) == 0x91) {
                        gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x92);
                    }
                }
                field_0x230 = 2;
            }
        }
        break;
    case 2:
        vfunc_0x24();
        break;
    }
}

// FIXME(shotgun): 99.44 %, scheduling+regalloc
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
            f32 s = JMath::sincosTable_.sinShort(0x4000);
            f32 c = JMath::sincosTable_.cosShort(0x4000);
            mtx[0][0] = c;
            mtx[0][2] = s;
            mtx[2][0] = -s;
            mtx[2][2] = c;
            mtx[0][1] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = 1.0f;
            mtx[1][2] = 0.0f;
            mtx[2][1] = 0.0f;
        }
        params.mMtx = mtx;
        {
            JGeometry::TVec3f saved(mPos);
            mPos.y += 1.0f;
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', name);
            fn_801EFD34(res, &params, 7, 0.0f, -1.0f, 0.0f);
            mPos = saved;
        }
        break;
    case 1:
        drawSprite_801F06F0("enemy_gc_sob.bin", 0x445, &params, 3, 1.0f, 1.0f);
        break;
    }
}

void Log_804ACD70::vfunc_0x24() {
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
    spawnEmitter_8013CC50(field_0x4, &mPos, a, 1, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, b, 1, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, c, 1, 0, NULL, NULL, true, 1.0f);
    startSound_801F0E34(0x30003, 0);
    fn_801F3668();
}

// FIXME(shotgun): 65.95 %, missing/extra
bool Log_804ACD70::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    if (field_0x230 == 0) {
        s32 kind = fn_8022461C(pos, layer);
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
        return ActorBase_804A5770::vfunc_0x4C(layer, pos);
    }
    return fn_801F132C(layer, pos, &field_0x2b8);
}

// FIXME(shotgun): 75.53 %, missing/extra
void Log_804ACD70::fn_80354FE8() {
    if (field_0x2d4.squared() <= 32.0f * FLT_EPSILON) {
        if (field_0x2cc == 1) {
            field_0x2cc = 0;
        } else {
            JGeometry::TBox2f box = field_0x80;
            box.set(box.i.x + mPos.x, box.i.y + mPos.y, box.f.x + mPos.x, box.f.y + mPos.y);
            s32 dirA = -1;
            s32 dirB = -1;
            if (fn_80239470(&box, field_0x4, mSerial) >= 0 && fn_80238A68(mSerial) >= 4) {
                s32 d = fn_8023885C(mSerial);
                if (d >= 0 && d < 4) {
                    if (d == 0) {
                        dirA = 1;
                    } else if (d == 1) {
                        dirA = 0;
                    } else if (d == 2) {
                        dirA = 3;
                    } else if (d == 3) {
                        dirA = 2;
                    }
                }
            }
            if (fn_80239108(mSerial) >= 4) {
                s32 d = fn_80238BC4(mSerial);
                if (d >= 0 && d < 4) {
                    dirB = d;
                }
            }
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
            u8 hit = fn_801F2618(NULL, false);
            mPos = saved;
            if (hit != true && fn_80354D64(dir)) {
                if (dirA == -1 && dirB != -1) {
                    fn_80238E08(mSerial);
                }
                field_0x3c.set(lbl_804722D0[dir][0], lbl_804722D0[dir][1], 0.0f);
                startSound_801F0E34(0x30004, 0);
                field_0x2d4.x = lbl_804722B0[dir][0];
                field_0x2d4.y = lbl_804722B0[dir][1];
                field_0x2d4.z = 0.0f;
                if (field_0x2c8 == 0) {
                    // FIXME(shotgun): the asm leaves the id unset when field_0x2c0/field_0x2c4 match no case
                    // (the original passes an indeterminate value); zeroed here, a behaviour difference.
                    s32 id = 0;
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
                        mEmitter2e0 = spawnEmitter_8013CC50(field_0x4, &mPos, id, 0, -2, NULL, NULL, true, 1.0f);
                    }
                }
            }
        }
    }
}

// FIXME(shotgun): 80.15 %, missing/extra
bool Log_804ACD70::fn_80354D64(u8 dir) {
    bool ok = false;
    JGeometry::TVec3f saved(mPos);
    field_0x3c.set(lbl_804722F0[dir][0], lbl_804722F0[dir][1], lbl_804722F0[dir][2]);
    mPos.add(field_0x3c);
    JGeometry::TVec3f out(0.0f, 0.0f, 0.0f);
    if (fn_801F2618(&out, false)) {
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
        JGeometry::TBox2f box;
        box.set(mBox.i.x + mPos.x, mBox.i.y + mPos.y, mBox.f.x + mPos.x, mBox.f.y + mPos.y);
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023B0F4(i, &box, field_0x4, false, 0.0f)) {
                JGeometry::TVec3f p(*getPlayerPos_8023E724(i));
                JGeometry::TVec3f off(lbl_80472320[dir][0], lbl_80472320[dir][1], lbl_80472320[dir][2]);
                p += off;
                if (vfunc_0x4C(field_0x4, &p) == 1) {
                    ok = false;
                }
            }
        }
    }
    mPos = saved;
    field_0x3c.zero();
    return ok;
}

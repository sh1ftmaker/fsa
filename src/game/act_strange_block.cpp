#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_strange_block.h"
#include "game/Map.h"
#include "game/Effect.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include <float.h>
#include "game/GameView.h"
#include "game/Message.h"

// The push step of each direction (x, y, z), .rodata at 0x80471E28 (the rest of that object is
// string literals of update/draw).
static const f32 lbl_80471E28[12] = {20.0f, 0.0f, 0.0f, -20.0f, 0.0f, 0.0f, 0.0f, 20.0f, 0.0f, 0.0f, -20.0f, 0.0f};

// .data at 0x804ABC80 (16 bytes, local to this unit).
extern const s32 lbl_804ABC80[4];
extern const f32 lbl_804A79E0[12]; // step per direction (game/act_block.cpp)
extern const f32 lbl_804AC2A8[12]; // push velocity per direction
extern const s32 lbl_804ABCA0[16]; // sprite frame offsets by animation step (and, 4 per row, by phase)

// 0x8033E188.
// FIXME(shotgun): 97.63 %, scheduling + reloc-name; behaviour clean.
void StrangeBlock_804AC2D8::init() {
    field_0x25c = (field_0x90 >> 12) & 0xF;
    switch (field_0x25c) {
    case 0:
    case 1:
    case 5:
        mPos.add(JGeometry::TVec3f(0.0f, 8.0f, 0.0f));
        mBox.set(-15.0f, -23.0f, 15.0f, 7.0f);
        field_0x80.set(-15.0f, -23.0f, 15.0f, 7.0f);
        field_0x1a4 = 2;
        field_0x260 = 2;
        break;
    case 2:
    case 3:
    case 6:
        mPos.add(JGeometry::TVec3f(0.0f, 16.0f, 0.0f));
        mBox.set(-31.0f, -47.0f, 31.0f, 15.0f);
        field_0x80.set(-31.0f, -47.0f, 31.0f, 15.0f);
        field_0x1a4 = 4;
        field_0x260 = 4;
        break;
    case 4:
        mBox.set(-31.0f, -31.0f, 31.0f, 31.0f);
        field_0x80.set(-31.0f, -31.0f, 31.0f, 31.0f);
        field_0x1a4 = 4;
        field_0x260 = 4;
        break;
    }
    mSpawnPos = mPos;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x244 = 0;
    field_0x248 = 0;
    field_0xb8 = 2;
    field_0x234 = 0;
    field_0x250 = 0;
    field_0x24c = lbl_804ABC80[0];
    field_0x254 = 2;
    field_0x258 = -1;
    field_0x272 = 0;
    field_0x268 = (field_0x90 & 0xFFF) + 0x12B;
    field_0x265 = 0;
    field_0x264 = (field_0x90 >> 17) & 1;
    if (field_0x264 == 0) {
        if (fn_8028D4DC(&mPos, field_0x260 - 1, field_0x16c)) {
            field_0x265 = 1;
            vfunc_0x30(0xFF, true);
        }
    }
    field_0x238 = mPos;
    field_0x270 = 1;
    if (field_0x25c == 0 || field_0x25c == 2) {
        field_0x270 = 0;
    }
    field_0x26c = 0;
    field_0x271 = 0;
    field_0x230 = 0;
}

// 0x8033DA78. States of field_0x234: 0 the block waits to be pushed or slides, 1 a player's
// message is open, then back to 0.
// FIXME(shotgun): 83.02 %, missing/extra + reloc-name; behaviour drift.
void StrangeBlock_804AC2D8::update() {
    JGeometry::TBox2f box;
    box.i.x = mBox.i.x + mPos.x;
    box.i.y = mBox.i.y + mPos.y;
    box.f.x = mBox.f.x + mPos.x;
    box.f.y = mBox.f.y + mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    if (field_0x25c != 0 && field_0x25c != 2) {
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        boxes->mLists[boxes->field_0x0].fn_8020D7D0(mSerial);
    }
    switch (field_0x234) {
    case 0: {
        f32 reach;
        f32 dist = PSVECDistance(&mPos, &mSpawnPos);
        if (dist < 13.0f) {
            reach = 11.9f;
        } else if (dist >= (f32)(field_0x260 << 4) - 19.9f) {
            reach = 19.9f;
        } else {
            reach = 15.9f;
        }
        if (PSVECDistance(&mPos, (const Vec*)&field_0x238) >= reach) {
            if (mPos.x <= mSpawnPos.x + 11.5f && mPos.x >= mSpawnPos.x - 11.5f) {
                mPos.x = mSpawnPos.x;
            } else if (mPos.x < field_0x238.x) {
                mPos.x = field_0x238.x - (reach + 0.1f);
            } else {
                mPos.x = field_0x238.x + (reach + 0.1f);
            }
            if (mPos.y <= mSpawnPos.y + 11.5f && mPos.y >= mSpawnPos.y - 11.5f) {
                mPos.y = mSpawnPos.y;
            } else if (mPos.y < field_0x238.y) {
                mPos.y = field_0x238.y - (reach + 0.1f);
            } else {
                mPos.y = field_0x238.y + (reach + 0.1f);
            }
            field_0x238 = mPos;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x272 = 0;
            mPrevPos = mPos;
            if (field_0x230 != NULL) {
                releaseEmitter_801EE434(field_0x230);
            }
            if (!((field_0x90 >> 16) & 1) &&
                PSVECDistance(&mPos, &mSpawnPos) >= (f32)(field_0x260 << 4) - 0.1f) {
                field_0x244 = -1;
                vfunc_0x30(0xFF, true);
                if (field_0x25c != 4) {
                    fn_801F0D20(3);
                }
            } else {
                field_0x244 = 8;
                field_0x248 = 30;
            }
        } else if (field_0x265) {
            JGeometry::TBox2f talkBox;
            talkBox.i.x = mBox.i.x + mPos.x;
            talkBox.i.y = mBox.i.y + mPos.y;
            talkBox.f.x = mBox.f.x + mPos.x;
            talkBox.f.y = mBox.f.y + mPos.y;
            if (field_0x25c == 0 || field_0x25c == 2) {
                fn_8022FEB8(&talkBox, field_0x4, mSerial, true, false);
                field_0x258 = fn_8022FD8C(mSerial);
                if (field_0x258 >= 0 && getPlayerPos_8023E724(field_0x258)->z == 0.0f &&
                    fn_802300EC(field_0x258)) {
                    JGeometry::TVec3f p = mPos;
                    if (fn_8038C678(field_0x258, field_0x268, mSerial, field_0x4, p, 0)) {
                        field_0x234 = 1;
                        field_0x254 = fn_80230CFC(field_0x258);
                        field_0x254 = field_0x254 ^ 1;
                    }
                }
            }
        } else {
            mPos += field_0x3c;
            if (field_0x244 > 0) {
                field_0x244--;
            } else if (field_0x244 == 0) {
                f32 sq = field_0x3c.x * field_0x3c.x + field_0x3c.y * field_0x3c.y + field_0x3c.z * field_0x3c.z;
                if (sq <= 32.0f * FLT_EPSILON) {
                    fn_8033D4CC();
                } else {
                    JGeometry::TVec3f saved = field_0x3c;
                    bool keep = true;
                    if (field_0x260 == 2 && reach == 19.9f &&
                        PSVECDistance(&mPos, (const Vec*)&field_0x238) >= 8.0f && field_0x26c != 0 &&
                        !fn_8033D264(field_0x26c - 1)) {
                        keep = false;
                    } else {
                        field_0x3c = saved;
                    }
                    if (keep == false) {
                        field_0x244 = 8;
                        field_0x271 = 1;
                    }
                }
            } else if (field_0x244 < 0) {
                JGeometry::TBox2f talkBox;
                talkBox.i.x = mBox.i.x + mPos.x;
                talkBox.i.y = mBox.i.y + mPos.y;
                talkBox.f.x = mBox.f.x + mPos.x;
                talkBox.f.y = mBox.f.y + mPos.y;
                if (field_0x25c == 0 || field_0x25c == 2) {
                    fn_8022FEB8(&talkBox, field_0x4, mSerial, true, false);
                    field_0x258 = fn_8022FD8C(mSerial);
                    if (field_0x258 >= 0 && getPlayerPos_8023E724(field_0x258)->z == 0.0f &&
                        fn_802300EC(field_0x258)) {
                        JGeometry::TVec3f p = mPos;
                        if (fn_8038C678(field_0x258, field_0x268, mSerial, field_0x4, p, 0)) {
                            field_0x234 = 1;
                            field_0x254 = fn_80230CFC(field_0x258);
                            field_0x254 = field_0x254 ^ 1;
                        }
                    }
                }
            } else if (field_0x230 != NULL) {
                setEmitterPos_8013CB44(field_0x230, &mPos, 0, 0, true);
            }
        }
        break;
    }
    case 1:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x258, mSerial)) {
            field_0x234 = 0;
            field_0x254 = 2;
        }
        break;
    }
    if (!field_0x272) {
        if (field_0x25c == 0 || field_0x25c == 2) {
            field_0x24c--;
            if (field_0x24c <= 0) {
                field_0x250 = (field_0x250 + 1) & 3;
                field_0x24c = lbl_804ABC80[field_0x250];
            }
        }
    }
}

// 0x8033D4CC. Called when the block is idle: pushes it one step when a player has been
// shoving it long enough, or lets it slide back to its spawn position.
// FIXME(shotgun): 76.67 %, missing/extra + reloc-name; behaviour drift.
void StrangeBlock_804AC2D8::fn_8033D4CC() {
    s32 pushers = fn_80239108(mSerial);
    if (pushers >= field_0x260) {
        JGeometry::TVec3f saved = mPos;
        if (PSVECDistance(&mPos, &mSpawnPos) < (f32)(field_0x260 << 4) - 0.1f) {
            s32 dir = fn_80238BC4(mSerial);
            JGeometry::TVec3f step(lbl_804A79E0[dir * 3], lbl_804A79E0[dir * 3 + 1], lbl_804A79E0[dir * 3 + 2]);
            mPos += step;
            if (fn_801F2618(NULL, false)) {
                mPos = saved;
                return;
            }
            mPos = saved;
            if (field_0x270 == 1) {
                if (field_0x26c == 0) {
                    field_0x26c = dir + 1;
                } else if (dir + 1 != field_0x26c) {
                    return;
                }
            }
            if (field_0x260 == 2 && !fn_8033D264(dir)) {
                return;
            }
            if (!field_0x264) {
                getGameView_802D6A9C()->fn_802D7584(field_0x16c, dir + 1);
            }
            if (dir >= 0 && dir < 4) {
                fn_80238E08(mSerial);
                f32 speed = 0.0f;
                switch (field_0x25c) {
                case 0:
                case 1:
                case 5:
                    speed = 0.7f;
                    break;
                case 2:
                case 3:
                case 4:
                case 6:
                    speed = 0.4f;
                    break;
                }
                field_0x3c.x = lbl_804AC2A8[dir * 3];
                field_0x3c.y = lbl_804AC2A8[dir * 3 + 1];
                field_0x3c.z = lbl_804AC2A8[dir * 3 + 2];
                field_0x3c.x *= speed;
                field_0x3c.y *= speed;
                field_0x3c.z *= speed;
                if (field_0x271 == 1) {
                    field_0x271 = 0;
                } else {
                    field_0x238.x = mPos.x;
                    field_0x238.y = mPos.y;
                    field_0x238.z = mPos.z;
                }
                startSound_801F0E34(0x30004, 0);
                field_0x272 = 1;
            }
            if (field_0x25c == 4) {
                s32 id;
                if (dir == 0) {
                    id = 0x31A;
                } else if (dir == 1) {
                    id = 0x31B;
                } else if (dir == 2) {
                    id = 0x31C;
                } else {
                    id = 0x319;
                }
                field_0x230 = spawnEmitter_8013CC50(field_0x4, &mPos, id, 1, 1, NULL, NULL, true, 1.0f);
            }
        }
    } else if (((field_0x90 >> 16) & 1) && PSVECDistance(&mPos, &mSpawnPos) > 1.0f) {
        if (field_0x248 == 0) {
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            f32 speed;
            switch (field_0x25c) {
            case 0:
            case 1:
            case 5:
                speed = 0.5f;
                break;
            case 2:
            case 3:
            case 4:
            case 6:
                speed = 0.2f;
                break;
            }
            if (mPos.x > mSpawnPos.x) {
                field_0x3c.x = -speed;
            } else if (mPos.x < mSpawnPos.x) {
                field_0x3c.x = speed;
            } else if (mPos.y > mSpawnPos.y) {
                field_0x3c.y = -speed;
            } else if (mPos.y < mSpawnPos.y) {
                field_0x3c.y = speed;
            }
            field_0x238.x = mPos.x;
            field_0x238.y = mPos.y;
            field_0x238.z = mPos.z;
            startSound_801F0E34(0x30004, 0);
        } else {
            field_0x248--;
        }
    } else {
        JGeometry::TBox2f box;
        box.i.x = mBox.i.x + mPos.x;
        box.i.y = mBox.i.y + mPos.y;
        box.f.x = mBox.f.x + mPos.x;
        box.f.y = mBox.f.y + mPos.y;
        if (field_0x25c == 0 || field_0x25c == 2) {
            fn_8022FEB8(&box, field_0x4, mSerial, true, false);
            field_0x258 = fn_8022FD8C(mSerial);
            if (field_0x258 >= 0 && getPlayerPos_8023E724(field_0x258)->z == 0.0f && fn_802300EC(field_0x258)) {
                JGeometry::TVec3f p = mPos;
                if (fn_8038C678(field_0x258, field_0x268, mSerial, field_0x4, p, 0)) {
                    field_0x234 = 1;
                    field_0x254 = fn_80230CFC(field_0x258);
                    field_0x254 = field_0x254 ^ 1;
                }
            }
        }
    }
}

// 0x8033D4AC.
// FIXME(shotgun): 98.75 %, regalloc-only; behaviour clean.
f32 fn_8033D4AC(f32 value, f32 center, f32 margin) {
    f32 offset = 0.1f + margin;
    if (value < center) {
        return center - offset;
    }
    return center + offset;
}

// 0x8033D264.
// FIXME(shotgun): 82.51 %, missing/extra; behaviour near.
bool StrangeBlock_804AC2D8::fn_8033D264(u8 dir) {
    bool movable = false;
    JGeometry::TVec3f savedPos = mPos;
    field_0x3c.x = lbl_80471E28[dir * 3];
    field_0x3c.y = lbl_80471E28[dir * 3 + 1];
    field_0x3c.z = lbl_80471E28[dir * 3 + 2];
    mPos.add(field_0x3c);
    JGeometry::TVec3f out(0.0f, 0.0f, 0.0f);
    if (fn_801F2618(&out, false)) {
        switch (dir) {
        case 0:
            if (out.x >= 0.0f) {
                movable = true;
            }
            break;
        case 1:
            if (out.x <= 0.0f) {
                movable = true;
            }
            break;
        case 2:
            if (out.y >= 0.0f) {
                movable = true;
            }
            break;
        case 3:
            if (out.y <= 0.0f) {
                movable = true;
            }
            break;
        }
    } else {
        movable = true;
    }
    if (!movable) {
        movable = true;
        JGeometry::TBox2f box(mBox);
        box.addPos(mPos.x, mPos.y);
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023B0F4(i, &box, field_0x4, false, 0.0f)) {
                if (fn_8022CAF8(i) != 5 || !fn_8022F478(i)) {
                    if (fn_80238CB4(i) != mSerial) {
                        movable = false;
                    }
                }
            }
        }
    }
    mPos = savedPos;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    return movable;
}

// 0x8033CD44.
// FIXME(shotgun): 97.00 %, missing/extra + reloc-name; behaviour drift.
void StrangeBlock_804AC2D8::draw() {
    switch (field_0x25c) {
    case 0:
    case 1:
    case 5: {
        const char* name;
        s32 frame;
        if (field_0x25c == 0) {
            frame = lbl_804ABCA0[field_0x250] + 0x475;
            name = "enemy_gc_sob.bin";
        } else if (field_0x25c == 1) {
            name = "enemy_both2_sob.bin";
            frame = 0x1C1;
        } else {
            frame = 0x37C;
            name = "enemy_both2_sob.bin";
        }
        drawSprite_801F06F0(name, frame, NULL, 3, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, frame, 0, 2, -1, false, 0, 0);
        break;
    }
    case 2:
    case 6: {
        if (field_0x25c == 2) {
            s32 frame = lbl_804ABCA0[field_0x254 * 4 + field_0x250] + 0x3B4;
            for (s32 i = 0; i < 2; i++) {
                for (s32 j = 0; j < 2; j++) {
                    JGeometry::TVec3f saved = mPos;
                    JGeometry::TVec3f offset(32.0f * i - 16.0f, 32.0f * j - 24.0f, 0.0f);
                    mPos += offset;
                    drawSprite_801F06F0("enemy_both2_sob.bin", frame, NULL, 3, 1.0f, 1.0f);
                    mPos = saved;
                }
            }
        } else {
            JGeometry::TVec3f saved = mPos;
            JGeometry::TVec3f offset(0.0f, -16.0f, 0.0f);
            mPos += offset;
            drawSprite_801F06F0("enemy_both2_sob.bin", 0x37D, NULL, 3, 1.0f, 1.0f);
            mPos = saved;
        }
        s32 frame2;
        if (field_0x25c == 2) {
            frame2 = lbl_804ABCA0[field_0x250] + 0x3CA;
        } else {
            frame2 = 0x37D;
        }
        JGeometry::TVec3f saved = mPos;
        JGeometry::TVec3f offset(0.0f, -16.0f, 0.0f);
        mPos += offset;
        fn_803075AC(mSerial, field_0x4, &mPos, frame2, 0, 2, -1, false, 0, 0);
        mPos = saved;
        break;
    }
    case 3: {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        PSMTXIdentity(mtx);
        mtx[0][0] = 2.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 2.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 2.0f;
        params.mMtx = mtx;
        drawSprite_801F06F0("enemy_both2_sob.bin", 0x1C1, &params, 3, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, 0x1C1, 1, 2, -1, false, 0, 0);
        break;
    }
    case 4: {
        void* res = fn_804032E8('TIMG', "kazari_piramid.bti");
        fn_801EFD34(res, NULL, 7, 0.0f, 0.0f, 0.0f);
        break;
    }
    }
}

// Destructor at 0x8033CCE4.
StrangeBlock_804AC2D8::~StrangeBlock_804AC2D8() {}

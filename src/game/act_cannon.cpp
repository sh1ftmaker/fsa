#include "game/GameAudio.h"
#include "game/act/act_cannon.h"
#include "game/Effect.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"


// The animation: (cel, duration) pairs, ended by a cel of -1.
static const s16 lbl_80463FD0[] = {0, 3, 1, 2, 2, 2, 3, 3, 4, 3, 5, 3, -1, -1};
static const s16* lbl_80539ED0[] = {lbl_80463FD0, NULL};

// The swing (state 0 and 2): moves mPos.x between mHomePos.x +- 8 * field_0x280
// with a speed that falls off near the ends.
static inline void swing(Cannon_8049CCF0* c) {
    if (c->field_0x280 <= 0.0f) {
        return;
    }
    f32 speed;
    f32 rate;
    switch (c->field_0x27c) {
    case 1:
        speed = 1.0f;
        rate = 4.0f;
        break;
    default:
        speed = 0.5f;
        rate = 2.0f;
        break;
    }
    f32 dist = __fabsf((c->mPos.x - c->mHomePos.x) / (8.0f * c->field_0x280));
    if (dist >= 0.8f) {
        speed -= rate * (dist - 0.8f);
    }
    if (c->field_0x268 != 0) {
        c->field_0x3c.x = speed;
        c->field_0x3c.y = 0.0f;
        c->field_0x3c.z = 0.0f;
    } else {
        c->field_0x3c.x = -speed;
        c->field_0x3c.y = 0.0f;
        c->field_0x3c.z = 0.0f;
    }
    c->mPos += c->field_0x3c;
    if (c->mPos.x >= c->mHomePos.x + 8.0f * c->field_0x280) {
        c->mPos.x = c->mHomePos.x + 8.0f * c->field_0x280;
        c->field_0x268 = 0;
    } else if (c->mPos.x <= c->mHomePos.x - 8.0f * c->field_0x280) {
        c->mPos.x = c->mHomePos.x - 8.0f * c->field_0x280;
        c->field_0x268 = 1;
    }
}

void Cannon_8049CCF0::update() {
    if (vfunc_0x3C()) {
        if (field_0x274 == 0 || field_0x274 == 1) {
            if (!vfunc_0x34()) {
                field_0x269 = 0;
                return;
            }
            if (field_0x269 == 0) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x503, 0, 0, NULL, NULL, true, 1.5f);
                startSound_801F0E34(0x30003, 0);
                fn_801F0D20(0x42);
            }
            field_0x269 = 1;
        }
    }
    if (field_0x274 == 0 || field_0x274 == 1) {
        JGeometry::TBox2f box(mBox);
        JGeometry::TVec2f p(mPos.x, mPos.y - mPos.z);
        box.addPos(p);
        s32 owner;
        for (owner = 0; owner < 4; owner++) {
            u8 bit = 1 << owner;
            if ((gGameRoot_8053AAF8->mPlayerMask & bit) != 0 || fn_8022CAF8(owner) == 4) {
                if (fn_8023B0F4(owner, &box, field_0x4, false, 1000.0f) && field_0x26c < 0 &&
                    fn_8023162C(owner, mSerial)) {
                    break;
                }
            }
        }
        if (owner == 4) {
            owner = -1;
        }
        if (owner >= 0) {
            field_0x26c = owner;
            field_0x278 = field_0x274;
            field_0x274 = 2;
            // FIXME: 94.96 %. The owner search's `break` and the state dispatch
            // below differ only in branch layout and register choice (see
            // fn_80107360's FIXME); the box and the sin lookup are placed as
            // the original's, but the box corners are not added in its order.
            switch (field_0x274) {
            case 2:
                fn_80230FD0(field_0x26c, 0x20035);
                break;
            case 1:
                break;
            }
        }
    }
    switch (field_0x274) {
    case 0:
        swing(this);
        break;
    case 2:
        swing(this);
        field_0x284 += 0.05f;
        {
            f32 t = 2.0f * (0.5f - field_0x284);
            JGeometry::TVec3f v(0.0f, t, 0.0f);
            mPos += v;
        }
        if (mPos.y < mHomePos.y) {
            mPos.y = mHomePos.y;
        }
        if (field_0x284 >= 1.0f) {
            field_0x284 = 1.0f;
        }
        field_0x270++;
        if (field_0x270 >= 180 && field_0x270 < 300) {
            s16 a = (field_0x270 - 180) * (field_0x270 - 180) / 6;
            f32 sinv = JMath::sincosTable_.sinDegree(a);
            mPos.y = mHomePos.y + (f32)(12 * ((200 - (field_0x270 - 180)) / 100) + 2) * sinv;
        } else if (field_0x270 >= 300) {
            fn_80231590(field_0x26c, mSerial);
        }
        if (fn_801F285C(NULL, NULL, true, -1) >= 0) {
            fn_80231590(field_0x26c, mSerial);
        } else {
            // Declared in this order: MWCC gives the later aggregate the lower
            // stack slot (docs/RULES_CARD.md, two stack structs).
            JGeometry::TVec3f vec;
            JGeometry::TBox2f box;
            JGeometry::TVec2f out;
            out.set(0.0f, 0.0f);
            box.set(mBox2);
            JGeometry::TVec2f p(mPos.x, mPos.y - mPos.z);
            box.addPos(p);
            bool hit = false;
            if (fn_8023AA18(&box, field_0x4, &out) >= 0) {
                hit = true;
            } else {
                s32 player = fn_8023AE34(&box, field_0x4, &out);
                if (player >= 0) {
                    fn_801CC820(player, NULL);
                    hit = true;
                } else {
                    player = fn_8023AC74(&box, field_0x4, &out);
                    if (player >= 0) {
                        fn_801CC538(player, &out, &vec, NULL);
                        hit = true;
                    } else {
                        player = fn_8023AD54(&box, field_0x4, &out);
                        if (player >= 0) {
                            fn_801CC640(player, NULL, NULL);
                            hit = true;
                        }
                    }
                }
            }
            if (hit) {
                fn_80231590(field_0x26c, mSerial);
            } else if (fn_801F2DB4(mActorId, 'NULL', NULL)) {
                fn_80231590(field_0x26c, mSerial);
            }
        }
        break;
    case 3:
        static s32 lbl_8053AA70 = 0;
        static f32 lbl_8053AA78 = 0.0f;
        lbl_8053AA78 = lbl_8053AA78 + 0.1f;
        if (lbl_8053AA78 >= 1.0f) {
            lbl_8053AA78 = 1.0f;
        }
        lbl_8053AA70++;
        {
            f32 tmp;
            if (lbl_8053AA78 <= 0.2f) {
                tmp = 12.0f * (0.5f - lbl_8053AA78);
            } else {
                tmp = 4.0f * (0.2f - lbl_8053AA78);
            }
            JGeometry::TVec3f v(0.0f, tmp, 0.0f);
            mPos += v;
        }
        if (mPos.y < mHomePos.y) {
            mPos.y = mHomePos.y;
        }
        {
            bool done = false;
            if (lbl_8053AA78 >= 1.0f) {
                if (lbl_8053AA70 >= 30) {
                    lbl_8053AA70 = 0;
                    lbl_8053AA78 = 0.0f;
                    done = true;
                }
            }
            if (done) {
                field_0x270 = 0;
                field_0x26c = -1;
                field_0x274 = 0;
            }
        }
        break;
    }
    mAnim.fn_801D1C18();
}

bool Cannon_8049CCF0::fn_80107360() {
    if (field_0x274 != 2) {
        return false;
    }
    mPos.y = mHomePos.y;
    JGeometry::TVec3f pos = mPos;
    pos.y -= 16.0f;
    if (field_0x4 == 0) {
        spawnEmitter_8013CC50(field_0x4, &pos, 0x5A, 0, 1, NULL, NULL, true, 1.0f);
    }
    fn_80230FD0(field_0x26c, 0x20036);
    gGameRoot_8053AAF8->fn_80134DE4(1, 0);
    field_0x278 = field_0x274;
    field_0x274 = 3;
    // FIXME: 98.15 %. Only the branch layout differs: the original has one
    // more unreachable `b` after the dispatch (beq, bge, b, b; ours beq,
    // bge, b). The empty case 1 is there to produce the bge/b pair; its
    // meaning is unknown (init() sets state 1 for some spawns).
    switch (field_0x274) {
    case 2:
        fn_80230FD0(field_0x26c, 0x20035);
        break;
    case 1:
        break;
    }
    field_0x284 = 0.0f;
    return true;
}

void Cannon_8049CCF0::draw() {
    if (vfunc_0x3C()) {
        if (field_0x274 == 0 || field_0x274 == 1) {
            if (!vfunc_0x34()) {
                return;
            }
        }
    }
    SpriteParams_801F5AE0 params;
    params.field_0x1d = 1;
    s32 frame = mAnim.mCel + 0x14C;
    JGeometry::TVec3f* pos = &mPos;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(pos, res, frame, &params, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, mAnim.mCel + 0x14C, false, 1, -1, 0, 0, 0);
}

f32 Cannon_8049CCF0::fn_801080B8() const {
    if (field_0x4 == 0) {
        return 8.0f;
    }
    if (field_0x26a != 0) {
        return 8.0f;
    }
    return 13.0f;
}

Cannon_8049CCF0::~Cannon_8049CCF0() {}

void Cannon_8049CCF0::init() {
    if ((field_0x90 & 0xF) == 1) {
        field_0x274 = 1;
    }
    field_0x27c = field_0x90 >> 4 & 0xF;
    field_0x280 = (f32)(field_0x90 >> 8 & 0x1F);
    if ((field_0x90 >> 16 & 1) == 1) {
        field_0x26a = 1;
    } else {
        field_0x26a = 0;
    }
    mBox.set(-8.0f, -8.0f, 8.0f, -6.0f);
    mBox2.set(mBox);
    field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
    mLife = 0;
    field_0xb0 = 0x50303;
    mAnim.fn_801D2608(lbl_80539ED0, 1);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.fn_801D1F14(0.0f);
    mHomePos.set(mPos);
    if (vfunc_0x3C()) {
        if (vfunc_0x34()) {
            field_0x269 = 1;
        }
    }
}

Cannon_8049CCF0::Cannon_8049CCF0() {
    field_0x268 = 1;
    field_0x269 = 0;
    field_0x26a = 0;
    field_0x26c = -1;
    field_0x270 = 0;
    field_0x274 = 0;
    field_0x278 = 0;
    field_0x27c = 0;
    field_0x280 = 0.0f;
    field_0x284 = 0.0f;
    mHomePos.x = 0.0f;
    mHomePos.y = 0.0f;
    mHomePos.z = 0.0f;
}

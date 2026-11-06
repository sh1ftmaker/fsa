#include "game/GameAudio.h"
#include "game/act/act_bumper.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/RotMatrix.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

static s32 lbl_804A0228[7] = {0, 1, 2, 0, 1, 0, 1};
static s32 lbl_804A0244[7] = {1, 1, 1, 1, 1, 2, 2};

void Bumper_804A0260::init() {
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x268 = 0.0f;
    field_0x264 = 0.0f;
    field_0x260 = 0.0f;
    field_0x94 = 0;
    field_0x98 = (field_0x90 >> 8) & 3;
    field_0x238 = (field_0x90 >> 16) & 1;
    field_0x90 &= 7;
    field_0x25c = 0;
    mSpawnPos.x = mPos.x;
    mSpawnPos.y = mPos.y;
    mSpawnPos.z = mPos.z;
    field_0xb8 = 2;
    field_0x258 = 0;
    field_0x26c = mPos;
    field_0x24c = fn_801F6874(field_0x98);
    field_0x250 = 100;
    field_0x234 = 0.0f;
    if (field_0x90 == 2 || field_0x90 == 3) {
        field_0x234 = 30.0f;
        mPos.x = field_0x26c.x + field_0x234 * JMath::sincosTable_.sinShort(field_0x24c);
        mPos.y = field_0x26c.y + field_0x234 * JMath::sincosTable_.cosShort(field_0x24c);
    }
    if (field_0x90 == 4 || field_0x90 == 5) {
        field_0x234 = 80.0f;
        mPos.x = field_0x26c.x + field_0x234 * JMath::sincosTable_.sinShort(field_0x24c);
        mPos.y = field_0x26c.y + field_0x234 * JMath::sincosTable_.cosShort(field_0x24c);
    }
    if (field_0x90 == 2 || field_0x90 == 4) {
        field_0x250 = -100;
    }
    if (field_0x238 != 0) {
        field_0x250 <<= 1;
    }
    field_0xbc = 4;
    field_0x230 = 0;
}

// FIXME: 97.13 %. The loop bases sit in other registers (and the boxA / pos
// stores are scheduled in another order). The layer is read through
// getField_0x4() (before getPlayerLayer_8023DE58, as the original). The candidate test materialises `ok` (li,
// mr, clrlwi.) where the original branches on the fn_8022F478 flag directly
// (rewriting it as `continue` made the match worse). 4 instructions more.
void Bumper_804A0260::update() {
    mBox.i.x = -17.0f;
    mBox.i.y = -17.0f;
    mBox.f.x = 17.0f;
    mBox.f.y = 17.0f;
    field_0x80.i.x = -10.0f;
    field_0x80.i.y = -10.0f;
    field_0x80.f.x = 10.0f;
    field_0x80.f.y = 10.0f;
    mLife = 1000;
    field_0x25c -= 0x100;

    JGeometry::TBox2f boxA(field_0x80);
    JGeometry::TBox2f boxB(mBox);
    f32 dist[4];
    s32 idx[4];
    JGeometry::TVec2f pos;
    JGeometry::TVec2f c;

    boxA.addPos(mPos.x, mPos.y);
    pos.x = mPos.x;
    pos.y = mPos.y;
    fn_80239914(&boxA, field_0x4, mSerial);
    boxB.addPos(pos);

    for (s32 i = 0; i < 4; i++) {
        dist[i] = -1.0f;
        idx[i] = -1;
        if (getPlayerLayer_8023DE58(i) == getField_0x4()) {
            f32 dx = getPlayerPos_8023E724(i)->x - mPos.x;
            f32 dy = getPlayerPos_8023E724(i)->y - mPos.y;
            dist[i] = std::sqrtf(dx * dx + dy * dy);
            idx[i] = i;
        }
    }

    for (s32 i = 0; i < 4; i++) {
        for (s32 j = 0; j < 4; j++) {
            if (i < j && getPlayerLayer_8023DE58(j) == getPlayerLayer_8023DE58(i)) {
                if (dist[i] > dist[j]) {
                    idx[i] = j;
                    idx[j] = i;
                    dist[i] = dist[j];
                    dist[j] = (f32)idx[i]; // sic: the original stores the swapped index, not the distance
                }
            }
        }
    }

    s32 found = -1;
    for (s32 k = 0; k < 4; k++) {
        if (idx[k] < 0) {
            continue;
        }
        bool ok = false;
        if (fn_8022CAF8(idx[k]) == 4 || fn_8022CAF8(idx[k]) == 5) {
            if (!fn_8022F478(idx[k])) {
                ok = true;
            }
        } else {
            ok = true;
        }
        if (ok == true && fn_8023B0F4(idx[k], &boxB, field_0x4, false, 0.0f)) {
            found = idx[k];
            break;
        }
    }

    s32 touchPlayer = fn_801F285C(NULL, NULL, false, -1);
    s32 actorHit = getActorManager_801F856C()->fn_801F82CC(this, field_0x195, 32.0f);
    field_0xb0 = 0x42304;

    bool clear = false;
    if (found < 0 && touchPlayer < 0 && actorHit < 0 && field_0x9c < 0 && field_0x94 != 1) {
        clear = fn_801A4D1C(mSerial, &boxA, &pos) == 0 && fn_801A6108(mSerial, &boxA, &pos) == 0 &&
                fn_801A46FC(mSerial, &boxA, &pos) == 0 && fn_801A4F5C(mSerial, &boxA, &pos) == 0;
    }
    if (!clear) {
        c.x = 0.5f * (boxB.i.x - boxB.f.x) + boxB.f.x;
        c.y = 0.5f * (boxB.i.y - boxB.f.y) + boxB.f.y;
        field_0x94 = 0;
        if (found >= 0) {
            gGameRoot_8053AAF8->fn_80134DE4(1, 0);
            fn_802378AC(found, &c);
        } else if (touchPlayer >= 0) {
            gGameRoot_8053AAF8->fn_80134DE4(1, 0);
            fn_802378AC(touchPlayer, &c);
        }
        if (field_0x230 != 1) {
            startSound_801F0E34(0x30012, 0);
            field_0x230 = 1;
        }
    }
    field_0xb0 = 0x2000;

    s32 state = field_0x90;
    switch (state) {
    case 1:
        if (field_0x240 == 0) {
            f32 speed = 0.3f * (2.0f * (f32)field_0x238) + 0.3f;
            switch (field_0x98) {
            case 0:
                field_0x260 = speed;
                field_0x264 = 0.0f;
                field_0x268 = 0.0f;
                break;
            case 1:
                field_0x260 = -speed;
                field_0x264 = 0.0f;
                field_0x268 = 0.0f;
                break;
            case 2:
                field_0x260 = 0.0f;
                field_0x264 = speed;
                field_0x268 = 0.0f;
                break;
            case 3:
                field_0x260 = 0.0f;
                field_0x264 = -speed;
                field_0x268 = 0.0f;
                break;
            }
            field_0x240 = 0x1f4;
            if (field_0x238 != 0) {
                field_0x240 = 0xa5;
            }
            field_0x98 ^= 1;
        }
        addCalc2_801F71A4(&field_0x3c.x, field_0x260, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x264, 0.3f, 1.0f);
        mPos += field_0x3c;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        field_0x24c += field_0x250;
        mPos.x = field_0x26c.x + field_0x234 * JMath::sincosTable_.sinShort(field_0x24c);
        mPos.y = field_0x26c.y + field_0x234 * JMath::sincosTable_.cosShort(field_0x24c);
        break;
    }

    if (field_0x230 == 1 && field_0x23c == 0) {
        field_0x248 += 1;
        if (field_0x248 > 6) {
            field_0x258 = 0;
            field_0x248 = 0;
            field_0x230 = 0;
        } else {
            field_0x258 = lbl_804A0228[field_0x248];
            field_0x23c = lbl_804A0244[field_0x248];
        }
    }
    if (field_0x23c > 0) {
        field_0x23c -= 1;
    }
    if (field_0x240 > 0) {
        field_0x240 -= 1;
    }
    if (field_0x244 > 0) {
        field_0x244 -= 1;
    }
}

// FIXME: 96.68 %. scheduling+regalloc only: the original keeps sin x in f29,
// sin z in f27 and the angle in f30, ours sin x in f27, sin z in f29 and the
// angle in f31 (the same setRotScaleMtx arithmetic, same call order).
void Bumper_804A0260::draw() {
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    SpriteParams_801F5AE0 params;
    f32 angle = (f32)field_0x25c / -10430.228f;
    JGeometry::TVec3f rot(0.0f, angle, 0.0f);
    setRotScaleMtx(mtx, rot, 1.0f);
    params.mMtx = mtx;
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x258 + 0x70, &params, 0, 1.0f, 1.0f);
}

Bumper_804A0260::~Bumper_804A0260() {}

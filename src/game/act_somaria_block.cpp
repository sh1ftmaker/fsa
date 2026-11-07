#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/act/act_obj_lift.h"
#include "game/act/act_somaria_block.h"

// Offsets (x, y, z) that init walks mPos along; the index is fn_80230CFC ^ 1.
static const f32 lbl_80471368[4][3] = {
    {0.5f, 0.0f, 0.0f}, {-0.5f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.0f, -0.5f, 0.0f},
};

// The sound bits update's push state (3) passes to fn_8022C52C, by direction.
static const u32 lbl_80471398[4] = {2, 1, 4, 8};

inline bool CaneOfSomariaBlock_804AA9F0::checkGround() {
    JGeometry::TBox2f box = field_0x80;
    box.addPos(mPos.x, mPos.y);
    JGeometry::TVec3f centre;
    centre.x = box.f.x + 0.5f * (box.i.x - box.f.x);
    centre.y = box.f.y + 0.5f * (box.i.y - box.f.y);
    u16 tile = fn_802265FC(&centre, field_0x4);
    if (tile == 3 || tile == 0xD || tile == 0xE || tile == 0x3B) {
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        mState = 6;
        field_0x23c = tile;
        return false;
    }
    return true;
}

void CaneOfSomariaBlock_804AA9F0::init() {
    // FIXME: 99.59 %. Same instructions; the sdata2 literals are @486-@492 here
    // (lbl_ in the original, a literal-split issue).
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    mBox.set(-6.0f, -6.0f, 6.0f, 6.0f);
    field_0x80.set(-4.0f, -4.0f, 4.0f, 4.0f);
    field_0x1a4 = 0;
    field_0xb8 = 2;
    mState = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x240 = -1;
    field_0x245 = 0;
    field_0x246[0] = 0;
    field_0x246[1] = 0;
    field_0x246[2] = 0;
    field_0x246[3] = 0;
    field_0x23c = -1;
    field_0x244 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;

    s32 count = 0;
    while (fn_801F2618(NULL, false)) {
        s32 dir = fn_80230CFC(field_0x110) ^ 1;
        count++;
        mPos.x += 2.0f * lbl_80471368[dir][0];
        mPos.y += 2.0f * lbl_80471368[dir][1];
        if (count > 8) {
            break;
        }
    }
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
}

// FIXME(shotgun): 90.3 %, missing/extra + reloc-name
// Lands on an OBJLift in state 0xB (taking over its 'SMRI' passenger), then: 0 spawns (two
// emitters), 1 waits 20 frames, 2 idles (tracks which players are clear of it, gets pushed by
// fn_80238BC4's direction or picked up through fn_8022F9E4), 3 slides one push, 4 is carried,
// 5 is thrown and bounces once, 6 sinks into the pit tile, 7 breaks into four 'FIRE', 8 rides
// the lift, 9 leaves it.
void CaneOfSomariaBlock_804AA9F0::update() {
    JGeometry::TBox2f box = mBox;
    box.addPos(mPos.x, mPos.y);
    if (mState <= 6 && field_0xc4 == 0 && mPos.z <= 0.0f) {
        s32 end = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < end; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
            if (actor == NULL || !actor->mAlive || actor->field_0x4 != field_0x4 ||
                actor->mActorId != 'OBLF')
            {
                continue;
            }
            JGeometry::TBox2f liftBox;
            actor->fn_801F31C4(&liftBox);
            OBJLift_804ACA00* lift = static_cast<OBJLift_804ACA00*>(actor);
            if (lift->field_0x258 != 0xB) {
                continue;
            }
            JGeometry::TBox2f overlap = box;
            if (!overlap.intersect(liftBox)) {
                continue;
            }
            s32 serial = -1;
            while ((serial = getActorManager_801F856C()->fn_801F78F4('SMRI', serial)) >= 0) {
                CaneOfSomariaBlock_804AA9F0* other = static_cast<CaneOfSomariaBlock_804AA9F0*>(
                    getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (other->mState == 8 && other->field_0x110 == field_0x110) {
                    other->mState = 9;
                    break;
                }
            }
            lift->fn_803513C0();
            mState = 8;
            field_0x240 = actor->mSerial;
            break;
        }
    }
    switch (mState) {
    case 0:
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x457, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x458, 0, 0, NULL, NULL, true, 1.0f);
        mState = 1;
        field_0x238 = 0x14;
        startSound_801F0E34(0x30064, 0);
        break;
    case 1:
        field_0x238--;
        if (field_0x238 <= 0) {
            mState = 2;
        }
        break;
    case 2: {
        if (!checkGround()) {
            break;
        }
        JGeometry::TBox2f here = mBox;
        here.addPos(mPos.x, mPos.y);
        for (s32 i = 0; i < 4; i++) {
            if (field_0x246[i] == 1) {
                if (!fn_8022DB04(i) || fn_80238CB4(i) == mSerial) {
                    fn_80239878(i, &here, field_0x4, mSerial);
                }
            } else if (!fn_8023B0F4(i, &here, field_0x4, false, 8.0f)) {
                field_0x246[i] = 1;
            }
        }
        s32 dir = fn_80238BC4(mSerial);
        if (dir >= 0 && dir < 4) {
            JGeometry::TVec3f saved = mPos;
            JGeometry::TVec3f step(lbl_80471368[dir][0], lbl_80471368[dir][1], lbl_80471368[dir][2]);
            mPos.add(step);
            if (fn_801F2618(NULL, false)) {
                mPos = saved;
                break;
            }
            mPos = saved;
            if (fn_80239558(&box, field_0x4, mSerial) < 0) {
                break;
            }
            if (fn_80233DA4(fn_80239040(mSerial))) {
                break;
            }
            field_0x3c.x = lbl_80471368[dir][0];
            field_0x3c.y = lbl_80471368[dir][1];
            field_0x3c.z = lbl_80471368[dir][2];
            mState = 3;
            field_0x234 = dir;
            field_0x238 = 1;
            field_0x244 = 0;
        } else {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && fn_80239C00(i, &box, field_0x4) &&
                    fn_8022F9E4(i, mSerial, true, false))
                {
                    mState = 4;
                    field_0x238 = 0;
                    field_0x246[0] = 0;
                    field_0x246[1] = 0;
                    field_0x246[2] = 0;
                    field_0x246[3] = 0;
                    break;
                }
            }
        }
        break;
    }
    case 3: {
        mPos.add(field_0x3c);
        box = mBox;
        box.addPos(mPos.x, mPos.y);
        JGeometry::TBox2f here = mBox;
        here.addPos(mPos.x, mPos.y);
        for (s32 i = 0; i < 4; i++) {
            if (field_0x246[i] == 1) {
                if (!fn_8022DB04(i) || fn_80238CB4(i) == mSerial) {
                    fn_80239878(i, &here, field_0x4, mSerial);
                }
            } else if (!fn_8023B0F4(i, &here, field_0x4, false, 8.0f)) {
                field_0x246[i] = 1;
            }
        }
        if ((field_0x244 & 0x1F) == 0) {
            startSound_801F0E34(0x30004, 0);
        }
        field_0x244++;
        if (!checkGround()) {
            break;
        }
        if (!fn_801F2618(NULL, false)) {
            field_0x238--;
            if (field_0x238 > 0) {
                break;
            }
            s32 player = fn_80239040(mSerial);
            if (player >= 0 && !fn_80230188(player) && !fn_8022D75C(player) &&
                fn_8022C52C(player, lbl_80471398[field_0x234]) != 0)
            {
                field_0x238 = 1;
                break;
            }
        }
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        mState = 2;
        field_0x238 = 0;
        field_0x244 = 0;
        break;
    }
    case 4:
        if (field_0xc4 != 0) {
            JGeometry::TVec3f* pos = fn_8023E7B0(field_0xc8);
            mPos.x = pos->x;
            mPos.y = pos->y;
            mPos.z = pos->z;
            field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
            break;
        }
        field_0x3c.x *= 0.4f;
        field_0x3c.y *= 0.4f;
        field_0x3c.z = 2.0f;
        mState = 5;
        // fall through
    case 5:
        mPos.add(field_0x3c);
        if (field_0x238 == 0) {
            field_0x3c.z -= 0.2f;
        } else {
            field_0x3c.z -= 0.8f;
        }
        fn_801F2618(NULL, false);
        if (mPos.z < 0.0f && field_0x3c.z < 0.0f) {
            if (!checkGround()) {
                break;
            }
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x3c.x *= 0.3f;
            field_0x3c.y *= 0.3f;
            if (field_0x238 == 0) {
                field_0x3c.z = 3.0f;
                field_0x238++;
            } else {
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
                mState = 2;
                field_0x238 = 0;
            }
            field_0x245 = 0;
            startSound_801F0E34(0x5007F, 0);
        }
        break;
    case 6:
        mPos.add(field_0x3c);
        if (mPos.z < -4.0f) {
            if (field_0x23c == 0xE && fn_802DE9A0() == true) {
                startSound_801F0E34(0x5005F, 0);
            } else if (field_0x23c == 0xE && fn_802DEB58() == true) {
                startSound_801F0E34(0x5003F, 0);
            } else if (field_0x23c == 0xD || field_0x23c == 0x3B) {
                startSound_801F0E34(0x30022, 0);
            } else {
                startSound_801F0E34(0x3007C, 0);
            }
            mAlive = false;
        } else {
            field_0x3c.z -= 0.15f;
        }
        break;
    case 7:
        for (u32 i = 0; i < 4; i++) {
            getActorManager_801F856C()->spawnActor_801F9484('FIRE', field_0x4, &mPos,
                                                            (i << 4) | 0x1000 | (field_0x110 & 3),
                                                            -1, field_0x110);
        }
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x95, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x96, 0, 0, NULL, NULL, true, 1.0f);
        mAlive = false;
        break;
    case 8:
        break;
    case 9: {
        OBJLift_804ACA00* lift = static_cast<OBJLift_804ACA00*>(
            getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240));
        if (lift != NULL) {
            lift->fn_803511F0();
        }
        mAlive = false;
        break;
    }
    }
}

void CaneOfSomariaBlock_804AA9F0::draw() {
    // FIXME: 99.86 %. Only the literal names differ: the 1.0f scales and the
    // "enemy_both2_sob.bin" string are @457 / @458 here, lbl_ in the original.
    if (mState > 7) {
        return;
    }
    SpriteParams_801F5AE0 params;
    if (field_0xc4 != 0) {
        params.field_0x20 = 0;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
    }
    drawSprite_801F06F0("enemy_both2_sob.bin", field_0x24c + 0x2FF, &params, 0, 1.0f, 1.0f);
    fn_803075AC(mSerial, field_0x4, &mPos, 0x2FF, false, 2, -1, field_0xc4, 0, 0);
}

// Same shape as act_bombchu's vfunc_0x4C (the blocking tile codes as a != chain, the 0x25 /
// 0x26 results narrowed as bytes).
// FIXME: 96.79. Scheduling and register allocation only: the target copies layer to r31
// after the field_0x245 test (pos to r29 in the prologue), ours both in the prologue (pos
// r30, layer r29, the tile code r31).
bool CaneOfSomariaBlock_804AA9F0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    if (field_0x245 != 0) {
        return false;
    }
    s32 tile = fn_8022461C(pos, layer);
    s32 code = fn_80226850(pos, layer);
    if (tile == 0x10 || tile == 0x11 || tile == 0x12 || tile == 0x13 || tile == 0x14 ||
        tile == 0x15 || tile == 0x16 || tile == 0x17 || (code >= 0x140 && code <= 0x15F)) {
        field_0x245 = 1;
        return false;
    }
    if (tile != 0 && tile != 4 && tile != 5 && tile != 6 && tile != 7 && tile != 2 &&
        tile != 3 && tile != 0xE && tile != 0x50 && tile != 0xC && tile != 0x27 &&
        tile != 0x28 && tile != 0x29 && tile != 0x2A && tile != 0x2B && tile != 0x2C &&
        tile != 0x2D && tile != 0x2E && tile != 0x2F && tile != 0x20 && tile != 0xD &&
        tile != 0x3B && tile != 0x39 && tile != 0x3A && tile != 0x4C && tile != 0x4D &&
        tile != 0x4E && tile != 0x4F && tile != 0x24) {
        if (tile == 0x25) {
            return (u8)(getGameView_802D6A9C()->field_0x171 == 0);
        }
        if (tile == 0x26) {
            return (u8)(getGameView_802D6A9C()->field_0x171 == 1);
        }
        if (tile != 0x22 || getGameView_802D6A9C()->field_0x170 == 1) {
            return true;
        }
    }
    return false;
}

bool CaneOfSomariaBlock_804AA9F0::fn_8030EA0C() const {
    return mState < 6;
}

void CaneOfSomariaBlock_804AA9F0::fn_8030EA00() {
    mState = 7;
}

CaneOfSomariaBlock_804AA9F0::~CaneOfSomariaBlock_804AA9F0() {}

#include "game/GameAudio.h"
#include "game/act/act_vaati_boss.h"
#include "game/Player.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/ViewLayer.h"
#include "JSystem/JMath/JMath.h"

extern const s32 lbl_8053F0C0; // 12 (game/act_vaati_boss.cpp)
extern const u8 lbl_8053F0C4;  // 0x1E



void VaatiBoss_804A4958::fn_801E4790(bool big) {
    f32 s = lbl_8053F0BC;
    if (big) {
        s = 1.0f;
    }
    mBox.i.x = -16.0f * s;
    mBox.i.y = -26.0f * s;
    mBox.f.x = 16.0f * s;
    mBox.f.y = 4.0f * s;
    mBox2.set(mBox);
}

void VaatiBoss_804A4958::fn_801E4758() {
    mMapBox_0x80.i.x = -16.0f * lbl_8053F0BC;
    mMapBox_0x80.i.y = -26.0f * lbl_8053F0BC;
    mMapBox_0x80.f.x = 16.0f * lbl_8053F0BC;
    mMapBox_0x80.f.y = 4.0f * lbl_8053F0BC;
}

// FIXME(shotgun): 91.05 %, scheduling+regalloc; behaviour clean.
void VaatiBoss_804A4958::fn_801E4618(JGeometry::TBox2f* out, bool region) {
    if (region) {
        out->set(field_0x5e0.x, field_0x5e0.y, field_0x5e0.x + 16.0f * (f32)field_0x5e8,
                 field_0x5e0.y + 16.0f * (f32)field_0x5e9);
    } else {
        JGeometry::TBox2f* box = layerBox(field_0x5d8);
        if (mFlags & 0x2000) {
            out->set(160.0f + box->i.x, 80.0f + box->i.y, box->f.x - 160.0f, box->f.y - 112.0f);
        } else {
            out->set(80.0f + box->i.x, 128.0f + box->i.y, box->f.x - 80.0f, box->f.y - 96.0f);
        }
    }
}

// FIXME(shotgun): 87.64 %, missing/extra; behaviour clean.
u8 VaatiBoss_804A4958::fn_801E44CC(const JGeometry::TBox2f* area, JGeometry::TVec3f* pos, JGeometry::TVec3f* outDir) {
    JGeometry::TBox2f box = mMapBox_0x80;
    box.i.x += pos->x;
    box.i.y += pos->y;
    box.f.x += pos->x;
    box.f.y += pos->y;
    JGeometry::TVec2f push;
    bool moved = fn_801CC29C(area, &box, &push);
    pos->x += push.x;
    pos->y += push.y;
    if (outDir != NULL) {
        push.normalize();
        outDir->set(push.x, push.y, 0.0f);
    }
    return moved;
}

// Vaati's per-frame player check: when the boss is in one of its states and
// a player is in its box, the attack states answer with a sound or effect.
bool VaatiBoss_804A4958::vfunc_0x58() {
    bool result = false;
    bool stateOk = true;
    switch (mState) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
        stateOk = false;
        break;
    }
    if (stateOk && (mFlags & 0x2000)) {
        JGeometry::TBox2f box;
        fn_801F317C(&box);
        JGeometry::TVec2f xy;
        s32 player = fn_8023AE34(&box, mLayer_0x4, &xy);
        if (player >= 0) {
            switch (mState) {
            case 0xc:
            case 0xd:
            case 0x13:
                if (fn_801CC730(player, NULL, NULL)) {
                    result = true;
                }
                break;
            default:
                if (fn_801CC820(player, NULL)) {
                    JGeometry::TVec3f pos(xy.x, xy.y + mPos.z, mPos.z);
                    fn_801CB688(mLayer_0x4, &pos, 1, 1);
                    startSound_801F0E34(0x5003C, 0);
                    switch (mState) {
                    case 0xa:
                    case 0xe:
                    case 0xf:
                    case 0x10:
                    case 0x11:
                    case 0x12:
                    case 0x14:
                        setState_801D5D84(0xb);
                        break;
                    }
                    result = true;
                }
                break;
            }
        }
    }
    return result;
}

bool VaatiBoss_804A4958::fn_801E4094() {
    bool result = false;
    bool stateOk = true;
    switch (mState) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        stateOk = false;
        break;
    }
    if (stateOk && !(mFlags & 0x2000) && mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        JGeometry::TBox2f saved;
        saved.set(mBox);
        mBox.i.scale(1.5f);
        mBox.f.scale(1.5f);
        s32 serial = getActorManager_801F856C()->findOverlappingActor_801F82CC(this, field_0x195, 48.0f);
        mBox.set(saved);
        if (serial >= 0 && serial != mSerial) {
            ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (bomb != NULL && bomb->mActorId == 'HBMB' && bomb->field_0x94 == 1) {
                JGeometry::TVec3f d(mPos);
                d.sub(bomb->mVel_0x3c);
                result = takeHit_801F37AC(0, &d, 2, bomb->field_0x168, bomb->field_0x110);
                if (result) {
                    bomb->field_0x164 = 1;
                    mKnockbackKind_0xf8 = 0;
                    mVel_0x3c.set(0.0f, 0.0f, 0.0f);
                    mHitTimer_0xd8 = 0xB4;
                    mHitTimerStart_0xdc = 0xB4;
                    mSpeed = 0.0f;
                    fn_801D63D0();
                    startSound_801F0E34(0x40005, 0);
                    fn_801E4D54();
                    field_0x448.fn_80217A74();
                    JGeometry::TVec2f center;
                    getBoxCenter_801F3128(&center);
                    JGeometry::TVec2f shake(center.x - bomb->mPos.x, center.y - (bomb->mPos.y - bomb->mPos.z));
                    field_0x52c.fn_801D6D38(&shake);
                    field_0x5cd++;
                    if ((s32)field_0x5cd >= field_0x5cc) {
                        setState_801D5D84(6);
                    }
                }
            }
        }
    }
    return result;
}

static inline void checkPlayersInBox(VaatiBoss_804A4958* self, JGeometry::TBox2f& box) {
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive_8023F264(i) && !isPlayerDead_80236B50(i) && !fn_80230188(i) && !fn_8022D75C(i) &&
            getPlayerLayer_8023DE58(i) == self->getField_0x4() && fn_80235448(i) &&
            getPlayerPos_8023E724(i)->z <= 40.0f) {
            f32 limit = fn_8023E578(i)->z;
            if (getPlayerPos_8023E724(i)->z < limit) {
                const JGeometry::TVec3f* p = getPlayerPos_8023E724(i);
                bool inside = p->x >= box.i.x && p->y >= box.i.y && p->x < box.f.x &&
                              p->y < box.f.y;
                if (inside) {
                    const JGeometry::TVec3f* playerPos = getPlayerPos_8023E724(i);
                    f32 dy = self->mPos.y - playerPos->y;
                    f32 dx = self->mPos.x - playerPos->x;
                    self->field_0x5d1 = lbl_8053F0C4;
                    self->field_0x5d2 = (s16)(JMath::atanTable_.atan2_(dx, dy) *
                                        (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
                    self->field_0x448.fn_80217A74();
                    self->startSound_801F0E34(0x40062, 0);
                    break;
                }
            }
        }
    }
}

// vtable slot 0x5C. The bomb/player hit test: a player on the boss's layer that is inside its
// area (and below its head) turns it towards that player; otherwise each player that hit it
// this frame is run through takeHit_801F37AC.
u8 VaatiBoss_804A4958::checkHitByPlayer_0x5C() {
    u8 result = 0;
    s32 state = mState;
    bool stateOk = true;
    switch (state) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        stateOk = false;
        break;
    }
    if (stateOk && state != 0x13 && (mFlags & 0x2000) && mBox.f.x - mBox.i.x > 0.0f &&
        mBox.f.y - mBox.i.y > 0.0f) {
        // one hit type for both loops below: the original keeps it in a single stack slot (0x8)
        s32 type;
        if (gGameRoot_8053AAF8->field_0x7e && !isUpperLayer_801CD2F8(mLayer_0x4) && mState != 0xB && !field_0x5d1) {
            JGeometry::TBox2f box;
            getBoxAtPos_801F31C4(&box);
            box.i.x -= 8.0f;
            box.i.y -= 8.0f;
            box.f.x += 8.0f;
            box.f.y += 8.0f;
            checkPlayersInBox(this, box);
        }
        if (isUpperLayer_801CD2F8(mLayer_0x4)) {
            for (s32 i = 0; i < 4; i++) {
                if (i == findOverlappingPlayer_801F285C(NULL, &type, true, i)) {
                    if (takeHit_801F37AC(fn_8023B6CC(i), getPlayerPos_8023E724(i), 2, type, i)) {
                        result = 1;
                        if (fn_80235448(i)) {
                            mFlags |= 0x10000;
                            fn_801D63D0();
                        }
                    }
                }
            }
            if (result) {
                mKnockbackKind_0xf8 = 0;
                startSound_801F0E34(0x40005, 0);
                if (field_0x4c4.field_0x44 != 4) {
                    field_0x4c4.fn_801D7638();
                    field_0x4c4.fn_801D76B4();
                }
                if (mLife <= lbl_8053F0C0) {
                    mFlags |= 0x8000;
                    mLife = 10;
                }
            }
        } else {
            if (gGameRoot_8053AAF8->field_0x7e && mState != 0xB) {
                return 0;
            }
            if (mState != 9) {
                for (s32 i = 0; i < 4; i++) {
                    if (i == findOverlappingPlayer_801F285C(NULL, &type, true, i) && fn_80235448(i) &&
                        takeHit_801F37AC(fn_8023B6CC(i), getPlayerPos_8023E724(i), 2, type, i)) {
                        mFlags |= 0x10000;
                        mKnockbackKind_0xf8 = 0;
                        mVel_0x3c.set(0.0f, 0.0f, 0.0f);
                        mSpeed = 0.0f;
                        field_0x52c.fn_801D6D08();
                        field_0x448.fn_80217A74();
                        startSound_801F0E34(0x40005, 0);
                        if (mLife <= 0) {
                            mHitTimer_0xd8 = 16;
                            mHitTimerStart_0xdc = 16;
                            setState_801D5D84(0x18);
                            field_0x4c4.fn_801D7638();
                            field_0x4c4.fn_801D76B4();
                        } else {
                            mHitTimer_0xd8 = 0xB4;
                            mHitTimerStart_0xdc = 0xB4;
                            fn_801D63D0();
                            if (field_0x4c4.field_0x44 != 4) {
                                field_0x4c4.fn_801D7638();
                                field_0x4c4.fn_801D76B4();
                            }
                            if (mLife <= lbl_8053F0C0) {
                                setState_801D5D84(10);
                            } else {
                                mFlags |= 0x8000;
                            }
                        }
                        result = 1;
                        break;
                    }
                }
            }
        }
    }
    return result;
}

u8 VaatiBoss_804A4958::attackPlayers_0x68() {
    u8 result = 0;
    bool stateOk = true;
    switch (mState) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        stateOk = false;
        break;
    }
    if (stateOk && (mFlags & 0x2000) && isUpperLayer_801CD2F8(mLayer_0x4) && mBox.f.x - mBox.i.x > 0.0f &&
        mBox.f.y - mBox.i.y > 0.0f) {
        result = damagePlayersInBox_801F2B7C(mParamAtcp_0x114, -1, 1);
    }
    return result;
}

// vtable slot 0x64. Pushes Vaati out of the map-free area when a player-sized box is inside.
u8 VaatiBoss_804A4958::sweepMap_0x64() {
    u8 result = 0;
    bool stateOk = true;
    switch (mState) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        stateOk = false;
        break;
    }
    if (stateOk && !(mFlags & 0x1000) && !(mFlags & 0x2000) && mMapBox_0x80.f.x - mMapBox_0x80.i.x > 0.0f &&
        mMapBox_0x80.f.y - mMapBox_0x80.i.y > 0.0f) {
        JGeometry::TBox2f area;
        fn_801E4618(&area, isUpperLayer_801CD2F8(mLayer_0x4));
        result = fn_801E44CC(&area, &mPos, &mMapHitDir);
    }
    return result;
}

// FIXME(shotgun): 99.53 %, regalloc-only; behaviour clean.
u8 VaatiBoss_804A4958::fn_801E3618(s32 layer, const JGeometry::TVec2f* pos) {
    bool outside = false;
    JGeometry::TBox2f area;
    bool onLayer = isUpperLayer_801CD2F8(layer);
    bool inside = true;
    fn_801E4618(&area, onLayer);
    if (pos->x < area.i.x || pos->y < area.i.y || pos->x >= area.f.x || pos->y >= area.f.y) {
        inside = false;
    }
    if (!inside) {
        outside = true;
    }
    return outside;
}

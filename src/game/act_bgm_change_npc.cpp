#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BoxTest.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_bgm_change_npc.h"

// The music started for each BGM id (the spawn parameter & 0x7F), .rodata 0x8046F1C8 (38 words).
// Declared extern: the integer-literal tripwire rejects the hand-typed table in this pass.
extern const u32 lbl_8046F1C8[38];

void BGMChangeNPC_804A9DA0::init() {
    field_0x234 = field_0x90 & 0x7F;
    field_0x238 = ((field_0x90 >> 22) & 0x1F) + 1;
    field_0x23c = ((field_0x90 >> 17) & 0x1F) + 1;
    field_0x240 = (field_0x90 >> 12) & 0x1F;
    field_0x250 = (field_0x90 >> 7) & 1;
    GameView_804A9950* view = getGameView_802D6A9C();
    field_0x248 = view->field_0x34;
    f32 halfH = 16.0f * field_0x23c;
    f32 halfW = 16.0f * field_0x238;
    mBox.set(-halfW, -halfH, halfW, halfH);
    field_0x94 = 0xFFFF;
    field_0x230 = 0;
    field_0x24c = -1;
}

// FIXME(shotgun): 86.95 %, missing/extra + reloc-name
void BGMChangeNPC_804A9DA0::update() {
    JGeometry::TBox2f box(mBox);
    box.addPos(mPos.x, mPos.y);
    field_0x258 = -1;
    field_0x25c = -1;
    field_0x94 = 0xFFFF;
    if (field_0x240 != 2) {
        if (gGameRoot_8053AAF8->field_0x7e == 1) {
            for (s32 i = 0; i < 4; i++) {
                bool present = (gGameRoot_8053AAF8->mPlayerMask & (1 << i)) != 0;
                if (present && field_0x4 == getPlayerLayer_8023DE58(i)) {
                    JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                    if (containsPoint(box, pos)) {
                        field_0x258 = i;
                        field_0x94 = i;
                        break;
                    }
                }
            }
            if (field_0x258 >= 0) {
                field_0x25c = fn_8022EA0C(field_0x258);
            }
        } else {
            field_0x25c = fn_8022EA0C(0);
            if (field_0x4 == getPlayerLayer_8023DE58(field_0x25c)) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(field_0x25c);
                if (containsPoint(box, pos)) {
                    field_0x258 = field_0x25c;
                    field_0x94 = field_0x25c;
                }
            }
        }
    } else if (gGameRoot_8053AAF8->field_0x7e == 1) {
        for (s32 i = 0; i < 4; i++) {
            if (field_0x4 == getPlayerLayer_8023DE58(i)) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                if (containsPoint(box, pos)) {
                    field_0x258 = i;
                    field_0x94 = i;
                    break;
                }
            }
        }
        if (field_0x258 >= 0) {
            field_0x25c = fn_8022EA0C(field_0x258);
            if (field_0x4 == getPlayerLayer_8023DE58(field_0x25c)) {
                field_0x258 = field_0x25c;
                field_0x94 = field_0x25c;
            }
        }
    } else {
        field_0x25c = fn_8022EA0C(0);
        if (field_0x4 == getPlayerLayer_8023DE58(field_0x25c)) {
            field_0x258 = field_0x25c;
            field_0x94 = field_0x25c;
        }
    }
    if (field_0x25c >= 0) {
        field_0x254 = getPlayerLayer_8023DE58(field_0x25c);
    } else {
        field_0x254 = -1;
    }
    if (gGameRoot_8053AAF8->field_0x7e == 1) {
        if (field_0x258 < 0) {
            if (field_0x25c >= 0 && fn_80230188(field_0x25c)) {
                field_0x258 = field_0x24c;
            } else {
                s32 end = getActorManager_801F856C()->mActorEnd;
                for (s32 i = 0; i < end; i++) {
                    ActorBase_804A5770* other = getActorManager_801F856C()->mActors[i];
                    if (other != NULL && other->mActorId == ACTOR_BGMN &&
                        (other->field_0x90 & 0x7F) == (u32)field_0x234) {
                        s32 layer = other->field_0x4;
                        u32 kind = (other->field_0x90 >> 12) & 0x1F;
                        JGeometry::TVec3f otherPos = other->mPos;
                        mBox.set(-16.0f * (s32)(((other->field_0x90 >> 22) & 0x1F) + 1),
                                 -16.0f * (s32)(((other->field_0x90 >> 17) & 0x1F) + 1),
                                 16.0f * (s32)(((other->field_0x90 >> 22) & 0x1F) + 1),
                                 16.0f * (s32)(((other->field_0x90 >> 17) & 0x1F) + 1));
                        JGeometry::TBox2f otherBox(mBox);
                        otherBox.addPos(otherPos.x, otherPos.y);
                        for (s32 p = 0; p < 4; p++) {
                            if (layer == getPlayerLayer_8023DE58(p)) {
                                if (kind == 2) {
                                    return;
                                }
                                JGeometry::TVec3f pos = *getPlayerPos_8023E724(p);
                                if (containsPoint(otherBox, pos)) {
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (field_0x258 >= 0 && field_0x4 != getPlayerLayer_8023DE58(field_0x258)) {
            field_0x258 = -1;
        }
        if (field_0x250 != 0) {
            field_0x258 = -1;
        }
    } else if (field_0x254 != field_0x4) {
        bool free = true;
        s32 end = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < end; i++) {
            ActorBase_804A5770* other = getActorManager_801F856C()->mActors[i];
            if (other != NULL && i != mSerial && other->mActorId == ACTOR_BGMN &&
                field_0x254 == other->field_0x4) {
                if (other->fn_8018A590() != 0xFFFF) {
                    free = false;
                } else if ((u32)field_0x234 == (other->field_0x90 & 0x7F)) {
                    free = false;
                }
            }
        }
        if (!free) {
            return;
        }
    } else if (field_0x258 < 0) {
        if (fn_80230188(field_0x25c)) {
            field_0x258 = field_0x24c;
        } else {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(field_0x25c);
            s32 end = getActorManager_801F856C()->mActorEnd;
            for (s32 i = 0; i < end; i++) {
                ActorBase_804A5770* other = getActorManager_801F856C()->mActors[i];
                if (other != NULL && other->mActorId == ACTOR_BGMN && field_0x254 == other->field_0x4) {
                    if (((other->field_0x90 >> 12) & 0x1F) == 2) {
                        return;
                    }
                    if ((other->field_0x90 & 0x7F) == (u32)field_0x234) {
                        mBox.set(-16.0f * (s32)(((other->field_0x90 >> 22) & 0x1F) + 1),
                                 -16.0f * (s32)(((other->field_0x90 >> 17) & 0x1F) + 1),
                                 16.0f * (s32)(((other->field_0x90 >> 22) & 0x1F) + 1),
                                 16.0f * (s32)(((other->field_0x90 >> 17) & 0x1F) + 1));
                        JGeometry::TVec3f otherPos = other->mPos;
                        JGeometry::TBox2f otherBox(mBox);
                        otherBox.addPos(otherPos.x, otherPos.y);
                        if (containsPoint(otherBox, pos)) {
                            return;
                        }
                    }
                }
            }
        }
    }

    u32 current = gAudioMgr_8053AF80->field_0x60;
    switch (field_0x230) {
    case 0: {
        bool start = false;
        if (field_0x240 == 0 || field_0x240 == 3 || field_0x240 == 4) {
            if (field_0x258 >= 0 || (field_0x90 >> 27) == 0 || vfunc_0x34()) {
                start = true;
            }
        } else if (field_0x258 >= 0) {
            start = true;
        }
        if (start && getGameView_802D6A9C()->field_0x131f4 == 0) {
            if (field_0x240 != 4 && (current - 0x1000000 == 0x21 || current - 0x1000000 == 0x19)) {
                return;
            }
            s32 id = field_0x234;
            if (id == 0x11) {
                gAudioMgr_8053AF80->fn_80458F9C(0x3C);
            } else {
                if (id == 0x14 || id == 0x1E) {
                    gAudioMgr_8053AF80->fn_80458F04(lbl_8046F1C8[id]);
                } else {
                    gAudioMgr_8053AF80->fn_80458FF0(lbl_8046F1C8[id], false, 0);
                }
                s32 view = field_0x234;
                if (view < 0x11 || view == 0x26) {
                    getGameView_802D6A9C()->fn_802D7970(view);
                }
            }
            if (field_0x240 == 3 || field_0x240 == 4) {
                mAlive = false;
                return;
            }
            field_0x230 = 1;
            if (field_0x258 >= 0) {
                field_0x24c = field_0x258;
            }
        }
        break;
    }
    case 1:
        if (field_0x240 != 0 && field_0x258 < 0) {
            gGameRoot_8053AAF8->fn_80138B7C();
            field_0x230 = 0;
            field_0x24c = -1;
        } else {
            s32 id = field_0x234;
            if (id != 0x11 && current != lbl_8046F1C8[id] && current - 0x1000000 != 0x21 &&
                current - 0x1000000 != 0x19) {
                gAudioMgr_8053AF80->fn_80458FF0(lbl_8046F1C8[id], false, 0);
            }
        }
        break;
    }
}

void BGMChangeNPC_804A9DA0::draw() {}

BGMChangeNPC_804A9DA0::~BGMChangeNPC_804A9DA0() {}

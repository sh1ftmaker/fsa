#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/cls_804B1E1C.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_score.h"

// FIXME(shotgun): 97.98 %, different + reloc-name; behaviour near.
void Score_804AF4C0::init() {
    mValue = field_0x90 & 0xFFFF;
    mPlayer = (field_0x90 >> 16) & 0xFF;
    mState = field_0x90 >> 24;
    field_0x248 = mPos.x - getPlayerPos_8023E724(mPlayer)->x;
    field_0x24c = mPos.y - getPlayerPos_8023E724(mPlayer)->y - 24.0f;
    field_0x240 = mValue;
    field_0x244 = lbl_80529DEC.field_0x1f4[mPlayer];
    if (mState == 0 || mState == 4) {
        if (gGameRoot_8053AAF8->field_0x7e == 0 || fn_802DE814()) {
            mAlive = false;
        }
        if (mValue == 0) {
            mAlive = false;
        } else if (mValue >= 100) {
            mValue = 100;
        }
        if (mState == 0) {
            field_0x3c.x = 0.0f;
            field_0x3c.y = -0.2f;
            field_0x3c.z = 0.0f;
        } else {
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
        }
        mTimer = 90;
        lbl_80529DEC.field_0x1f4[mPlayer] = field_0x240;
    } else if (mState == 1) {
        field_0x3c.x = 0.0f;
        field_0x3c.y = -1.0f;
        field_0x3c.z = 0.0f;
        mTimer = 90;
        fn_801F0D20(0x41);
    } else if (mState == 2) {
        if (mValue >= 12) {
            mAlive = false;
        }
        field_0x3c.x = 0.0f;
        field_0x3c.y = -0.2f;
        field_0x3c.z = 0.0f;
        mTimer = 90;
    } else if (mState == 3) {
        mTimer = 0x257;
        f32 z = getPlayerPos_8023E724(mPlayer)->z;
        f32 y = getPlayerPos_8023E724(mPlayer)->y - 32.0f;
        mPos.x = getPlayerPos_8023E724(mPlayer)->x;
        mPos.y = y;
        mPos.z = z;
    } else if (mState == 5) {
        mTimer = 0x1a3;
        f32 z = getPlayerPos_8023E724(mPlayer)->z;
        f32 y = getPlayerPos_8023E724(mPlayer)->y - 32.0f;
        mPos.x = getPlayerPos_8023E724(mPlayer)->x;
        mPos.y = y;
        mPos.z = z;
    } else if (mState == 6) {
        mTimer = 0x12b;
        f32 z = getPlayerPos_8023E724(mPlayer)->z;
        f32 y = getPlayerPos_8023E724(mPlayer)->y - 32.0f;
        mPos.x = getPlayerPos_8023E724(mPlayer)->x;
        mPos.y = y;
        mPos.z = z;
    } else if (mState == 7) {
        mTimer = 0xb3;
        f32 z = getPlayerPos_8023E724(mPlayer)->z;
        f32 y = getPlayerPos_8023E724(mPlayer)->y - 32.0f;
        mPos.x = getPlayerPos_8023E724(mPlayer)->x;
        mPos.y = y;
        mPos.z = z;
    }
}

// FIXME(shotgun): 99.66 %, regalloc-only + reloc-name; behaviour clean.
void Score_804AF4C0::update() {
    if (mState == 0 || mState == 4) {
        fn_80393514();
    } else if (mState == 1) {
        if (mTimer >= 0x50) {
            s32 d = 90 - mTimer;
            field_0x3c.y = -3.0f * (1.0f - (f32)(d * d / 100));
            mPos.add(field_0x3c);
        }
        if (mTimer-- == 0) {
            mAlive = false;
        }
    } else if (mState == 2) {
        field_0x3c.x = 0.2f * JMath::sincosTable_.sinShort((mTimer % 60) << 10);
        mPos.add(field_0x3c);
        if (mTimer-- == 0) {
            mAlive = false;
        }
    } else if (mState == 3 || mState == 5 || mState == 6 || mState == 7) {
        f32 z = getPlayerPos_8023E724(mPlayer)->z;
        f32 y = getPlayerPos_8023E724(mPlayer)->y - 32.0f;
        mPos.x = getPlayerPos_8023E724(mPlayer)->x;
        mPos.y = y;
        mPos.z = z;
        if (!fn_80230188(mPlayer) && fn_8023BC38(mPlayer) <= 0) {
            mAlive = false;
        }
    }
}

void Score_804AF4C0::draw() {
    if (mState == 0 || mState == 4) {
        fn_80393228();
    } else if (mState == 1) {
    } else if (mState == 2) {
        fn_80391F58();
    } else if (mState == 3 || mState == 5 || mState == 6 || mState == 7) {
        if (!fn_80230188(mPlayer)) {
            fn_80391D44();
        }
    }
}

// Per-player digit colours.
static const u8 lbl_80543180[4] = {0x28, 0xFF, 0x32, 0xBE};
static const u8 lbl_8054317C[4] = {0xFF, 0x50, 0xC8, 0x78};
static const u8 lbl_80543184[4] = {0x28, 0x50, 0xFF, 0xFF};
static const u8 lbl_80543170[4] = {0x28, 0xFF, 0x32, 0xBE};
static const u8 lbl_80543174[4] = {0xFF, 0x50, 0xC8, 0x78};
static const u8 lbl_80543178[4] = {0x28, 0x50, 0xFF, 0xFF};

// Not auto-inlined: draw and update call these out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// Force Gem / fairy sounds by tens digit.
static const u32 sScoreSounds[10] = {0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C};

// FIXME(shotgun): 90.97 %, missing/extra + reloc-name; behaviour drift.
void Score_804AF4C0::fn_80393514() {
    if (mTimer == 90) {
        // The original copies this 12-entry table (rodata 0x80475738) to the stack and passes entry 0.
        const f32 emitScale[12] = {0.3f, 0.3f, 0.3f, 0.35f, 0.35f, 0.4f, 0.45f, 0.5f, 0.55f, 0.6f, 0.6f, 0.6f};
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xCB, 0, 0, NULL, NULL, true, emitScale[0]);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xCC, 0, 0, NULL, NULL, true, emitScale[0]);
        s32 idx = 9;
        if (mValue < 10) {
            idx = mValue - 1;
        }
        gAudioMgr_8053AF80->fn_80458880(sScoreSounds[idx], -1);
    }
    s16 angle = (mTimer % 60) << 10;
    if (mState == 0) {
        field_0x3c.x = 0.2f * JMASSin(angle);
        mPos += field_0x3c;
    } else {
        field_0x248 += 0.2f * JMASSin(angle);
        field_0x24c -= 0.2f;
        mPos.x = getPlayerPos_8023E724(mPlayer)->x + field_0x248;
        mPos.y = getPlayerPos_8023E724(mPlayer)->y + field_0x24c;
    }
    if (mTimer-- == 0) {
        s32 tensNew = field_0x240 / 10;
        s32 tensOld = field_0x244 / 10;
        s32 n = tensNew - tensOld;
        s32 kind = tensOld + 1;
        for (s32 i = 0; i < n; i++, kind++) {
            s32 serial = -1;
            if (kind >= 1 && kind < 3) {
                JGeometry::TVec3f p(getPlayerPos_8023E724(mPlayer)->x, getPlayerPos_8023E724(mPlayer)->y, 300.0f);
                serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &p, 2);
            } else if (kind >= 3 && kind < 6) {
                JGeometry::TVec3f p(getPlayerPos_8023E724(mPlayer)->x, getPlayerPos_8023E724(mPlayer)->y, 300.0f);
                serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &p, 3);
            } else {
                getActorManager_801F856C()->spawnActor_801F9484('FARY', field_0x4, getPlayerPos_8023E724(mPlayer), 0, -1, -1);
            }
            if (serial >= 0) {
                ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (gem != NULL) {
                    gem->field_0x3c.set(0.0f, 0.0f, -1.0f);
                }
            }
        }
        if (field_0x240 >= 15 && field_0x244 < 15) {
            JGeometry::TVec3f p(getPlayerPos_8023E724(mPlayer)->x, getPlayerPos_8023E724(mPlayer)->y, 300.0f);
            s32 serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &p, 2);
            if (serial >= 0) {
                ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (gem != NULL) {
                    gem->field_0x3c.set(0.0f, 0.0f, -1.0f);
                }
            }
        }
        mAlive = false;
    }
}

// FIXME(shotgun): 20.93 %, missing/extra + reloc-name; behaviour drift.
// (the original builds each TColor on the stack)
void Score_804AF4C0::fn_80393228() {
    f32 fade = 255.0f * ((f32)mTimer / 90.0f);
    if (mValue < 10) {
        if (field_0x4 < 8) {
            JGeometry::TVec3f pos = mPos;
            lbl_8053AF50->fn_80455770(0.5f, mValue, &pos,
                                      JUtility::TColor(lbl_80543170[mPlayer], lbl_80543174[mPlayer], lbl_80543178[mPlayer], (s32)fade));
        }
    } else if (mValue < 100) {
        if (field_0x4 < 8) {
            JGeometry::TVec3f pos = mPos;
            pos.x = mPos.x - 8.333f;
            lbl_8053AF50->fn_80455770(0.5f, mValue / 10, &pos,
                                      JUtility::TColor(lbl_80543170[mPlayer], lbl_80543174[mPlayer], lbl_80543178[mPlayer], (s32)fade));
        }
        if (field_0x4 < 8) {
            JGeometry::TVec3f pos = mPos;
            pos.x = 8.333f + mPos.x;
            lbl_8053AF50->fn_80455770(0.5f, mValue - (mValue / 10) * 10, &pos,
                                      JUtility::TColor(lbl_80543170[mPlayer], lbl_80543174[mPlayer], lbl_80543178[mPlayer], (s32)(255.0f * ((f32)mTimer / 90.0f))));
        }
    } else {
        if (field_0x4 < 8) {
            JGeometry::TVec3f pos = mPos;
            lbl_8053AF50->fn_80455770(0.5f, 10, &pos,
                                      JUtility::TColor(lbl_80543170[mPlayer], lbl_80543174[mPlayer], lbl_80543178[mPlayer], (s32)fade));
        }
    }
}

// Draws one digit of the gem value at x + dx, in the gem-yellow colour.
static inline void putScoreDigit(const Score_804AF4C0* self, f32 fade, f32 dx, u8 digit) {
    Vec pos;
    pos.x = self->mPos.x + dx;
    pos.y = self->mPos.y;
    pos.z = self->mPos.z;
    if (self->field_0x4 < 8) {
        lbl_8053AF50->fn_80455770(0.3f, digit, &pos, JUtility::TColor(255, 0xD2, 0, (s32)fade));
    }
}

// FIXME(shotgun): 15.93 %, missing/extra + reloc-name; behaviour drift.
// (cases 0-3 of the 12-entry jump table only; unrolled in the original)
void Score_804AF4C0::fn_80391F58() {
    f32 fade = 255.0f * ((f32)mTimer / 90.0f);
    switch (mValue) {
    case 0:
        putScoreDigit(this, fade, 0.0f, 1);
        break;
    case 1:
        putScoreDigit(this, fade, 0.0f, 5);
        break;
    case 2:
        putScoreDigit(this, fade, -5.0f, 2);
        putScoreDigit(this, fade, 5.0f, 0);
        break;
    case 3:
        putScoreDigit(this, fade, -5.0f, 5);
        putScoreDigit(this, fade, 5.0f, 0);
        break;
    default:
        // FIXME(shotgun): cases 4-11 not written
        break;
    }
}

// FIXME(shotgun): 63.83 %, missing/extra + reloc-name; behaviour drift.
// (the original builds a TVec3f copy of mPos and passes it on)
void Score_804AF4C0::fn_80391D44() {
    s32 n = fn_8023BC38(mPlayer) / 60 + 1;
    if (n < 10) {
        Vec pos;
        pos.x = mPos.x;
        pos.y = mPos.y;
        pos.z = mPos.z;
        if (field_0x4 < 8) {
            lbl_8053AF50->fn_80455770(0.5f, n, &pos,
                                      JUtility::TColor(lbl_8054317C[mPlayer], lbl_80543180[mPlayer], lbl_80543184[mPlayer], 255));
        }
    } else {
        Vec pos;
        pos.x = mPos.x - 10.0f;
        pos.y = mPos.y;
        pos.z = mPos.z;
        if (field_0x4 < 8) {
            lbl_8053AF50->fn_80455770(0.5f, 1, &pos,
                                      JUtility::TColor(lbl_8054317C[mPlayer], lbl_80543180[mPlayer], lbl_80543184[mPlayer], 255));
        }
        pos.x = mPos.x + 10.0f;
        pos.y = mPos.y;
        pos.z = mPos.z;
        if (field_0x4 < 8) {
            lbl_8053AF50->fn_80455770(0.5f, 0, &pos,
                                      JUtility::TColor(lbl_8054317C[mPlayer], lbl_80543180[mPlayer], lbl_80543184[mPlayer], 255));
        }
    }
}
#pragma auto_inline reset

Score_804AF4C0::~Score_804AF4C0() {}

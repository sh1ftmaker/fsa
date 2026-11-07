#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include <math.h>
#include "game/act/act_fairy.h"
#include "game/tu_803B8C74.h"

// Deletes the emitter at once but leaves the member as it is (update() runs this when the
// fairy is about to go away).
static inline void deleteEmitterKeep(JPABaseEmitter* emitter) {
    if (emitter != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(emitter);
    }
}

// 0x80161638.
Fairy_804A03B8::~Fairy_804A03B8() {
    releaseEmitter_801EE434(mEmitter290);
    releaseEmitter_801EE434(mEmitter294);
}

// 0x801615EC.
void Fairy_804A03B8::vfunc_0x44() {
    if (mEmitter290 != NULL) {
        mEmitter290->setGlobalTranslation(mPos);
    }
    if (mEmitter294 != NULL) {
        mEmitter294->setGlobalTranslation(mPos);
    }
}

// FIXME: 97.86 %. Apart from the float literals (`@NNN` against lbl_8053D6xx, a data split), the
// save-file block differs in registers: the original keeps the row pointer in r6, loads
// lbl_80529DEC's count before the 0x339 argument and keeps the table base in r3/r5; ours
// keeps the row in r5 and loads the count after li r4, 0x339.
// 0x80161290.
void Fairy_804A03B8::init() {
    mBox.i.x = -4.0f;
    mBox.i.y = -4.0f;
    mBox.f.x = 4.0f;
    mBox.f.y = 4.0f;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    mCounters[0] = 0;
    mCounters[1] = 0;
    mCounters[2] = 0;
    mCounters[3] = 0;
    field_0x250 = 0;
    field_0x26c = 0.0f;
    field_0x278 = 0.0f;
    field_0x27c = 0.0f;
    field_0x280 = 0.0f;
    mCounters[0] = 0x400;
    field_0x280 = 10.0f;
    mCounters[2] = 10;
    mCounters[3] = (s32)(30.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 30;
    field_0x260 = (field_0x90 >> 22) & 0x1F;
    field_0x258 = (field_0x90 >> 4) & 0xF;
    field_0x25c = field_0x90 & 0xF;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x270 = 255.0f;
    if (field_0x25c > 0) {
        field_0x25c--;
    } else {
        field_0x25c = -1;
    }
    field_0x248 = (s32)(32768.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    field_0x230 = 0;
    field_0xb0 = 0x20000;
    field_0x254 = -1;
    if (field_0x258 == 2) {
        field_0x11f = 1;
    }
    mEmitter290 = NULL;
    mEmitter294 = NULL;
    field_0x284.set(mPos);
    if (field_0x258 == 2) {
        mCounters[0] = 0;
        mCounters[1] = 0;
        field_0x248 = 0;
        field_0x230 = 4;
    }
    if (field_0x258 == 3) {
        mCounters[0] = 10000;
        field_0x280 = 100.0f;
        if (lbl_80529DEC.field_0x24f < 0x63) {
            lbl_80529DEC.field_0x24f++;
        } else {
            lbl_80529DEC.field_0x24f = 0x63;
        }
        mCounters[2] = 30;
        startSound_801F0E34(0x400E5, 0);
        field_0x230 = 5;
    }
    if ((field_0x90 >> 27) != 0 && !vfunc_0x34()) {
        field_0xb0 = 0;
        return;
    }
    mEmitter290 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x183, 0, 0, NULL, NULL, true, 1.0f);
    mEmitter294 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x184, 0, 0, NULL, NULL, true, 1.0f);
    if (field_0x230 == 0 || field_0x230 == 5) {
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            // FAKEMATCH(crack-pass): row view (tu_803B8C74.h SaveFileRow_CrackPass).
            SaveFileRow_CrackPass& row = lbl_8050EC80.getCurrentRow_CrackPass();
            if ((row.field_0x87 & 1) == 0) {
                row.field_0x87 |= 1;
                lbl_8053AED0->fn_8043D67C(0x339, 0, lbl_80529DEC.field_0x24f);
            }
        }
    }
}

// 0x8016088C.
// FIXME(shotgun): 94.67 %, different + reloc-name
void Fairy_804A03B8::update() {
    if (field_0x268 == 0 && (field_0x90 >> 27) != 0) {
        if (!vfunc_0x34()) {
            return;
        }
        field_0x268 = 1;
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            // FAKEMATCH(crack-pass): row view (tu_803B8C74.h SaveFileRow_CrackPass).
            SaveFileRow_CrackPass& row = lbl_8050EC80.getCurrentRow_CrackPass();
            if ((row.field_0x87 & 1) == 0) {
                row.field_0x87 |= 1;
                lbl_8053AED0->fn_8043D67C(0x339, 0, lbl_80529DEC.field_0x24f);
            }
        }
        field_0xb0 = 0x20000;
        if (mEmitter290 == NULL) {
            mEmitter290 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x183, 0, 0, NULL, NULL, true, 1.0f);
        }
        if (mEmitter294 == NULL) {
            mEmitter294 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x184, 0, 0, NULL, NULL, true, 1.0f);
        }
    }
    if (field_0x258 != 2) {
        addCalc2_801F71A4(&field_0x3c.x, field_0x278, 0.1f, 0.5f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x27c, 0.1f, 0.5f);
        addCalc2_801F71A4(&mPos.z, field_0x280, 0.3f, 0.5f);
        mPos.add(field_0x3c);
    }
    setEmitterPos_8013CB44(mEmitter290, &mPos, 0, 0, true);
    setEmitterPos_8013CB44(mEmitter294, &mPos, 0, 0, true);
    switch (field_0x230) {
    case 0:
        fn_801F3604();
        field_0x230 = 1;
        // fall through
    case 1:
        if (mCounters[2] == 0) {
            field_0x244 += 0x300;
            field_0x280 = 2.0f * JMath::sincosTable_.sinShort(field_0x244) + 7.0f;
            if (field_0x24c == 0) {
                if (mCounters[3] != 0) {
                    if ((mCounters[3] & 7) == 0) {
                        field_0x278 = 0.5f * JMath::sincosTable_.sinShort(field_0x248);
                        field_0x27c = 0.5f * JMath::sincosTable_.cosShort(field_0x248);
                    }
                    field_0x248 += 0x100;
                } else {
                    mCounters[3] = (s32)(30.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 30;
                    field_0x24c = 1;
                    field_0x278 = 0.0f;
                    field_0x27c = 0.0f;
                }
            } else if (mCounters[3] == 0) {
                field_0x248 += (s32)(32768.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                mCounters[3] = (s32)(40.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 40;
                field_0x24c = 0;
            }
            JGeometry::TVec2f center(mPos.x, mPos.y);
            JGeometry::TBox2f box(mBox);
            box.addPos(mPos.x, mPos.y);
            bool strict = true;
            if (gGameRoot_8053AAF8->field_0x7f != 0) {
                strict = false;
            }
            s32 player = fn_8023AF14(&box, field_0x4, strict, 0.0f);
            if (player < 0) {
                player = fn_8023B8E4(&box, field_0x4, &center, true);
            }
            if (player >= 0) {
                field_0x244 = 0;
                field_0x248 = 0;
                field_0x24c = 0;
                mCounters[0] = 500;
                field_0x244 = player;
                fn_80236554(field_0x244, 0x20, false);
                s32 layer = field_0x4;
                if (layer >= 8) {
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_PEFF, layer, &mPos,
                                                                   (field_0x244 & 3) | 4, -1, -1);
                }
                field_0x230 = 2;
            } else {
                field_0x254 = fn_801F2718('ZLD2');
                s32 serial = field_0x254;
                if (serial >= 0 && getActorManager_801F856C()->findActorBySerial_801F666C(serial) != NULL) {
                    lbl_8053AB9C = 0x20;
                    field_0x230 = 2;
                } else {
                    field_0x254 = fn_801F2718('ZLD4');
                    serial = field_0x254;
                    if (serial >= 0 && getActorManager_801F856C()->findActorBySerial_801F666C(serial) != NULL) {
                        lbl_8053ABEC = 0x20;
                        field_0x230 = 2;
                    }
                }
            }
        }
        break;
    case 2:
        if (field_0x254 < 0) {
            fn_80230FD0(field_0x244, 0x2000B);
        } else {
            startSound_801F0E34(0x400C8, 0);
        }
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            if (lbl_80529DEC.field_0x24f < 0x63) {
                lbl_80529DEC.field_0x24f++;
            } else {
                lbl_80529DEC.field_0x24f = 0x63;
            }
        }
        mCounters[2] = 0;
        field_0x248 = 0;
        field_0x24c = 0;
        field_0xb0 = 0;
        field_0x11e = 1;
        field_0x264 = field_0x4;
        field_0x230 = 3;
        // fall through
    case 3: {
        mCounters[0] = 500;
        field_0x248++;
        s32 serial = field_0x254;
        f32 targetX = mPos.x;
        f32 targetY = mPos.y;
        if (serial < 0) {
            if (field_0x264 == getPlayerLayer_8023DE58(field_0x244)) {
                targetX = getPlayerPos_8023E724(field_0x244)->x;
                targetY = getPlayerPos_8023E724(field_0x244)->y;
                field_0x284 = *getPlayerPos_8023E724(field_0x244);
            } else {
                field_0x248 = 200;
                targetX = field_0x284.x;
                targetY = field_0x284.y;
            }
        } else {
            ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (target != NULL) {
                if (field_0x264 == target->field_0x4) {
                    targetX = target->mPos.x;
                    targetY = target->mPos.y;
                    field_0x284 = target->mPos;
                } else {
                    field_0x248 = 200;
                    targetX = field_0x284.x;
                    targetY = field_0x284.y;
                }
            }
        }
        field_0x26c = 10.0f;
        if (mCounters[2] == 0) {
            field_0x274 = (s16)(JMath::atanTable_.atan2_(targetY - mPos.y, targetX - mPos.x) * 10430.378f);
            mCounters[2] = 4;
        }
        field_0x278 = field_0x26c * JMath::sincosTable_.sinShort(field_0x274);
        field_0x27c = field_0x26c * JMath::sincosTable_.cosShort(field_0x274);
        if (field_0x248 > 100 || getGameView_802D6A9C()->fn_802DCD0C()) {
            deleteEmitterKeep(mEmitter290);
            deleteEmitterKeep(mEmitter294);
            if (gGameRoot_8053AAF8->field_0x7f == 1) {
                lbl_8053AED0->fn_8043D67C(0x33A, 0, lbl_80529DEC.field_0x24f);
            }
            mAlive = false;
        }
        break;
    }
    case 4:
        if (mCounters[1] == 0) {
            if (field_0x248 == 0) {
                field_0x250++;
                if (field_0x250 > 2) {
                    field_0x250 = 0;
                }
                if (field_0x25c >= 0) {
                    f32 targetY = getPlayerPos_8023E724(field_0x25c)->y;
                    addCalc2_801F71A4(&mPos.y, targetY, 0.3f, 2.0f);
                    if (fabs(mPos.y - targetY) < 1.0f) {
                        field_0x250 = 0;
                        field_0x248 = 1;
                    }
                }
            } else {
                field_0x250++;
                field_0x250 &= 7;
                if (field_0x250 == 0) {
                    mAlive = false;
                }
            }
            mCounters[1] = 4;
        }
        break;
    case 5:
        if (mCounters[2] == 0) {
            addCalc2_801F71A4(&field_0x270, 0.0f, 0.3f, 3.0f);
            if (field_0x270 < 1.0f) {
                mAlive = false;
            }
        }
        break;
    }
    if (field_0x230 != 4 && mCounters[1] == 0) {
        mCounters[1] = 4;
        field_0x250++;
        if (field_0x250 > 2) {
            field_0x250 = 0;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (mCounters[i] > 0) {
            mCounters[i]--;
        }
        if (field_0x230 != 4) {
            if (field_0x258 == 0) {
                if (mCounters[0] <= 0) {
                    deleteEmitterKeep(mEmitter290);
                    deleteEmitterKeep(mEmitter294);
                    mAlive = false;
                }
            } else {
                mCounters[0] = 0x400;
            }
        }
    }
}

// 0x80160650.
void Fairy_804A03B8::draw() {
    SpriteParams_801F5AE0 params;
    if ((field_0x90 >> 27) != 0 && !vfunc_0x34()) {
        return;
    }
    if (field_0x258 != 2) {
        if (mCounters[0] < 0x100 && (mCounters[0] & 2)) {
            return;
        }
        params.field_0x2d = 1;
        if (field_0x258 == 3) {
            params.field_0x10.a = (u8)field_0x270;
            params.field_0x30 = 0x11;
        }
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x250 + 0x5E, &params, 1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x250 + 0x5E, false, 2, -1, 0, 0, 1);
    } else {
        s32 frame = 0x5E;
        if (field_0x248 != 0) {
            frame = 0x7D;
        }
        fn_803075AC(mSerial, field_0x4, &mPos, frame + field_0x250, false, 2, -1, 1, 0, 1);
    }
}

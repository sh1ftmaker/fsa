#include "game/GameAudio.h"
#include "game/act/act_bee_manager.h"
#include "game/act/tu_803489A4.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"

BeeManager_804AC810::~BeeManager_804AC810() {
    for (int i = 0; i < 14; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
        if (actor != NULL) {
            actor->fn_801F3668();
        }
    }
}

void BeeManager_804AC810::init() {
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x238 = -1;
    field_0x23c = -1;
    field_0x278 = field_0x90 & 0xF;
    field_0x280 = 0;
    field_0x282 = 0;
    for (int i = 0; i < 14; i++) {
        field_0x240[i] = -1;
    }
}

// The `== true` tests on bool locals/results are the form that matches (plain truth tests: 94.06 %, 8 instructions short).
// FIXME: 95.79% (scheduling+regalloc): same 538 instructions as target; register allocation in the 14-bee loops shifts by one GPR (r27/r28/r29 vs r25/r27/r28), and the 0.85f velocity scale and ACTOR_RUPY spawn argument setup schedule a few instructions in a different order.
void BeeManager_804AC810::update() {
    if (field_0x230 != 0 && field_0x230 != 4) {
        field_0x280 += 0x222;
        if (field_0x238 >= 0 && getPlayerLayer_8023DE58(field_0x238) >= 8) {
            field_0x282 = 1;
        }
        if (field_0x238 < 0) {
            field_0x282 = 1;
        }
        if (field_0x282 == 0) {
            for (int i = 0; i < 14; i++) {
                BeeFormation_804AC7B8* bee = (BeeFormation_804AC7B8*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
                if (bee != NULL) {
                    if (field_0x238 >= 0 && fn_8022F514(field_0x238) == true) {
                        field_0x238 = fn_8022F3DC(field_0x238);
                    }
                    bee->field_0x244 = field_0x238;
                    if (bee->field_0x230 != 4) {
                        bee->field_0x260 = field_0x280;
                    }
                }
            }
            field_0x23c = field_0x238;
            int deadCount = 0;
            for (int i = 0; i < 14; i++) {
                BeeFormation_804AC7B8* bee = (BeeFormation_804AC7B8*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
                if (bee == NULL) {
                    deadCount++;
                    if (deadCount >= 5) {
                        field_0x282 = 1;
                        break;
                    }
                } else {
                    if (bee->field_0x230 == 4) {
                        field_0x282 = 1;
                        break;
                    }
                }
            }
        }
    }

    if (field_0x282 == 1) {
        bool allReady = true;
        for (int i = 0; i < 14; i++) {
            BeeFormation_804AC7B8* bee = (BeeFormation_804AC7B8*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
            if (bee != NULL) {
                bee->field_0x230 = 4;
                // Explicit copy+scale matches the original single stack temporary at 0x2c(r1) (without operator*'s second return temporary).
                JGeometry::TVec3f vel = bee->field_0x3c;
                vel.scale(0.85f);
                bee->field_0x3c = vel;
                if (bee->field_0x264 == 0) {
                    allReady = false;
                }
            }
        }
        field_0x230 = 4;
        if (allReady == true) {
            for (int i = 0; i < 14; i++) {
                BeeFormation_804AC7B8* bee = (BeeFormation_804AC7B8*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
                if (bee != NULL) {
                    bee->field_0x265 = 1;
                }
            }
        }
    }

    if (field_0x230 != 0) {
        startSound_801F0E34(0x4007E, 0);
    }

    switch (field_0x230) {
    case 0: {
        bool triggered = false;
        if (field_0x278 == 2) {
            if (vfunc_0x34() == true) {
                field_0x230 = 1;
                triggered = true;
            }
        } else {
            for (int i = 0; i < 4; i++) {
                if (field_0x278 == 1 && !fn_8023A51C(i)) {
                    continue;
                }
                f32 dx = getPlayerPos_8023E724(i)->x - mPos.x;
                f32 dy = getPlayerPos_8023E724(i)->y - mPos.y;
                if (dx * dx + dy * dy < 9216.0f) {
                    triggered = true;
                    break;
                }
            }
        }
        if (triggered == true) {
            fn_801EEC98(mSerial, NULL, NULL, NULL, &field_0x238, 0, 0);
            if (field_0x238 >= 0) {
                field_0x230 = 1;
            }
        }
        if (field_0x230 == 1) {
            for (int i = 0; i < 14; i++) {
                field_0x240[i] = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FOBE, field_0x4, &mPos, 0, -1, -1);
                BeeFormation_804AC7B8* bee = (BeeFormation_804AC7B8*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
                if (bee != NULL) {
                    bee->field_0x250 = mSerial;
                    bee->field_0x254 = i;
                }
            }
        }
        break;
    }

    case 1: {
        bool noneInState1 = true;
        bool allInState2 = true;
        for (int i = 0; i < 14; i++) {
            BeeFormation_804AC7B8* bee = (BeeFormation_804AC7B8*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
            if (bee != NULL) {
                if (noneInState1 == true && bee->field_0x230 == 1) {
                    noneInState1 = false;
                }
                if (bee->field_0x230 != 2) {
                    allInState2 = false;
                    break;
                }
            }
        }
        if (allInState2 == true) {
            field_0x234 = (s32)(60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 180;
            field_0x230 = 2;
        } else if (noneInState1 == true) {
            fn_801EEC98(mSerial, NULL, NULL, NULL, &field_0x238, 0, 0);
            for (int i = 0; i < 14; i++) {
                BeeFormation_804AC7B8* bee = (BeeFormation_804AC7B8*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
                if (bee != NULL) {
                    bee->field_0x244 = field_0x238;
                }
            }
        }
        break;
    }

    case 2:
        if (field_0x234 <= 0) {
            for (int i = 0; i < 14; i++) {
                BeeFormation_804AC7B8* bee = (BeeFormation_804AC7B8*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
                if (bee != NULL) {
                    bee->field_0x25c = 64.0f;
                    bee->field_0x230 = 3;
                }
            }
            field_0x230 = 3;
        }
        break;

    case 3: {
        bool foundState2 = false;
        for (int i = 0; i < 14; i++) {
            BeeFormation_804AC7B8* bee = (BeeFormation_804AC7B8*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
            if (bee != NULL) {
                if (bee->field_0x230 == 2) {
                    foundState2 = true;
                    field_0x234 = (s32)(60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 180;
                    break;
                }
            }
        }
        if (foundState2 == true) {
            for (int i = 0; i < 14; i++) {
                BeeFormation_804AC7B8* bee = (BeeFormation_804AC7B8*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
                if (bee != NULL) {
                    bee->field_0x230 = 2;
                }
            }
            field_0x230 = 1;
        }
        break;
    }

    case 4: {
        bool noneExist = true;
        bool allDone = true;
        for (int i = 0; i < 14; i++) {
            BeeFormation_804AC7B8* bee = (BeeFormation_804AC7B8*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
            if (bee != NULL) {
                noneExist = false;
                if (bee->field_0x262 == 0) {
                    if (bee->field_0x230 != 4) {
                        bee->field_0x230 = 4;
                    }
                    allDone = false;
                }
            }
        }
        if (noneExist == true && field_0x27c >= 0) {
            JGeometry::TVec3f pos;
            pos.zero();
            fn_80349914(&pos);
            s16 baseAngle = (s32)(65536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            for (int i = 0; i < 14; i++) {
                u32 arg = (i & 1) + 1;
                s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &pos, arg | 0x1000000, -1, -1);
                ActorBase_804A5770* rupy = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (rupy != NULL) {
                    s16 angle = baseAngle + (i << 16) / 14;
                    rupy->field_0x3c = JGeometry::TVec3f(
                        2.0f * JMath::sincosTable_.sinShort(angle),
                        2.0f * JMath::sincosTable_.cosShort(angle),
                        4.0f
                    );
                }
            }
        }
        if (allDone == true) {
            for (int i = 0; i < 14; i++) {
                ActorBase_804A5770* bee = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x240[i]);
                if (bee != NULL) {
                    bee->fn_801F3668();
                }
            }
            fn_801F3668();
            return;
        }
        break;
    }
    }

    if (field_0x234 > 0) {
        field_0x234--;
    }
}

void BeeManager_804AC810::draw() {
}

#pragma auto_inline off
// Out-of-line in the original asm: BeeManager_804AC810::update calls fn_80349914 via bl at 0x8034A1F4.
void BeeManager_804AC810::fn_80349914(JGeometry::TVec3f* outPos) {
    outPos->set(*getPlayerPos_8023E724(field_0x27c));
    s32 idx = fn_8022E970(field_0x27c);
    switch (fn_8022CAF8(field_0x27c)) {
    case 0:
        if (idx != 1 && idx >= 1 && idx < 5) {
            outPos->x += 8.0f * (idx - 1);
        }
        break;
    case 1:
        if (idx != 1 && idx >= 1 && idx < 5) {
            outPos->y += 8.0f * (idx - 1);
        }
        break;
    case 2:
        switch (idx) {
        case 1:
            break;
        case 2:
            outPos->x += 8.0f;
            break;
        case 3:
        case 4:
            outPos->x += 8.0f;
            outPos->y += 8.0f;
            break;
        }
        break;
    case 3:
        if (idx != 1) {
            switch (field_0x238) {
            case 0:
                outPos->y += 16.0f;
                break;
            case 1:
                outPos->x += 16.0f;
                break;
            case 2:
                outPos->x -= 16.0f;
                break;
            case 3:
                outPos->y -= 16.0f;
                break;
            }
        }
        break;
    case 5:
        break;
    }
}
#pragma auto_inline reset

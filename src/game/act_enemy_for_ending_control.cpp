#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_enemy_for_ending_control.h"
#include "game/act/act_princess_zelda_ending.h"

// A step of a sequence table: the value (negative ends the sequence) and the
// time the sequencer waits before reading the next step.
struct EndingStep_804AD460 {
    s32 value;
    s32 time;
};

// The sequence tables (.rodata), each ended by {-1, -1}.
static const EndingStep_804AD460 lbl_804724F8[] = {{0, 8}, {1, 8}, {2, 40}, {-1, -1}};
static const EndingStep_804AD460 lbl_80472518[] = {{0, 9}, {1, 9}, {2, 9}, {-1, -1}};
static const EndingStep_804AD460 lbl_80472538[] = {{3, 9}, {4, 9}, {5, 9}, {-1, -1}};
static const EndingStep_804AD460 lbl_80472558[] = {{0, 8}, {3, 8}, {-1, -1}};
static const EndingStep_804AD460 lbl_80472570[] = {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7},
                                                   {5, 7}, {6, 7}, {7, 7}, {-1, -1}};
static const EndingStep_804AD460 lbl_804725B8[] = {{8, 7},  {9, 7},  {10, 7}, {11, 7}, {12, 7},
                                                   {13, 7}, {14, 7}, {15, 7}, {-1, -1}};
static const EndingStep_804AD460 lbl_80472600[] = {{16, 7}, {17, 7}, {18, 7}, {19, 7}, {20, 7},
                                                   {21, 7}, {22, 7}, {23, 7}, {-1, -1}};
static const EndingStep_804AD460 lbl_80472648[] = {{0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {5, 9}, {-1, -1}};
static const EndingStep_804AD460 lbl_80472680[] = {{14, 9}, {15, 9}, {16, 9}, {17, 9}, {18, 9}, {19, 9}, {-1, -1}};
static const EndingStep_804AD460 lbl_804726B8[] = {{14, 16}, {-1, -1}};
static const EndingStep_804AD460 lbl_804726C8[] = {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {-1, -1}};

// The sequence tables by fn_8035C15C's argument (.data).
const EndingStep_804AD460* lbl_804AD460[11] = {
    lbl_804724F8, lbl_80472518, lbl_80472538, lbl_80472558, lbl_80472570, lbl_804725B8,
    lbl_80472600, lbl_80472648, lbl_80472680, lbl_804726B8, lbl_804726C8,
};

EnemyForEndingControl_804AD48C::~EnemyForEndingControl_804AD48C() {}

// Phase handler in lbl_804AD3A0 (read by fn_8035C650).
void EnemyForEndingControl_804AD48C::fn_8035B650() {
    if (field_0x234 == 0) {
        fn_80119DD0()->fn_80119998(0x96, 0, 1, 0, 0, 0xFA, 0);
        field_0x234++;
        field_0x260 = 0x11C;
    } else if (field_0x260 == 0) {
        if (mEmitter298 != NULL) {
            mEmitter298->setStatus(JPAEMIT_ForceDelete);
            mEmitter298 = NULL;
        }
        if (mEmitter29C != NULL) {
            mEmitter29C->setStatus(JPAEMIT_ForceDelete);
            mEmitter29C = NULL;
        }
        if (mEmitter2A0 != NULL) {
            mEmitter2A0->setStatus(JPAEMIT_ForceDelete);
            mEmitter2A0 = NULL;
        }
        mAlive = false;
    } else if (field_0x260 == 1) {
        fn_801F856C()->fn_801F9484(ACTOR_ENDM, field_0x4, &mPos, 0, -1, -1);
        JGeometry::TVec3f pos(-100.0f, -100.0f, 0.0f);
        for (s32 i = 0; i < 4; i++) {
            fn_80230498(i, &pos, 0, 0xFF, -0x80, -0x80, true);
        }
    }
}

// Once the setup has run, flags the EZLD and ELNK actors and clears the
// setup flag.
void EnemyForEndingControl_804AD48C::fn_8035B7A8() {
    if (field_0x234 != 0) {
        cls_804A5958* ezld =
            static_cast<cls_804A5958*>(fn_801F856C()->fn_801F666C(field_0x278));
        if (ezld != NULL) {
            ezld->field_0x238 = 1;
        }
        cls_804A5958* elnk =
            static_cast<cls_804A5958*>(fn_801F856C()->fn_801F666C(field_0x294));
        if (elnk != NULL) {
            elnk->field_0x238 = 1;
        }
        field_0x234 = 0;
    }
}

// Moves field_0x240 toward 1765 and field_0x254 toward 125, each by its step;
// once both have arrived, flags EZLD and counts the step in 0x230.
void EnemyForEndingControl_804AD48C::fn_8035B81C() {
    s32 arrived = 0;
    if (field_0x240 < 1765.0f) {
        field_0x240 = field_0x240 + field_0x258;
    } else {
        field_0x240 = 1765.0f;
        arrived = 1;
    }
    if (field_0x254 > 125.0f) {
        field_0x254 = field_0x254 - field_0x25c;
    } else {
        field_0x254 = 125.0f;
        arrived++;
    }
    if (arrived >= 2) {
        cls_804A5958* ezld =
            static_cast<cls_804A5958*>(fn_801F856C()->fn_801F666C(field_0x278));
        if (ezld != NULL) {
            ezld->field_0x238 = 1;
        }
        field_0x230++;
    }
}

void EnemyForEndingControl_804AD48C::fn_8035BE00() {
    if (field_0x254 > 110.0f) {
        field_0x254 -= 1.0f;
    }
}

// Empty phase handler in the member-pointer table lbl_804AD3A0 (read by fn_8035C650).
void EnemyForEndingControl_804AD48C::fn_8035BE20() {}

// Advances the sequencer; true once the sequence is over.
bool cls_804A5958::fn_8035C15C(s32 seq) {
    if (field_0x244 == 0) {
        const EndingStep_804AD460* steps = lbl_804AD460[seq];
        if (steps[field_0x24c].value < 0) {
            return true;
        }
        field_0x244 = steps[field_0x24c].time;
        field_0x248 = steps[field_0x24c].value;
        field_0x24c++;
    }
    return false;
}

// Restarts the sequencer on table `seq`: stores `arg` (0x250), clears the
// timer and the step index, then runs the first step (fn_8035C15C inlined).
void cls_804A5958::fn_8035C1C4(s32 seq, s32 arg) {
    field_0x250 = arg;
    field_0x244 = 0;
    field_0x24c = 0;
    fn_8035C15C(seq);
}

// Draws the sprite with mPos raised by 1 for this call only.
void EnemyForEndingControl_804AD48C::draw() {
    if (field_0x238 != 0) {
        JGeometry::TVec3f saved(mPos);
        mPos.y += 1.0f;
        SpriteParams_801F5AE0 params;
        params.field_0x1d = -1;
        fn_801F06F0("enemy_gc_sob.bin", 0x179, &params, -1, 1.0f, 1.0f);
        mPos = saved;
    }
}

void EnemyForEndingControl_804AD48C::vfunc_0x10() {}

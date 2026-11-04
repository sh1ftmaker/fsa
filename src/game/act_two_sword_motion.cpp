#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/Effect.h"
#include "game/Message.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_two_sword_motion.h"

// Offsets from the actor's position for each swing phase: row = fn_80230CFC's
// result, column = the phase column of fn_80336040.
static const f32 lbl_80471AE0[16][3] = {
    {16.0f, 0.0f, 0.0f},   {12.0f, 0.0f, 8.0f},   {0.0f, 0.0f, 16.0f},   {16.0f, 0.0f, -1.0f},
    {-16.0f, 0.0f, 0.0f},  {-12.0f, 0.0f, 8.0f},  {0.0f, 0.0f, 16.0f},   {-16.0f, 0.0f, -1.0f},
    {0.0f, 16.0f, 0.0f},   {0.0f, 12.0f, 8.0f},   {0.0f, 0.0f, 16.0f},   {0.0f, 16.0f, -1.0f},
    {0.0f, -16.0f, 0.0f},  {0.0f, -12.0f, 8.0f},  {0.0f, 0.0f, 16.0f},   {0.0f, -16.0f, -1.0f},
};

// FIXME: 91.65 %. Instruction set matches; the mPos copy and the paired-single
// sum are scheduled differently (the original loads mPos.z early and rebuilds
// it through a stack word for the ps_add), so the stack slots shift.
bool TwoSwordMotion_804ABA90::fn_80336040() {
    s32 column;
    bool result = false;
    field_0x26c++;
    if (field_0x26c < 8) {
        field_0x260 = 0;
        column = 0;
    } else if (field_0x26c < 0x10) {
        field_0x260 = 1;
        column = 0;
    } else if (field_0x26c < 0x18) {
        field_0x260 = 2;
        column = 1;
    } else {
        field_0x260 = 3;
        column = 2;
    }
    s32 row = fn_80230CFC(field_0x24c);
    if (row == 0) {
        field_0x270 = 0x2b0;
    } else if (row == 1) {
        field_0x270 = 0x2b4;
    } else if (row == 2) {
        field_0x270 = 0x2b8;
    } else {
        field_0x270 = 0x2bc;
    }
    field_0x270 += field_0x260;
    s32 frame = field_0x26c % 36;
    if (frame < 6) {
        field_0x264 = 0;
    } else if (frame < 12) {
        field_0x264 = 1;
    } else if (frame < 18) {
        field_0x264 = 0;
    } else if (frame < 24) {
        field_0x264 = 2;
    } else if (frame < 30) {
        field_0x264 = 3;
    } else {
        field_0x264 = 2;
    }
    u16 id = field_0x264 + 0x296;
    field_0x274 = id;
    if (field_0x26c > 0x28) {
        result = true;
    }
    JGeometry::TVec3f pos;
    JGeometry::TVec3f swing;
    const f32* table = lbl_80471AE0[row * 4 + column];
    JGeometry::TVec3f offset(table[0], table[1], table[2]);
    swing.add(mPos, offset);
    pos = mPos;
    fn_80230498(field_0x24c, &pos, field_0x270, 0xff, -0x80, -0x80, true);
    fn_80230498(field_0x250, &swing, field_0x274, 0xff, -0x80, -0x80, true);
    return result;
}

void TwoSwordMotion_804ABA90::draw() {}

// The step's offset along the swing: the loop index minus half of (phase - 1),
// computed from field_0x278 after each call (the original reloads it there).
static inline f32 swingPhase(s32 index, s32 phase) {
    return (f32)index - ((f32)phase - 1.0f) * 0.5f;
}

// FIXME: 96.1 %. At the fn_8038BD08 call the original loads field_0x4 and
// the address of its by-value mPos copy before the psq copy and builds the
// copy at sp+0x44 (frame 0x90); ours builds it at sp+0x8, so the stack slots
// of the later position temporaries differ.
void TwoSwordMotion_804ABA90::update() {
    switch (field_0x230) {
    case 0:
        if (field_0x285 == 1) {
            field_0x230 = 1;
            for (s32 i = 0; i < 4; i++) {
                if (i != field_0x24c && i != field_0x250) {
                    if (fn_8022E438(i, field_0x24c) != true) {
                        continue;
                    }
                    if (fn_8022CAF8(i) == 4 && fn_8022F478(i)) {
                        continue;
                    }
                }
                field_0x278++;
            }
        } else {
            field_0x285 = 1;
            for (s32 i = 0; i < 4; i++) {
                if (i != field_0x24c && i != field_0x250) {
                    if (fn_8022E438(i, field_0x24c) != true) {
                        continue;
                    }
                    if (fn_8022CAF8(i) == 4 && fn_8022F478(i)) {
                        continue;
                    }
                }
                if (!fn_802300EC(i)) {
                    field_0x285 = 0;
                    break;
                }
            }
            if (field_0x285 == 1) {
                for (s32 i = 0; i < 4; i++) {
                    if (i != field_0x24c && i != field_0x250) {
                        if (fn_8022E438(i, field_0x24c) != true) {
                            continue;
                        }
                        if (fn_8022CAF8(i) == 4 && fn_8022F478(i)) {
                            continue;
                        }
                    }
                    fn_80230AD4(i);
                }
            }
        }
        break;
    case 1:
        if (fn_80336040() == true) {
            for (s32 i = 0; i < 4; i++) {
                if (i != field_0x24c && i != field_0x250) {
                    if (fn_8022E438(i, field_0x24c) != true) {
                        continue;
                    }
                    if (fn_8022CAF8(i) == 4 && fn_8022F478(i)) {
                        continue;
                    }
                }
                if (field_0x2a0[i] == NULL) {
                    switch (i) {
                    case 0:
                        field_0x2a0[i] = fn_8013CC50(field_0x4, &mPos, 0x477, 0, -1, NULL, NULL, true, 1.0f);
                        break;
                    case 1:
                        field_0x2a0[i] = fn_8013CC50(field_0x4, &mPos, 0x479, 0, -1, NULL, NULL, true, 1.0f);
                        break;
                    case 2:
                        field_0x2a0[i] = fn_8013CC50(field_0x4, &mPos, 0x476, 0, -1, NULL, NULL, true, 1.0f);
                        break;
                    case 3:
                        field_0x2a0[i] = fn_8013CC50(field_0x4, &mPos, 0x478, 0, -1, NULL, NULL, true, 1.0f);
                        break;
                    }
                }
                if (field_0x2b0[i] == NULL) {
                    field_0x2b0[i] = fn_8013CC50(field_0x4, &mPos, 0x475, 0, -1, NULL, NULL, true, 1.0f);
                }
                field_0x281[i] = 0;
            }
            field_0x230 = 2;
            field_0x234 = 0x5a;
            field_0x25c = 10;
            field_0x294 = *fn_8023E724(field_0x24c);
        }
        break;
    case 2:
        if (field_0x234 <= 0) {
            if (field_0x254 == 0) {
                fn_8038BD08(field_0x24c, 0xd4, field_0x4, mPos, 1);
            }
            field_0x230 = 3;
        } else {
            field_0x25c--;
            if (field_0x25c == 0) {
                field_0x258++;
                if (field_0x258 == 0) {
                    field_0x25c = 8;
                } else if (field_0x258 == 1) {
                    field_0x25c = 3;
                    fn_80230FD0(field_0x24c, 0x10008);
                    fn_80230F44(field_0x24c, 0x1000002);
                } else if (field_0x258 == 2) {
                    field_0x25c = 0x1e;
                } else if (field_0x258 >= 3) {
                    field_0x258 = 2;
                    field_0x25c = 0x5a;
                }
            }
        }
        {
            s32 count = 0;
            for (s32 i = 0; i < 4; i++) {
                if (i != field_0x24c && i != field_0x250) {
                    if (fn_8022E438(i, field_0x24c) != true) {
                        continue;
                    }
                    if (fn_8022CAF8(i) == 4 && fn_8022F478(i)) {
                        continue;
                    }
                }
                JGeometry::TVec3f pos(field_0x294.x + field_0x27c * swingPhase(count, field_0x278),
                                      field_0x294.y, field_0x294.z);
                fn_80230498(i, &pos, field_0x258 + 0x31a, 0xff, -0x80, -0x80, true);
                if (field_0x2a0[i] != NULL) {
                    fn_8013CB44(field_0x2a0[i], &pos, 0, -1, true);
                }
                if (field_0x2b0[i] != NULL) {
                    JGeometry::TVec3f pos2(field_0x294.x + field_0x27c * swingPhase(count, field_0x278),
                                           field_0x294.y, field_0x294.z);
                    fn_8013CB44(field_0x2b0[i], &pos2, 0, -1, true);
                }
                if (field_0x280 == 0) {
                    JGeometry::TVec3f p20((field_0x27c + 0.5f) * swingPhase(count, field_0x278) + field_0x294.x,
                                          field_0x294.y, field_0x294.z);
                    if (vfunc_0x4C(field_0x4, &p20) == true) {
                        field_0x280 = 1;
                    }
                }
                if (field_0x27c < 16.0f && field_0x258 >= 2 && field_0x280 == 0) {
                    field_0x27c += 0.5f;
                } else if (field_0x258 >= 2 || field_0x280 == 1) {
                    if (field_0x281[i] == 0) {
                        JGeometry::TVec3f a(pos.x + 6.0f, pos.y - 29.0f, pos.z);
                        fn_8013CC50(field_0x4, &a, 0xcb, 0, 1, NULL, NULL, true, 1.0f);
                        JGeometry::TVec3f b(pos.x + 6.0f, pos.y - 29.0f, pos.z);
                        fn_8013CC50(field_0x4, &b, 0xcc, 0, 1, NULL, NULL, true, 1.0f);
                        field_0x281[i] = 1;
                    }
                }
                count++;
            }
        }
        break;
    case 3:
        if (field_0x234 <= 0 && fn_80388EEC(field_0x24c)) {
            bool none = true;
            for (s32 i = 0; i < 4; i++) {
                if (i != field_0x24c && i != field_0x250) {
                    if (fn_8022E438(i, field_0x24c) != true) {
                        continue;
                    }
                    if (fn_8022CAF8(i) == 4 && fn_8022F478(i)) {
                        continue;
                    }
                }
                if (fn_80230224(i, false) == true) {
                    continue;
                }
                none = false;
                break;
            }
            if (none == true) {
                for (s32 i = 0; i < 4; i++) {
                    if (field_0x2a0[i] != NULL) {
                        field_0x2a0[i]->becomeInvalidEmitter();
                    }
                    if (field_0x2b0[i] != NULL) {
                        field_0x2b0[i]->becomeInvalidEmitter();
                    }
                }
                mAlive = false;
            }
        }
        break;
    }
    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
}

// FIXME: 84.07 %. The original keeps layer in r29 and the serial (loaded
// before the fn_8020D58C call) in r30; ours assigns them the other way round,
// which adds an mr pair. Same instructions otherwise.
bool TwoSwordMotion_804ABA90::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes;
    boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL) == true) {
        return true;
    }
    u16 code = fn_8022461C(pos, layer);
    if (code == 0 || code == 2 || code == 0x50 || code == 0x39 || code == 0xd || code == 0x61) {
        return false;
    }
    if (code == 0x22 && fn_802D6A9C()->field_0x170 == 0) {
        return false;
    }
    if (code == 0x25 && fn_802D6A9C()->field_0x171 == 0) {
        return false;
    }
    if (code == 4) {
        if (((s32)pos->y & 0xF) >= 8) {
            return false;
        }
    } else if (code == 5) {
        if (((s32)pos->y & 0xF) < 8) {
            return false;
        }
    } else if (code == 6) {
        if (((s32)pos->x & 0xF) >= 8) {
            return false;
        }
    } else if (code == 7) {
        if (((s32)pos->x & 0xF) < 8) {
            return false;
        }
    }
    return fn_802265DC(pos, layer, true);
}

void TwoSwordMotion_804ABA90::init() {
    field_0x24c = field_0x90 & 3;
    field_0x250 = (field_0x90 >> 2) & 3;
    field_0x254 = 1;
    mPos = *fn_8023E724(field_0x24c);
    mSpawnPos = mPos;
    field_0x4 = fn_8023DE58(field_0x24c);
    for (s32 i = 0; i < 4; i++) {
        field_0x2a0[i] = NULL;
        field_0x2b0[i] = NULL;
        field_0x281[i] = 1;
    }
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x285 = 1;
    for (s32 i = 0; i < 4; i++) {
        if (i != field_0x24c && i != field_0x250) {
            if (fn_8022E438(i, field_0x24c) != true) {
                continue;
            }
            if (fn_8022CAF8(i) == 4 && fn_8022F478(i)) {
                continue;
            }
        }
        if (!fn_802300EC(i)) {
            field_0x285 = 0;
            break;
        }
    }
    if (field_0x285 == 1) {
        for (s32 i = 0; i < 4; i++) {
            if (i != field_0x24c && i != field_0x250) {
                if (fn_8022E438(i, field_0x24c) != true) {
                    continue;
                }
                if (fn_8022CAF8(i) == 4 && fn_8022F478(i)) {
                    continue;
                }
            }
            fn_80230AD4(i);
        }
    }
    field_0x258 = 0;
    field_0x27c = 0.0f;
    field_0x280 = 0;
    field_0x260 = 0;
    field_0x268 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    field_0x274 = 0;
    field_0x264 = 0;
    field_0x278 = 0;
    field_0x294.zero();
    field_0x11f = 1;
    field_0x230 = 0;
}

TwoSwordMotion_804ABA90::~TwoSwordMotion_804ABA90() {
    for (s32 i = 0; i < 4; i++) {
        fn_801EE434(field_0x2a0[i]);
        fn_801EE434(field_0x2b0[i]);
    }
}

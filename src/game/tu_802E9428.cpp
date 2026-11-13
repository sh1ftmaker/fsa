#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameView.h"
#include "game/Camera.h"
#include "game/ActorManager.h"
#include "JSystem/JMath/JMath.h"
#include "game/Unk_8045A080.h"
#include "game/act/act_kaepora_gaebora_ending.h"
#include "game/act/act_link_ending_2.h"
#include "game/act/tu_80362634.h"
#include "game/act/tu_804432DC.h"
#include "game/act/tu_8044B80C.h"
#include "game/act/tu_802E9428.h"

struct EndmStagePair {
    u16 mStage;
    u16 mLevel;
};

static const s32 lbl_8046F280_0[17] = {
    0, 0, 1, 1, 2, 2, 2, 3, 3, 0, 0, 4, 5, 5, 6, 7, 7,
};

static const s32 lbl_8046F280_44[17] = {
    0, 0, 1, 1, 1, 1, 0, 1, 3, 3, 0, 0, 0, 0, 3, 3, 0,
};

static const s16 lbl_8046F280_88[18] = {
    0xB4, 0xB4, 0xB4, 0xB4, 0xB4, 0xB4, 0xB4, 0xB4, 0xB4,
    0xB4, 0xB4, 0xB4, 0xB4, 0xB4, 0xB4, 0xB4, 0xB4, 0x00,
};

static const EndmStagePair lbl_8046F280_AC[17] = {
    { 13, 0 }, { 13, 1 }, { 13, 2 }, { 13, 3 },
    { 14, 0 }, { 14, 1 }, { 14, 2 }, { 14, 3 },
    { 15, 0 }, { 15, 1 }, { 16, 3 }, { 15, 2 },
    { 15, 3 }, { 17, 0 }, { 16, 0 }, { 16, 1 },
    { 16, 2 },
};

static const u8 lbl_8046F280_F0[20] = {
    0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 5, 5, 6, 7, 7, 0, 0, 0,
};

static const u8 lbl_8046F280_104[20] = {
    1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0,
};

extern u8* lbl_804AA0B8[17];
JGeometry::TVec2f lbl_804D56A8[17] = {
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f),
    JGeometry::TVec2f(50.0f, 40.0f)
};
JGeometry::TVec2f lbl_804D5730[49] = {
    JGeometry::TVec2f(2740.0f, 1726.0f),
    JGeometry::TVec2f(2000.0f, 100.0f),
    JGeometry::TVec2f(250.0f, 100.0f),
    JGeometry::TVec2f(612.0f, 520.0f),
    JGeometry::TVec2f(330.0f, 3286.0f),
    JGeometry::TVec2f(1170.0f, 1728.0f),
    JGeometry::TVec2f(1890.0f, 310.0f),
    JGeometry::TVec2f(768.0f, 3250.0f),
    JGeometry::TVec2f(1744.0f, 2520.0f),
    JGeometry::TVec2f(768.0f, 2850.0f),
    JGeometry::TVec2f(768.0f, 690.0f),
    JGeometry::TVec2f(768.0f, 1710.0f),
    JGeometry::TVec2f(2200.0f, 1600.0f),
    JGeometry::TVec2f(1600.0f, 1344.0f),
    JGeometry::TVec2f(1280.0f, 3215.0f),
    JGeometry::TVec2f(768.0f, 3700.0f),
    JGeometry::TVec2f(664.0f, 3704.0f)
};

// FIXME(shotgun): 99.75 %, operands + reloc-name; behaviour near.
void UnknownENDM_804AA0FC::init() {
    field_0x284 = getGameView_802D6A9C()->field_0x4;
    field_0x280 = getGameView_802D6A9C()->field_0x8;
    field_0x23c = 0;
    field_0x230 = 0;
    field_0x28d = 0;
    field_0x298 = 0;
    field_0x240 = 0;
    field_0x294 = 0;
    field_0x295 = 0;
    field_0x299 = 1;
    field_0x270 = 300.0f;
    field_0x274 = 300.0f;
    field_0x278 = 0.0f;
    gAudioMgr_8053AF80->fn_80458FF0(0x100028, true, 0);
    field_0x234 = 0;
    field_0x231 = 0;
    field_0x29a = lbl_8053AF30->fn_8044B80C(field_0x294);
}

// FIXME(shotgun): 60.20 %, missing/extra + reloc-name; behaviour drift.
void UnknownENDM_804AA0FC::update() {
    field_0x234++;
    switch (field_0x23c) {
    case 0:
        if (field_0x29a == 0) {
            field_0x29a = lbl_8053AF30->fn_8044B80C(field_0x294);
            if (field_0x29a != 1) {
                break;
            }
        }
        field_0x29a = 0;
        if (field_0x294 == 0) {
            field_0x27c = 300;
        } else if (field_0x294 == 7) {
            field_0x27c = 480;
        } else {
            field_0x27c = 420;
        }
        lbl_8053AF30->mDraw = true;
        field_0x294++;
        if (field_0x294 > 7) {
            field_0x294 = 7;
        }
        field_0x23c = 1;
        if (field_0x299 == 1) {
            fn_80119DD0()->fn_80119958();
            fn_80119DD0()->fn_80119998(50, 0, 0, 0, 0, 0, 0);
            field_0x299 = 0;
        }
        fn_8016A3AC()->field_0x1d8 = -1;
        fn_8016A3AC()->fn_8016A2B4();
        break;
    case 1:
        lbl_8053AF30->mDraw = true;
        if (field_0x27c != 0) {
            field_0x27c--;
        }
        if (field_0x27c == 0) {
            fn_80119DD0()->fn_80119998(100, 0, 2, 0, 0, 20, 0);
            field_0x27c = 60;
            field_0x23c = 2;
        }
        break;
    case 2:
        if (field_0x27c != 0) {
            field_0x27c--;
        }
        if (field_0x27c == 0) {
            if (field_0x230 == 15) {
                field_0x23c = 4;
            } else {
                field_0x23c = 3;
            }
        } else {
            lbl_8053AF30->mDraw = true;
        }
        break;
    case 3: {
        u8 kind = lbl_8046F280_F0[field_0x230];
        if (kind == 0) {
            lbl_8053AEF0->fn_804432DC(kind, 200, 200);
            field_0x27c = 430;
        } else {
            lbl_8053AEF0->fn_804432DC(kind, 0, 200);
            field_0x27c = 200;
        }
        field_0x23c = 4;
        field_0x28c = field_0x230 & 0x3F;
        break;
    }
    case 4:
        if (field_0x27c != 0) {
            field_0x27c--;
        }
        if (field_0x27c == 0) {
            if (lbl_8046F280_104[field_0x230] != 0) {
                lbl_8053AED0->fn_8043A268(lbl_8046F280_0[field_0x230], lbl_8046F280_44[field_0x230],
                                          lbl_8046F280_88[field_0x230], lbl_804D56A8[field_0x230].x,
                                          lbl_804D56A8[field_0x230].y);
            }
            field_0x274 = 300.0f;
            fn_802E948C(300.0f, 1);
            u16 stage = lbl_8046F280_AC[field_0x28c & 0x7F].mStage;
            u16 level = lbl_8046F280_AC[field_0x28c & 0x7F].mLevel;
            getGameView_802D6A9C()->field_0x4 = stage;
            getGameView_802D6A9C()->field_0x8 = level;
            getGameView_802D6A9C()->fn_802D9CD8();
            s32 prevLevel = field_0x280;
            s32 prevStage = field_0x284;
            getGameView_802D6A9C()->field_0x4 = prevStage;
            getGameView_802D6A9C()->field_0x8 = prevLevel;
            s32 count = getActorManager_801F856C()->mActorEnd;
            for (s32 i = 0; i < count; i++) {
                field_0x240 = getActorManager_801F856C()->mActors[i];
                if (field_0x240 != NULL && field_0x240->mActorId == 'EOWL' && field_0x240->mAlive == true &&
                    (field_0x240->field_0x90 & 0x100) == 0 &&
                    (field_0x230 < 4 || field_0x230 > 6 || (field_0x240->field_0x90 & 7) != 0)) {
                    field_0x23c = 5;
                    field_0x244 = field_0x240->mPos;
                    field_0x250 = lbl_804D5730[field_0x230].x;
                    field_0x254 = lbl_804D5730[field_0x230].y;
                    JGeometry::TVec2f focus;
                    focus.x = field_0x250;
                    focus.y = field_0x254 + -30.0f;
                    gGameRoot2_8053AB10->field_0x24->fn_8030C7C4(&focus, (f32)(s32)field_0x270);
                    field_0x290 = lbl_804AA0B8[field_0x230];
                    field_0x260 = (f32)(u32)(field_0x250 / 512.0f) * 512.0f;
                    field_0x264 = (f32)(u32)(field_0x254 / 384.0f) * 384.0f;
                    if (field_0x230 == 5) {
                        field_0x26c -= 30.0f;
                    } else if (field_0x230 == 16) {
                        field_0x260 += 75.0f;
                    }
                    field_0x268 = field_0x260 + 512.0f;
                    field_0x26c = field_0x264 + 384.0f;
                    if (*field_0x290 != 0xFF) {
                        if (*field_0x290 == 0) {
                            field_0x264 -= 384.0f;
                        } else if (*field_0x290 == 1) {
                            field_0x26c += 384.0f;
                        } else if (*field_0x290 == 2) {
                            field_0x260 -= 512.0f;
                        } else if (*field_0x290 == 3) {
                            field_0x268 += 512.0f;
                        }
                    }
                    return;
                }
            }
            mAlive = false;
        } else if (field_0x27c == 60) {
            fn_80119DD0()->fn_80119998(100, 0, 2, 0, 0, 20, 0);
        }
        break;
    case 5: {
        field_0x244 = field_0x240->mPos;
        if (static_cast<KaeporaGaeboraEnding_804ADAC8*>(field_0x240)->field_0x230 == 1) {
            fn_802E94F4();
            field_0x297 = 1;
        } else {
            field_0x298 = 0;
            fn_802E94F4();
            break;
        }
        field_0x28d = *field_0x290;
        if (field_0x28d == 0xFF) {
            field_0x23c = 7;
            break;
        }
        u16 stage = lbl_8046F280_AC[field_0x28c & 0x7F].mStage;
        u16 level = lbl_8046F280_AC[field_0x28c & 0x7F].mLevel;
        getGameView_802D6A9C()->field_0x4 = stage;
        getGameView_802D6A9C()->field_0x8 = level;
        if (field_0x28d == 0) {
            s32 dir[2] = { 0, -1 };
            getGameView_802D6A9C()->fn_802DCD1C(dir);
        } else if (field_0x28d == 1) {
            s32 dir[2] = { 0, 1 };
            getGameView_802D6A9C()->fn_802DCD1C(dir);
        } else if (field_0x28d == 2) {
            s32 dir[2] = { -1, 0 };
            getGameView_802D6A9C()->fn_802DCD1C(dir);
        } else if (field_0x28d == 3) {
            s32 dir[2] = { 1, 0 };
            getGameView_802D6A9C()->fn_802DCD1C(dir);
        }
        s32 prevLevel = field_0x280;
        s32 prevStage = field_0x284;
        getGameView_802D6A9C()->field_0x4 = prevStage;
        getGameView_802D6A9C()->field_0x8 = prevLevel;
        field_0x23c = 6;
        field_0x288 = (s32)(field_0x244.x / 512.0f);
        field_0x28a = (s32)(field_0x244.y / 384.0f);
        if (*field_0x290 != 0xFF) {
            field_0x260 = (f32)(u32)(field_0x250 / 512.0f) * 512.0f;
            field_0x264 = (f32)(u32)(field_0x254 / 384.0f) * 384.0f;
            field_0x268 = field_0x260 + 512.0f;
            field_0x26c = field_0x264 + 384.0f;
            if (field_0x230 == 5) {
                field_0x26c -= 30.0f;
            } else if (field_0x230 == 16) {
                field_0x260 += 75.0f;
            }
            if (*field_0x290 == 0) {
                field_0x264 -= 384.0f;
            } else if (*field_0x290 == 1) {
                field_0x26c += 384.0f;
            } else if (*field_0x290 == 2) {
                field_0x260 -= 512.0f;
            } else if (*field_0x290 == 3) {
                field_0x268 += 512.0f;
            }
        }
        field_0x290++;
        break;
    }
    case 6:
        field_0x244 = field_0x240->mPos;
        fn_802E94F4();
        if (field_0x288 != (u16)(s32)(field_0x244.x / 512.0f) ||
            field_0x28a != (u16)(s32)(field_0x244.y / 384.0f)) {
            JGeometry::TVec2f pos;
            pos.x = field_0x244.x;
            pos.y = field_0x244.y;
            if (fn_802E9884(&pos)) {
                field_0x23c = 5;
                field_0x297 = 1;
            }
        }
        break;
    case 7:
        if (field_0x29a == 0) {
            field_0x29a = lbl_8053AF30->fn_8044B80C(field_0x294);
        }
        if (field_0x27c == 0) {
            field_0x244 = field_0x240->mPos;
            fn_802E94F4();
            s32 owlState = static_cast<KaeporaGaeboraEnding_804ADAC8*>(field_0x240)->field_0x230;
            if (owlState == 2 || owlState == 3) {
                field_0x295 = owlState;
                field_0x27c = 200;
            }
        } else {
            fn_802E94F4();
            field_0x27c--;
            if (field_0x27c != 0) {
                ActorBase_804A5770* owl = field_0x240;
                JGeometry::TVec3f owlPos = owl->mPos;
                JGeometry::TBox2f view = gGameRoot2_8053AB10->field_0x24->field_0x164;
                if ((owl->field_0x90 & 7) == 0) {
                    view.i.x -= 16.0f;
                    view.i.y -= 16.0f;
                    view.f.x += 16.0f;
                    view.f.y += 16.0f;
                }
                bool inside = owlPos.x >= view.i.x && owlPos.y >= view.i.y && owlPos.x < view.f.x && owlPos.y < view.f.y;
                if (inside && owl->mAlive != false) {
                    break;
                }
            }
            field_0x27c = 60;
            fn_80119DD0()->fn_80119998(100, 0, 2, 0, 0, 20, 0);
            field_0x23c = 8;
        }
        break;
    case 8:
        if (field_0x27c != 0) {
            field_0x27c--;
        }
        if (field_0x27c == 0) {
            if (field_0x295 == 2) {
                field_0x23c = 0;
            } else {
                field_0x23c = 4;
                field_0x27c = 10;
            }
            field_0x230++;
            field_0x28c = field_0x230 & 0x3F;
            field_0x258 = lbl_804D5730[field_0x230].x;
            field_0x25c = lbl_804D5730[field_0x230].y;
            mPos.x = field_0x258;
            mPos.y = field_0x25c;
            field_0x240 = NULL;
            s32 count = getActorManager_801F856C()->mActorEnd;
            for (s32 i = 0; i < count; i++) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                if (actor != NULL && actor->mAlive == true) {
                    if (actor->mActorId == 'EOWL') {
                        static_cast<KaeporaGaeboraEnding_804ADAC8*>(actor)->fn_803613D0();
                        actor->mAlive = false;
                    } else if (actor->mActorId == 'EETC') {
                        static_cast<cls_804ADF28*>(actor)->fn_80362634();
                        actor->mAlive = false;
                    } else if (actor->mActorId == 'ELK2') {
                        static_cast<Link2Ending_804AE338*>(actor)->fn_8036E544();
                        actor->mAlive = false;
                    } else if (actor->mActorId == 'RUPY') {
                        actor->mAlive = false;
                    }
                }
            }
            field_0x231 = 0;
        } else {
            fn_802E94F4();
        }
        break;
    }
    if (field_0x240 != NULL && field_0x23c < 7) {
        mPos = field_0x244;
    }
}

// Whether the focus `pos` has reached the edge of the 512 x 384 tile (the walk direction in
// field_0x290: 0 up, 1 down, 2 left, 3 right); in the settling state (field_0x297 set) it
// clears the flag once the edge is passed.
// FIXME(shotgun): 98.02 %, scheduling+regalloc; behaviour clean.
bool UnknownENDM_804AA0FC::fn_802E9884(const JGeometry::TVec2f* pos) {
    s32 ry = (s32)pos->y % 384 - 30;
    s32 rx = (s32)pos->x % 512;
    if (field_0x297 == 0) {
        u8 dir = *field_0x290;
        if (dir == 0xFF) {
            field_0x23c = 7;
            return false;
        }
        switch (dir) {
        case 0:
            if (ry - ((s32)field_0x270 * 3 / 4) / 2 < 0x10) {
                return true;
            }
            break;
        case 1:
            if (ry + ((s32)field_0x270 * 3 / 4) / 2 > 0x170) {
                return true;
            }
            break;
        case 2:
            if (rx - (s32)field_0x270 / 2 < 0x10) {
                return true;
            }
            break;
        case 3:
            if (rx + (s32)field_0x270 / 2 > 0x1F0) {
                return true;
            }
            break;
        }
    } else {
        switch (field_0x28d) {
        case 0: {
            s32 v = ry + ((s32)field_0x270 * 3 / 4) / 2;
            if (v < 0x170 && v > 0x160) {
                field_0x297 = 0;
            }
            break;
        }
        case 1: {
            s32 v = ry - ((s32)field_0x270 * 3 / 4) / 2;
            if (v > 0x20 && v < 0x30) {
                field_0x297 = 0;
            }
            break;
        }
        case 2: {
            s32 v = rx + (s32)field_0x270 / 2;
            if (v < 0x1F0 && v > 0x1E0) {
                field_0x297 = 0;
            }
            break;
        }
        case 3: {
            s32 v = rx - (s32)field_0x270 / 2;
            if (v > 0x10 && v < 0x20) {
                field_0x297 = 0;
            }
            break;
        }
        }
    }
    if (field_0x297 == 0) {
        getGameView_802D6A9C()->fn_802D9CB0();
    }
    return false;
}

// Follows the target focus: closes in on (field_0x244, field_0x248) by easing, keeps the camera box
// inside the limits, eases the zoom to its target and hands both to the camera.
// FIXME(shotgun): 84.70 %, missing/extra + reloc-name; behaviour drift.
void UnknownENDM_804AA0FC::fn_802E94F4() {
    f32 tx = field_0x244.x;
    f32 ty = field_0x244.y;
    if (field_0x298 != 0) {
        field_0x250 = tx;
        field_0x254 = ty;
    } else {
        JGeometry::TVec3f vel = field_0x240->field_0x3c;
        f32 dx = field_0x250 - tx;
        f32 dy = field_0x254 - ty;
        f32 sq = square(dx) + square(dy);
        f32 dist = sq;
        if (!(sq <= 0.0f)) {
            dist = sq * JGeometry::TUtil<f32>::inv_sqrt(sq);
        }
        f32 speed = PSVECMag(&vel);
        if (dist <= 1.5f * speed) {
            field_0x250 = tx;
            field_0x254 = ty;
            field_0x298 = 1;
        } else if (dist <= 2.0f * speed) {
            field_0x250 = 0.5f * (field_0x244.x - field_0x250) + field_0x250;
            field_0x254 = 0.5f * (field_0x244.y - field_0x254) + field_0x254;
        } else if (0.1f * dist < 2.5f) {
            field_0x250 = 0.1f * (field_0x244.x - field_0x250) + field_0x250;
            field_0x254 = 0.1f * (field_0x244.y - field_0x254) + field_0x254;
        } else {
            s32 angle = (s32)(JMath::atanTable_.atan2_(field_0x244.y - field_0x254, field_0x244.x - field_0x250) * 10430.378f);
            field_0x250 = 2.5f * JMath::sincosTable_.cosShort(angle) + field_0x250;
            field_0x254 = 2.5f * JMath::sincosTable_.sinShort(angle) + field_0x254;
        }
    }
    JGeometry::TVec2f focus;
    focus.x = field_0x250 + 0.0f;
    focus.y = field_0x254 + -30.0f;
    JGeometry::TBox2f view = gGameRoot2_8053AB10->field_0x24->field_0x164;
    f32 halfW = (view.f.x - view.i.x) * 0.5f;
    f32 halfH = (view.f.y - view.i.y) * 0.5f;
    if (focus.x + halfW >= field_0x268) {
        focus.x = field_0x268 - halfW;
    } else if (focus.x - halfW <= field_0x260) {
        focus.x = field_0x260 + halfW;
    }
    if (focus.y + halfH >= field_0x26c) {
        focus.y = field_0x26c - halfH;
    } else if (focus.y - halfH <= field_0x264) {
        focus.y = field_0x264 + halfH;
    }
    if (field_0x270 != field_0x274) {
        field_0x270 += field_0x278;
        if (field_0x278 < 0.0f) {
            if (field_0x270 < field_0x274) {
                field_0x270 = field_0x274;
            }
        } else if (field_0x270 > field_0x274) {
            field_0x270 = field_0x274;
        }
    }
    gGameRoot2_8053AB10->field_0x24->fn_8030C7C4(&focus, (f32)(s32)field_0x270);
}

// FIXME: 99.81 %, reloc-name; behaviour clean.
// only: the 2^52 float-conversion constant is @954 in ours, lbl_805412F0 in the original
bool UnknownENDM_804AA0FC::fn_802E948C(f32 target, s32 frames) {
    if (field_0x270 == target) {
        return false;
    }
    f32 step = __fabsf(field_0x270 - target) / (f32)frames;
    if (field_0x270 < target) {
        field_0x278 = step;
    } else {
        field_0x278 = -step;
    }
    field_0x274 = target;
    return true;
}

void UnknownENDM_804AA0FC::draw() {}

UnknownENDM_804AA0FC::~UnknownENDM_804AA0FC() {}

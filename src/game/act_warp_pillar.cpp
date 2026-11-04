#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_warp_pillar.h"

// Emitters are released as fn_801EE434 does (the nearest helper). FIXME: the
// original clears the emitter's status word directly (word 0xF4 |= 1, word
// 0x24 = 1) and nulls the member itself, so these sites differ.
static inline void releaseEmitter(JPABaseEmitter*& emitter) {
    if (emitter != NULL) {
        fn_801EE434(emitter);
        emitter = NULL;
    }
}

static const u16 lbl_805420A0[4] = {0x4E3, 0x4E5, 0x4E2, 0x4E4};

// FIXME: 86.31 %. The six inline emitter releases (lwz 0xF4; ori 1; stw 0xF4;
// stw 1 to 0x24) are a bl fn_801EE434 here (30 instructions fewer). The
// original keeps this in r28 and saves r27-r31 with one stmw; ours keeps it
// in r29 and saves r28-r31 with stw. The stack slots of order, pos and a/b
// also differ (pos at 0x48 in the fourth block, 0x44 in the original).
void WarpPillar_804AC0A8::update() {
    f32 top = mPos.y + mBox.i.y;
    f32 bottom = mPos.y + mBox.f.y;
    f32 left = mPos.x + mBox.i.x;
    f32 right = mPos.x + mBox.f.x;

    fn_801F0E34(0x5003B, 0);

    cls_804A0160* shadow = lbl_8053AB10->field_0x2c;
    if (shadow->field_0xc > 0x80) {
        u8 alpha = shadow->field_0xc - 1;
        if (alpha <= 0x80) {
            alpha = 0x80;
        }
        shadow->field_0xc = alpha;
    }

    for (s32 i = 0; i < 4; i++) {
        if (!fn_80230188(i)) {
            continue;
        }
        if (field_0x230 == 2 || field_0x230 == 3) {
            JGeometry::TVec3f pos(*fn_8023E724(i));
            if (field_0x230 != 1) {
                pos += field_0x3c;
                fn_801F71A4(&pos.x, mPos.x - 24.0f + 16.0f * field_0x248[i], 0.02f, 10.0f);
            }
            if (field_0x230 == 3) {
                fn_80230498(i, &pos, 0x3CF, (u8)(field_0x244 * 0xFF / 50), -0x80, -0x80, true);
            } else {
                fn_80230498(i, &pos, 0x3CF, 0xFF, -0x80, -0x80, true);
            }
        }
    }

    if (field_0x230 != 0 && field_0x230 != 1) {
        field_0x3c.y -= 0.05f;
    }

    field_0x234 = 0;
    switch (field_0x230) {
    case 0: {
        for (s32 i = 0; i < 4; i++) {
            if (!fn_8023F264(i) || fn_8023EA28(i) || !fn_8023561C(i) || fn_80236B50(i)) {
                continue;
            }
            JGeometry::TVec3f* p = fn_8023E724(i);
            bool inside = p->x >= left && p->y >= top && p->x < right && p->y < bottom;
            field_0x25c[i] = inside;
            if (field_0x25c[i]) {
                field_0x234++;
                if (mEmitter26c[i] == NULL) {
                    JGeometry::TVec3f pos(mPos.x, 16.0f + mPos.y, mPos.z);
                    mEmitter26c[i] = fn_8013CC50(field_0x4, &pos, lbl_805420A0[i], 1, 2, NULL, NULL, true, 1.0f);
                }
            } else {
                releaseEmitter(mEmitter26c[i]);
            }
        }
        if (field_0x234 >= 4 && fn_8023077C()) {
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos(*fn_8023E724(i));
                if (fn_8022E970(i) > 1 && fn_80234A88(i)) {
                    fn_8022CA6C(i, 5, true);
                    field_0x260[i] = true;
                }
                fn_80230498(i, &pos, 0x3CD, 0xFF, -0x80, -0x80, true);
            }
            field_0x240 = 50;
            field_0x230 = 1;
        }
        break;
    }
    case 1: {
        field_0x240--;
        if (field_0x240 <= 0) {
            for (s32 i = 0; i < 4; i++) {
                if (field_0x258 < fn_8023E724(i)->y) {
                    field_0x258 = fn_8023E724(i)->y;
                }
            }
            s32 order[4] = {0, 1, 2, 3};
            for (s32 n = 4; n > 0; n--) {
                s32* cur = &order[1];
                for (s32 j = 1; j < n; j++, cur++) {
                    JGeometry::TVec3f a(*fn_8023E724(cur[-1]));
                    JGeometry::TVec3f b(*fn_8023E724(*cur));
                    if (a.x > b.x) {
                        s32 tmp = cur[-1];
                        cur[-1] = *cur;
                        *cur = tmp;
                    }
                }
            }
            for (s32 k = 0; k < 4; k++) {
                field_0x248[order[k]] = k;
            }
            field_0x3c.y = -0.5f;
            field_0x230 = 2;
        } else {
            for (s32 i = 0; i < 4; i++) {
                s32 owner = i;
                if (fn_8022F514(i)) {
                    owner = fn_8022F3DC(i);
                }
                JGeometry::TVec3f pos(*fn_8023E724(i));
                if (field_0x260[owner]) {
                    JGeometry::TVec3f other(*fn_8023E724(owner));
                    f32 dx = pos.x - other.x;
                    s32 n = fn_8022E8D4(i);
                    if (dx < 16.0f * n && dx > -16.0f * n) {
                        if (other.x > mPos.x) {
                            pos.x -= n;
                        } else {
                            pos.x += n;
                        }
                    }
                }
                s32 id = 0x3CD;
                if (field_0x240 <= 46) {
                    id = 0x3CE;
                }
                fn_80230498(i, &pos, id, 0xFF, -0x80, -0x80, true);
            }
        }
        break;
    }
    case 2: {
        fn_801F0E34(0x5003A, 0);
        f32 maxY = 0.0f;
        for (s32 i = 0; i < 4; i++) {
            if (maxY < fn_8023E724(i)->y) {
                maxY = fn_8023E724(i)->y;
            }
        }
        if (field_0x258 - maxY > 30.0f) {
            field_0x244 = 50;
            field_0x230 = 3;
        }
        break;
    }
    case 3:
        fn_801F0E34(0x5003A, 0);
        field_0x244--;
        if (field_0x244 <= 0) {
            fn_802D6A9C()->fn_802D9FF4(field_0x238, field_0x23c);
            lbl_8053AAF8->fn_80136798(0xF, 0);
            releaseEmitter(mEmitter264);
            releaseEmitter(mEmitter268);
            for (s32 i = 0; i < 4; i++) {
                releaseEmitter(mEmitter26c[i]);
            }
            mAlive = false;
        }
        break;
    default:
        break;
    }
}

void WarpPillar_804AC0A8::draw() {}

void WarpPillar_804AC0A8::init() {
    field_0x94 = field_0x90 & 0xF;
    mLife = 10000;
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x238 = (field_0x90 >> 4) & 0xF;
    field_0x23c = (field_0x90 >> 8) & 0xF;
    field_0x258 = 0.0f;
    mEmitter264 = NULL;
    mEmitter268 = NULL;
    field_0x25c[0] = false;
    mEmitter26c[0] = NULL;
    field_0x248[0] = 0;
    field_0x260[0] = false;
    field_0x25c[1] = false;
    mEmitter26c[1] = NULL;
    field_0x248[1] = 0;
    field_0x260[1] = false;
    field_0x25c[2] = false;
    mEmitter26c[2] = NULL;
    field_0x248[2] = 0;
    field_0x260[2] = false;
    field_0x25c[3] = false;
    mEmitter26c[3] = NULL;
    field_0x248[3] = 0;
    field_0x260[3] = false;

    if (mEmitter264 == NULL) {
        JGeometry::TVec3f pos(mPos.x, 16.0f + mPos.y, mPos.z);
        mEmitter264 = fn_8013CC50(field_0x4, &pos, 0x4E0, 1, 2, NULL, NULL, true, 1.0f);
    }
    if (mEmitter268 == NULL) {
        JGeometry::TVec3f pos(mPos.x, 16.0f + mPos.y, mPos.z);
        mEmitter268 = fn_8013CC50(field_0x4, &pos, 0x4E1, 1, 2, NULL, NULL, true, 1.0f);
    }

    mBox.i.x = -47.0f;
    mBox.i.y = -47.0f;
    mBox.f.x = 47.0f;
    mBox.f.y = 15.0f;
}

WarpPillar_804AC0A8::~WarpPillar_804AC0A8() {
    fn_801EE434(mEmitter264);
    fn_801EE434(mEmitter268);
    for (s32 i = 0; i < 4; i++) {
        fn_801EE434(mEmitter26c[i]);
    }
}

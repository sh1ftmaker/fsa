#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_rainbow_bridge.h"

static const u16 lbl_80465098[5] = {0x0396, 0x0397, 0x0398, 0x0399, 0x039A};
static const u16 lbl_804650A4[5] = {0x0127, 0x0128, 0x0129, 0x012A, 0x012B};

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec3f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

inline bool cls_8049E610::isPlayerOn(s32 player) {
    if (player < 0) {
        return false;
    }
    if (field_0x4 != getPlayerLayer_8023DE58(player)) {
        return false;
    }
    JGeometry::TBox2f box(mBox);
    for (s32 i = 0; i < 8; i++) {
        JGeometry::TVec3f v;
        fn_8022E088(player, i, &v);
        v -= mPos;
        v.z = 0.0f;
        s32 angle = (s32)(JMath::atanTable_.atan2_(v.y, v.x) *
                          (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
        f32 len = JGeometry::TUtil<f32>::sqrt(v.x * v.x + v.y * v.y);
        v.z = 0.0f;
        v.x = len * JMACosShort(angle);
        v.y = len * JMASinShort(angle);
        if (contains(box, v)) {
            fn_8022DF48(player);
            return true;
        }
    }
    return false;
}

inline void cls_8049E610::checkActivate() {
    if (vfunc_0x34()) {
        field_0x238 = 1;
        // The original stores the state and then switches on it (a state setter inlined here, asm 0x8012A058).
        switch (field_0x238) {
        case 0:
            break;
        case 1:
            if (!getGameView_802D6A9C()->fn_802D800C(field_0x230)) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x39f, 0, -1, NULL, NULL, true, 1.0f);
                for (int i = 0; i < 5; i++) {
                    if (field_0x23c[i] == NULL) {
                        field_0x23c[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80465098[i], 0, -1, NULL, NULL, true, 1.0f);
                    }
                }
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3a0, 0, -1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x39b, 0, -1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x39c, 0, -1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x39d, 0, -1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x39e, 0, -1, NULL, NULL, true, 1.0f);
            } else {
                for (int i = 0; i < 5; i++) {
                    if (field_0x23c[i] == NULL) {
                        field_0x23c[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804650A4[i], 0, -1, NULL, NULL, true, 1.0f);
                    }
                }
            }
            if (!getGameView_802D6A9C()->fn_802D800C(field_0x230)) {
                startSound_801F0E34(0x5004d, 0);
                field_0x234 = 0;
            }
            break;
        }
    }
}

cls_8049E610::cls_8049E610()
    : field_0x230(0), field_0x234(-1), field_0x238(0) {
    for (int i = 0; i < 5; i++) {
        field_0x23c[i] = NULL;
    }
}

// FIXME: 87.42% (try.py class different, but instruction count and order are identical: only the register assignment differs. The original keeps the loop index in r30, the field pointer in r29 and the NULL in r31; ours has index r29, pointer r30).
cls_8049E610::~cls_8049E610() {
    for (int i = 0; i < 5; i++) {
        releaseEmitter_801EE434(field_0x23c[i]);
        field_0x23c[i] = NULL;
    }
}

// FIXME: 99.78% (reloc-name only: the unit's .sdata2 literals (1.0f, -40.0f, ...) are @NNN here, lbl_8053CB38..48 in the original, i.e. pool order differs; code is identical).
void cls_8049E610::init() {
    ActorBase_804A5770::init();
    field_0x230 = (field_0x90 >> 22) & 0x1F;
    checkActivate();
    mBox.i.x = -40.0f;
    mBox.i.y = -64.0f;
    mBox.f.x = 40.0f;
    mBox.f.y = 64.0f;
    mBox2.set(mBox);
    field_0x80.set(mBox);
}

// FIXME: 97.69% (missing/extra + reloc-name: one extra instruction, a copy of the player index (mr r27,r26) from the inlined isPlayerOn parameter, which also costs one more saved register (stmw r26 vs r27); the rest is the same register renaming as __dt__ in the emitter loops, plus .sdata2 literal pool order).
void cls_8049E610::update() {
    ActorBase_804A5770::update();
    switch (field_0x238) {
    case 0:
        checkActivate();
        break;
    case 1:
        for (int i = 0; i < 4; i++) {
            if (isPlayerOn(i)) {
                fn_8023DF68(i, &(mPos - mPrevPos));
            }
        }
        if (field_0x234 >= 0) {
            field_0x234++;
            if (field_0x234 >= 420) {
                startSound_801F0E34(0x5004e, 0);
                field_0x234 = -1;
            }
        }
        if (field_0x230 >= 0) {
            getGameView_802D6A9C()->fn_802D8050(field_0x230, true, 0xFF);
        }
        break;
    }
}

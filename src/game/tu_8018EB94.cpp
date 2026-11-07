#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/tu_8018EB94.h"
#include "game/GameGlobals.h"
#include "JSystem/JMath/JMath.h"

extern s32 lbl_8053AB88;

// Destructor at 0x8018FB24.
cls_804A1FA0::~cls_804A1FA0() {
    for (int i = 0; i < 3; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// FIXME(shotgun): 85.97 %, missing/extra + reloc-name
void cls_804A1FA0::init() {
    field_0x80.i.x = -6.0f;
    field_0x80.i.y = -6.0f;
    field_0x80.f.x = 6.0f;
    field_0x80.f.y = 6.0f;
    mBox.i.x = -6.0f;
    mBox.i.y = -6.0f;
    mBox.f.x = 6.0f;
    mBox.f.y = 6.0f;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x288[0].set(mPos);
    field_0x288[1].set(mPos);
    field_0x288[2].set(mPos);
    field_0x288[3].set(mPos);
    field_0x288[4].set(mPos);
    field_0x288[5].set(mPos);
    for (u8 i = 0; i < 20; i++) {
        field_0x2d0[i].set(mPos);
    }
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    field_0x98 = (s32)(15.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    field_0x25c = field_0x98;
    field_0x260 = field_0x98;
    field_0x264 = -1;
    field_0x27c = field_0x278 * JMath::sincosTable_.sinShort((s32)(4000.0f * (f32)field_0x98));
    field_0x280 = field_0x278 * JMath::sincosTable_.cosShort((s32)(4000.0f * (f32)field_0x98));
    field_0x284 = 0.0f;
    field_0x23c = 8;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x268 = 0.3f;
    field_0x26c = 1.0f;
    field_0x270 = 1.0f;
    field_0x274 = 0.0f;
    field_0x278 = 1.3f;
    if ((lbl_8053AB88 & 1) == 0) {
        field_0x254 = 1;
    } else {
        field_0x254 = -1;
    }
    lbl_8053AB88++;
    lbl_8053AB88 = lbl_8053AB88 & 1;
    field_0xb0 = 0x12202;
    field_0x230 = 0;
    s16 angle = (s32)(4000.0f * (f32)field_0x98);
    field_0x27c = field_0x278 * JMath::sincosTable_.sinShort(angle);
    field_0x280 = field_0x278 * JMath::sincosTable_.cosShort((s32)(4000.0f * (f32)field_0x98));
    field_0x288[4].set(mPos);
    angle = (s32)(4000.0f * (f32)((field_0x98 - 2) & 0xF));
    field_0x288[4].x += 6.0f * JMath::sincosTable_.sinShort(angle);
    field_0x288[4].y += 6.0f * JMath::sincosTable_.cosShort(angle);
    field_0x288[5].set(mPos);
    angle = (s32)(4000.0f * (f32)((field_0x98 + 2) & 0xF));
    field_0x288[5].x += 6.0f * JMath::sincosTable_.sinShort(angle);
    field_0x288[5].y += 6.0f * JMath::sincosTable_.cosShort(angle);
}

// 0x8018FAB4.
// FIXME(shotgun): 67.61 %, missing/extra
void cls_804A1FA0::vfunc_0x44() {
    if (mEmitters[0] != NULL) {
        mEmitters[0]->becomeInvalidEmitter();
        mEmitters[0] = NULL;
    }
    if (mEmitters[1] != NULL) {
        mEmitters[1]->becomeInvalidEmitter();
        mEmitters[1] = NULL;
    }
    if (mEmitters[2] != NULL) {
        mEmitters[2]->becomeInvalidEmitter();
        mEmitters[2] = NULL;
    }
}

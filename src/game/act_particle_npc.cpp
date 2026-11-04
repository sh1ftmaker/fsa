#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/act/act_particle_npc.h"

void ParticleNpc_804A9AC8::draw() {}

ParticleNpc_804A9AC8::~ParticleNpc_804A9AC8() {
    if (field_0x234 != NULL) {
        fn_801EE434(field_0x234);
    }
    if (field_0x238 != NULL) {
        fn_801EE434(field_0x238);
    }
    if (field_0x23c != NULL) {
        fn_801EE434(field_0x23c);
    }
    if (mSound != NULL) {
        delete mSound;
    }
}

void ParticleNpc_804A9AC8::update() {
    if (field_0x250 <= 1) {
        fn_801F0E34(0x30033, 0);
    }
    switch (field_0x240) {
    case 0:
        if (!vfunc_0x34()) {
            return;
        }
        switch (field_0x250) {
        case 0: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x188, field_0x254, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos2(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x238 = fn_8013CC50(field_0x4, &pos2, 0x189, field_0x254, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos3(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x23c = fn_8013CC50(field_0x4, &pos3, 0x18a, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 1: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x18b, field_0x254, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos2(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x238 = fn_8013CC50(field_0x4, &pos2, 0x18c, field_0x254, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos3(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x23c = fn_8013CC50(field_0x4, &pos3, 0x18d, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 2: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x552, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 3: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x17e, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 4: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x17f, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 5: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x180, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 6: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x6e, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 7: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0xa9, -1, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos2(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x238 = fn_8013CC50(field_0x4, &pos2, 0xaa, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 8: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x6f, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        }
        field_0x240 = 1;
        break;
    case 1:
        switch (field_0x250) {
        case 6:
        case 8: {
            mSound->startSound(0x30074);
            break;
        }
        }
        break;
    }
}

void ParticleNpc_804A9AC8::init() {
    field_0x250 = field_0x90 & 0xFF;
    field_0x258 = (field_0x90 >> 12) & 0xF;
    field_0x254 = (field_0x90 >> 16) & 0xF;
    field_0x25c = (field_0x90 >> 20) & 0xF;
    field_0x254 = 1;
    if (fn_802DEBA0() && field_0x250 == 9) {
        field_0x254 = 0;
    }
    if (field_0x90 & 0x04000000) {
        field_0x260 = true;
    } else {
        field_0x260 = false;
    }
    field_0x244 = 0.0f;
    field_0x248 = 0.0f;
    field_0x24c = 0.0f;
    if (field_0x90 & 0x800) {
        field_0x244 += 512.0f;
    }
    if (field_0x90 & 0x400) {
        field_0x244 -= 512.0f;
    }
    if (field_0x90 & 0x200) {
        field_0x248 += 384.0f;
    }
    if (field_0x90 & 0x100) {
        field_0x248 -= 384.0f;
    }
    field_0x234 = NULL;
    field_0x238 = NULL;
    field_0x23c = NULL;
    if (field_0x25c == 0) {
        switch (field_0x250) {
        case 0: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x188, field_0x254, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos2(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x238 = fn_8013CC50(field_0x4, &pos2, 0x189, field_0x254, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos3(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x23c = fn_8013CC50(field_0x4, &pos3, 0x18a, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 1: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x18b, field_0x254, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos2(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x238 = fn_8013CC50(field_0x4, &pos2, 0x18c, field_0x254, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos3(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x23c = fn_8013CC50(field_0x4, &pos3, 0x18d, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 2: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x552, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 3: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x17e, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 4: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x17f, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 5: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x180, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 6: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x6e, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 7: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0xa9, -1, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos2(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x238 = fn_8013CC50(field_0x4, &pos2, 0xaa, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 8: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x6f, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 9: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x185, field_0x254, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos2(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x238 = fn_8013CC50(field_0x4, &pos2, 0x186, field_0x254, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos3(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x23c = fn_8013CC50(field_0x4, &pos3, 0x187, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        case 10: {
            JGeometry::TVec3f pos(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x234 = fn_8013CC50(field_0x4, &pos, 0x88, -1, 0, NULL, NULL, true, 1.0f);
            JGeometry::TVec3f pos2(mPos.x + field_0x244, mPos.y + field_0x248, 0.0f);
            field_0x238 = fn_8013CC50(field_0x4, &pos2, 0x89, field_0x254, 0, NULL, NULL, true, 1.0f);
            break;
        }
        }
        field_0x240 = 1;
    } else {
        field_0x240 = 0;
    }
    mSound = new ActorSoundObj_804578F4(NULL, NULL, 8);
}

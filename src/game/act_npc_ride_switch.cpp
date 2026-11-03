#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameMath.h"
#include "game/act/act_log.h"
#include "game/act/act_npc_ride_switch.h"

NPCRideSwitch_804AD220::~NPCRideSwitch_804AD220() {}

void NPCRideSwitch_804AD220::draw() {}

// FIXME: 99.1 %. Only the scheduling of the log box's centre differs: the
// target loads 0x60, 0xC, 0x68, 0x64 and adds them in that order; ours loads
// 0x68 first, with registers shuffled the same way.
void NPCRideSwitch_804AD220::update() {
    s32 serial = fn_801F2718(ACTOR_MRTA);
    if (serial >= 0) {
        Log_804ACD70* log = static_cast<Log_804ACD70*>(fn_801F856C()->fn_801F666C(serial));
        if (log != NULL && log->isAtRest() == true) {
            // Pressed: a log at rest lies within 4 units of this actor's centre.
            f32 x0 = log->mBox.i.x + log->mPos.x;
            f32 x1 = log->mBox.f.x + log->mPos.x;
            f32 y0 = log->mBox.i.y + log->mPos.y;
            f32 y1 = log->mBox.f.y + log->mPos.y;
            f32 dx = mPos.x - (x1 + 0.5f * (x0 - x1));
            f32 dy = mPos.y - (y1 + 0.5f * (y0 - y1));
            if (JGeometry::TUtil<f32>::sqrt(square(dx) + square(dy)) < 4.0f) {
                if (!vfunc_0x34()) {
                    vfunc_0x30(0xFF, true);
                }
                return;
            }
        }
    }
    if (vfunc_0x34() == true) {
        vfunc_0x30(0xFF, false);
    }
}

void NPCRideSwitch_804AD220::init() {
    field_0x80.set(-12.0f, -12.0f, 12.0f, 12.0f);
    mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
    mPos.x = 16.0f * ((s32)mPos.x / 16);
    mPos.y = 16.0f * ((s32)mPos.y / 16);
    field_0x230 = 0;
}

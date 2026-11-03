// Actor MRTC ("Log monitor NPC" in docs/names/actors.tsv): the spawner
// (fn_801F9484) allocates 0x238 bytes for it (0x801FF370) and calls its
// constructor fn_801FF780, which stores the vtable 0x804AD278. It watches
// for a log (MRTA) on top of it, or beside it, and when there is none plays
// sound id 3 through fn_801F0D20 and removes itself.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/act/act_log_monitor_npc.h"

// The actor in slot i if it has the given id.
static inline ActorBase_804A5770* getActorWithId(s32 i, u32 id) {
    ActorBase_804A5770* actor = fn_801F856C()->mActors[i];
    if (actor != NULL && actor->mActorId == id) {
        return actor;
    }
    return NULL;
}

// The serial of a log whose box overlaps this actor's and that is not
// raised (z <= 0), or -1.
// FIXME: update() is 90.6 %: the target hoists the first search's
// serial = -1 to the top of update(), checks the slot's actor for NULL a
// second time without a branch for the NULL result, and keeps the boxes at
// 0x34/0x44 (ours 0x28/0x38); otherwise the same.
inline s32 LogMonitorNpc_804AD278::findLog() {
    s32 serial = -1;
    s32 end = fn_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* log = getActorWithId(i, ACTOR_MRTA);
        if (log != NULL) {
            JGeometry::TBox2f box;
            JGeometry::TBox2f logBox;
            fn_801F31C4(&box);
            log->fn_801F31C4(&logBox);
            JGeometry::TBox2f overlap = box;
            if (overlap.intersect(logBox) && log->mPos.z <= 0.0f) {
                serial = log->mSerial;
                break;
            }
        }
    }
    return serial;
}

LogMonitorNpc_804AD278::~LogMonitorNpc_804AD278() {}

void LogMonitorNpc_804AD278::draw() {}

void LogMonitorNpc_804AD278::update() {
    switch (mMode) {
    case 0:
    case 1:
        if (findLog() < 0) {
            fn_801F0D20(3);
            fn_801F3668();
        }
        break;
    case 2:
    case 3: {
        // Look 40 units to either side of the actor.
        JGeometry::TVec3f pos = mPos;
        switch (mMode) {
        case 2:
            mPos.x -= 40.0f;
            break;
        case 3:
            mPos.y -= 40.0f;
            break;
        }
        for (s32 i = 0; i < 2; i++) {
            if (findLog() < 0) {
                fn_801F0D20(3);
                fn_801F3668();
                break;
            }
            switch (mMode) {
            case 2:
                mPos.x += 80.0f;
                break;
            case 3:
                mPos.y += 80.0f;
                break;
            }
        }
        mPos = pos;
        break;
    }
    }
}

void LogMonitorNpc_804AD278::init() {
    mMode = field_0x90 & 0xF;
    switch (mMode) {
    case 0:
    case 2:
    case 3:
        field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
        break;
    case 1:
        field_0x80.set(-16.0f, -16.0f, 16.0f, 16.0f);
        mBox.set(-16.0f, -16.0f, 16.0f, 16.0f);
        break;
    }
    field_0x230 = 0;
}

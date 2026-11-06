#include "game/GameAudio.h"
#include "game/cls_804B20F8.h"
#include "game/GameGlobals.h"
#include "JSystem/JAudio/System/JASTrack.h"
#include "JSystem/JMath/JMath.h"
#include "float.h"

static const f32 lbl_80491680[4] = {1.0f, 1.1f, 0.9f, 0.85f};

cls_804B20F8::cls_804B20F8(s8 kind, JGeometry::TVec3f* pos, JKRHeap* heap, u8 numHandles)
    : ActorSoundObj_804578F4(pos, heap, numHandles), field_0x5c(kind) {}

// FIXME: 95.7%. (1) The original reads the group byte once into r31 and does clrlwi before each
// compare; ours compares r31 directly. (2) The original's sound-less early return jumps straight to
// the epilogue (no mr r3, r30); ours folds it into the nested if. (3) kind is in r4 in the original
// (r0 in ours) and the three pitch constants load in a different order, so the registers of the
// random pitch expression differ.
JAISoundHandle* cls_804B20F8::startSound(JAISoundID id) {
    u8 type = id.mId.mBytes.mGroupId;
    JAISeMgr* seMgr = gAudioMgr_8053AF80->field_0x34;
    switch ((s32)id) {
    case 0x20037:
        startSound(JAISoundID(0x10002));
        break;
    case 0x2001F:
    case 0x20035:
    case 0x20054:
    case 0x20055:
    case 0x20056:
    case 0x2005F:
        seMgr->stopSoundID(JAISoundID(0x10003));
        break;
    case 0x20029:
        startSound(JAISoundID(0x10009));
        break;
    case 0x20032:
        seMgr->stopSoundID(JAISoundID(0x20032));
        break;
    case 0x20049:
        if (getHandleSoundID(id) != NULL) {
            return NULL;
        }
        break;
    case 0x20036:
        break;
    }
    if (type == 1) {
        for (int i = 0; i < numHandles_; i++) {
            JAISound* sound = handles_[i].sound_;
            if (sound != NULL) {
                JAISoundID playing = sound->getID();
                if (playing.mId.mBytes.mGroupId == 1 && playing - 0x10000 != 9 &&
                    playing - 0x10000 != 0xD) {
                    sound->stop(0);
                    break;
                }
            }
        }
    }
    JAISoundHandle* handle = fn_80456334(id, 0);
    if (handle == NULL) {
        return NULL;
    }
    if (handle->sound_ != NULL && (u32)id - 0x20000 != 0x33 && (u32)id - 0x20000 != 0x49) {
        static JMath::TRandom_fast_ lbl_8053AF8C(0);
        s32 kind = field_0x5c;
        if (kind >= 0 && kind < 4) {
            if (type == 1) {
                handle->sound_->getAuxiliary().movePitch(lbl_80491680[kind], 0);
            } else if (type == 2) {
                f32 pitch = 1.0f + (0.03f * lbl_8053AF8C.get_ufloat_1() - 0.015f * (1.0f - FLT_EPSILON));
                handle->sound_->getAuxiliary().movePitch(pitch, 0);
            }
        }
    }
    return handle;
}

// FIXME: 98.6%. The original keeps the id in r4 for the fn_80458F04 call; ours reloads it from the
// stack (one extra lwz r4, 0x10(r1) before that call).
void fn_8045B030(ActorSoundObj_804578F4* obj, u8 index, s8 port9) {
    JAISoundID id = lbl_8052DEE0[index];
    JAISoundHandle* handle = NULL;
    u8 section = id.mId.mBytes.mSectionId;
    u8 group = id.mId.mBytes.mGroupId;
    if (section == 1) {
        if (id == 0x1000002 || id == 0x1000023) {
            gAudioMgr_8053AF80->fn_80458F04(id);
        } else {
            gAudioMgr_8053AF80->fn_80458FF0(id, true, false);
        }
        return;
    }
    if (group == 2 || group == 1) {
        handle = obj->startSound(id);
    } else if (group == 0) {
        gAudioMgr_8053AF80->fn_80458880(id, -1);
    } else {
        handle = obj->fn_80456334(id, 0);
    }
    if (port9 != 0 && handle != NULL && handle->sound_ != NULL) {
        handle->sound_->getTrack()->writePort(9, port9);
    }
}

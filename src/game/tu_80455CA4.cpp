#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "JSystem/JAudio/System/JASTrack.h"
#include "Inagaki/GameSoundTable.h"

// FIXME(shotgun): 92.9 %, extra/missing insns (the original also stores 0x54/0x58 to stack temporaries)
ActorSoundObj_804578F4::ActorSoundObj_804578F4(JGeometry::TVec3f* pos, JKRHeap* heap, u8 numHandles)
    : JAUSoundObject(pos, numHandles, heap) {
    field_0x31 = 1;
    field_0x34 = pos;
    if (pos != NULL) {
        field_0x38.x = pos->x;
        field_0x38.y = pos->y;
        field_0x38.z = pos->z;
        mSoundPos = &field_0x38;
    } else {
        mSoundPos = NULL;
    }
    field_0x44 = 0;
    field_0x45 = 0;
    field_0x50 = 0;
    field_0x51 = 0;
    field_0x52 = 0;
    field_0x4c = 1.0f;
    field_0x48 = 0.0f;
    field_0x54 = 0x400A5;
    field_0x58 = 0x3006D;
    field_0x30 = 0;
}

ActorSoundObj_804578F4::~ActorSoundObj_804578F4() {}

void ActorSoundObj_804578F4::fn_80457840(JGeometry::TVec3f* pos) {
    field_0x34 = pos;
    if (pos != NULL) {
        field_0x38.x = pos->x;
        field_0x38.y = pos->y;
        field_0x38.z = pos->z;
        mSoundPos = &field_0x38;
    } else {
        mSoundPos = NULL;
    }
}

void fn_80455CA4(ActorSoundObj_804578F4* obj, JAISoundHandle* handle, JGeometry::TVec2f* pos, JAISoundID* id);
void fn_8045BA84(const Vec* pos, Vec* out);

// FIXME(shotgun): 76.3 %
JAISoundHandle* ActorSoundObj_804578F4::fn_80456334(JAISoundID id, u32 fade) {
    if (field_0x31 == 0) {
        return NULL;
    }
    if (gAudioMgr_8053AF80->field_0x89 != 0) {
        return NULL;
    }
    GameAudio::CustomSoundTable* soundTable = gAudioMgr_8053AF80->field_0x40;
    u8 group = id.mId.mBytes.mGroupId;
    JAISeMgr* seMgr = gAudioMgr_8053AF80->field_0x34;
    switch ((s32)id) {
    case 0x4003E:
    case 0x40070:
    case 0x50018:
    case 0x50019:
    case 0x5001A:
    case 0x5001B:
    case 0x5001C:
    case 0x5001D:
    case 0x50050:
    case 0x50051:
    case 0x50052:
    case 0x50053:
    case 0x50054:
    case 0x50057:
        seMgr->stopSoundID(JAISoundID(0x4003E));
        seMgr->stopSoundID(JAISoundID(0x40070));
        seMgr->stopSoundID(JAISoundID(0x50018));
        seMgr->stopSoundID(JAISoundID(0x50019));
        seMgr->stopSoundID(JAISoundID(0x5001A));
        seMgr->stopSoundID(JAISoundID(0x5001B));
        seMgr->stopSoundID(JAISoundID(0x5001C));
        seMgr->stopSoundID(JAISoundID(0x5001D));
        seMgr->stopSoundID(JAISoundID(0x50050));
        seMgr->stopSoundID(JAISoundID(0x50051));
        seMgr->stopSoundID(JAISoundID(0x50052));
        seMgr->stopSoundID(JAISoundID(0x50053));
        seMgr->stopSoundID(JAISoundID(0x50054));
        seMgr->stopSoundID(JAISoundID(0x50057));
        break;
    }
    f32 count = 0.0f;
    switch ((s32)id) {
    case 0x4:
    case 0x7:
    case 0x2000B:
    case 0x2000D:
    case 0x2000E:
    case 0x20011:
    case 0x20028:
    case 0x30003:
    case 0x30006:
    case 0x30007:
    case 0x30008:
    case 0x3000A:
    case 0x30010:
    case 0x30015:
    case 0x30016:
    case 0x30026:
    case 0x30034:
    case 0x40000:
    case 0x40001:
    case 0x40002:
    case 0x40003:
    case 0x40009:
    case 0x4000A:
    case 0x40010:
    case 0x400D4:
    case 0x5002F:
    case 0x50036:
    case 0x50037:
    case 0x50038:
    case 0x50039:
    case 0x5004F: {
        JAISeCategoryMgr* cat = seMgr->getCategory(group);
        for (JSULink<JAISe>* link = cat->getSeList()->getFirst(); link != NULL; link = link->getNext()) {
            JAISe* se = link->getObject();
            if (se->getID() == id && se->getCount() == 0) {
                count += 1.0f;
                if ((int)count > cat->getMaxActiveSe()) {
                    return NULL;
                }
            }
        }
        if (count != 0.0f) {
            f32 scale = count / (1.0f + count);
            for (JSULink<JAISe>* link = cat->getSeList()->getFirst(); link != NULL; link = link->getNext()) {
                JAISe* se = link->getObject();
                if (se->getID() == id && se->getCount() == 0) {
                    se->params_.mProperty._0 *= scale;
                }
            }
        }
        break;
    }
    case 0x30001:
        if (gAudioMgr_8053AF80->field_0x6b != 0) {
            return NULL;
        }
        gAudioMgr_8053AF80->field_0x6b = 3;
        break;
    case 0x3002F: {
        JGeometry::TVec3f* pos = field_0x34;
        if (pos != NULL) {
            field_0x38.x = pos->x;
            field_0x38.y = pos->y;
            field_0x38.z = pos->z;
            mSoundPos = &field_0x38;
        } else {
            mSoundPos = NULL;
        }
        gAudioMgr_8053AF80->field_0x75 = 1;
        break;
    }
    case 0x30025:
        seMgr->stopSoundID(JAISoundID(0x30025));
        mSoundPos = NULL;
        break;
    case 0x20023:
        seMgr->stopSoundID(JAISoundID(0x20023));
        mSoundPos = NULL;
        break;
    case 0x30021:
        if (field_0x50 != 0) {
            return NULL;
        }
        field_0x50 = 8;
        break;
    case 0x4003C:
        if (field_0x51 != 0) {
            return NULL;
        }
        field_0x51 = 8;
        break;
    case 0x50006:
        if (field_0x52 == 1) {
            field_0x52 = 0;
            return NULL;
        }
        field_0x52 = 1;
        break;
    case 0x40046:
        seMgr->stopSoundID(JAISoundID(0x40045));
        break;
    case 0x40062:
        seMgr->stopSoundID(JAISoundID(0x40061));
        break;
    case 0x20038:
        seMgr->stopSoundID(JAISoundID(0x3002B));
        break;
    case 0x3002B:
        seMgr->stopSoundID(JAISoundID(0x20038));
        break;
    case 0x40079:
        seMgr->stopSoundID(JAISoundID(0x40078));
        break;
    case 0x400A4:
        if (field_0x54 - 0x40000 == 0xA4) {
            field_0x54 = JAISoundID(0x400A5);
            id = field_0x54;
        } else {
            field_0x54 = JAISoundID(0x400A4);
            id = field_0x54;
        }
        break;
    case 0x400D9:
        switch (gAudioMgr_8053AF80->field_0x70) {
        case 0:
            id = JAISoundID(0x400D9);
            break;
        case 1:
            id = JAISoundID(0x400DA);
            break;
        case 2:
            id = JAISoundID(0x400DB);
            break;
        case 3:
            id = JAISoundID(0x400DC);
            break;
        case 4:
            id = JAISoundID(0x400DD);
            break;
        case 5:
            id = JAISoundID(0x400DE);
            break;
        default:
            id = JAISoundID(0x400D9);
            break;
        }
        gAudioMgr_8053AF80->field_0x70++;
        if (gAudioMgr_8053AF80->field_0x70 > 5) {
            gAudioMgr_8053AF80->field_0x70 = 0;
        }
        break;
    case 0x3005A: {
        static JMath::TRandom_fast_ lbl_8053AF5C(0);
        if (0.01f * lbl_8053AF5C.get_ufloat_1() < 0.005f) {
            startSound(JAISoundID(0x30028));
            return NULL;
        }
        break;
    }
    case 0x50055: {
        static JMath::TRandom_fast_ lbl_8053AF64(0);
        if (0.01f * lbl_8053AF64.get_ufloat_1() < 0.005f) {
            startSound(JAISoundID(0x50056));
            return NULL;
        }
        break;
    }
    case 0x3006A:
        switch ((s32)field_0x58) {
        case 0x3006D:
            field_0x58 = JAISoundID(0x3006A);
            break;
        case 0x3006A:
            startSound(JAISoundID(0x3006B));
            field_0x58 = JAISoundID(0x3006B);
            return NULL;
        case 0x3006B:
            startSound(JAISoundID(0x3006C));
            field_0x58 = JAISoundID(0x3006C);
            return NULL;
        case 0x3006C:
            startSound(JAISoundID(0x3006D));
            field_0x58 = JAISoundID(0x3006D);
            return NULL;
        }
        break;
    case 0x400D0:
        startSound(JAISoundID(0x400CE));
        break;
    case 0x5001F:
        startSound(JAISoundID(0x50020));
        break;
    }
    JGeometry::TVec3f* pos = field_0x34;
    if (pos != NULL) {
        field_0x38.x = pos->x;
        field_0x38.y = pos->y;
        field_0x38.z = pos->z;
        mSoundPos = &field_0x38;
    } else {
        mSoundPos = NULL;
    }
    Vec posVec = field_0x38;
    Vec screenVec;
    fn_8045BA84(&posVec, &screenVec);
    if (mSoundPos != NULL) {
        f32 y = mSoundPos->y;
        mSoundPos->y = mSoundPos->z;
        mSoundPos->z = y;
    }
    if (JASPoolAllocObject<JAISe>::getFreeMemCount() == 0) {
        return NULL;
    }
    JAISoundHandle* handle;
    if (soundTable->getLevelFlag(id)) {
        handle = getHandleSoundID(id);
        if (handle != NULL) {
            (*handle)->updateLifeTime(1);
            if (mSoundPos != NULL) {
                (*handle)->setPos(*mSoundPos);
            }
            JAISound* sound = handle->sound_;
            if (sound != NULL) {
                if (mSoundPos != NULL) {
                    u32 v = id;
                    if (v - 0x30000 == 0x1E || v - 0x30000 == 0x3C || v - 0x30000 == 0x1F ||
                        v - 0x30000 == 0x30 || v - 0x50000 == 0x27 || v - 0x50000 - 0x78 <= 1U ||
                        v - 0x4F <= 1U || v - 0x30000 == 0x74 || v - 0x50000 == 0x3B ||
                        v - 0x30000 == 0x32 || group == 6) {
                        sound->getAuxiliary().movePan(0.5f, 0);
                    } else {
                        JGeometry::TVec2f screenPos;
                        screenPos.x = screenVec.x;
                        screenPos.y = screenVec.y;
                        screenPos.y = screenVec.z;
                        JAISoundID idCopy = v;
                        fn_80455CA4(this, handle, &screenPos, &idCopy);
                    }
                } else {
                    sound->getAuxiliary().movePan(0.5f, 0);
                }
                if (0.0f != count) {
                    handle->sound_->params_.mProperty._0 /= 1.0f + count;
                }
                switch ((s32)id) {
                case 0x400A3:
                    handle->sound_->getAuxiliary().moveVolume(field_0x4c, 0);
                    break;
                case 0x4002F:
                    handle->sound_->getAuxiliary().movePitch(1.0f + field_0x48, 0);
                    break;
                }
            }
            fn_80455F40(handle);
            return handle;
        }
        handle = getFreeHandle();
        if (handle == NULL) {
            handle = getLowPrioSound(id);
        }
        if (handle == NULL) {
            return NULL;
        }
        seMgr->startSound(id, handle, mSoundPos);
        JAISound* sound = handle->sound_;
        if (sound != NULL) {
            if (mSoundPos != NULL) {
                u32 v = id;
                if (v - 0x30000 == 0x1E || v - 0x30000 == 0x3C || v - 0x30000 == 0x1F ||
                    v - 0x30000 == 0x30 || v - 0x50000 == 0x27 || v - 0x50000 - 0x78 <= 1U ||
                    v - 0x4F <= 1U || v - 0x30000 == 0x74 || v - 0x50000 == 0x3B ||
                    v - 0x30000 == 0x32 || group == 6) {
                    sound->getAuxiliary().movePan(0.5f, 0);
                } else {
                    JGeometry::TVec2f screenPos;
                    screenPos.x = screenVec.x;
                    screenPos.y = screenVec.y;
                    screenPos.y = screenVec.z;
                    JAISoundID idCopy = v;
                    fn_80455CA4(this, handle, &screenPos, &idCopy);
                }
            } else {
                sound->getAuxiliary().movePan(0.5f, 0);
            }
            if (0.0f != count) {
                handle->sound_->params_.mProperty._0 /= 1.0f + count;
            }
            switch ((s32)id) {
            case 0x400A3:
                handle->sound_->getAuxiliary().moveVolume(field_0x4c, 0);
                break;
            case 0x3002D:
            case 0x3003E:
            case 0x30073:
            case 0x400C1:
            case 0x5000B:
            case 0x50010: {
                static JMath::TRandom_fast_ lbl_8053AF6C(0);
                f32 pitch = 1.0f + (0.08f * lbl_8053AF6C.get_ufloat_1() - 0.04f * (1.0f - FLT_EPSILON));
                handle->sound_->getAuxiliary().movePitch(pitch, 0);
                break;
            }
            case 0x4002F:
                handle->sound_->getAuxiliary().movePitch(1.0f + field_0x48, 0);
                break;
            }
            (*handle)->setLifeTime(1, true);
            if (handle->sound_ != NULL && fade != 0) {
                (*handle)->fadeIn(fade);
            }
        }
        fn_80455F40(handle);
        return handle;
    } else {
        handle = getHandleSoundID(id);
        if (handle == NULL) {
            handle = getFreeHandle();
        }
        if (handle == NULL) {
            handle = getLowPrioSound(id);
        }
        if (handle == NULL) {
            return NULL;
        }
        seMgr->startSound(id, handle, mSoundPos);
        JAISound* sound = handle->sound_;
        if (sound != NULL) {
            if (mSoundPos != NULL) {
                u32 v = id;
                if (v - 0x30000 == 0x36 || v - 0x30000 == 0x3D || v - 0x30000 == 0x90 ||
                    v - 0x30000 == 0x5C || v - 0x30000 == 0x31 || v - 0x30000 == 0x79 ||
                    v - 0x50000 == 0x57 || v - 0x30000 == 0x86 || v - 0x50000 == 0x76 ||
                    v - 0x30000 == 0x70 || group == 6) {
                    sound->getAuxiliary().movePan(0.5f, 0);
                } else if (v - 0x40000 == 0x57 || v - 0x30000 == 0x3 ||
                           v - 0x30000 - 0x15 <= 1U || v - 0x30000 == 0x61 ||
                           v - 0x40000 == 0x30 || v - 0x30000 == 0x8F ||
                           v - 0x30000 - 0x7 <= 2U || v - 0x40000 - 0x63 <= 1U ||
                           v - 0x40000 == 0x88 || v - 0x50000 == 0x1B || v - 0x50000 == 0x50) {
                    f32 x = screenVec.x;
                    if (x < 0.0f) {
                        sound->getAuxiliary().movePan(0.0f, 0);
                    } else if (x > 1.0f) {
                        sound->getAuxiliary().movePan(1.0f, 0);
                    } else {
                        sound->getAuxiliary().movePan(x, 0);
                    }
                } else {
                    JGeometry::TVec2f screenPos;
                    screenPos.x = screenVec.x;
                    screenPos.y = screenVec.y;
                    screenPos.y = screenVec.z;
                    JAISoundID idCopy = v;
                    fn_80455CA4(this, handle, &screenPos, &idCopy);
                }
            } else {
                sound->getAuxiliary().movePan(0.5f, 0);
            }
            if (0.0f != count) {
                if (count > 20.0f) {
                    handle->sound_->params_.mProperty._0 /= 20.0f;
                } else {
                    handle->sound_->params_.mProperty._0 /= 1.0f + count;
                }
            }
            switch ((s32)id) {
            case 0x30037:
            case 0x40007:
            case 0x4004E:
            case 0x400D8:
            case 0x5000C:
            case 0x50011:
            case 0x5002C:
            case 0x50046:
            case 0x50047:
            case 0x50070: {
                static JMath::TRandom_fast_ lbl_8053AF74(0);
                f32 pitch = 1.0f + (0.08f * lbl_8053AF74.get_ufloat_1() - 0.04f * (1.0f - FLT_EPSILON));
                handle->sound_->getAuxiliary().movePitch(pitch, 0);
                break;
            }
            case 0x4009D:
            case 0x4009E:
                handle->sound_->getAuxiliary().movePitch(1.1f, 0);
                break;
            case 0x4009A:
            case 0x4009B:
                handle->sound_->getAuxiliary().movePitch(1.2f, 0);
                break;
            }
            if (fade != 0) {
                (*handle)->fadeIn(fade);
            }
        }
        u32 v = id;
        if (v - 0x30000 != 0x3 && v - 0x30000 != 0x7 && v - 0x30000 != 0x15 &&
            v - 0x30000 != 0x8 && v - 0x30000 != 0x16 && v - 0x30000 != 0x8F &&
            v - 0x30000 != 0x9) {
            fn_80455F40(handle);
        }
        return handle;
    }
}

// FIXME(shotgun): 96.2 %, missing/extra + reloc-name
void ActorSoundObj_804578F4::fn_804561D8(JAISoundID id, u8 a, u32 fade, u8 c) {
    field_0x44 = c;
    JAISoundHandle* handle = fn_80456334(id, fade);
    JAISound* sound = handle->sound_;
    if (sound != NULL) {
        f32 pan;
        f32 vol;
        switch (a) {
        case 3:
            pan = 0.0f;
            vol = 0.4f;
            break;
        case 2:
            pan = 1.0f;
            vol = 0.4f;
            break;
        case 0:
        case 1:
            pan = 0.5f;
            vol = 0.4f;
            break;
        default:
            pan = 0.5f;
            vol = 0.0f;
            break;
        }
        sound->getAuxiliary().movePan(pan, 0);
        if (c != 0) {
            handle->sound_->getAuxiliary().moveVolume(1.0f, 0);
        } else {
            handle->sound_->getAuxiliary().moveVolume(vol, 0);
        }
        if (field_0x44 != 1) {
            if (gAudioMgr_8053AF80->field_0x6 == 1) {
                handle->sound_->getAuxiliary().moveVolume(0.35f, 0);
            } else {
                handle->sound_->getAuxiliary().moveVolume(vol, 0);
            }
        }
    }
}

void ActorSoundObj_804578F4::fn_80456154() {
    JGeometry::TVec3f* pos = field_0x34;
    if (pos != NULL) {
        field_0x38.x = pos->x;
        field_0x38.y = pos->y;
        field_0x38.z = pos->z;
        mSoundPos = &field_0x38;
    } else {
        mSoundPos = NULL;
    }
    if (mSoundPos != NULL) {
        f32 y = mSoundPos->y;
        mSoundPos->y = mSoundPos->z;
        mSoundPos->z = y;
    }
    if (field_0x50 != 0) {
        field_0x50--;
    }
    if (field_0x51 != 0) {
        field_0x51--;
    }
}

void ActorSoundObj_804578F4::fn_80456120() {
    field_0x31 = 0;
    dispose();
}

// A sound may stop unless its sound-table switch bits have 0x8000 set.
// FIXME(shotgun): 89.0 %, scheduling+regalloc
bool ActorSoundObj_804578F4::stopOK(JAISoundHandle& handle) {
    u32 bits = gAudioMgr_8053AF80->field_0x40->getSwBit(handle->getID());
    return !(bits & 0x8000);
}

void ActorSoundObj_804578F4::fn_80455F40(JAISoundHandle* handle) {
    if (handle == NULL) {
        return;
    }
    JAISound* sound = handle->sound_;
    if (sound == NULL) {
        return;
    }
    JASTrack* track = sound->getTrack();
    if (field_0x44 == 1) {
        track->writePort(0xA, 1);
        switch (field_0x45) {
        case 2:
            track->writePort(0x9, 0x78);
            track->writePort(0xB, 0);
            break;
        case 3:
            track->writePort(0xB, 1);
            track->writePort(0x9, 0x3C);
            break;
        // Case 1 has the default's body; the empty label is what makes the
        // switch's compare tree match.
        case 1:
        default:
            track->writePort(0x9, 0);
            track->writePort(0xB, 0);
            break;
        }
    } else {
        track->writePort(0xA, 0);
        switch (field_0x45) {
        case 2:
            track->writePort(0x9, 0x78);
            track->writePort(0xB, 0);
            break;
        case 3:
            track->writePort(0xB, 1);
            track->writePort(0x9, 0x3C);
            break;
        // Case 1 has the default's body; the empty label is what makes the
        // switch's compare tree match.
        case 1:
        default:
            track->writePort(0x9, 0);
            track->writePort(0xB, 0);
            break;
        }
    }
}

// fn_80455CA4: pans and sets the volume of a started sound from a screen position
// (pos x, y in 0..1).
// FIXME(shotgun): 96.3 %, missing/extra
void fn_80455CA4(ActorSoundObj_804578F4* obj, JAISoundHandle* handle, JGeometry::TVec2f* pos, JAISoundID* id) {
    f32 x = pos->x;
    if (x < 0.0f) {
        handle->sound_->getAuxiliary().movePan(0.0f, 0);
        handle->sound_->getAuxiliary().moveVolume(0.4f, 0);
    } else if (x > 1.0f) {
        handle->sound_->getAuxiliary().movePan(1.0f, 0);
        handle->sound_->getAuxiliary().moveVolume(0.4f, 0);
    } else {
        f32 y = pos->y;
        if (y < 0.0f) {
            handle->sound_->getAuxiliary().movePan(x, 0);
            handle->sound_->getAuxiliary().moveVolume(0.4f, 0);
        } else if (y > 1.0f) {
            handle->sound_->getAuxiliary().movePan(x, 0);
            handle->sound_->getAuxiliary().moveVolume(0.4f, 0);
        } else if (x < 0.1f || y < 0.1f) {
            f32 m = x <= y ? x : y;
            f32 vol = 10.0f * m + 0.4f;
            if (vol > 1.0f) {
                vol = 1.0f;
            }
            handle->sound_->getAuxiliary().moveVolume(vol, 0);
            handle->sound_->getAuxiliary().movePan(pos->x, 0);
        } else if (x > 0.9f || y > 0.9f) {
            f32 m = x >= y ? x : y;
            f32 vol = 10.0f * (1.0f - m) + 0.4f;
            if (vol > 1.0f) {
                vol = 1.0f;
            }
            handle->sound_->getAuxiliary().moveVolume(vol, 0);
            handle->sound_->getAuxiliary().movePan(pos->x, 0);
        } else {
            handle->sound_->getAuxiliary().movePan(x, 0);
            if (id->mId.mBytes.mGroupId != 0) {
                u32 v = *id;
                if (v - 0x30000 != 9 && v - 0x30000 != 0x51 && v - 0x30000 != 0x4B && v - 0x50000 != 0x6D) {
                    if (obj->field_0x44 != 1) {
                        if (gAudioMgr_8053AF80->field_0x6 == 1) {
                            handle->sound_->getAuxiliary().moveVolume(0.35f, 0);
                        } else {
                            handle->sound_->getAuxiliary().moveVolume(1.0f, 0);
                        }
                    }
                }
            }
        }
    }
}

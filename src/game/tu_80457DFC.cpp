#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "JSystem/JAudio/JAUAudioMgr.h"
#include "JSystem/JAudio/JAUSectionHeap.h"
#include "JSystem/JAudio/System/JASAudioReseter.h"
#include "JSystem/JAudio/System/JASWaveArcLoader.h"
#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JAudio/System/JASCriticalSection.h"
#include "JSystem/JAudio/System/JASTrack.h"
#include "JSystem/JAudio/Interface/JAISeq.h"
#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/JAUSoundMgr.h"
#include "JSystem/JAudio/JAUSoundObject.h"

// Tuning values (.sdata, in address order).
static u32 lbl_8053A220 = 0x78;
static u32 lbl_8053A224 = 0xA;
static u32 lbl_8053A228 = 0xC8;
static f32 lbl_8053A22C = 16670.0f;
static f32 lbl_8053A230 = 333.0f;
static u8 lbl_8053A234 = 0x1E;
static f32 lbl_8053A238 = -100.0f;
static f32 lbl_8053A23C = 500.0f;
static f32 lbl_8053A240 = 1.0f;
static const char* lbl_8053A244 = "GC4Sword_usa/AudioRes/JaiInit.baa";
static const char* lbl_8053A248 = "GC4Sword_usa/AudioRes/Waves/";
static const char* lbl_8053A24C = "GC4Sword_usa/AudioRes/Seqs/JaiSeqs.arc";
static u32 lbl_8053A250 = 5;
static u32 lbl_8053A254 = 0x14;
static u32 lbl_8053A258 = 5; // the music / second sequence volume fade (fn_80458744)
static u32 lbl_8053A25C = 0x5A; // the music fade-out when another is queued (fn_80458FF0)
static f32 lbl_8053A260 = 1.3f; // fast tempo (fn_80458508 kind 1)
static u32 lbl_8053A264 = 0x1E; // its frames
static f32 lbl_8053A268 = 0.7f; // slow tempo (fn_80458508 kind 2)
static u32 lbl_8053A26C = 5; // its frames

// Moves a sequence's tempo to `tempo` over `frames` (at once for 0); the
// inline every tempo change here goes through.
static inline void moveTempo(JAITempoMgr* tempoMgr, f32 tempo, u32 frames) {
    if (frames == 0) {
        tempoMgr->setTempo(tempo);
    } else {
        tempoMgr->mTransition.set(tempo, tempoMgr->mTempo, frames);
    }
}

// The bodies of fn_80458508 and fn_80458F04. Both are inlined into functions
// that come before their out-of-line copies (fn_80457E54, fn_8045815C;
// fn_80458880), which MWCC does only for an inline defined earlier, while
// other units call the copies (18 and 10 calls): so the original defined them
// inline ahead of their users, and the copies at their addresses call them,
// as Player_804A6F38::fn_802413F0 does playerFlags_802413F0.

// The music tempo: lbl_8053A260 (kind 1) or lbl_8053A268 (kind 2) over their
// frame counts, else 1.0 at once.
inline void AudioMgr_8045A080::moveMusicTempo_80458508(u8 kind) {
    if (field_0x80) {
        JAITempoMgr* tempoMgr = field_0x80->getTempoMgr();
        if (tempoMgr != NULL) {
            switch (kind) {
            case 1:
                moveTempo(tempoMgr, lbl_8053A260, lbl_8053A264);
                break;
            case 2:
                moveTempo(tempoMgr, lbl_8053A268, lbl_8053A26C);
                break;
            default:
                tempoMgr->setTempo(1.0f);
                break;
            }
        }
    }
}

// Stops sounds 0x13-0x1C of `object`; fn_80458880 does it before every system
// sound it starts. An inline of its own: its ten sound-ID temporaries take the
// lowest stack slots, below fn_80458880's own, as an inlined body's do.
static inline void stopSounds13To1C(JAUSoundObject* object) {
    object->stopSound(0x13, 0);
    object->stopSound(0x14, 0);
    object->stopSound(0x15, 0);
    object->stopSound(0x16, 0);
    object->stopSound(0x17, 0);
    object->stopSound(0x18, 0);
    object->stopSound(0x19, 0);
    object->stopSound(0x1A, 0);
    object->stopSound(0x1B, 0);
    object->stopSound(0x1C, 0);
}

// The pan of player `player`'s system sounds: left (0.2) for players 0 and 2,
// right (0.8) for 1 and 3 in a 2-4 player game, else the centre. Inlined into
// fn_80458880, which tests player < 0 a second time for it.
inline f32 AudioMgr_8045A080::getPlayerPan(s8 player) {
    if (player < 0) {
        return 0.5f;
    }
    if (player >= field_0x5) {
        return 0.5f;
    }
    f32 pan = 0.5f;
    switch (field_0x5) {
    case 1:
        pan = 0.5f;
        break;
    case 2:
    case 3:
    case 4:
        switch (player) {
        case 0:
        case 2:
            pan = 0.2f;
            break;
        case 1:
        case 3:
            pan = 0.8f;
            break;
        }
        break;
    }
    return pan;
}

// Starts the second sequence `id` in the handle at 0x84 at volume field_0x50
// (not while field_0x4 is set, nor again in a one-player game).
inline void AudioMgr_8045A080::startSecondSeq_80458F04(u32 id) {
    if (field_0x4 != 0) {
        return;
    }
    if (field_0x5 == 1 && field_0x84 && id == field_0x84->getID()) {
        return;
    }
    field_0x30->startSound(id, &field_0x84, NULL);
    if (field_0x84) {
        field_0x84->getAuxiliary().moveVolume(field_0x50, 0);
    }
}

// 0x8045A0D8
u8 fn_8045A0D8() {
    return gGameRoot_8053AAF8->field_0x96;
}

// 0x8045A080
AudioMgr_8045A080::AudioMgr_8045A080() {
    gAudioMgr_8053AF80 = this;
    field_0x30 = NULL;
    field_0x34 = NULL;
    field_0x38 = NULL;
    field_0x0 = 0;
    field_0x6b = 0;
    field_0x70 = 0;
    field_0x6c = 0;
    field_0x6d = 0;
    field_0x74 = 0;
    field_0x4 = 0;
    field_0x5 = 1;
    field_0x6 = 0;
    field_0x8.set(0.0f, 0.0f, 0.0f);
}

// 0x80459624
// FIXME: 79.35. The original loads and stores the three coordinates one at a time
// (lfs/stfs through f0, the mtx pointer moved in between); ours loads x and z
// (f0, f1) before the first store and the register assignment follows.
void AudioMgr_8045A080::fn_80459624(const JGeometry::TVec3f* pos, Mtx mtx, s32 index) {
    field_0x8.set(pos->x, pos->z, pos->y);
    field_0x3c->setAudienceMtx(mtx, index, false);
}

// 0x804595E4
void AudioMgr_8045A080::fn_804595E4(bool flag) {
    field_0x2c->start(5, flag);
    field_0x7c = 1;
}

// 0x8045959C
int AudioMgr_8045A080::fn_8045959C() {
    if (field_0x7c == 0) {
        return 0;
    }
    return (field_0x2c->checkDone() != 0) + 1;
}

// 0x80459508
void AudioMgr_8045A080::fn_80459508() {
    field_0x30->stop();
    field_0x4c = 1.0f;
    field_0x50 = 1.0f;
    field_0x54 = 1.0f;
    field_0x58 = 0;
    field_0x5c = 0;
    field_0x60 = 0;
    field_0x64 = -1;
    field_0x68 = 0;
    field_0x89 = 0;
    field_0x69 = 0;
    field_0x7c = 0;
    field_0x6a = 0;
    field_0x88 = 0;
    field_0x8a = 0;
    field_0x8b = 0;
    field_0x8c = 0;
    field_0x8d = 0;
    field_0x8e = 0;
    field_0x75 = 0;
    field_0x78 = 0;
    field_0x2c->resume();
}

// 0x804594E4
void AudioMgr_8045A080::fn_804594E4(u32 mode) {
    JASDriver::setOutputMode(mode);
}

// 0x80458FF0
// FIXME: 99.91, reloc-name only: jumptable_804B1FB4, as fn_80458744's.
// Inlines fn_80458408 (case 0x100001A).
void AudioMgr_8045A080::fn_80458FF0(s32 id, bool restart, u8 flag) {
    if (field_0x4 != 0) {
        return;
    }
    if (id == 0xFFFFFFFF) {
        return;
    }
    switch (id) {
    case 0x1000028:
        field_0x89 = 1;
    case 0x1000019:
        restart = true;
        break;
    case 0x100001C:
    case 0x100000C:
        field_0x70 = 0;
        break;
    }
    if (field_0x68 == 1 && !restart && field_0x80) {
        if (id == field_0x58 && field_0x64 == -1) {
            return;
        }
        if (id == field_0x5c) {
            return;
        }
        if (field_0x80) {
            field_0x80->stop(lbl_8053A25C);
            field_0x64 = 0;
        }
        field_0x5c = id;
        field_0x60 = id;
        field_0x8d = flag;
    } else {
        field_0x30->startSound(id, &field_0x80, NULL);
        fn_8045815C(field_0x8b);
        switch (field_0x5c) {
        case 0x100001A:
            fn_80458408(field_0x5c, false);
            break;
        case 0x100000D:
        case 0x100001C:
        case 0x1000021:
        case 0x1000024:
            field_0x4c = 1.0f;
            break;
        }
        if (field_0x80) {
            field_0x80->getAuxiliary().moveVolume(field_0x4c * field_0x54, 0);
            if (flag) {
                field_0x80->getTrack()->writePort(0, 1);
            }
        }
        field_0x58 = id;
        field_0x60 = id;
        field_0x5c = 0;
        field_0x64 = -1;
    }
    field_0x68 = 1;
}

// 0x80458F9C
void AudioMgr_8045A080::fn_80458F9C(s32 frames) {
    if (field_0x80) {
        field_0x80->stop(frames);
    }
    field_0x68 = 0;
    field_0x64 = -1;
    field_0x58 = 0;
    field_0x5c = 0;
    field_0x60 = 0;
}

// 0x80458F04
void AudioMgr_8045A080::fn_80458F04(u32 id) {
    startSecondSeq_80458F04(id);
}

// 0x80458880
// FIXME: 99.66. One instruction short in the inlined getPlayerPan: for 1 or
// 5+ players the original reloads 0.5f (lfs) before the movePan call; ours
// keeps the 0.5f already in f1 (b only). Returns, an uninitialised pan and a
// shared case 1 / default were tried (97.4-99.2 %). Also reloc-name: the
// .sdata2 literals 0.5 / 0.2 / 0.8 come after 0.7, first used by the
// unwritten fn_80459674.
void AudioMgr_8045A080::fn_80458880(u32 id, s8 player) {
    if (player >= 4) {
        return;
    }
    if (id == 0x48 && field_0x6a == 0) {
        field_0x6a = 1;
    }
    if (id == 0xC) {
        field_0x34->stop(1);
    }
    // Sounds played on the shared object at 0x14 whatever the player.
    switch (id) {
    case 0x00: case 0x01: case 0x02: case 0x03:
    case 0x08:
    case 0x0B: case 0x0C: case 0x0D: case 0x0E: case 0x0F: case 0x10: case 0x11:
    case 0x1D:
    case 0x1F: case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25:
    case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C:
    case 0x2E: case 0x2F: case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35:
    case 0x36: case 0x37:
    case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3E: case 0x3F: case 0x40:
    case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47: case 0x48:
    case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D:
    case 0x4F: case 0x50:
    case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: case 0x58: case 0x59:
    case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F: case 0x60: case 0x61:
    case 0x62:
        player = -1;
        break;
    }
    switch (id) {
    case 0xA:
        if (field_0x6c != 0) {
            return;
        }
        field_0x6c = 3;
        break;
    case 0x3:
        if (field_0x74 != 0) {
            return;
        }
        if (field_0x6d != 0) {
            return;
        }
        if (field_0x14->getHandleSoundID(3) != NULL) {
            return;
        }
        field_0x14->stopSound(0x42, 0);
        field_0x14->stopSound(0x41, 0);
        break;
    case 0x42:
        if (field_0x74 != 0) {
            return;
        }
        if (field_0x6d != 0) {
            return;
        }
        if (field_0x14->getHandleSoundID(3) != NULL) {
            return;
        }
        if (field_0x14->getHandleSoundID(0x41) != NULL) {
            return;
        }
        if (field_0x14->getHandleSoundID(0x42) != NULL) {
            return;
        }
        break;
    case 0x41:
        if (field_0x14->getHandleSoundID(3) != NULL) {
            return;
        }
        if (field_0x14->getHandleSoundID(0x41) != NULL) {
            return;
        }
        field_0x14->stopSound(0x42, 0);
        break;
    case 0x38:
    case 0x57:
        startSecondSeq_80458F04(id);
        return;
    }
    if (player >= 0) {
        if (id != 0xE && id != 0x1F && id != 0x36 && id != 0x54 && id != 0x55 && id != 0x4F &&
            id != 0x50) {
            field_0x18[player]->stopSound(id, 0);
        }
        stopSounds13To1C(field_0x18[player]);
        JAISoundHandle* handle = field_0x18[player]->startSound(id);
        if (handle != NULL && *handle) {
            (*handle)->getAuxiliary().movePan(getPlayerPan(player), 0);
        }
    } else {
        if (id != 0xE && id != 0x1F && id != 0x36 && id != 0x54 && id != 0x55 && id != 0x4F &&
            id != 0x50) {
            field_0x14->stopSound(id, 0);
        }
        stopSounds13To1C(field_0x14);
        field_0x14->startSound(id);
    }
}

// 0x80458744
// The music volumes per mode: music, second sequence, SE category 0, then six
// values no function here reads. They lead the original .data, ahead of this
// unit's jump tables, and the original addresses each table on its own
// (lis / lfsu): an extern. Under deferred codegen a table defined anywhere in
// this file is addressed off one ...data.0 base, so they are defined in the
// preceding unit, game/tu_80457C08.cpp.
extern f32 lbl_804B1E60[9];
extern f32 lbl_804B1E84[9];
extern f32 lbl_804B1EA8[9];
extern f32 lbl_804B1ECC[9];

// Not auto-inlined: fn_80457E04 and fn_80457E54 call fn_80458744 out of line in the original
// (so do the unwritten fn_80459274 / fn_804593D4; docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void AudioMgr_8045A080::fn_80458744(s32 mode) {
    f32 musicVolume;
    f32 seqVolume;
    f32 seVolume;
    switch (mode) {
    case 0:
        musicVolume = lbl_804B1E60[0];
        seqVolume = lbl_804B1E60[1];
        seVolume = lbl_804B1E60[2];
        break;
    case 1:
        musicVolume = lbl_804B1E84[0];
        seqVolume = lbl_804B1E84[1];
        seVolume = lbl_804B1E84[2];
        break;
    case 2:
        musicVolume = lbl_804B1EA8[0];
        seqVolume = lbl_804B1EA8[1];
        seVolume = lbl_804B1EA8[2];
        break;
    case 3:
        switch (field_0x58) {
        case 0x1000000:
        case 0x1000003:
        case 0x1000008:
        case 0x1000009:
        case 0x100000A:
        case 0x100000B:
        case 0x100000E:
        case 0x1000010:
        case 0x1000011:
        case 0x1000012:
        case 0x1000013:
        case 0x1000014:
        case 0x1000015:
        case 0x1000016:
        case 0x1000018:
        case 0x1000029:
        case 0x100002E:
        case 0x1000030:
            musicVolume = lbl_804B1ECC[0];
            seqVolume = lbl_804B1ECC[1];
            seVolume = lbl_804B1ECC[2];
            break;
        default:
            return;
        }
        break;
    default:
        return;
    }
    field_0x4c = musicVolume;
    field_0x50 = seqVolume;
    if (field_0x80) {
        field_0x80->getAuxiliary().moveVolume(field_0x4c * field_0x54, lbl_8053A258);
    }
    if (field_0x84) {
        field_0x84->getAuxiliary().moveVolume(field_0x50, lbl_8053A258);
    }
    field_0x34->getCategory(0)->getParams()->moveVolume(seVolume, 0);
}
#pragma auto_inline reset

// 0x8045873C
void AudioMgr_8045A080::fn_8045873C(u8 value) {
    field_0x69 = value;
}

// 0x80458658
// FIXME: 99.65, regalloc only: the original keeps this in r30 and pause in
// r31 (mr r30, r3 first); ours copies pause first.
void AudioMgr_8045A080::fn_80458658(bool pause) {
    field_0x8a = pause;
    field_0x34->getCategory(3)->pause(pause);
    field_0x34->getCategory(4)->pause(pause);
    field_0x34->getCategory(5)->pause(pause);
    field_0x34->getCategory(6)->pause(pause);
    f32 volume = 1.0f;
    if (pause) {
        volume = 0.0f;
    }
    field_0x34->getCategory(3)->getParams()->moveVolume(volume, 3);
    field_0x34->getCategory(4)->getParams()->moveVolume(volume, 3);
    field_0x34->getCategory(5)->getParams()->moveVolume(volume, 3);
    field_0x34->getCategory(6)->getParams()->moveVolume(volume, 3);
}

// 0x80458508
// FIXME: 98.55. The switch tree (in moveMusicTempo_80458508) differs: the
// original tests kind == 1 first (cmpwi 1; beq; bge; b default; cmpwi 3; bge
// default), ours tests 2 first; two branches more here.
void AudioMgr_8045A080::fn_80458508(u8 kind) {
    moveMusicTempo_80458508(kind);
}

// 0x80458408
void AudioMgr_8045A080::fn_80458408(s32 id, bool flag) {
    if (field_0x80) {
        switch (id) {
        case 0x100001A: {
            JAISeq* seq = field_0x80->asSeq();
            if (seq != NULL) {
                JASCriticalSection cs;
                for (int i = 0; i < 16; i++) {
                    JASTrack* track = seq->getChildTrack(i);
                    if (track != NULL) {
                        if (flag) {
                            if (i == 2) {
                                track->mute(true);
                            } else {
                                track->mute(false);
                            }
                        } else if (i == 2 || i == 7) {
                            track->mute(false);
                        } else {
                            track->mute(true);
                        }
                    }
                }
            }
            break;
        }
        }
    }
}

// 0x8045815C
// FIXME: 99.97, reloc-name only: 0.5f (movePitch) is lbl_80543EC4, pooled after
// 0.7f, which only the unwritten fn_80459674 uses.
void AudioMgr_8045A080::fn_8045815C(bool flag) {
    field_0x8b = flag;
    if (!field_0x80 && !field_0x84) {
        return;
    }
    JAISoundID id = 0xFFFFFFFF;
    if (field_0x80) {
        id = field_0x80->getID();
    }
    if (field_0x84) {
        id = field_0x84->getID();
    }
    if (id == 0x1000002 || id == 0x1000023 || id == 0x100002D || id == 0x1000001) {
        return;
    }
    if (field_0x80) {
        id = field_0x80->getID();
    }
    switch (id) {
    case 0x100000D:
    case 0x1000004:
        return;
    }
    if (flag) {
        moveMusicTempo_80458508(2);
    } else {
        moveMusicTempo_80458508(0);
    }
    if (flag) {
        field_0x80->getAuxiliary().movePitch(0.5f, 0);
    } else {
        field_0x80->getAuxiliary().movePitch(1.0f, 0);
    }
    if (flag) {
        field_0x80->getAuxiliary().moveFxMix(1.0f, 0);
    } else {
        field_0x80->getAuxiliary().moveFxMix(0.0f, 0);
    }
    JAISeq* seq;
    if (field_0x84) {
        seq = field_0x84->asSeq();
    } else {
        seq = field_0x80->asSeq();
    }
    if (seq != NULL) {
        JASCriticalSection cs;
        for (int i = 0; i < 16; i++) {
            JASTrack* track = seq->getChildTrack(i);
            if (track != NULL) {
                track->writePort(0, flag);
            }
        }
        field_0x8b = flag;
    }
}

// 0x804580DC
bool AudioMgr_8045A080::fn_804580DC(u8 index) {
    if (field_0x44 != NULL) {
        JASWaveBank* bank = field_0x44->getWaveBankTable().getWaveBank(index);
        if (bank != NULL && bank->getWaveArc(0)->_4c == 2) {
            return true;
        }
    }
    return false;
}

// 0x80458090
void AudioMgr_8045A080::fn_80458090() {
    field_0x44->loadWaveArc(2);
    field_0x44->loadWaveArc(3);
    field_0x44->loadWaveArc(4);
}

// 0x80457F80
bool AudioMgr_8045A080::fn_80457F80(JAISoundID id, s8 player) {
    if (player == -2) {
        if (field_0x14->getHandleSoundID(id) != NULL) {
            return false;
        }
        for (int i = 0; i < 4; i++) {
            if (field_0x18[i]->getHandleSoundID(id) != NULL) {
                return false;
            }
        }
    } else if (player == -1) {
        if (field_0x14->getHandleSoundID(id) != NULL) {
            return false;
        }
    } else {
        for (int i = 0; i < 4; i++) {
            if (field_0x18[i]->getHandleSoundID(id) != NULL) {
                return false;
            }
        }
    }
    return true;
}

// 0x80457F6C
void AudioMgr_8045A080::fn_80457F6C() {
    field_0x74 = 0;
    field_0x6d = 0xB4;
}

// 0x80457F60
void AudioMgr_8045A080::fn_80457F60() {
    field_0x74 = 1;
}

// 0x80457E54
void AudioMgr_8045A080::fn_80457E54(bool flag) {
    field_0x8c = flag;
    if (flag) {
        fn_80458744(2);
        moveMusicTempo_80458508(2);
    } else {
        fn_80458744(0);
        moveMusicTempo_80458508(0);
    }
}

// 0x80457E04
void AudioMgr_8045A080::fn_80457E04(bool flag) {
    if (field_0x8c == 0 && field_0x8b == 0) {
        if (flag) {
            fn_80458744(3);
        } else {
            fn_80458744(0);
        }
    }
}

// 0x80457DFC
void AudioMgr_8045A080::fn_80457DFC(u8 value) {
    field_0x6 = value;
}

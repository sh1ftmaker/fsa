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

// 0x80457DFC
void AudioMgr_8045A080::fn_80457DFC(u8 value) {
    field_0x6 = value;
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

// 0x80457F60
void AudioMgr_8045A080::fn_80457F60() {
    field_0x74 = 1;
}

// 0x80457F6C
void AudioMgr_8045A080::fn_80457F6C() {
    field_0x74 = 0;
    field_0x6d = 0xB4;
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

// 0x80458090
void AudioMgr_8045A080::fn_80458090() {
    field_0x44->loadWaveArc(2);
    field_0x44->loadWaveArc(3);
    field_0x44->loadWaveArc(4);
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

// 0x80458508
// FIXME: 98.55. The switch tree differs: the original tests kind == 1 first
// (cmpwi 1; beq; bge; b default; cmpwi 3; bge default), ours tests 2 first;
// two branches more here.
void AudioMgr_8045A080::fn_80458508(u8 kind) {
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

// 0x8045873C
void AudioMgr_8045A080::fn_8045873C(u8 value) {
    field_0x69 = value;
}

// 0x80458744
// FIXME: 99.81, reloc-name only: the jump table (jumptable_804B1EF0) sits after
// the volume tables in the original .data. Defined above the function, the
// tables are addressed off one ...data.0 base (85 %); defined at the end of
// the file (the original's separate lis/lfsu per table) they come after the
// jump tables. Not inlined by fn_80459274 / fn_804593D4 in the original,
// which is why those two are not written yet.
extern f32 lbl_804B1E60[9];
extern f32 lbl_804B1E84[9];
extern f32 lbl_804B1EA8[9];
extern f32 lbl_804B1ECC[9];


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

// 0x80458F04
void AudioMgr_8045A080::fn_80458F04(u32 id) {
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

// 0x804594E4
void AudioMgr_8045A080::fn_804594E4(u32 mode) {
    JASDriver::setOutputMode(mode);
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

// 0x8045959C
int AudioMgr_8045A080::fn_8045959C() {
    if (field_0x7c == 0) {
        return 0;
    }
    return (field_0x2c->checkDone() != 0) + 1;
}

// 0x804595E4
void AudioMgr_8045A080::fn_804595E4(bool flag) {
    field_0x2c->start(5, flag);
    field_0x7c = 1;
}

// 0x80459624
// FIXME: 79.35. The original loads and stores the three coordinates one at a time
// (lfs/stfs through f0, the mtx pointer moved in between); ours loads x and z
// (f0, f1) before the first store and the register assignment follows.
void AudioMgr_8045A080::fn_80459624(const JGeometry::TVec3f* pos, Mtx mtx, s32 index) {
    field_0x8.set(pos->x, pos->z, pos->y);
    field_0x3c->setAudienceMtx(mtx, index, false);
}

// 0x8045A080
AudioMgr_8045A080::AudioMgr_8045A080() {
    lbl_8053AF80 = this;
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

// 0x8045A0D8
u8 fn_8045A0D8() {
    return lbl_8053AAF8->field_0x96;
}

// The music volumes per mode (fn_80458744): music, second sequence, SE
// category 0, then six values no function here reads. Defined after their
// user: the original addresses each table on its own (lis/lfsu per table),
// which MWCC does only for data defined later (see fn_80458744's FIXME).
f32 lbl_804B1E60[9] = {1.0f, 1.0f, 0.7f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f};
f32 lbl_804B1E84[9] = {0.0f, 0.0f, 0.7f, 0.441f, 0.441f, 0.441f, 0.441f, 0.441f, 0.441f};
f32 lbl_804B1EA8[9] = {0.15f, 1.0f, 0.7f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f};
f32 lbl_804B1ECC[9] = {0.3f, 1.0f, 0.7f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f};

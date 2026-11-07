#ifndef GAME_UNK_8045A080_H
#define GAME_UNK_8045A080_H

// The game's audio manager (provisional name after its constructor,
// 0x8045A080; game/tu_80457DFC.cpp). One 0x90-byte instance, built by main
// in the main heap and stored in gAudioMgr_8053AF80 (and GameState field_0xc).
// fn_80459AE4 sets JAudio up from "GC4Sword_usa/AudioRes/JaiInit.baa",
// ".../Waves/" and ".../Seqs/JaiSeqs.arc" (the unit's .sdata string
// pointers) and owns the sound manager, the SE / sequence managers, the
// audience and the section heap. Its members play system sounds
// (fn_80458880, 393 calls), start and stop the background music (the
// sequence handle at 0x80: fn_80458FF0, fn_80458F9C), the second sequence
// at 0x84 (fn_80458F04), and pause / fade the SE categories (fn_80458658,
// fn_80459274, fn_804593D4). Its role matches Twilight Princess's
// Z2AudioMgr (a structural reference only; the layout is FSA's).
//
// Included at the end of game/GameAudio.h so that every unit that includes
// the game's sound header sees it; game/GameGlobals.h only forward-declares
// it and declares gAudioMgr_8053AF80.

#include "types.h"
#include "dolphin/mtx.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JAudio/Interface/JAISound.h"

class JAISeMgr;
class JAISeqMgr;
struct JASAudioReseter;
class JAUSoundMgr;
class JAUSectionHeap;
class JAUSoundObject;
class JAUDopplerAudienceState;
class JAUAudience_withSetting_doppler;
template <int N> class JAUDopplerAudible;
template <int N, class Audible, class State, class Audience> class JAUAudience_;
class JKRSolidHeap;

struct AudioMgr_8045A080 {
    /* 0x00 */ u32 field_0x0;
    /* 0x04 */ u8 field_0x4; // set: no music / second sequence starts (fn_80458FF0, fn_80458F04)
    /* 0x05 */ u8 field_0x5; // the number of players: system sounds are panned by player when 2-4
    /* 0x06 */ u8 field_0x6;
    /* 0x07 */ u8 field_0x7;
    /* 0x08 */ JGeometry::TVec3f field_0x8;
    /* 0x14 */ JAUSoundObject* field_0x14;    // system sounds (fn_80458880 with player < 0)
    /* 0x18 */ JAUSoundObject* field_0x18[4]; // per-player system sounds
    /* 0x28 */ u8 field_0x28[0x2C - 0x28];
    /* 0x2C */ JASAudioReseter* field_0x2c; // fn_80459AE4 news it
    /* 0x30 */ JAUSoundMgr* field_0x30; // fn_80459AE4 news it; starts the sequences (JAISoundStarter)
    /* 0x34 */ JAISeMgr* field_0x34; // its categories 0-6 are paused / faded here
    /* 0x38 */ JAISeqMgr* field_0x38; // fn_80459AE4: field_0x30 + 0x728
    /* 0x3C */ JAUAudience_<4, JAUDopplerAudible<4>, JAUDopplerAudienceState, JAUAudience_withSetting_doppler>* field_0x3c; // fn_80459AE4 news it
    /* 0x40 */ u8 field_0x40[0x44 - 0x40];
    /* 0x44 */ JAUSectionHeap* field_0x44; // fn_80458090 loads wave archives of it
    /* 0x48 */ u8 field_0x48[0x4C - 0x48];
    /* 0x4C */ f32 field_0x4c; // music volume (fn_80458744 tables), times field_0x54
    /* 0x50 */ f32 field_0x50; // second sequence volume (fn_80458744 tables)
    /* 0x54 */ f32 field_0x54; // music volume factor
    /* 0x58 */ u32 field_0x58; // the music id last started (fn_80458FF0)
    /* 0x5C */ u32 field_0x5c; // music id waiting for the current one to stop
    /* 0x60 */ u32 field_0x60;
    /* 0x64 */ s32 field_0x64;
    /* 0x68 */ u8 field_0x68; // music started
    /* 0x69 */ u8 field_0x69;
    /* 0x6A */ u8 field_0x6a; // set the first time system sound 0x48 plays
    /* 0x6B */ u8 field_0x6b;
    /* 0x6C */ u8 field_0x6c; // system sound 0xA is skipped while nonzero; set to 3 when it plays
    /* 0x6D */ u8 field_0x6d;
    /* 0x6E */ u8 field_0x6e[0x70 - 0x6E];
    /* 0x70 */ u32 field_0x70;
    /* 0x74 */ u8 field_0x74;
    /* 0x75 */ u8 field_0x75;
    /* 0x76 */ u8 field_0x76[0x78 - 0x76];
    /* 0x78 */ u32 field_0x78;
    /* 0x7C */ u8 field_0x7c; // set by fn_804595E4 (audio reset started)
    /* 0x7D */ u8 field_0x7d[0x80 - 0x7D];
    /* 0x80 */ JAISoundHandle field_0x80; // the music
    /* 0x84 */ JAISoundHandle field_0x84; // the second sequence (fn_80458F04)
    /* 0x88 */ u8 field_0x88;
    /* 0x89 */ u8 field_0x89;
    /* 0x8A */ u8 field_0x8a; // SE categories 3-6 paused (fn_80458658)
    /* 0x8B */ bool field_0x8b; // fn_8045815C's flag (fn_80458FF0 passes it on unconverted)
    /* 0x8C */ u8 field_0x8c; // fn_80457E54's flag
    /* 0x8D */ u8 field_0x8d;
    /* 0x8E */ u8 field_0x8e; // set by fn_804593D4, cleared by fn_80459274
    /* 0x8F */ u8 field_0x8f;
    // 0x80457E54: stores flag at 0x8c, sets the mode (fn_80458744 2 / 0)
    // and the music tempo (moveMusicTempo_80458508 2 / 0, inlined).
    void fn_80457E54(bool flag);
    // 0x80458508: the music tempo: lbl_8053A260 (kind 1) or lbl_8053A268
    // (kind 2) over their frame counts, else 1.0 at once.
    void fn_80458508(u8 kind);
    // fn_80458508's body, inline (game/tu_80457DFC.cpp; used only there).
    inline void moveMusicTempo_80458508(u8 kind);
    // 0x80459508: stops every sound (JAUSoundMgr::stop), resets the music
    // state and resumes the audio reseter.
    void fn_80459508();
    // game/tu_80457DFC.cpp (0x80459674, not written yet): the per-frame update; GameRoot's fn_80137300 calls it.
    void fn_80459674();
    // game/tu_80457DFC.cpp: stores `pause` at 0x8a and passes it to
    // JAISeCategoryMgr::pause(bool) of three categories of the object at
    // 0x34 (0x8045868C-0x804586A8); called by act_stop_npc.
    void fn_80458658(bool pause);
    // game/tu_80457DFC.cpp. Sets up JAudio from the two loaded audio files
    // (the buffers at 0x14 of the loaders in tu_80442810): `heap` is the
    // heap lbl_80529DEC.field_0x4, 0x800000 the audio memory size stored in
    // the JAU_JASInitializer (fn_80459AE4 stores r5 at its 0x4).
    void fn_80459AE4(JKRSolidHeap* heap, u32 audioMemSize, void* initData, void* seqData);
    // game/tu_80457DFC.cpp: JASDriver::setOutputMode(mode).
    void fn_804594E4(u32 mode);
    // game/tu_80457DFC.cpp (0x80458FF0). act_enemy_generator_3 calls it with
    // (0x1000021, 1, 0) when field_0x58 differs from 0x1000021; the body
    // compares r4 against 0x1000000+n, tests r5 with clrlwi. 24 (0x8045908C) and
    // stores r6 (a byte) at 0x8d. The third argument is a u8: GameRoot's
    // fn_80138B7C (tu_80134C04) passes an s32 count with clrlwi 24.
    void fn_80458FF0(s32 id, bool arg1, u8 flag);
    // game/tu_80457DFC.cpp (0x8045A080): the constructor main calls on the
    // 0x90-byte object it allocates in the main heap.
    AudioMgr_8045A080();
    // game/tu_80457DFC.cpp (0x80458880). Plays system sound `id` (a register,
    // not a JAISoundID; equality tests are cmplwi) for `player` (extsb on
    // entry; callers pass -1 for the shared object at 0x14, 331 of 393
    // calls; a fixed set of ids always plays there), panned by player when
    // field_0x5 is 2-4 (getPlayerPan); ids 0x38 and 0x57 start the second
    // sequence instead (startSecondSeq_80458F04, inlined). fn_8045B030 (game/tu_8045B010.cpp)
    // passes an id from lbl_8052DEE0.
    void fn_80458880(u32 id, s8 player);
    // The pan of a player's system sounds (game/tu_80457DFC.cpp, inline there).
    inline f32 getPlayerPan(s8 player);
    // game/tu_80457DFC.cpp (0x804593D4, not written yet). Sets 0x8e and pauses
    // the sound categories of the object at 0x34 (called by fn_80134D20).
    void fn_804593D4();
    // game/tu_80457DFC.cpp (0x80458F04). Callers pass a table entry in r4: if
    // the sound at 0x84 is not already id, creates it through the category
    // object at 0x30 (vtable slot 0xC) and starts a volume move.
    void fn_80458F04(u32 id);
    // fn_80458F04's body, inline (game/tu_80457DFC.cpp; used only there).
    inline void startSecondSeq_80458F04(u32 id);
    // game/tu_80457DFC.cpp (0x80458F9C). 30+ units pass 0x3C (the argument is
    // not read): stops the sound at 0x80 and clears the fields at
    // 0x58-0x68.
    void fn_80458F9C(s32 frames);
    // game/tu_80457DFC.cpp (0x80459274, same unit). Called on gAudioMgr_8053AF80
    // by cls_804B19F0::fn_80441A1C (game/tu_80441A1C.cpp) after the 0x0 call.
    void fn_80459274();
    // game/tu_80457DFC.cpp (0x80458090): loads wave archives 2, 3 and 4 of
    // the JAUSection at 0x44 (loadWaveArc). Called by tu_8039003C's
    // constructor when fn_804580DC returned 0.
    void fn_80458090();
    // game/tu_80457DFC.cpp (0x804580DC). Looks up entry `index` (a byte) of
    // the JAUSection at 0x44 and returns a byte (1 when its state is 2);
    // tu_8039003C's constructor passes 3.
    bool fn_804580DC(u8 index);
    // game/tu_80457DFC.cpp (0x80458408). Acts only for id 0x100001A on the
    // sound at 0x80; r5 is tested with clrlwi. 24 (0x80458488). Called by
    // tu_8039003C's constructor with (0x100001A, true).
    void fn_80458408(s32 id, bool flag);
    // SHARED-EDIT (tu_803DAC3C fn_803DD764): game/tu_80457DFC.cpp
    // (0x80457F80, not written yet). Takes a JAISoundID by value and a signed
    // byte (-2 skips the first test); returns a byte the caller tests with
    // clrlwi. 24. tu_803DAC3C passes (11, -2).
    bool fn_80457F80(JAISoundID id, s8 arg);
    // game/tu_80457DFC.cpp (0x8045815C): slows the music (tempo kind 2,
    // pitch 0.5, fx mix 1) and writes `flag` to port 0 of its tracks unless
    // the music is 0x1000001/2/4/0xD/0x23/0x2D. Called by
    // fn_80290E90 with false when the player's 0xCE4 is below 8, with true when it is 8.
    void fn_8045815C(bool flag);
    // game/tu_80457DFC.cpp (0x80457DFC): stores r4 (a byte) at 0x6.
    void fn_80457DFC(u8 value);
    // game/tu_80457DFC.cpp (0x80457E04): when 0x8c and 0x8b are 0, calls
    // fn_80458744 with 3 (flag set) or 0.
    void fn_80457E04(bool flag);
    // game/tu_80457DFC.cpp (0x80458744): switch on r4 (0-3).
    void fn_80458744(s32 mode);
    // game/tu_80457DFC.cpp (0x80457F60): sets the byte at 0x74.
    void fn_80457F60();
    // game/tu_80457DFC.cpp (0x80457F6C): clears the byte at 0x74, sets 0x6d to 0xB4.
    void fn_80457F6C();
    // game/tu_80457DFC.cpp (0x8045873C): stores r4 (a byte) at 0x69.
    void fn_8045873C(u8 value);
    // game/tu_80457DFC.cpp (0x8045959C): 0 until fn_804595E4 ran, then
    // 1 / 2 by JASAudioReseter::checkDone of the object at 0x2C.
    int fn_8045959C();
    // game/tu_80457DFC.cpp (0x804595E4): JASAudioReseter::start(5, flag) on the object at 0x2C.
    void fn_804595E4(bool flag);
    // game/tu_80457DFC.cpp (0x80459624): stores the position at 0x8/0xC/0x10 (x, z, y)
    // and copies `mtx` into listener `index` of the audience at 0x3C.
    void fn_80459624(const JGeometry::TVec3f* pos, Mtx mtx, s32 index);
}; // Size: 0x90


#endif

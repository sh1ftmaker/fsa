#ifndef JAUDIO_JAUAUDIOMGR_H
#define JAUDIO_JAUAUDIOMGR_H

#include "JSystem/JAudio/JAUAudience.h"
#include "JSystem/JAudio/JAUSoundMgr.h"
#include "JSystem/JAudio/JAUSectionHeap.h"
#include "JSystem/JAudio/System/JASHeap.h"

// fabricated, likely inheris JAUStdSoundInfo? however i'm not willing to dig into it as i'd need to base it of unused symbols
// probably similar to this https://github.com/zeldaret/tp/blob/main/include/Z2AudioLib/Z2SoundInfo.h
// https://github.com/zeldaret/tp/blob/main/include/Z2AudioLib/Z2AudioMgr.h
class JAUAudioMgr : public JAUSoundMgr, JAUSoundInfo {
public:
    JAUAudioMgr(JAUSectionHeap *);
private:
    // FSA's JAUAudioMgr.cpp keeps the weak dtors and vtables of
    // JAUAudience_withSetting_doppler and JAUAudience_withSetting (the doppler
    // dtor first), the JAUAudibleChannel ctor and the JAUAudible<4> pool, but
    // no JAUDopplerAudible<4> pool: plain audibles on the doppler setting
    // base. The real member type is unknown (all JAUAudioMgr code is
    // dead-stripped); this one reproduces those weak copies.
    JAUAudience_<4, JAUAudible<4>, JAUAudienceState, JAUAudience_withSetting_doppler> mAudience;
    JASHeap mHeaps[4];
};

#endif

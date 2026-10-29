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
    //JAUAudience<4> audience;
    JAUAudience<4> mAudience;
    JASHeap mHeaps[4];
};

#endif

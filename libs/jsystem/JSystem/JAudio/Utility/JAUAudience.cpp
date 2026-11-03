#include "JSystem/JAUdio/JAUAudience.h"
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JUtility/JUTAssert.h"

void JAUAudience3DSetting::init() {
    _04 = 7000.0f;
    _08 = 800.0f;
    minVolume = 0.005f;
    _14 = 8000.0f;
    _10 = -4000.0f;
    _28 = 0.1f;
    _24 = 0.0f;
    _18 = 1.0f;
    _1c = 0.8f;
    _20 = 0.0f;
    _2c = 0.5f;
    _38 = 0xff;
    _30 = 3000.0f;
    _34 = 1.5f;
}

void JAUAudience_withSetting::updateSetting() {
    JUT_ASSERT(setting_.minVolume >= 0.0f);
    _3c = (setting_._20 - setting_._1c) / setting_._10;
    _40 = (setting_._18 - setting_._1c) / setting_._14;
    _44 = (setting_.minVolume - 1.0f) / (setting_._04 - setting_._08);
    _48 = (f32)setting_._38 / (setting_._04 - setting_._08);
    _4c = (setting_._28 - setting_._24) / (setting_._04 - setting_._08);
}

JAUAudience_withSetting::JAUAudience_withSetting() {
    setting_.init();
    updateSetting();
}

namespace {

    void testInstanciate() {
        //JAUGenericAudible_<4, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos> *audible = new JAUGenericAudible_<4, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos>[4];
       
        new JAUDopplerAudience<4>;
        JASPoolAllocObject< JAUDopplerAudible<4> >::newMemPool(0x4e);

    }
}

// FIXME: __sinit_JAUAudience_cpp (0x8003D264) is not emitted yet. FSA constructs nine
// pools: the JASTrack::TChannelMgr, JASTrack and JASChannel multi-threaded pools, then
// six per-TU single-threaded pools (0x8052E78C/9C/AC, 0x804B21EC, 0x8052E7BC, and
// JASPoolAllocObject<JAUDopplerAudible<4>>::memPool_ at 0x804B21DC). It needs the generic
// memPool_ definition (JASHeapCtrl.h, pending); the first five pools' classes are not
// identified yet.

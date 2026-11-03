#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/System/JASInst.h"
#include "JSystem/JUtility/JUTAssert.h"

JASBasicInst::JASBasicInst() : mVolume(1.0f), mPitch(1.0f), mEffects(), mKeyRegionCount(0), mKeymap(NULL) {
    JASCalc::bzero(mOsc, sizeof(mOsc));
}

JASBasicInst::~JASBasicInst() {
    delete[] mKeymap;
}

bool JASBasicInst::getParam(int key, int velocity, JASInstParam *instParam) const {
    instParam->_14 = 0;
    instParam->_24 = 0;
    instParam->_1c = (JASOscillator::Data **)&mOsc;
    instParam->_20 = 2;
    instParam->mVolume = mVolume;
    instParam->mPitch = mPitch;

    for (JASPtrListAdaptor<JASInstEffect> *it = mEffects.begin(); it != NULL; it = it->getNext()) {
        it->get()->effect(key, velocity, instParam);
    }

    const TKeymap *keyMap = NULL;
    for (int i = 0; i < mKeyRegionCount; i++) {
        if (key <= mKeymap[i].mHighKey) {
            keyMap = &mKeymap[i];
            break;
        }
    }

    if (!keyMap) {
        return false;
    }

    for (int j = 0; j < keyMap->mVeloRegionCount; j++) {
        const JASVelo *velo = keyMap->getVeloRegion(j);
        if (velocity <= velo->_0) {
            instParam->mVolume *= velo->_8;
            instParam->mPitch *= velo->_c;
            instParam->_18 = velo->_4;
            return true;
        }
    }

    return false;
}

void JASBasicInst::setKeyRegionCount(u32 count, JKRHeap *pHeap) {
    delete[] mKeymap;
    mKeymap = new (pHeap, 0) TKeymap[count];
#line 110
    JUT_ASSERT(mKeymap != 0);
    mKeyRegionCount = count;
}

void JASBasicInst::appendEffect(JASPtrListAdaptor<JASInstEffect> *effect) {
    JASPtrListAdaptor<JASInstEffect> *it;
    if ((it = mEffects.begin()) == NULL) {
        mEffects.mList = effect;
        return;
    }

    while (it->getNext() != NULL) {
        it = it->getNext();
    }
    it->mNext = effect;
}

void JASBasicInst::setOsc(int index, JASOscillator::Data *osc) {
#line 134
    JUT_ASSERT(index < OSC_MAX);
    JUT_ASSERT(index >= 0);
    mOsc[index] = osc;
}

JASBasicInst::TKeymap *JASBasicInst::getKeyRegion(int index) {
#line 152
    JUT_ASSERT(index >= 0);
    if (index >= mKeyRegionCount) {
        return NULL;
    }
    return &mKeymap[index];
}

JASBasicInst::TKeymap *JASBasicInst::getKeyRegion(int index) const {
    JUT_ASSERT(index >= 0);
    if (index >= mKeyRegionCount) {
        return NULL;
    }
    return &mKeymap[index];
}

JASBasicInst::TKeymap::~TKeymap() {
    delete[] mVelomap;
}

void JASBasicInst::TKeymap::setVeloRegionCount(u32 count, JKRHeap *pHeap) {
#line 177
    delete[] mVelomap;
    mVelomap = new (pHeap, 0) JASVelo[count];
    JUT_ASSERT(mVelomap != 0);
    mVeloRegionCount = count;
}

JASVelo *JASBasicInst::TKeymap::getVeloRegion(int index) {
#line 187
    JUT_ASSERT(index >= 0);
    if (index >= mVeloRegionCount) {
        return NULL;
    }
    return &mVelomap[index];
}

JASVelo *JASBasicInst::TKeymap::getVeloRegion(int index) const {
#line 196
    JUT_ASSERT(index >= 0);
    if (index >= mVeloRegionCount) {
        return NULL;
    }
    return &mVelomap[index];
}

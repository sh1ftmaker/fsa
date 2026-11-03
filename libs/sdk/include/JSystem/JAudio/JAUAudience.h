#ifndef JAUDIO_JAUAUDIENCE_H
#define JAUDIO_JAUAUDIENCE_H

#include "JSystem/JAudio/Interface/JAIAudible.h"
#include "JSystem/JAudio/Interface/JAIAudience.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JAudio/System/JASSoundParams.h"
#include "JSystem/JGeometry/Matrix.h"
#include "JSystem/JGeometry/Util.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/mtx.h"

// Copied from https://github.com/zeldaret/tp/blob/main/include/Z2AudioLib/Z2Audience.h
// Z2Audio probably used this as a base but refactored it

struct JAUAudibleParam {
    f32 getDopplerPower() const {
        return _00.bytes.b0_0 * (1.0f / 15.0f);
    }
    union {
        struct {
            u16 f0;
            u16 f1;
        } half;
        struct {
            u8 b0_0 : 4;
            u8 b0_4 : 1;
            u8 b0_5 : 1;
            u8 b0_6 : 1;
            u8 b0_7 : 1;
            u8 b1_0 : 1;
            u8 b1_1 : 1;
            u8 b1_2_7 : 6;
            u8 b2;
            u8 b3;
        } bytes;
        u32 raw;
    } _00;
};

class JAUSoundInfo : public JASGlobalInstance<JAUSoundInfo>
{
public:
    JAUSoundInfo(bool param_1) : JASGlobalInstance<JAUSoundInfo>(param_1) {}
    virtual u16 getAudibleSw(JAISoundID) const = 0;
    virtual u16 getBgmSeqResourceID(JAISoundID) const = 0;

    JAUAudibleParam getAudibleSwFull(JAISoundID soundID) {
        JAUAudibleParam audibleParams;
        audibleParams._00.half.f0 = getAudibleSw(soundID);
        audibleParams._00.half.f1 = 0xffff;
        return audibleParams;
    }
};

// FSA's audible keeps no previous position or velocity: the generic
// audible's ctor only copies the position (nothing is stored at +0x14), and
// its calc (0x8003D260, in both audible vtables) is a bare blr.
class JAUAudibleAbsPos {
public:
    void init(JGeometry::TVec3f *pos, const JGeometry::TVec3f &newPos, const JGeometry::TVec3f *) {
        pos->set(newPos);
    }
    void calc(const JGeometry::TVec3f &) {}
};

class JAUDopplerAudibleAbsPos {
public:
    void init(JGeometry::TVec3f *pos, const JGeometry::TVec3f &newPos, const JGeometry::TVec3f *) {
        pos->set(newPos);
    }
    void calc(const JGeometry::TVec3f &) {}
};

// 0x24 bytes (newAudible's __construct_array); the relative position and
// distance are written by the audience state's convertAbsToRel.
class JAUAudibleChannel {
public:
    JAUAudibleChannel() {}

    void init() {
        params_.init();
    }

    JASSoundParams params_;  // 00
    JGeometry::TVec3f relPos_; // 14
    f32 dist_;               // 20
};

// SMG's map has JAUAudience3DSetting::init with these 14 fields in this
// order; FSA's .sdata2 starts with init's constants (7000.0f ... 1.5f) ahead
// of updateSetting's, so init was compiled first (and then inlined into the
// ctor and dead-stripped).
class JAUAudience3DSetting {
public:
    void init();

    f32 _04; // 04 (far distance)
    f32 _08; // 08 (near distance)
    f32 minVolume; // 0c
    f32 _10;
    f32 _14;
    f32 _18;
    f32 _1c;
    f32 _20;
    f32 _24;
    f32 _28;
    f32 _2c;
    f32 _30;
    f32 _34;
    u32 _38;
};

class JAUAudience_withSetting : public JAIAudience {
public:
    JAUAudience_withSetting();
    ~JAUAudience_withSetting() {}

    //virtual JAIAudible *newAudible(const JGeometry::TVec3f &, JAISoundID soundID, const JGeometry::TVec3f *, u32 channels) = 0;
    //virtual int getMaxChannels() = 0;
    //virtual void deleteAudible(JAIAudible *audible) = 0;
    //virtual u32 calcPriority(JAIAudible *audible) = 0;
    //virtual void mixChannelOut(const JASSoundParams &params, JAIAudible *audible, int channel) = 0;

    void updateSetting();

    JAUAudience3DSetting setting_; // 04
    f32 _3c;
    f32 _40;
    f32 _44;
    f32 _48;
    f32 _4c;
};

class JAUAudience_withSetting_doppler : public JAUAudience_withSetting {
public:
    JAUAudience_withSetting_doppler() {}

    ~JAUAudience_withSetting_doppler() {
        
    }
};




// FIXME: FSA's newAudible passes __ct__17JAUAudibleChannelFv (0x8003B0B0) to
// __construct_array; ours emits and passes __ct__24JAUDopplerAudibleChannelFv
// (same bytes). See the checkpoint.
class JAUDopplerAudibleChannel : public JAUAudibleChannel {
}; // Size: 0x24

// FSA layout: JAUDopplerAudible<4> is 0x40 bytes (pool alloc/free 0x40):
// JAIAudible (0x10), audible param 0x10, empty AbsPos 0x14, one channel
// pointer at 0x18 and one 0x24-byte channel at 0x1C. newAudible constructs
// a single channel (__construct_array count 1, no loop) whatever N is.
template <int N, class Channel, class AbsPos>
class JAUGenericAudible_ : public JAIAudible {
public:
    JAUGenericAudible_(const JGeometry::TVec3f &pos, const JGeometry::TVec3f *p3, u32 channel) {
        audiblep._00.raw = 0xffffffff;
        absPos_.init(&mPos, pos, p3);
        for (int i = 0; i < MAX_CHANNELS; i++) {
            if ((channel & (1 << i)) == 0) {
                channels[i].init();
                channel_[i] = &channels[i];
            }
            else {
                channel_[i] = NULL;
            }
        }
    }
    ~JAUGenericAudible_() {}

    virtual JASSoundParams *getOuterParams(int index) {
        Channel *channel = getChannel(index);
        if (channel) {
            return &channel->params_;
        }
        return NULL;
    }

    virtual void calc() {
        absPos_.calc(mPos);
    }

    Channel *getChannel(int index) {
        return channel_[index];
    }

    JAUAudibleParam &getAudibleParam() { return audiblep; }
    const JAUAudibleParam getAudibleParam() const { return audiblep; }

    void setAudibleParam(JAUAudibleParam param) {
        audiblep = param;
    }

    static const int MAX_CHANNELS = 1;

    JAUAudibleParam audiblep;              // 10
    AbsPos absPos_;                        // 14
    Channel *channel_[MAX_CHANNELS];       // 18
    Channel channels[MAX_CHANNELS];        // 1C
};

template <int N, class Audible, class State, class Audience>
class JAUAudience_ : public Audience {
public:
    JAUAudience_() : numPlayers_(N) {
        for (int i = 0; i < N; i++) {
            state[i].init();
        }
    }
    ~JAUAudience_() {
#line 617
        JUT_ASSERT(!isActive());
    }

    bool isActive() const { return Audible::getTotalMemCount() != Audible::getFreeMemCount(); };

    int getMaxChannels() { return numPlayers_; }

    void setMaxChannels(int numPlayers) {
#line 627
        JUT_ASSERT(numPlayers <= MAX_PLAYERS);
        JUT_ASSERT(numPlayers >= 1);
        JUT_ASSERT(! isActive());
        numPlayers_ = numPlayers;
    }

    f32 calcVolume_(f32 p1) const {
        if (p1 > this->setting_._04) {
            return this->setting_.minVolume;
        } 

        if (p1 < this->setting_._08) {
            return 1.0f;
        }

        return JGeometry::TUtilf::clamp(1.0f + this->_44 * (p1 - this->setting_._08), 0.0f, 1.0f);
    }

    void mixChannelOut(const JASSoundParams &params, JAIAudible *pAudible, int index) {
        Audible *audible = (Audible *)pAudible;
        JAUAudibleChannel *channel = audible->getChannel(index);
        if (!channel) {
            return;
        }

        JASSoundParams newParams;
        if (audible->getAudibleParam()._00.bytes.b0_5) {
            newParams.mVolume = calcVolume_(channel->dist_);
        } else {
            newParams.mVolume = 1.0f;
        }

        if (audible->getAudibleParam()._00.bytes.b1_1) {
            f32 dolby;
            if (channel->relPos_.z > this->setting_._14) {
                dolby = this->setting_._18;
            } else if (channel->relPos_.z < this->setting_._10) {
                dolby = this->setting_._20;
            } else if (channel->relPos_.z < 0.0f) {
                dolby = this->_3c * channel->relPos_.z + this->setting_._1c;
            } else {
                dolby = this->_40 * channel->relPos_.z + this->setting_._1c;
            }
            newParams.mDolby = dolby;
        } else {
            newParams.mDolby = this->setting_._1c;
        }

        if (audible->getAudibleParam()._00.bytes.b1_0) {
            if (numPlayers_ > 2) {
                newParams.mPan = (index & 1) ? 1.0f : 0.0f;
            } else {
                f32 pan = channel->dist_ <= JGeometry::TUtilf::epsilon() ? 0.0f : channel->relPos_.x / channel->dist_;
                newParams.mPan = pan * this->setting_._2c + 0.5f;
            }
        } else {
            newParams.mPan = 0.5f;
        }

        newParams.mPitch = 1.0f;

        f32 fxMix;
        if (audible->getAudibleParam()._00.bytes.b0_6) {
            if (channel->dist_ > this->setting_._04) {
                fxMix = this->setting_._28;
            } else if (channel->dist_ < this->setting_._08) {
                fxMix = this->setting_._24;
            } else {
                fxMix = this->_4c * (channel->dist_ - this->setting_._08) + this->setting_._24;
            }
        } else {
            fxMix = 0.0f;
        }
        newParams.mFxMix = fxMix;

        audible->getOuterParams(index)->combine(params, newParams);
    }

    u32 calcDeltaPriority_(f32 dist, bool p2) const {
        if (dist > this->setting_._04) {
            if (p2) {
                return -1;
            }
            return this->setting_._38;
        }

        if (dist < this->setting_._08) {
            return 0;
        }

        return this->_48 * (dist - this->setting_._08);
    }

    // FIXME: 98.9 %: in the second loop FSA keeps channel in r29 and the
    // deltaPriority cursor in r26 (ours: r24 / r29); same instructions.
    u32 calcPriority(JAIAudible *i_audible) {
        u32 deltaPriority[Audible::MAX_CHANNELS];

        Audible *audible = (Audible *)i_audible;
        if (!audible->getAudibleParam()._00.bytes.b0_4) {
            for (int i = 0; i < numPlayers_; i++) {
                state[i].convertAbsToRel(audible, i);
            }
            return 0;
        }

        u32 rv = 0xffffffff;
        for (int i = 0; i < numPlayers_; i++) {
            JAUAudibleChannel *channel = audible->getChannel(i);
            if (channel) {
                state[i].convertAbsToRel(audible->mPos, channel);
                deltaPriority[i] = calcDeltaPriority_(channel->dist_, audible->getAudibleParam()._00.bytes.b0_7);
                if (deltaPriority[i] < rv) {
                    rv = deltaPriority[i];
                }
            }
        }
        return rv;
    }

    void deleteAudible(JAIAudible *audible) {
        delete audible;
    }

    JAIAudible *newAudible(const JGeometry::TVec3f &pos, JAISoundID soundID, const JGeometry::TVec3f *p3, u32 channels) {
        u32 channelNum = channels | ~((1 << numPlayers_) - 1);
        if (channelNum == 0xffffffff) {
            return NULL;
        }

        Audible *audible = new Audible(pos, p3, channelNum);
        if (audible == NULL) {
            return NULL;
        }

        for (int i = 0; i < numPlayers_; i++) {
            state[i].convertAbsToRel(audible, i);
        }

        if (JAUSoundInfo::getInstance()) {
            u16 sw = JAUSoundInfo::getInstance()->getAudibleSw(soundID);
            JAUAudibleParam audibleParams;
            audibleParams._00.half.f0 = sw;
            audibleParams._00.half.f1 = 0xffff;

            audible->setAudibleParam(audibleParams);
        }

        return audible;
    }

    void setAudienceMtx(MtxPtr m, int playerNumber, bool doReset) {
#line 756
        JUT_ASSERT(playerNumber >= 0);
        JUT_ASSERT(playerNumber < numPlayers_);
        if (doReset) {
            state[playerNumber].resetMtx(m);
        }
        else {
            state[playerNumber].setMtx(m);
        }
    }

    static const int MAX_PLAYERS = N;

    int numPlayers_;
    State state[N];
};

template <int N>
class JAUAudible : public JAUGenericAudible_<N, JAUAudibleChannel, JAUAudibleAbsPos>, public JASPoolAllocObject< JAUAudible<N> > {
public:
    typedef JAUGenericAudible_<N, JAUAudibleChannel, JAUAudibleAbsPos> BaseAudible;
    
    JAUAudible(const JGeometry::TVec3f &pos, const JGeometry::TVec3f *p3, u32 channel) : BaseAudible(pos, p3, channel) {
    }
    ~JAUAudible() {
       
    }
};

template <int N>
class JAUDopplerAudible : public JAUGenericAudible_<N, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos>, public JASPoolAllocObject< JAUDopplerAudible<N> > {
public:
    typedef JAUGenericAudible_<N, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos> BaseAudible;

    JAUDopplerAudible(const JGeometry::TVec3f &pos, const JGeometry::TVec3f *p3, u32 channel) : BaseAudible(pos, p3, channel) {
        
    }
    ~JAUDopplerAudible() {
        
    }

    
};

// FSA's audience state is the listener matrix alone (0x30 bytes: the
// states sit at audience+0x54 with a 0x30 stride); convertAbsToRel is
// PSMTXMultVec of the audible's position and PSVECMag of the result.
// init/resetMtx/setMtx are not in this unit and not checked against FSA.
class JAUAudienceState {
public:
    JAUAudienceState() {}

    void init() {
        PSMTXIdentity(mMtx);
    }
    void resetMtx(const Mtx in) {
        PSMTXCopy(in, mMtx);
    }
    void setMtx(const Mtx in) {
        PSMTXCopy(in, mMtx);
    }

    void convertAbsToRel(const JGeometry::TVec3f &pos, JAUAudibleChannel *channel) {
        PSMTXMultVec(mMtx, &pos, &channel->relPos_);
        channel->dist_ = PSVECMag(&channel->relPos_);
    }

    template <class Audible>
    void convertAbsToRel(Audible *audible, int i) {
        JAUAudibleChannel *channel = audible->getChannel(i);
        if (channel != NULL) {
            convertAbsToRel(audible->mPos, channel);
        }
    }

    Mtx mMtx;
};

class JAUDopplerAudienceState {
public:
    JAUDopplerAudienceState() {}

    void init() {
        PSMTXIdentity(mMtx);
    }
    void resetMtx(const Mtx in) {
        PSMTXCopy(in, mMtx);
    }
    void setMtx(const Mtx in) {
        PSMTXCopy(in, mMtx);
    }

    void convertAbsToRel(const JGeometry::TVec3f &pos, JAUAudibleChannel *channel) {
        PSMTXMultVec(mMtx, &pos, &channel->relPos_);
        channel->dist_ = PSVECMag(&channel->relPos_);
    }

    template <class Audible>
    void convertAbsToRel(Audible *audible, int i) {
        JAUAudibleChannel *channel = audible->getChannel(i);
        if (channel != NULL) {
            convertAbsToRel(audible->mPos, channel);
        }
    }

    Mtx mMtx;
}; // Size: 0x30


template <int N>
class JAUAudience : public JAUAudience_<N, JAUAudible<N>, JAUAudienceState, JAUAudience_withSetting> {
public:
    JAUAudience() {}
    ~JAUAudience() {
       
    }

};

template <int N>
class JAUDopplerAudience : public JAUAudience_<N, JAUDopplerAudible<N>, JAUDopplerAudienceState, JAUAudience_withSetting_doppler> {
public:
    JAUDopplerAudience() {
        
    }
    ~JAUDopplerAudience() {
    }

};

#endif

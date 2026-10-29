#ifndef GAMEAUDIOAUDIENCE_H
#define GAMEAUDIOAUDIENCE_H


#include "JSystem/JAudio/JAUAudience.h"
namespace GameAudio {

template <int N>
class CustomAudience : public JAUDopplerAudience<N> {
public:
    CustomAudience();
    ~CustomAudience(); // 08, override
    JAIAudible *newAudible(const JGeometry::TVec3f &p1, JAISoundID soundID, const JGeometry::TVec3f *p3, u32 channel); // 0C, override

    void init(int i) { // fabricated
        mEchoRate[i] = 0.0f;
        if (i < N) // ???
            smCameraVolume[i] = 1.0f;
    }

    void setEchoRate(int i, f32 rate) {
        mEchoRate[i] = rate;
    }

    void updateVolume() {
        smCameraVolume[0] = 1.0f;
        smCameraVolume[1] = 1.0f;
        smCameraVolume[2] = 1.0f;
        smCameraVolume[3] = 1.0f;
    }

    void initSlopesAndThresholds() { // fabricated
        const f32& minVolume = setting_.minVolume;
        const f32& refDistance = setting_._04;
        const f32& nearDistance = setting_._08;

        f32 fullVolume = 1.0f;
        f32 volume2x = 2.0f;
        f32 volume3x = 3.0f;
        f32 volume4x = 4.0f;
        f32 volumeHalf = 0.5f;
        f32 volumeQuarter = 0.25f;
        f32 volumeSixth = 0.16666667f;
        f32 volumeEighth = 0.125f;

        mVolumeSlope2x = (minVolume - fullVolume) / (( refDistance * volume2x) - nearDistance);
        mVolumeSlope3x = (minVolume - fullVolume) / (( refDistance * volume3x) - nearDistance);
        mVolumeSlope4x = (minVolume - fullVolume) / (( refDistance * volume4x) - nearDistance);
        mVolumeSlopeHalf = (minVolume - fullVolume) / (( refDistance * volumeHalf) - nearDistance);
        mVolumeSlopeQuarter = (minVolume - fullVolume) / (( refDistance * volumeQuarter) - nearDistance);
        mVolumeSlopeSixth = (minVolume - fullVolume) / (( refDistance * volumeSixth) - nearDistance);
        mVolumeSlopeEighth = (minVolume - fullVolume) / (( refDistance * volumeEighth) - nearDistance);

        mDistanceThreshold2x = volume2x * refDistance;
        mDistanceThreshold3x = volume3x * refDistance;
        mDistanceThreshold4x = volume4x * refDistance;
        mDistanceThresholdHalf = volumeHalf * refDistance;
        mDistanceThresholdQuarter = volumeQuarter * refDistance;
        mDistanceThresholdSixth = volumeSixth * refDistance;
        mDistanceThresholdEighth = volumeEighth * refDistance;
    }

    static f32 smCameraVolume[N];

    f32 mVolumeSlope2x; // _1a4
    f32 mVolumeSlope3x; // _1a8
    f32 mVolumeSlope4x; // _1ac
    f32 mVolumeSlopeHalf; // _1b0
    f32 mVolumeSlopeQuarter; // _1b4
    f32 mVolumeSlopeSixth; // _1b8
    f32 mVolumeSlopeEighth; // _1bc
    f32 mDistanceThreshold2x; // _1c0
    f32 mDistanceThreshold3x; // _1c4
    f32 mDistanceThreshold4x; // _1c8
    f32 mDistanceThresholdHalf; // _1cc
    f32 mDistanceThresholdQuarter; // 1d0
    f32 mDistanceThresholdSixth; // 1d4
    f32 mDistanceThresholdEighth; // 1d8
    f32 mEchoRate[N]; // 1dc
}; // Size: 0x1ec(probably variable)

}

#endif

#include <JSystem/JAudio/System/JASInst.h>
#include <JSystem/JAudio/System/JASChannel.h>  // FSA's __sinit constructs the JASChannel pool
#include <JSystem/JUtility/JUTDbg.h>

// FIXME: 97.6 %. FSA computes 2*(r-1) - 1.0f*(1.0f - FLT_EPSILON) with an unfolded multiply by
// 1.0f (fmuls, then fnmsubs); this source folds it into one fmsubs. The source form of that
// 1.0f factor is unknown. It also orders .sdata2 2.0f, 1.0f (ours 1.0f, 2.0f).
void JASInstRand::effect(int, int, JASSoundParams *params) const
{
    static JMath::TRandom_fast_ oRandom(0);
    f32 max = 1.0f - FLT_EPSILON;
    f32 val = oRandom.get_ufloat_1() * 2.0f - max;
    val *= mCeiling;
    val += mFloor;

    switch(mType) {
    case 0:
        params->mVolume *= val;
        break;
    case 1:
        params->mPitch *= val;
        break;
    case 2:
        params->mPan += val - 0.5; // yes, they forgot the f
        break;
    case 3:
        params->mFxMix += val;
        break;
    case 4:
        params->mDolby += val;
        break;
    default:
        JUT_ASSERT(0);
        break;
    }
}

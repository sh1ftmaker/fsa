//
// J3DStruct
//
// From zeldaret/tp's J3DStruct.cpp. FSA keeps only the four out-of-line
// operator=; tp's setEffectMtx and the operator== are dead-stripped here.
//

#include "JSystem/J3D/J3DTexMtx.h"
#include "JSystem/J3D/J3DInd.h"
#include "JSystem/J3D/J3DPE.h"
#include "JSystem/J3D/J3DTypes.h"
#include "JSystem/JMath/JMath.h"

J3DTexMtxInfo& J3DTexMtxInfo::operator=(const J3DTexMtxInfo& other)
{
	mProjection = other.mProjection;
	mInfo       = other.mInfo;
	JMath::gekko_ps_copy3(&mCenter, &other.mCenter);
	mSRT = other.mSRT;
	JMath::gekko_ps_copy16(&mEffectMtx, &other.mEffectMtx);
	return *this;
}

J3DIndTexMtxInfo& J3DIndTexMtxInfo::operator=(const J3DIndTexMtxInfo& other)
{
	JMath::gekko_ps_copy6(mOffsetMtx, other.mOffsetMtx);
	mScaleExp = other.mScaleExp;
	return *this;
}

J3DFogInfo& J3DFogInfo::operator=(const J3DFogInfo& other)
{
	mType      = other.mType;
	mAdjEnable = other.mAdjEnable;
	mCenter    = other.mCenter;
	mStartZ    = other.mStartZ;
	mEndZ      = other.mEndZ;
	mNearZ     = other.mNearZ;
	mFarZ      = other.mFarZ;
	mColor.r   = other.mColor.r;
	mColor.g   = other.mColor.g;
	mColor.b   = other.mColor.b;
	mColor.a   = other.mColor.a;
	for (int i = 0; i < 10; i++) {
		mFogAdjTable[i] = other.mFogAdjTable[i];
	}
	return *this;
}

J3DNBTScaleInfo& J3DNBTScaleInfo::operator=(const J3DNBTScaleInfo& other)
{
	mHasScale = other.mHasScale;
	mScale.x  = other.mScale.x;
	mScale.y  = other.mScale.y;
	mScale.z  = other.mScale.z;
	return *this;
}

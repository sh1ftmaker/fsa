#include "JSystem/J3D/J3DJointFactory.h"
#include "JSystem/J3D/J3DJoint.h"
#include "JSystem/JSupport.h"

/**
 * @note Address: 0x80071B20
 * @note Size: 0x58
 */
J3DJointFactory::J3DJointFactory(const J3DJointBlock& block)
{
	mInitData = JSUConvertOffsetToPtr<J3DJointInitData>(&block, block.mJointInitData);
	mIndexMap = JSUConvertOffsetToPtr<u16>(&block, block.mRemapTableOffset);
}

/**
 * @note Address: 0x80071B78
 * @note Size: 0x180
 */
J3DJoint* J3DJointFactory::create(int jointIndex)
{
	J3DJoint* joint              = new J3DJoint;
	joint->mJointIdx             = jointIndex;
	joint->mKind                 = mInitData[mIndexMap[jointIndex]].mKind;
	joint->mScaleCompensate      = mInitData[mIndexMap[jointIndex]].mIgnoreParentScaling;
	// FIXME: FSA copies the transform info member-wise: scale and translation as psq_l/psq_st of
	// x,y plus lfs/stfs of z, the rotation as lwz/stw of x,y plus lha/sth of z. Our Vec/S16Vec
	// copy gives three lfs/stfs and three lha/sth. tp gets it with an asm operator= (and a
	// pointer-cast TVec3<s16> copy).
	joint->mTransformInfo        = mInitData[mIndexMap[jointIndex]].mTransformInfo;
	joint->mBoundingSphereRadius = mInitData[mIndexMap[jointIndex]].mRadius;
	joint->mMin                  = mInitData[mIndexMap[jointIndex]].mMin;
	joint->mMax                  = mInitData[mIndexMap[jointIndex]].mMax;
	joint->mMtxCalc              = nullptr;
	if (joint->mScaleCompensate == 0xFF) {
		joint->mScaleCompensate = 0;
	}
	return joint;
}

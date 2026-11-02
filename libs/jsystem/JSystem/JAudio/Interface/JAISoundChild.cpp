#include "JSystem/JAudio/Interface/JAISound.h"

#ifdef DEBUG
#pragma sym on
#endif

void JAISoundChild::init() {
    mMove.init();
    mParams.init();
}

void JAISoundChild::mixOut(JASTrack *track)
{
    mParams = mMove;
    track->assignExtBuffer(0, &mParams);
}

void JAISoundChild::calc()
{
    mMove.calc();
}

#ifdef DEBUG
#pragma sym off
#endif

#ifndef JAUDIO_JAISOUNDCHILD_H
#define JAUDIO_JAISOUNDCHILD_H

#include "JSystem/JAudio/Interface/JAISoundParams.h"
#include "JSystem/JAudio/System/JASHeapCtrl.h"

class JASTrack;

class JAISoundChild : JASPoolAllocObject<JAISoundChild>
{
public:
    void init();
    void mixOut(JASTrack *track);
    void calc();

    JAISoundChild() { init(); }

    JAISoundParamsMove mMove; // 00
    JASSoundParams mParams;   // 50
}; // Size: 0x64

#endif // JAUDIO_JAISOUNDCHILD_H

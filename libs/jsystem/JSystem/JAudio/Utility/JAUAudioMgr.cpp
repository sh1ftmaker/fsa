// FSA's __sinit builds the JAISeq pool between the JAIStream and JAISe ones,
// so JAISeq is seen after JAISe and before JAIStream (JAUSoundMgr.h).
#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/Interface/JAISeq.h"
#include "JSystem/JAudio/JAUAudioMgr.h"
#include "JSystem/JAudio/System/JASGadget.h"

#include "macros.h"

template <>
DECL_WEAK JAUSoundInfo *JASGlobalInstance<JAUSoundInfo>::sInstance;

JAUAudioMgr::JAUAudioMgr(JAUSectionHeap *) : JAUSoundInfo(true) {

}


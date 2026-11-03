#include "JSystem/JAudio/Interface/JAIAudience.h"
#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/Interface/JAISeqMgr.h"
#include "JSystem/JAudio/Interface/JAISeq.h"
#include "JSystem/JAudio/Interface/JAIStreamMgr.h"
#include "JSystem/JUtility/JUTAssert.h"

JAIAudience::~JAIAudience()
{
    JUT_ASSERT(! JAISeMgr::getInstance() || ( JAISeMgr::getInstance() ->getAudience() != this ))
    JUT_ASSERT(! JAISeqMgr::getInstance() || ( JAISeqMgr::getInstance() ->getAudience() != this ))
    JUT_ASSERT(! JAIStreamMgr::getInstance() || ( JAIStreamMgr::getInstance() ->getAudience() != this ))
}

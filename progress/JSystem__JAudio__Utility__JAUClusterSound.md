# JSystem/JAudio/Utility/JAUClusterSound.cpp

Before: 3/4 (`__sinit_JAUClusterSound_cpp` 0 %, .bss unsplit). After: 4/4, .text/.ctors/.bss 100 %,
`link_check.py --unit` SHA-1 OK (with JAudio/Utility built without `-sym on`, infra/jaudio-interface-sym).

- Source: `#include "JSystem/JAudio/System/JASTrack.h"`. mkdd's own JAISound.h includes
  JASTrack.h (ours dropped it in 29cd4cc for JAUStreamFileTable), so a JAUClusterSound.cpp that
  includes JAISoundHandles.h saw JASTrack/JASChannel in mkdd's header set; with the generic
  `memPool_` definitions the TU then emits the three pools FSA's 0xD4 `__sinit` builds. Same
  per-file include fix as JAISoundHandles.cpp, JAISe.cpp, JAISeq.cpp.
- symbols.txt: lbl_804B4B18 (0x28) split into 3 x 0xC destructor-chain objects (0x804B4B18,
  0x804B4B24, 0x804B4B30), as JAISeq/JAISeMgr did; the last 4 bytes are padding.
- The body is mkdd's fabricated instantiation (the real functions are dead-stripped, tp uses a
  dummy function the same way); noted in the source.

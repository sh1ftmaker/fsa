#include "JSystem/JAudio/JAUClusterSound.h"
#include "JSystem/JAudio/System/JASTrack.h"

// fabricated (mkdd): the file's own functions are dead-stripped in FSA as in
// mkdd; this instantiation stands in for them, like tp's dummyCtor in its
// JAUClusterSound.cpp. JASTrack.h is what mkdd's JAISound.h includes; the
// TU seeing JASTrack/JASChannel gives the three pools of FSA's __sinit.

namespace {
    void testInstanciate() {
        JAUClusterSound<3> cluster3;
    }
}

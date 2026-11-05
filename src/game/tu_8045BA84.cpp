#include "game/GameAudio.h"
#include "game/GameGlobals.h"

// Projects pos to the screen through the view at lbl_8053AB10->field_0x24.
void fn_8045BA84(const Vec* pos, Vec* out)
{
    lbl_8053AB10->field_0x24->fn_8030C5F0(pos, true, true, out);
}

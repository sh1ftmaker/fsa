// All of this file's own functions were dead-stripped; only its __sinit,
// which builds the eight JAudio pools of game/GameAudioBase.h, remains.
#include "game/GameAudioBase.h"

// AudioMgr_8045A080::fn_80458744's music volumes per mode (game/tu_80457DFC.cpp):
// music, second sequence, SE category 0, then six values nothing reads. They
// sit between game/tu_80455CA4.cpp's .data and tu_80457DFC's jump tables, and
// fn_80458744 addresses each one as an extern (lis / lfsu per table), which
// under deferred codegen means they are not defined in its file.
f32 lbl_804B1E60[9] = {1.0f, 1.0f, 0.7f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f};
f32 lbl_804B1E84[9] = {0.0f, 0.0f, 0.7f, 0.441f, 0.441f, 0.441f, 0.441f, 0.441f, 0.441f};
f32 lbl_804B1EA8[9] = {0.15f, 1.0f, 0.7f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f};
f32 lbl_804B1ECC[9] = {0.3f, 1.0f, 0.7f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f, 0.882f};

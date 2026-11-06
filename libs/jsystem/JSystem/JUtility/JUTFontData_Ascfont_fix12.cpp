#include "types.h"

// JUtility's built-in ASCII font (FONTbfn1, 12-pixel fixed width; mkdd's
// JSystem/JUtility/FontData/Ascfont_fix12.bfn), used by JFWSystem as the
// system font. A byte-exact asset file, extracted from the original at build
// time (config.yml extract:, blobs.txt). JUTFont.h declares it as a ResFONT;
// tp and tww define it as a 32-byte-aligned u8 array, as here (tww's
// JUTFontData_Ascfont_fix12.h declaration; extern, or the const has internal
// linkage).
extern const u8 JUTResFONT_Ascfont_fix12[];

const u8 JUTResFONT_Ascfont_fix12[0x4160] ATTRIBUTE_ALIGN(32) = {
#include "assets/JSystem/JUtility/Ascfont_fix12.inc"
};

# dolphin/gx/GXTexture.c

Baseline (progress.py): total: 1132 / 11560 functions, 202096 / 4434096 bytes of code (4.56 %)
After:                  total: 1136 / 11560 functions, 204084 / 4434096 bytes of code (4.60 %)

Status: 19/19 functions, .text/.data/.sdata/.sdata2 100 %; configure.py flag -> Matching. gate: ok.

## What made it match
- .text range was short at both ends: start 0x8005D618 -> 0x8005D4BC (fn_8005D4BC = GXGetTexBufferSize),
  end 0x8005E150 -> 0x8005E4A4 (__GXSetTmemConfig, already in source, now counted).
- .data split added 0x80497288..0x804974AC: three switch jump tables
  (GXGetTexBufferSize, __GetImageTileCount, GXInitTexObj; all the __GXGetTexTileShift switch).
  lbl_804974AC (4 zero bytes) left out: alignment padding before GXTev's 8-aligned .data.
- GXGetTexBufferSize added from dolsdk2004 (asserts dropped, as in the release build); it was
  dead-stripped from oot-gc so the import lacked it.

## Names
- fn_8005D4BC -> GXGetTexBufferSize: args (u16 w, u16 h, u32 fmt, u8 mipmap, u8 max_lod), the
  61-entry tile-shift switch, 0x40/0x20 tile bytes for fmt 6/0x16 (RGBA8/Z24X8), mip loop;
  called from JUTTexture. Matches dolsdk2004's body byte for byte.

SHARED-EDITS: none.

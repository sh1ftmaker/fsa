# Dolphin SDK GX (from zeldaret/oot-gc)

Copied from https://github.com/zeldaret/oot-gc (CC0, see LICENSE) at commit
6e449e1748a02c12b4cb218453c529175b85364a: `src/dolphin/gx` -> `libs/sdk_gx/dolphin/gx`, plus `include/` and `libc/`.

Built with `-DVERSION=5` (CE_U), which selects the "Jul 23 2003 11:27:39" GX
build that FSA links. doldecomp/mkdd's GX (used for the rest of libs/sdk) is
not matched upstream, so GX comes from here. Compiled with its own include
roots (`cflags_sdk_gx` in configure.py).

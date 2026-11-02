# Dolphin SDK (from doldecomp/mkdd)

Source and headers copied from https://github.com/doldecomp/mkdd (CC0, see
LICENSE) at commit b56cc7677fa23058eff8f38784479c3b4b854060:

- `libs/sdk/dolphin/` <- mkdd `libs/dolphin/` (unused libs removed)
- `libs/sdk/include/` <- mkdd `include/dolphin/`, `types.h`, `macros.h`, `std/math.h`
- `libs/sdk/msl_include/` <- mkdd `libs/PowerPC_EABI_Support/include/`

Mario Kart: Double Dash!! links the same Dolphin SDK builds as FSA (OS/GX
Jul 23 2003, PAD Aug 6 2003, VI/SI/EXI/AI/AR/DSP/CARD Apr 17 2003). FSA's DVD
is a later build (Sep 16 2003). Files were mapped onto FSA's DOL with
`tools/donor_match.py` / `tools/donor_plan.py` and verified with objdiff.

These are compiled with their own include paths (see `cflags_sdk` in
configure.py) so they don't mix with the TWW-derived headers in `include/`.

`libs/sdk/dolphin/thp/` and `include/dolphin/thp.h` come from
https://github.com/doldecomp/sms (CC0) instead: FSA has that older THP
(no `OSRegisterVersion`). FSA builds it inside a game translation unit as
C++; see the THP entry in `docs/LIBMAP.md` for the flags and the changes.

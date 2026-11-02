# dolphin/exi/EXIBios.c

Status: Matching. 23/23 functions, .text/.data/.bss/.sdata/.sbss 100 %.

Was 15/19 (plus misses that were really split/symbol problems). Nothing in the
source changed; all fixes were in config:

- splits.txt: .text start 0x80071F64 -> 0x80071C14 (static SetExiInterruptMask
  and EXIImm were left in an auto gap, so calls to SetExiInterruptMask got the
  suffixed global name) and end 0x80073218 -> 0x800735F0 (UnlockedHandler and
  EXIGetID, which sit before EXIUart's ProbeBarnacle).
- symbols.txt: fn_800675AC -> __div2i (Runtime: between __div2u and __mod2u
  in runtime.c order; the body negates both 64-bit operands by sign, i.e. signed
  64-bit division; __EXIProbe calls it for OSTicksToSeconds on an s64).
- symbols.txt: placeholder blob lbl_804982D0 (0x110) split into the version
  string @3 and EXIGetTypeString's string literals @435..@446 (EXIGetType and
  EXIGetTypeString are dead-stripped; their strings remain in .data).

SHARED-EDITS: symbols.txt rename __div2i (also lifts CARDCreate, CARDStat,
OSResetSW by one function each).

Next: fn_800677C8 (after __mod2u, size 0x10C) is very likely __mod2i; not
named here because nothing in this unit calls it.

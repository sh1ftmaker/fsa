# AB report: game/tu_80401848.cpp (Opus)

Result: 13/13 functions at 100 % (honest); every section (.text, .data,
.bss, .sdata, .sbss, .ctors, extab, extabindex) 100 %; unit flipped to
Matching; full `ninja` SHA-1 OK (d86b3d9f...). Total 8731 -> 8744 (+13),
structural 9167 -> 9180. No function anywhere drops (per-address compare
against fsa-decomp's report_honest.json: 0 drops).
Checks: gate --all-units ok, gate --base b861c081 ok, gen_game --check ok,
order_check: unit in order (the 11 pre-existing out-of-order units are
unchanged and not touched).

What the unit is: FSA's DVD loader thread, the same design as TWW's
m_Do_dvd_thread.cpp (structure only). Command base cls_804B0FD0 (mNext at
0, done byte at 4, vptr at 8, pure vfunc_0xC), commands cls_804B0FA0
(loadToMainRAM), cls_804B0FB0 (JKRAramArchive::mountFixed), cls_804B0FC0
(JKRMemArchive), their creators fn_804019A8 / fn_80401BA4 / fn_80401E10, a
mutex-guarded queue (cls_80402054, inlined), fn_80401FEC (creates the
thread: OSThread 0x80528980, 0x1000 stack 0x80528CA0, queue 0x80529CA0),
fn_80402054 (thread loop), __sinit.

Proved / fixed:
- vptr placement: members declared before the virtuals put the vptr at 0x8.
- Queue loop written in fn_80402054 itself (inlining a member mainLoop
  hoists member addresses into extra registers); `cb(&cmd)` (TWW form)
  gives the stack slot the original keeps cmd in.
- Class operator new(size_t) / delete inline; the compiler emits an
  unreferenced weak `__dl__12cls_804B0FD0FPv` that the map lists UNUSED.
- Data: .bss split at proven boundaries (thread 0x318, stack 0x1000 at
  +0x320, queue 0x44 at +0x1320: the code adds those offsets separately),
  9 sinit chains @976..@984 and 9 weak memPool_ copies (0x10 each);
  lbl_8053A218 is a 1-byte u8 (=1); .sbss ends at 0x8053ADF8
  (0x8053ADF8 / 0x8053AE00 are other units' singleton pointers).
- Unit boundary: fn_80401848 (cls_804B0F90 dtor) + vtable 0x804B0F90 (+ its
  extab entry) belong to tu_80400DC4: the vtable is 0xC + 4 bytes padding,
  only tu_80400DC4 uses the class, and its inline dtor's weak copy is
  emitted right after its referencer, that unit's __sinit. Moved in
  splits.txt (SHARED-EDIT); the trial link proves the result byte-identical.

SHARED-EDITS: include/game/GameGlobals.h (command classes' layout/types;
fn_804019A8 / fn_80401E10 / fn_80401BA4 / fn_80401FEC now C++ with real
parameter types, symbols renamed); splits.txt stanza of tu_80400DC4;
symbols.txt renames/splits for this unit.

Left: nothing in this unit. Follow-ups: __vt__12cls_804B0F90 is 0xC (symbol
still says 0x10); 0x8053ADF8 / 0x8053AE00 want sbss stanzas in
tu_80403D84 / tu_80414F6C; names are placeholders (cls_/vfunc_0xC) since
the TWW names are structural evidence only.

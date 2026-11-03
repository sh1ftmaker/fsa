# game/act_tutorial_old_man.cpp

Baseline: total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
Now:      total: 4449 / 11561 functions, 799900 / 4442088 bytes of code (18.01 %) (after rebase onto matching 3729a91)

## Status: 5/5 functions (code 656/656), NonMatching (.text/.data/.bss 100 %; extab partial)

- `TutorialOldMan_804AE508` (actor TJIJ, "Tutorial old man" in actors.tsv;
  named after its vtable 0x804AE508, which the spawner's weak ctor
  fn_801FFC48 stores after `__ct__18ActorBase_804A5770Fv`).
- Overrides only init (0x0C), update (0x14), draw (0x20), all empty; emitted
  in source order draw, update, init, then the compiler-generated (weak)
  destructor, then `__sinit` (9 pools via game/GameAudio.h).
- No .rodata/.sdata; .data is only the vtable.

## symbols.txt (placeholder renames)
- fn_80372EA8/AC/B0 -> draw/update/init__23TutorialOldMan_804AE508Fv
- fn_80372EB4 -> __dt__23TutorialOldMan_804AE508Fv
- fn_80372F14 -> __sinit_act_tutorial_old_man_cpp (local)
- lbl_804AE508 (0x58) -> __vt__23TutorialOldMan_804AE508 (0x54; the last word is padding)
- lbl_8050CA00 (0x100) -> @430..@438 (0xC each) + lbl_8050CA6C..lbl_8050CAEC (nine 0x10 weak memPool_ copies)

## Remaining
- extab/extabindex: the 9 weak JASMemPool dtors' entries (weak-dedup report
  tooling, in review). Everything else is 100 % after the rebase onto
  matching 3729a91 (shared headers v2 named the ActorBase vtable slots).

## SHARED-EDITS
- None open (the v2 headers took this unit's requests; local TODO(shared)
  declarations removed).

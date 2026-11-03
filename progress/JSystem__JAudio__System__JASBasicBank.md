# JSystem/JAudio/System/JASBasicBank.cpp

**Update (infra/jaudio-sym): Matching.** 8/8 after JASBNKParser's merge; with the block's `-sym on` and setInst defined after getInstParam (FSA's function order: ctor, getInstParam, setInst, getInst) `link_check.py --unit` is SHA-1 OK. The notes below describe the earlier state.

Baseline: `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`
After (incl. cherry-picked memPool_ commits): 3714 / 11561, 713340 bytes. Gate ok.

Unit: 4/11 -> 5/8 functions (the old range was shifted: it started after
the ctor and ran 0x2F0 bytes into JASBasicInst). .bss/.ctors 100 %, .data 93.75 %.
The FIXME(link) about JASGenericMemPool's ctor is moot with the generic
memPool_ definition (cherry-picked); the unit is still NonMatching.

## Done (symbols/splits)
- .text 0x8002E2E0-0x8002E718 -> 0x8002E218-0x8002E428:
  0x8002E218 is the ctor (stores __vt__7JASBank then the JASBasicBank vtable),
  0x8002E428.. is JASBasicInst (its ctor stores __vt__7JASInst then
  __vt__12JASBasicInst). JASWSParser's .text now ends at 0x8002E218 and
  JASBasicInst's starts at 0x8002E428 (SHARED-EDITS).
- Names: __ct__12JASBasicBankFv, getInstParam__12JASBasicBankCFiiiP12JASInstParam
  (vtable slot 2 of 0x80491980), __vt__12JASBasicBank, __vt__12JASBasicInst,
  __ct__12JASBasicInstFv, __dt__12JASBasicInstFv / getParam__12JASBasicInstCFiiP12JASInstParam
  (vtable slots of 0x804919A8).
- .bss 0x804B2850-0x804B2860 (lbl_804B2850 0x10 -> 0xC): __sinit matches.

## Next
- JASBank.h: vtable slot 2 must be `getInstParam(int, int, int, JASInstParam*) const`
  (mkdd has getInst(u32)); then implement JASBasicBank::getInstParam (0x70)
  and fix JASVoiceBank/JASDrumSet users. Also fn_8002E2F0 (0x30), after setInst,
  is probably getInst; check its body.
- JASBasicInst (another unit) is FSA's effect-list version, like JASDrumSet's TPerc.

## SHARED-EDITS
- splits: JASWSParser .text end 0x8002E2E0 -> 0x8002E218, JASBasicInst .text
  start 0x8002E718 -> 0x8002E428; JASBasicInst symbol names above.
- memPool_ commits cherry-picked from the JASChannel branch.

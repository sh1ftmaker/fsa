# JSystem/JAudio/System/JASBNKParser.cpp

Baseline: `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`
After (incl. cherry-picked memPool_ commits): 3714 / 11561, 713340 bytes. Gate ok.

Unit: 2/16 -> 3/16 functions; .bss/.sbss/.ctors 100 %.

## Done
- __sinit matches: .bss 0x804B2870-0x804B2880 split (lbl_804B2870 0x10 -> 0xC).
- lbl_8053A348 -> sUsedHeapSize__12JASBNKParser (the dispatcher adds the heap
  delta to it, as tp/mkdd do), size 0x8 -> 0x4; .sbss 0x8053A348-0x8053A350 split.

## Not done: the unit is tp's two-version parser
FSA's layout is exactly tp's JASBNKParser.cpp:
- 0x8002F274 createBank (calls the next)
- 0x8002F294 (0xB8) createBasicBank: switches on header +0xC (version):
  1 -> 0x8002F34C, 0 -> 0x8002FB90; adds the heap delta to sUsedHeapSize.
- 0x8002F34C (0x844) Ver1::createBasicBank (stmw r14; findChunk inlined).
- 0x8002FB90 (0x6DC) Ver0::createBasicBank (what our mkdd source has).
- 0x8003026C (0x114) Ver0::findOscPtr, 0x80030380 getOscTableEndPtr.
- 0x80030394..0x80030454: nine 0x18-byte weak JSUConvertOffsetToPtr<T>
  instances (emitted in reverse order of first use; first used is at the
  highest address: TInst, TOsc, Point, ...).
Next step: port tp's JASBNKParser.cpp/.h (Ver0/Ver1 namespaces). Caveat:
getOscTableEndPtr is already named `getOscTableEndPtr__12JASBNKParserF...`
(mkdd, no Ver0 namespace); tp has it in Ver0. Changing a real name needs the
coordinator (approved_symbol_changes.txt). tp's Ver1 code also needs
JASBasicInst/JASDrumSet APIs our mkdd headers may lack.

## SHARED-EDITS
- memPool_ commits cherry-picked from the JASChannel branch.

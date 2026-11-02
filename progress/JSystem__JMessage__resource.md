# JSystem/JMessage/resource.cpp

Baseline (start): `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`

Now: `total: 3715 / 11561 functions, 711148 / 4442088 bytes of code (16.01 %)`

Unit: 10/16 -> 15/16 functions; .text 100 % except one reloc, .data 100 %.
Stays NonMatching until the linklist SHARED-EDIT below lands (then 16/16, Matching).

## Done
- symbols.txt (evidence: our compiled object's relocations / vtable slots and the donor order):
  - fn_80077278 -> `Do_destroy__...TCResourceFPQ28JMessage9TResource` (only `delete`; slot after Do_create, tp/pikmin2 order)
  - dtor_8007734C -> `__dt__Q28JMessage6TParseFv` (stores `__vt__Q28JMessage6TParse`)
  - dtor_800776D4 -> `__dt__Q28JMessage12TParse_colorFv` (stores `__vt__Q28JMessage12TParse_color`)
  - fn_80087714 -> `__dt__Q37JGadget6binary19TParse_header_blockFv` (stores `__vt__...TParse_header_block`; called by both TParse dtors)
  - lbl_8053B6B8/BC -> `ga4cSignature__Q28JMessage4data` / `ga4cSignature_color__Q28JMessage4data` ('MESG'/'MGCL', memcmp args in parseHeader_next)
- splits.txt: dropped resource's `.sdata2 0x8053B6B8..C0`: the signatures are defined in
  JMessage/data.cpp in every donor (tp, tww, pikmin2), not in resource.cpp. tww splits data.cpp as an sdata2-only unit.
- Removed the out-of-line `TCResource::TCResource()` (FSA has no such function; .data stays 100 % without it).
- `toMessageIndex_messageID` is not in FSA's range; kept in source (dead-stripped at link, gate ok).

## Resistant
- `__dt__Q38JMessage18TResourceContainer10TCResourceFv` 99.83 %: bytes identical, only reloc
  `bl fn_80087868` vs ours `bl __dt__Q27JGadget13TNodeLinkListFv`. Fixed by the SHARED-EDIT.

## SHARED-EDITS (verified locally: gate ok with resource Matching, total 3720 / 711560)
1. symbols.txt: `fn_80087868` -> `__dt__Q27JGadget13TNodeLinkListFv` (empty dtor + conditional delete; our TCResource dtor calls it at the same spot),
   `fn_800878A4` -> `erase__Q27JGadget13TNodeLinkListFQ37JGadget13TNodeLinkList8iterator` (builds it/next, calls erase(it,it)).
2. splits.txt: `JGadget/binary.cpp` .text end 0x800878F0 -> 0x80087868; `JGadget/linklist.cpp` .text start 0x800878F0 -> 0x80087868.
3. libs/jsystem/JSystem/JGadget/linklist.cpp: move `erase(iterator)` before `erase(iterator, iterator)` (tp's linklist.cpp order, and FSA's order 0x800878A4 < 0x800878F0).
   Without 2+3, renaming fn_80087868 makes Matching linklist.cpp a duplicate definition (link fails).
4. configure.py: resource.cpp -> Matching after 1-3.
5. A `JSystem/JMessage/data.cpp` unit owning `.sdata2 0x8053B6B8..0x8053B6C0` (tp's data.cpp: `const u32 ga4cSignature = 'MESG'; ... 'MGCL'`).
6. Also noted: binary.cpp's .text probably starts at 0x80087714 (its dtor), not 0x8008775C.

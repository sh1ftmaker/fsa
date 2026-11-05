
## Small-function pass (Sonnet 5.5)
Baseline: 7547 / 11557 functions, unit 24/254. After: 7649 / 11557, unit 126/254 (+102, no other unit dropped).
Full ninja: dol sha1 equals build.sha1; gate --all-units / --base matching ok; gen_game --check ok; order_check ok.

Matched: 100 getters/setters/forwarders (<=128 B), fields added to Player.h (splits of the u8 hole arrays),
cls_804A71A8 fields 0x4A-0x4F/0x98/0x99/0xB4 and fn_80269420/fn_8026942C, cls_804AA8C8::fn_8030A3B0.

Resistant:
- fn_80241920 (86.8 %, STOP): original `x & ((x >>> 31) - 1)` (srwi/subi/and), ours srawi/andc for `x < 0 ? 0 : x`.
- fn_802517E4 (88 %): original returns u8 (extrwi r3,r0,8,19); u8 would break wrapper fn_8022D894 (tu_8022AD64, bool pass-through).
- fn_80246F28 (94.8 %, from before).

Unwritten small functions and why:
- fn_8024FEFC: the original calls fn_8024FEBC/fn_8024FE78 out of line; MWCC inlines them here.
- fn_80248FF0: inlines fn_80249070 (defined later in the file).
- fn_8024ECCC: contains an infinite loop (`b .`) assert; not guessed.
- fn_80241B44/DD0/E28, fn_80252F50/5C/68/74: use field_0xbec as a cls_804AA8D8* (game/tu_8030AE84.h) but fn_8023FC24 (matched) returns it as s32 via fn_8022BCD4 (tu_8022AD64); needs field type + wrapper return changed together. DD0/E28 also copy a TVec3f to the stack and pass it as a TVec2f*.
- fn_802422C4, fn_80252F48, fn_8025196C: result types unknown (no callers' evidence).
- __dt__12cls_804B20F8: defining it out of line removes tu_8045B010's @24@__dt__ thunk (tried, reverted).

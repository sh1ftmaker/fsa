# dolphin/card/CARDCreate.c

Status: Matching (3/3 functions, .text 100 %).

- CreateCallbackFat, CARDCreateAsync: code already right; only relocations differed.
- CARDCreate: was an unsplit placeholder fn_80057A48 (calls CARDCreateAsync with
  __CARDSyncCallback, then __CARDSync); split end extended 0x80057A48 -> 0x80057A90.

symbols.txt renames (evidence):
- fn_80057A48 -> CARDCreate (body above; same as every donor's CARDCreate).
- fn_800675AC -> __div2i (runtime.c order __div2u/__div2i/__mod2u, size 0x138 as in
  mkdd/tp; called from OSTicksToSeconds(OSGetTime()) s64 divide).
- fn_8006D97C -> strncpy (MSL string.c order strcat 0x2C / strncpy 0x44 / strcpy 0xB8 /
  strlen 0x1C, identical sizes and order to mkdd/tp; body is strncpy's).

SHARED-EDITS: none beyond the symbols.txt renames above (they touch runtime.c / MSL string.c
symbols; progress total rose 1098 -> 1104).

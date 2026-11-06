# game/act_hyrule_castle_barrier.cpp

## Baseline
- `python3 tools/donor_leads.py game/act_hyrule_castle_barrier.cpp`: `NO LEAD`
- Baseline functions matched: 3/8 (`__ct__28HyruleCastleBarrier_8049DEE8Fv`, `__dt__28HyruleCastleBarrier_8049DEE8Fv`, `__sinit_act_hyrule_castle_barrier_cpp` at 100%; `init__28HyruleCastleBarrier_8049DEE8Fv` at 99.41% `reloc-name`; 4 functions unwritten).
- Baseline project progress (`python3 tools/progress.py | head -1`): `total: 9000 / 11557 functions, 1846220 / 4442032 bytes of code (41.56 %)`

## Current Status
- Functions matched: 5/8 at 100.00%, 3/8 instruction-exact at 99.71%–99.90% (`reloc-name` only).
- `HyruleCastleBarrier_8049DEE8::__ct` (`0x8011FF00`): 100.00%
- `HyruleCastleBarrier_8049DEE8::__dt` (`0x8011FE5C`): 100.00%
- `HyruleCastleBarrier_8049DEE8::init` (`0x8011FAF0`): 99.90% (`reloc-name` — `.sdata2` float literal pool order)
- `HyruleCastleBarrier_8049DEE8::update` (`0x8011F850`): 100.00%
- `HyruleCastleBarrier_8049DEE8::fn_8011F5C0` (`0x8011F5C0`): 100.00%
- `HyruleCastleBarrier_8049DEE8::fn_8011F314` (`0x8011F314`): 99.71% (`reloc-name` — `.sdata2` float literal pool order)
- `HyruleCastleBarrier_8049DEE8::fn_8011EEEC` (`0x8011EEEC`): 99.77% (`reloc-name` — `.sdata2` float literal pool order)
- `__sinit_act_hyrule_castle_barrier_cpp` (`0x8011FF6C`): 100.00%

## Resistant Functions (Instruction-Exact, `.sdata2` Pool Order Only)
- `fn_8011EEEC__28HyruleCastleBarrier_8049DEE8Fv` (99.77%, `reloc-name`), `fn_8011F314__28HyruleCastleBarrier_8049DEE8Fv` (99.71%, `reloc-name`), `init__28HyruleCastleBarrier_8049DEE8Fv` (99.90%, `reloc-name`):
  - Every instruction and register matches 100%; only `.sdata2` float literal offsets (`16.0f`, `0.0f`, `4503601774854144.0` vs `1.0f`, `1.5707964f`) differ in pool emission order.

## SHARED-EDITS
- None (only unit-private header `include/game/act/act_hyrule_castle_barrier.h` was updated).

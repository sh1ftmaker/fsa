# JSystem/J3DU/J3DUDL.cpp

Status: NonMatching, 0/1. Split added (.text 0x80079034-0x8007903C, no data);
source is mkdd's empty J3DUDL.cpp.

- All that survives of this TU in FSA is the weak
  J3DShapeMtx::getUseMtxNum the link kept (pikmin2 has the same lone copy
  in its J3DUDL.cpp split, right before J3DUMotion.cpp).
- Both donors make the weak copy appear with an invented function: tp's
  J3DUDL.cpp says "Fake function to make the weak function
  J3DShapeMtx::getUseMtxNum appear in this TU", pikmin2's says "Fabricated
  to generate getUseMtxNum weak function". That is a fakematch under this
  project's rules, so it is not used. The unit can match once J3DUDL's real
  (dead-stripped) code is known.
- configure.py: J3DU block after mkdd_JMessage (also added by
  J3DUMotion.cpp's branch; merge keeps one block with both lines, J3DUDL
  first). lib_map.tsv rows come with J3DUMotion.cpp's branch.

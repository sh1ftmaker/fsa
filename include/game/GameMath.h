#ifndef GAME_GAMEMATH_H
#define GAME_GAMEMATH_H

// Small scalar math helpers of FSA's own game code.
//
// Owner: the shared-header owner (docs/COORDINATOR.md).

#include "types.h"

// Provisional name (no symbol: always inlined). Evidence that FSA squared
// through an inline call: under -fp_contract on, MWCC (GC/2.6) contracts
// a * b + c into fmadds only while the product is a subexpression of the same
// expression; a product that reaches the add through an inline's return value
// (or a named local) stays fmuls; fmuls; fadds. The game asm has 104 unfused
// sq + sq sites (fmuls a,a; fmuls b,b; fadds) in 86 functions across 52 units,
// beside 132 fused ones (JGeometry dot()/squared(), which stay one
// expression); 17 units have both, so it is the source construct, not a flag.
// No licensed donor has such an inline (tww/tp's SQUARE and bfbb's SQR are
// macros, which fuse), so this is FSA's own helper. Write
// square(dx) + square(dy) where the target shows the unfused form; keep
// JGeometry squared()/dot() where it shows fmadds.
inline f32 square(f32 x) { return x * x; }

#endif

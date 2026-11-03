#!/usr/bin/env python3
"""Template generator for the biggest game clone families (GAME_PLAN G2/G3).

Families (templates: game/act_vaati_buster.cpp and, for fwd,
game/tu_8022AD64.cpp, hand-matched and reviewed):

  sinit  the 9-pool JAudio `__sinit` (0x224). Source: `#include
         "game/GameAudio.h"` first in the unit. symbols.txt: the function
         becomes `__sinit_<unit>_cpp` (scope:local) and the unit's 0x100 .bss
         placeholder becomes the 9 `@NNN` 0xC destructor-chain objects our
         compiled object emits, 9 unnamed 0x10 `lbl_` memPool_ copies and 4
         unsplit pad bytes (docs/COORDINATOR.md, "Game units").
  dtor   the simple actor destructor `cls::~cls() {}` (0x60) of a direct
         19-slot child of ActorBase_804A5770. The class goes in
         include/game/act/<unit>.h with its inline `Foo() {}` constructor, a
         declaration of every slot its vtable overrides (slots that differ
         from ActorBase's vtable, signatures from ActorBase.h), and padding up
         to the size every `new` of it allocates (`li r3, N` before the
         `__nw__` call preceding each call of a function that stores the
         vtable; "unknown" when there is none or they disagree). Name:
         `<EnglishName>_<vtable>` when config/G4SE01/game_units.tsv ties one
         of its constructors to exactly one actor ID whose actors.tsv name is
         plain words at high/med confidence, else `cls_<vtable>`.
         symbols.txt: the destructor, the vtable (0x58 -> 0x54, the last word
         is .data padding) and every overriding slot function get the names
         our compiled object emits; units whose names any other source or
         header spells out are skipped.

  fwd    the per-player forwarders of tu_8022AD64 (templates fn_8022B7C4 and
         fn_8022B860 there): `if (lbl_8053AC90 == NULL) return <fallback>;
         if (isPlayerActive(player)) return mPlayers[player]-><getter>(...);
         return <fallback>;` and its void form. The forwarder and its getter
         (a new Player_804A6F38 member) are declared in include/game/Player.h;
         symbols.txt renames both to the names our object emits. Types and
         the scripted check: see "family fwd" below. Run it as
         `gen_game.py tu_8022AD64`; manifest rows: function = forwarder,
         class = getter, vtable `-`, hdr_before `-` (Player.h is shared: the
         check requires each declaration once, not the whole file).

A unit qualifies for a family only if the target function is the template
up to identifiers: the same bytes outside relocation fields and the same
relocations (offset, type, addend, target), where only targets defined in
the unit itself (the vtable, the .bss chain) may be named differently.
Variants are skipped and listed (`--scan`).

Usage:
  python3 tools/gen_game.py --scan [--list]       families and skipped variants over all game units
  python3 tools/gen_game.py <unit> [<unit> ...]   generate, verify, back out failures
  python3 tools/gen_game.py --check [<unit> ...]  identifier-only check of every generated member
                                                  (default: all units in config/G4SE01/gen_game.tsv)

<unit> is `game/act_foo.cpp`, `act_foo` or `src/game/act_foo.cpp`.

Generation: skips units claimed in progress/claims.json; writes the
source/header (a new source file holds only the includes and the
destructors; an existing one gets only the GameAudio.h include, the header
include and the definitions, and is refused when a comment may describe the
generated code as unwritten); compiles each unit, reads the @NNN, sinit and
mangled names back from our object, edits each unit's own lines of
symbols.txt in place (never re-sorts it), re-splits, recompiles and runs
the member checks below plus objdiff on every unit. A unit with any
problem, or where a function that matched before drops, is restored
(source, header, symbols.txt lines) and the split redone. Generated members
are recorded in config/G4SE01/gen_game.tsv with the git blob of each file
before generation (`new` when the generator created it).

--check (for the reviewer and the gate; whole manifest by default, objdiff
runs in parallel), per recorded member:
  1. the target function (dtk's split of the original) is the template's
     target up to identifiers, and our compiled function is the template's
     compiled function up to identifiers;
  2. objdiff gives it 100 %;
  3. symbols.txt: sinit, the unit's .bss in dtk's split is exactly our 9
     @NNN (0xC, each 100 %), then 9 unnamed lbl_<addr> of 0x10 at
     +0x6C..+0xEC, then 4 unsplit bytes; dtor, the vtable it stores has our
     vtable's name, size 0x54, every slot's relocation target named as in
     our vtable, and 100 %;
  4. the files: a file the generator created equals its rendering exactly;
     an existing file equals the generator's edit re-run on its recorded
     pre-generation blob, so nothing but the rendered lines was added.
It prints one line per unit and `check: ok` or the problems; exit 1 on any.

Lifecycle: when a later worker hand-edits a generated member (fills in the
class's fields, rewrites the destructor, or edits anything else in a file
the generator wrote), that worker's commit deletes all of the unit's rows
from config/G4SE01/gen_game.tsv, and the member then gets normal review.
The file is protected: tools/gate.py accepts a change to it only when it
deletes whole rows and adds, edits or reorders nothing (deleting only some
of a unit's rows fails --check, since the files are checked against the
remaining rows). New rows come only from a generator batch merged by the
coordinator.
"""

import json
import re
import struct
import subprocess
import textwrap
from concurrent.futures import ThreadPoolExecutor
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from gate import Elf  # noqa: E402  (read-only reuse of the gate's ELF reader)

SYMBOLS = Path("config/G4SE01/symbols.txt")
SPLITS = Path("config/G4SE01/splits.txt")
UNITS_TSV = Path("config/G4SE01/game_units.tsv")
ACTORS_TSV = Path("docs/names/actors.tsv")
MANIFEST = Path("config/G4SE01/gen_game.tsv")
OBJDIFF = "build/tools/objdiff-cli"
TEMPLATE_UNIT = "act_vaati_buster"
TEMPLATE_DTOR = "__dt__20VaatiBuster_8049E3A8Fv"
TEMPLATE_SINIT = "__sinit_act_vaati_buster_cpp"
ACTORBASE_VT = 0x804A5770
SPAWNER_UNIT = "tu_801F9484"
ACTOR_BASE_SIZE = 0x230
VT_SLOTS = 19
VT_SIZE = 8 + 4 * VT_SLOTS  # 0x54
BSS_SIZE = 0x100            # 9 x 0xC chain + 9 x 0x10 pools + 4 pad
N_POOLS = 9

# ActorBase_804A5770 slot declarations (include/game/ActorBase.h). The slot
# offset is into the vtable (two header words, slot 0x08 = destructor).
SLOT_DECLS = {
    0x0C: "virtual void init();",
    0x10: "virtual void vfunc_0x10();",
    0x14: "virtual void update();",
    0x18: "virtual void vfunc_0x18();",
    0x1C: "virtual void savePrevPos();",
    0x20: "virtual void draw();",
    0x24: "virtual void vfunc_0x24();",
    0x28: "virtual void vfunc_0x28(s32, f32, f32);",
    0x2C: "virtual bool vfunc_0x2C(f32 x, f32 y, f32 scale);",
    0x30: "virtual void vfunc_0x30(u8, bool);",
    0x34: "virtual bool vfunc_0x34();",
    0x38: "virtual s32 vfunc_0x38();",
    0x3C: "virtual s32 vfunc_0x3C();",
    0x40: "virtual void vfunc_0x40();",
    0x44: "virtual void vfunc_0x44();",
    0x48: "virtual void vfunc_0x48();",
    0x4C: "virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);",
    0x50: "virtual bool vfunc_0x50(s32 layer, const JGeometry::TVec3f* pos);",
}

# ---------------------------------------------------------------- templates (the reviewed text)

SINIT_INCLUDE = '#include "game/GameAudio.h"'

HEADER = """\
#ifndef {guard}
#define {guard}

// Code: game/{unit}.cpp.

#include "game/ActorBase.h"

{classblock}
#endif
"""

CLASS_BLOCK = """\
{naming}
class {cls} : public ActorBase_804A5770 {{
public:
    {cls}() {{}}
    /* 0x08 */ virtual ~{cls}();
{slots}{fields}}}; // Size: {size}
"""

SLOT_LINE = "    /* 0x{off:02X} */ {decl}\n"
PAD_LINE = "    /* 0x{off:03X} */ u8 field_0x{off:x}[0x{n:X}]; // not yet known\n"
DTOR_DEF = "{cls}::~{cls}() {{}}\n"

# ---------------------------------------------------------------- helpers


def die(msg):
    print(msg)
    sys.exit(1)


def run(cmd, timeout=1800):
    return subprocess.run(["timeout", str(timeout), *cmd], capture_output=True, text=True)


def unit_slug(arg):
    a = arg.strip()
    a = re.sub(r"^(src/)?game/", "", a)
    a = re.sub(r"^fsa_game_code/game/", "", a)
    return re.sub(r"\.cpp$", "", a)


def target_obj(u):
    return Path(f"build/G4SE01/obj/game/{u}.o")


def our_obj(u):
    return Path(f"build/G4SE01/src/game/{u}.o")


def src_path(u):
    return Path(f"src/game/{u}.cpp")


def hdr_path(u):
    return Path(f"include/game/act/{u}.h")


def game_units():
    rows = UNITS_TSV.read_text().splitlines()[1:]
    return [unit_slug(r.split("\t")[0]) for r in rows if r.strip()]


class Obj:
    """Functions, data symbols and typed relocations of one ELF object."""

    def __init__(self, path):
        self.e = e = Elf(path)
        self.secname = e.names
        self.relocs = {}  # section index -> [(off, type, sym index, addend)]
        for i, h in enumerate(e.sh):
            if h[1] == 4:
                self.relocs.setdefault(h[7], []).extend(
                    (off, info & 0xFF, info >> 8, add) for off, info, add in e.rela(i))
        self.syms = e.syms  # (name, value, size, bind, type, shndx, ...)

    def sec(self, name):
        return next((i for i, n in enumerate(self.secname) if n == name), None)

    def defined(self, secname=None, kind=None):
        k = self.sec(secname) if secname else None
        return [(i, s) for i, s in enumerate(self.syms)
                if s[5] not in (0, 0xFFF1, 0xFFF2) and s[0] and (k is None or s[5] == k)
                and (kind is None or s[4] == kind)]

    def func(self, name):
        for i, s in self.defined(".text", 2):
            if s[0] == name:
                return i, s
        return None

    def funcs(self):
        return sorted((s for _, s in self.defined(".text", 2)), key=lambda s: s[1])

    def bytes_of(self, s):
        return self.e.data(s[5])[s[1]:s[1] + s[2]]

    def relocs_in(self, shndx, start, size):
        return sorted(r for r in self.relocs.get(shndx, []) if start <= r[0] < start + size)


MASKS = {1: (0, 0xFFFFFFFF), 4: (2, 0xFFFF), 5: (2, 0xFFFF), 6: (2, 0xFFFF),
         10: (0, 0x03FFFFFC), 109: (0, 0x001FFFFF)}


def fingerprint(o, s, roles):
    """Identifier-only form of function s: masked bytes + relocations whose
    targets are named, except targets defined (non-weak) in this object,
    which become their role (`roles(o, sym)`)."""
    code = bytearray(o.bytes_of(s))
    rel = []
    for off, typ, si, add in o.relocs_in(s[5], s[1], s[2]):
        rel_off = off - s[1]
        if MASKS.get(typ, (0,))[0] == 2:  # 16-bit field at the relocation offset
            code[rel_off:rel_off + 2] = b"\0\0"
        else:
            w = rel_off & ~3
            m = MASKS.get(typ, (0, 0xFFFFFFFF))[1]
            v, = struct.unpack_from(">I", code, w)
            struct.pack_into(">I", code, w, v & ~m)
        t = o.syms[si]
        local_def = t[5] not in (0, 0xFFF1, 0xFFF2) and t[3] != 2
        name = roles(o, t) if local_def else t[0]
        rel.append((rel_off, typ, name, add))
    return bytes(code), rel


def role_of(o, t):
    n = o.secname[t[5]]
    if n == ".data":
        return "<this unit's vtable>"
    if n == ".bss":
        return f"<this unit's .bss+0x{t[1]:X}>"
    return f"<this unit's {n} {t[0]}>"


_tmpl = {}


def template(kind, ours=False):
    key = (kind, ours)
    if key not in _tmpl:
        o = Obj(our_obj(TEMPLATE_UNIT) if ours else target_obj(TEMPLATE_UNIT))
        name = TEMPLATE_SINIT if kind == "sinit" else TEMPLATE_DTOR
        f = o.func(name)
        if not f:
            die(f"template {name} not in {TEMPLATE_UNIT}'s {'compiled' if ours else 'target'} object")
        _tmpl[key] = fingerprint(o, f[1], role_of)
    return _tmpl[key]


def same_as_template(o, s, kind, ours=False):
    return s[2] == len(template(kind, ours)[0]) and fingerprint(o, s, role_of) == template(kind, ours)


def symbols_lines():
    return SYMBOLS.read_text().splitlines(keepends=True)


SYMLINE = re.compile(r"^(\S+) = (\.\w+):0x([0-9A-Fa-f]+); // (.*)$")


def actor_names():
    out = {}
    for r in ACTORS_TSV.read_text().splitlines()[1:]:
        c = r.split("\t")
        if len(c) > 5:
            out[c[0]] = (c[2], c[5])
    return out


def evidence_for(unit):
    for r in UNITS_TSV.read_text().splitlines()[1:]:
        c = r.split("\t")
        if unit_slug(c[0]) == unit:
            return c[5] if len(c) > 5 else ""
    return ""


_objs = {}


def obj(unit):
    if unit not in _objs:
        _objs[unit] = Obj(target_obj(unit))
    return _objs[unit]


def ctors_of(unit, vt_name, dtor_name):
    """Names of the functions that store this vtable other than its dtor:
    the class's own constructor in the unit, or the spawner's inline copy."""
    out = set()
    for u in (unit, SPAWNER_UNIT):
        o = obj(u)
        t = o.sec(".text")
        for off, typ, si, add in o.relocs.get(t, []):
            if o.syms[si][0] == vt_name and typ == 6:
                f = next((s for s in o.funcs() if s[1] <= off < s[1] + s[2]), None)
                if f and f[0] != dtor_name:
                    out.add(f[0])
    return out


_calls = None


def call_sites():
    """callee name -> [(unit, offset of the bl)] over all game units."""
    global _calls
    if _calls is None:
        _calls = {}
        for u in game_units():
            if not target_obj(u).exists():
                continue
            o = obj(u)
            t = o.sec(".text")
            for off, typ, si, add in o.relocs.get(t, []):
                if typ == 10:
                    _calls.setdefault(o.syms[si][0], []).append((u, off))
    return _calls


def alloc_size(ctors):
    """The size every `new` of this class allocates: `li r3, N` before the
    `__nw__` call that precedes each call of one of its constructors."""
    sizes = set()
    for c in ctors:
        for u, off in call_sites().get(c, []):
            o = obj(u)
            t = o.sec(".text")
            code = o.e.data(t)
            rels = {r[0]: o.syms[r[2]][0] for r in o.relocs.get(t, []) if r[1] == 10}
            k, seen_nw, size = off - 4, False, None
            while k >= max(0, off - 0x40) and size is None:
                if rels.get(k, "").startswith("__nw__"):
                    seen_nw = True
                elif seen_nw and k not in rels:
                    w, = struct.unpack_from(">I", code, k)
                    if w >> 16 == 0x3860:  # li r3, N
                        size = w & 0xFFFF
                elif k in rels and not seen_nw:
                    break  # another call between: not this ctor's allocation
                k -= 4
            sizes.add(size)
    sizes.discard(None)  # calls not preceded by a `new` (e.g. a base-class ctor call)
    if len(sizes) == 1:
        return sizes.pop(), None
    if not sizes:
        return None, "unknown, no `new` of it found"
    return None, "unknown, its `new`s allocate " + ", ".join(f"0x{n:X}" for n in sorted(sizes))


def class_name(unit, vt, ctors):
    """`<EnglishName>_<vtable>` when game_units.tsv ties one of this class's
    constructors to exactly one actor ID whose actors.tsv name is plain words
    (high/med confidence); `cls_<vtable>` otherwise. Returns (name, why)."""
    ev = evidence_for(unit)
    addrs = {c[3:].upper() for c in ctors if re.fullmatch(r"fn_[0-9A-Fa-f]{8}", c)}
    # "ID (vtable V set by ctor C)" with V this vtable, or "ID (ctor C)" with C
    # one of the functions that store it
    hits = [(i, c) for i, v, c in re.findall(r"([A-Z0-9_]{4}) \((?:vtable ([0-9A-F]{8}) set by )?ctor ([0-9A-F]{8})\)", ev)
            if c in addrs and (not v or int(v, 16) == vt)]
    names = actor_names()
    if len({i for i, _ in hits}) == 1 and hits[0][0] in names:
        aid, c = hits[0]
        eng, conf = names[aid]
        if conf in ("high", "med") and re.fullmatch(r"[A-Za-z][A-Za-z0-9 ]*", eng):
            camel = "".join(w[:1].upper() + w[1:] for w in eng.split())
            return (f"{camel}_{vt:08X}",
                    f"The {aid} actor (\"{eng}\", docs/names/actors.tsv, {conf} confidence):"
                    f" the spawner constructs it with fn_{c}, which stores this vtable"
                    f" (config/G4SE01/game_units.tsv). Named after its vtable, 0x{vt:08X}.")
    return f"cls_{vt:08X}", f"Named after its vtable, 0x{vt:08X}."


def text_base(unit):
    for l in SPLITS.read_text().split("\n\n"):
        if l.startswith(f"game/{unit}.cpp:"):
            m = re.search(r"\.text\s+start:0x([0-9A-F]+)", l)
            return int(m[1], 16)
    return 0


# ---------------------------------------------------------------- analysis


def analyse(unit):
    """What the generator could emit for one unit, from dtk's split of the
    original. Returns {"sinit": {...} | None, "dtor": [...], "skip": [reasons]}."""
    res = {"unit": unit, "sinit": None, "dtor": [], "skip": [], "done": []}
    p = target_obj(unit)
    if not p.exists():
        res["skip"].append("no target object")
        return res
    if unit == SPAWNER_UNIT:
        res["skip"].append("the spawner unit (its weak inline constructors are G5 work)")
        return res
    o = Obj(p)
    ctors = o.sec(".ctors")
    sinit_syms = set()
    if ctors is not None:
        for off, typ, si, add in o.relocs.get(ctors, []):
            sinit_syms.add(o.syms[si][0])
    for s in o.funcs():
        if s[0] not in sinit_syms:
            continue
        if not same_as_template(o, s, "sinit"):
            res["skip"].append(f"sinit {s[0]} (0x{s[2]:X}): not the 9-pool template"
                               + (" (extra statics or pools)" if s[2] != 0x224
                               else " (same size, other code or targets: statics before the chain,"
                                    " or the unit that owns the pools)"))
            continue
        bss = o.sec(".bss")
        size = o.e.sh[bss][5] if bss is not None else 0
        if size != BSS_SIZE:
            res["skip"].append(f"sinit {s[0]}: unit .bss is 0x{size:X}, not 0x{BSS_SIZE:X} (file statics)")
            continue
        if s[0].startswith("__sinit_"):
            res["done"].append(s[0])
            continue
        if not s[0].startswith("fn_"):
            res["skip"].append(f"sinit {s[0]}: not a placeholder name")
            continue
        bsyms = sorted((t for _, t in o.defined(".bss")), key=lambda t: t[1])
        if [(t[1], t[2]) for t in bsyms] != [(0, BSS_SIZE)] or not bsyms[0][0].startswith("lbl_"):
            res["skip"].append(f"sinit {s[0]}: .bss already split or not one placeholder")
            continue
        res["sinit"] = {"name": s[0], "addr": text_base(unit) + s[1], "bss": int(bsyms[0][0][4:], 16),
                        "bss_name": bsyms[0][0]}
    # dtors: 0x60 template functions
    for s in o.funcs():
        if s[2] != 0x60 or not same_as_template(o, s, "dtor"):
            continue
        d, why = dtor_info(o, unit, s)
        if why:
            res["skip"].append(f"dtor {s[0]}: {why}")
        elif s[0].startswith("__dt__"):
            res["done"].append(s[0])
        elif not s[0].startswith("fn_"):
            res["skip"].append(f"dtor {s[0]}: not a placeholder name")
        elif not d["vt_name"].startswith("lbl_"):
            res["skip"].append(f"dtor {s[0]}: vtable already named {d['vt_name']}")
        else:
            res["dtor"].append(d)
    return res


def dtor_info(o, unit, s):
    """The vtable a template dtor stores and the slots it overrides, or
    (None, reason) when it is not a 19-slot direct child of ActorBase."""
    data = o.sec(".data")
    rels = o.relocs_in(s[5], s[1], s[2])
    vt_sym = o.syms[[r for r in rels if r[1] == 6][0][2]]
    if vt_sym[5] != data:
        return None, f"vtable {vt_sym[0]} not in this unit"
    vt_addr = data_base(unit) + vt_sym[1]
    why = vtable_problem(o, vt_sym, s)
    if why:
        return None, f"vtable 0x{vt_addr:08X}: {why}"
    slots = {off - vt_sym[1]: o.syms[si][0] for off, typ, si, add in o.relocs_in(data, vt_sym[1], VT_SIZE)}
    base = actorbase_slots()
    overrides = {k: slots[k] for k in range(0x0C, VT_SIZE, 4) if slots[k] != base[k]}
    bad = [n for n in overrides.values() if o.func(n) is None]
    if bad:
        return None, f"vtable 0x{vt_addr:08X}: slot functions outside the unit ({', '.join(bad[:3])})"
    return {"name": s[0], "addr": text_base(unit) + s[1], "vt": vt_addr, "vt_name": vt_sym[0],
            "vt_size": vt_sym[2], "overrides": overrides}, None


def data_base(unit):
    for l in SPLITS.read_text().split("\n\n"):
        if l.startswith(f"game/{unit}.cpp:"):
            m = re.search(r"\.data\s+start:0x([0-9A-F]+)", l)
            return int(m[1], 16) if m else 0
    return 0


_abase = None


def actorbase_slots():
    global _abase
    if _abase is None:
        for u in game_units():
            p = target_obj(u)
            if not p.exists():
                continue
            o = Obj(p)
            d = o.sec(".data")
            for _, s in o.defined(".data"):
                if s[0] == "__vt__18ActorBase_804A5770":
                    _abase = {off - s[1]: o.syms[si][0] for off, t, si, a in o.relocs_in(d, s[1], VT_SIZE)}
                    return _abase
        die("ActorBase_804A5770's vtable not found")
    return _abase


def vtable_problem(o, vt, dtor):
    """None if vt is a 19-slot direct ActorBase child's vtable whose slot 0
    is this dtor."""
    if vt[2] not in (VT_SIZE, VT_SIZE + 4):
        return f"vtable symbol is 0x{vt[2]:X}, not 0x{VT_SIZE:X} (+4 padding): not 19 slots"
    d = o.sec(".data")
    raw = o.e.data(d)[vt[1]:vt[1] + vt[2]]
    if raw[:8] != b"\0" * 8 or raw[VT_SIZE:] != b"\0" * (vt[2] - VT_SIZE):
        return "vtable header or padding not zero"
    rels = {off - vt[1]: (t, o.syms[si][0], a) for off, t, si, a in o.relocs_in(d, vt[1], vt[2])}
    if sorted(rels) != list(range(8, VT_SIZE, 4)) or any(t != 1 or a for t, n, a in rels.values()):
        return "vtable relocations are not 19 plain slots"
    if rels[8][1] != dtor[0]:
        return f"slot 0x08 is {rels[8][1]}, not the dtor"
    return None


# ---------------------------------------------------------------- rendering


def comment(text):
    return "\n".join("// " + l for l in textwrap.wrap(text, 76))


def render_class(cls, naming, overrides, size, why=None):
    slots = "".join(SLOT_LINE.format(off=k, decl=SLOT_DECLS[k]) for k in sorted(overrides))
    fields = ""
    if size and size > ACTOR_BASE_SIZE:
        fields = PAD_LINE.format(off=ACTOR_BASE_SIZE, n=size - ACTOR_BASE_SIZE)
    return CLASS_BLOCK.format(naming=comment(naming), cls=cls, slots=slots, fields=fields,
                              size=f"0x{size:X}" if size else why)


def dtor_plan(unit, d):
    ctors = ctors_of(unit, d["vt_name"], d["name"])
    cls, naming = class_name(unit, d["vt"], ctors)
    size, why = alloc_size(ctors)
    if size is not None and size < ACTOR_BASE_SIZE:
        return None
    return {"cls": cls, "naming": naming, "size": size, "block": render_class(cls, naming, d["overrides"], size, why),
            "def": DTOR_DEF.format(cls=cls)}


# ---------------------------------------------------------------- scan


def scan(argv):
    fam = {"sinit": [], "dtor": []}
    skips, done = [], {"sinit": 0, "dtor": 0}
    for u in game_units():
        r = analyse(u)
        if r["sinit"]:
            fam["sinit"].append(u)
        if r["dtor"]:
            fam["dtor"].append(u)
        for d in r["done"]:
            done["sinit" if d.startswith("__sinit") else "dtor"] += 1
        skips += [(u, s) for s in r["skip"]]
    print(f"sinit: {len(fam['sinit'])} units to generate, {done['sinit']} already done")
    print(f"dtor:  {sum(len(analyse(u)['dtor']) for u in fam['dtor'])} dtors in {len(fam['dtor'])} units, "
          f"{done['dtor']} already done")
    print(f"skipped variants: {len(skips)}")
    for u, s in skips:
        print(f"  {u}: {s}")
    if "--list" in argv:
        both = [u for u in fam["sinit"] if u in fam["dtor"]]
        print("both:", " ".join(both))
        print("sinit only:", " ".join(u for u in fam["sinit"] if u not in fam["dtor"]))
        print("dtor only:", " ".join(u for u in fam["dtor"] if u not in fam["sinit"]))




# ---------------------------------------------------------------- build + diff


def ninja(*targets, keep_going=False):
    cmd = ["ninja"] + (["-k", "0"] if keep_going else []) + list(targets)
    return run(cmd)


def resplit():
    """dtk re-splits only when config.json goes (CLAUDE.md)."""
    Path("build/G4SE01/config.json").unlink(missing_ok=True)
    out = ninja("build/G4SE01/config.json")
    if out.returncode:
        die("split failed:\n" + (out.stdout + out.stderr)[-2000:])
    subprocess.run(["python3", "configure.py"], capture_output=True)


def objdiff(unit):
    """{target symbol name: match % (None when we lack it)}, functions and data."""
    cmd = [OBJDIFF, "diff", "-p", ".", "-u", f"fsa_game_code/game/{unit}", "-o", "-",
           "-c", "functionRelocDiffs=all"]
    out = subprocess.run(cmd, capture_output=True, text=True)
    if out.returncode:
        return None
    d = json.loads(out.stdout)
    return {s["name"]: (float(s.get("match_percent") or 0) if s.get("target_symbol") is not None else None)
            for s in d["left"]["symbols"] if s.get("kind") in ("SYMBOL_FUNCTION", "SYMBOL_OBJECT")}


def matched(diff):
    return {n for n, p in (diff or {}).items() if p == 100}


# ---------------------------------------------------------------- source edits

INCLUDE = re.compile(r'^\s*#\s*include\s+[<"]([^>"]+)[>"]', re.M)
DEF = re.compile(r"^(?!(?:if|for|while|switch|return)\b)[A-Za-z_][\w:<>,*&\s]*?\b(?:(\w+)::)?(~?\w+)\s*\([^;{]*\)\s*(?:const\s*)?\{", re.M)


def demangle_qual(name):
    """`init__13cls_8049E3A8Fv` -> ('cls_8049E3A8', 'init'); dtor -> '~cls'."""
    m = re.match(r"^(\w+?)__(\d+)(\w+)F", name)
    if not m:
        return None
    meth, n, rest = m[1], int(m[2]), m[3]
    cls = rest[:n]
    return cls, ("~" + cls if meth == "__dt" else meth)


def top_level_defs(text):
    """[(offset, class or None, name)] of function definitions at brace depth 0."""
    depth, out, i = 0, [], 0
    starts = {m.start(): m for m in DEF.finditer(text)}
    for k, ch in enumerate(text):
        if depth == 0 and k in starts:
            m = starts[k]
            out.append((k, m[1], m[2]))
        if ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
    return out


def insert_def(text, unit, dtor_addr, definition):
    """Put the definition before the first defined function that follows it
    in the original's .text, else at the end."""
    addr_of = {}
    for l in symbols_lines():
        m = SYMLINE.match(l.rstrip("\n"))
        if m and m[2] == ".text":
            a = int(m[3], 16)
            q = demangle_qual(m[1])
            if q:
                addr_of[q] = a
            addr_of[(None, m[1].split("__")[0] or m[1])] = a
    after = [(off, addr_of[(c, f)]) for off, c, f in top_level_defs(text)
             if (c, f) in addr_of and addr_of[(c, f)] > dtor_addr]
    if not after:
        return text.rstrip("\n") + "\n\n" + definition
    off = min(after, key=lambda x: x[1])[0]
    # keep a comment block that belongs to the following definition with it
    head = text[:off]
    lines = head.split("\n")
    k = len(lines) - 1
    while k > 0 and lines[k - 1].lstrip().startswith("//"):
        k -= 1
    cut = len("\n".join(lines[:k])) + (1 if k else 0)
    return text[:cut] + definition + "\n" + text[cut:]


STALE = re.compile(r"//.*(__sinit|sinit|destructor|dtor|pool).*\b(not|still|yet|missing|unwritten|todo)\b"
                   r"|//.*\b(not|still|yet)\b.*(__sinit|sinit|destructor|dtor)", re.I)


def new_source(unit, plan):
    """A new unit's source: the includes and the destructors, nothing else."""
    out = f"{SINIT_INCLUDE}\n"
    if plan["dtors"]:
        out += f'#include "game/act/{unit}.h"\n'
        for d in plan["dtors"]:
            out += "\n" + d["def"]
    return out


def edit_source(unit, plan, text):
    """Returns (new text, problem). Deterministic: --check re-runs it on the
    file as it was before generation."""
    if text is None:
        return new_source(unit, plan), None
    stale = [l.strip() for l in text.splitlines() if STALE.search(l)]
    if stale:
        return None, f"a comment may describe the generated code as unwritten ({stale[0][:60]!r}); edit it by hand"
    incs = INCLUDE.findall(text)
    if "game/GameAudio.h" not in incs:
        if any(i.startswith(("JSystem/JAudio", "game/GameAudioBase.h")) for i in incs):
            return None, "has JAudio includes but not game/GameAudio.h"
        m = INCLUDE.search(text)
        if m:
            pos = m.start()
            sep = "\n" if pos and text[:pos].rstrip("\n").split("\n")[-1].lstrip().startswith("//") \
                and not text[:pos].endswith("\n\n") else ""
            text = text[:pos] + sep + SINIT_INCLUDE + "\n" + text[pos:]
        else:
            text = (text.rstrip("\n") + "\n\n" if text.strip() else "") + SINIT_INCLUDE + "\n"
    elif incs[0] != "game/GameAudio.h":
        return None, "game/GameAudio.h is not its first include (not reordering hand-written code)"
    if plan["dtors"]:
        own = f"game/act/{unit}.h"
        if own not in INCLUDE.findall(text):
            last = list(INCLUDE.finditer(text))[-1]
            text = text[:last.end()] + f'\n#include "{own}"' + text[last.end():]
        for d in plan["dtors"]:
            if re.search(r"\b%s::~%s\s*\(" % (d["cls"], d["cls"]), text):
                return None, f"{d['cls']}::~{d['cls']} already defined"
            text = insert_def(text, unit, d["addr"], d["def"])
    return text, None


def edit_header(unit, plan, text):
    blocks = "\n".join(d["block"] for d in plan["dtors"])
    if text is None:
        guard = f"GAME_ACT_{unit.upper()}_H"
        return HEADER.format(guard=guard, unit=unit, classblock=blocks), None
    for d in plan["dtors"]:
        if re.search(r"\b%s\b|_%08X\b" % (d["cls"], d["vt"]), text):
            return None, f"header already declares the class of vtable 0x{d['vt']:08X}"
    if '#include "game/ActorBase.h"' not in text:
        last = list(INCLUDE.finditer(text))
        pos = last[-1].end() if last else text.index("\n", text.index("#define")) 
        text = text[:pos] + '\n#include "game/ActorBase.h"' + text[pos:]
    k = text.rindex("#endif")
    return text[:k] + blocks + "\n" + text[k:], None


def referenced_elsewhere(names, unit):
    """Names that hand-written code or headers spell out (extern "C" etc.)."""
    hits = set()
    pat = re.compile(r"\b(" + "|".join(map(re.escape, names)) + r")\b") if names else None
    if not pat:
        return hits
    for root in ("src", "include", "libs"):
        for p in Path(root).rglob("*"):
            if p.suffix in (".c", ".cpp", ".h", ".hpp", ".inc") and p.is_file():
                if p in (src_path(unit), hdr_path(unit)):
                    continue
                hits |= set(pat.findall(p.read_text(errors="replace")))
    return hits


def class_declared_elsewhere(vt, unit):
    pat = re.compile(r"\bclass\s+\w*_%08X\b|\bcls_%08X\b" % (vt, vt))
    for p in Path("include").rglob("*.h"):
        if p != hdr_path(unit) and pat.search(p.read_text(errors="replace")):
            return str(p)
    for p in Path("src").rglob("*.cpp"):
        if p != src_path(unit) and pat.search(p.read_text(errors="replace")):
            return str(p)
    return None


# ---------------------------------------------------------------- symbols.txt edits (in place, per unit)


def find_line(lines, sec, addr, text):
    for k, l in enumerate(lines):
        if l.startswith(text) and f"= {sec}:0x{addr:08X};" in l:
            return k
    return None


def edits_ok(edits):
    return all(old[0] is not None for _, _, old, _ in edits)


def apply_edits(edits, revert=False):
    """edits: [(section, addr, [old lines], [new lines])]; replaces the old
    run of lines (or the new one, with revert) in place."""
    lines = symbols_lines()
    for sec, addr, old, new in edits:
        a, b = (new, old) if revert else (old, new)
        k = find_line(lines, sec, addr, a[0])
        if k is None or lines[k:k + len(a)] != a:
            raise RuntimeError(f"symbols.txt: expected {a[0].strip()!r} at {sec}:0x{addr:08X}")
        lines[k:k + len(a)] = b
    SYMBOLS.write_text("".join(lines))


def sym_line(lines, sec, addr, name=None):
    for l in lines:
        m = SYMLINE.match(l.rstrip("\n"))
        if m and m[2] == sec and int(m[3], 16) == addr and (name is None or m[1] == name):
            return l
    return None


def rename_line(line, new_name, size=None, local=False):
    m = SYMLINE.match(line.rstrip("\n"))
    attrs = m[4]
    if size is not None:
        attrs = re.sub(r"size:0x[0-9A-Fa-f]+", f"size:0x{size:X}", attrs)
    if local and "scope:" not in attrs:
        attrs += " scope:local"
    return f"{new_name} = {m[2]}:0x{m[3]}; // {attrs}\n"


def symbol_edits(unit, plan, ours):
    """From our compiled object: the edits for this unit, or (None, problem)."""
    lines = symbols_lines()
    edits = []
    if plan["sinit"]:
        si = plan["sinit"]
        sin = [s for s in ours.funcs() if s[0].startswith("__sinit_")]
        if len(sin) != 1:
            return None, "our object has no single __sinit"
        if not same_as_template(ours, sin[0], "sinit", ours=True):
            return None, f"our {sin[0][0]} differs from the template's compiled sinit"
        old = sym_line(lines, ".text", si["addr"], si["name"])
        edits.append((".text", si["addr"], [old], [rename_line(old, sin[0][0], local=True)]))
        bss = sorted((t for _, t in ours.defined(".bss")), key=lambda t: t[1])
        chain = [t for t in bss if re.fullmatch(r"@\d+", t[0])]
        pools = [t for t in bss if t[0].startswith("memPool_")]
        if [(t[1], t[2]) for t in chain] != [(0xC * k, 0xC) for k in range(N_POOLS)] or \
                sorted((t[1], t[2]) for t in pools) != [(0xC * N_POOLS + 0x10 * k, 0x10) for k in range(N_POOLS)]:
            return None, "our .bss is not 9 x 0xC chain + 9 x 0x10 pools"
        old = sym_line(lines, ".bss", si["bss"], si["bss_name"])
        new = [f"{t[0]} = .bss:0x{si['bss'] + t[1]:08X}; // type:object size:0xC scope:local\n" for t in chain]
        new += [f"lbl_{si['bss'] + t[1]:08X} = .bss:0x{si['bss'] + t[1]:08X}; // type:object size:0x10\n"
                for t in sorted(pools, key=lambda t: t[1])]
        edits.append((".bss", si["bss"], [old], new))
    for d in plan["dtors"]:
        f = ours.func(f"__dt__{len(d['cls'])}{d['cls']}Fv")
        if not f:
            return None, f"our object lacks {d['cls']}'s destructor"
        if not same_as_template(ours, f[1], "dtor", ours=True):
            return None, f"our {f[1][0]} differs from the template's compiled dtor"
        old = sym_line(lines, ".text", d["addr"], d["name"])
        edits.append((".text", d["addr"], [old], [rename_line(old, f[1][0])]))
        vts = [s for _, s in ours.defined(".data") if s[0] == f"__vt__{len(d['cls'])}{d['cls']}"]
        if len(vts) != 1 or vts[0][2] != VT_SIZE:
            return None, "our vtable is missing or not 0x54"
        vt = vts[0]
        old = sym_line(lines, ".data", d["vt"], d["vt_name"])
        edits.append((".data", d["vt"], [old], [rename_line(old, vt[0], size=VT_SIZE)]))
        dsec = ours.sec(".data")
        oslots = {off - vt[1]: ours.syms[si][0] for off, t, si, a in ours.relocs_in(dsec, vt[1], VT_SIZE)}
        tbase = text_base(unit)
        tobj = Obj(target_obj(unit))
        for k, tname in d["overrides"].items():
            want = oslots.get(k)
            if want == tname:
                continue
            if not want or f"__{len(d['cls'])}{d['cls']}F" not in want:
                return None, f"our slot 0x{k:02X} is {want}, not this class's override"
            if not tname.startswith("fn_"):
                return None, f"slot 0x{k:02X} target is already named {tname}"
            addr = tbase + tobj.func(tname)[1][1]
            if any(e[1] == addr for e in edits):
                return None, f"{tname} is in two vtables"
            old = sym_line(lines, ".text", addr, tname)
            edits.append((".text", addr, [old], [rename_line(old, want)]))
    return edits, None


# ---------------------------------------------------------------- generate


def plan_unit(unit):
    r = analyse(unit)
    plan = {"unit": unit, "sinit": r["sinit"], "dtors": [], "skipped": list(r["skip"])}
    for d in r["dtor"]:
        p = dtor_plan(unit, d)
        if p is None:
            plan["skipped"].append(f"dtor {d['name']}: spawner allocates less than ActorBase")
            continue
        where = class_declared_elsewhere(d["vt"], unit)
        if where:
            plan["skipped"].append(f"dtor {d['name']}: class of 0x{d['vt']:08X} already declared in {where}")
            continue
        refs = referenced_elsewhere([d["name"], d["vt_name"], *d["overrides"].values()], unit)
        if refs:
            plan["skipped"].append(f"dtor {d['name']}: {', '.join(sorted(refs))} spelled out in hand-written code")
            continue
        plan["dtors"].append({**d, **p})
    if plan["sinit"] and referenced_elsewhere([plan["sinit"]["name"], plan["sinit"]["bss_name"]], unit):
        plan["skipped"].append(f"sinit {plan['sinit']['name']}: spelled out in hand-written code")
        plan["sinit"] = None
    return plan


def snapshot(unit):
    return {p: (p.read_text() if p.exists() else None) for p in (src_path(unit), hdr_path(unit))}


def restore(snap):
    for p, t in snap.items():
        if t is None:
            p.unlink(missing_ok=True)
        else:
            p.write_text(t)


MANIFEST_HEAD = "unit\tfamily\tfunction\tclass\tvtable\tsrc_before\thdr_before"


def members(plan, names):
    out = []
    if plan["sinit"]:
        out.append(("sinit", names.get(plan["sinit"]["addr"]), "-", "-"))
    for d in plan["dtors"]:
        out.append(("dtor", f"__dt__{len(d['cls'])}{d['cls']}Fv", d["cls"], f"0x{d['vt']:08X}"))
    return out


def blob(path):
    """`new` if the generator creates the file, else the git blob of the file
    before generation (written to the object store, so --check can re-run the
    edit on it)."""
    if not path.exists():
        return "new"
    return subprocess.run(["git", "hash-object", "-w", str(path)], capture_output=True,
                          text=True, check=True).stdout.strip()


def claimed():
    try:
        return {unit_slug(k) for k in json.loads(Path("progress/claims.json").read_text())}
    except (OSError, ValueError):
        return set()


def generate(units):
    ninja(str(our_obj(TEMPLATE_UNIT)))
    plans, report = {}, {}
    taken = claimed()
    for u in units:
        if u in taken:
            report[u] = ("skipped: claimed in progress/claims.json", [])
            continue
        p = plan_unit(u)
        if not p["sinit"] and not p["dtors"]:
            report[u] = ("nothing to generate", p["skipped"])
            continue
        plans[u] = p
    snaps, before = {}, {}
    for u, p in list(plans.items()):
        snaps[u] = snapshot(u)
        p["src_before"] = blob(src_path(u))
        p["hdr_before"] = blob(hdr_path(u)) if p["dtors"] else "-"
        if src_path(u).exists():
            ninja(str(our_obj(u)))
            before[u] = matched(objdiff(u))
        else:
            before[u] = set()
        src, why = edit_source(u, p, snaps[u][src_path(u)])
        hdr = None
        if not why and p["dtors"]:
            hdr, why = edit_header(u, p, snaps[u][hdr_path(u)])
        if why:
            report[u] = (f"skipped: {why}", p["skipped"])
            del plans[u]
            continue
        src_path(u).write_text(src)
        if hdr is not None:
            hdr_path(u).parent.mkdir(parents=True, exist_ok=True)
            hdr_path(u).write_text(hdr)
    if not plans:
        return report
    subprocess.run(["python3", "configure.py"], capture_output=True)
    ninja(*[str(our_obj(u)) for u in plans], keep_going=True)
    all_edits = {}
    for u, p in list(plans.items()):
        why = None
        if not our_obj(u).exists() or our_obj(u).stat().st_mtime < src_path(u).stat().st_mtime:
            why = "does not compile"
        else:
            edits, why = symbol_edits(u, p, Obj(our_obj(u)))
            if not why and not edits_ok(edits):
                why = "a symbols.txt line to rename is not where dtk's split has it"
        if why:
            restore(snaps[u])
            report[u] = (f"backed out: {why}", p["skipped"])
            del plans[u]
            continue
        all_edits[u] = edits
        apply_edits(edits)
    split_done = False
    while plans:
        resplit()
        split_done = True
        ninja(*[str(our_obj(u)) for u in plans], keep_going=True)
        bad = []
        for u, p in plans.items():
            p["names"] = {e[1]: e[3][0].split(" = ")[0] for e in all_edits[u] if e[0] == ".text"}
            diff = objdiff(u) or {}
            lost = sorted(before[u] - matched(diff))
            probs = member_problems(u, p, diff) + ([f"lost {', '.join(lost)}"] if lost else [])
            if probs:
                bad.append(u)
                report[u] = ("backed out: " + "; ".join(probs), p["skipped"])
            else:
                gen = [m[1] for m in members(p, p["names"])]
                report[u] = (f"ok: {len(before[u])} -> {len(matched(diff) & set(funcs_of(u)))} functions match ("
                             + ", ".join(sorted(gen)) + ")", p["skipped"])
        for u in bad:
            apply_edits(all_edits.pop(u), revert=True)
            restore(snaps[u])
            del plans[u]
            split_done = False
        if not bad:
            break
    if not split_done and any(r[0].startswith("backed out") for r in report.values()):
        resplit()  # everything was backed out after a split: split the restored symbols again
    rows = MANIFEST.read_text().splitlines()[1:] if MANIFEST.exists() else []
    for u, p in plans.items():
        for fam, fn, cls, vt in members(p, p["names"]):
            rows.append(f"{u}\t{fam}\t{fn}\t{cls}\t{vt}\t{p['src_before']}\t{p['hdr_before']}")
    MANIFEST.write_text("\n".join([MANIFEST_HEAD] + sorted(set(rows))) + "\n")
    return report


def funcs_of(unit):
    return [s[0] for s in obj_fresh(target_obj(unit)).funcs()]


def obj_fresh(path):
    return Obj(path)


# ---------------------------------------------------------------- per-member checks


def member_problems(u, p, diff):
    """Everything --check verifies about one unit's members except the file
    text: template identity on both sides, objdiff 100 %, and the
    symbols.txt side (sinit: the exact .bss layout; dtor: the vtable's name,
    size, slot names and 100 %)."""
    bad = []
    t, o = obj_fresh(target_obj(u)), (obj_fresh(our_obj(u)) if our_obj(u).exists() else None)
    if o is None:
        return ["no compiled object"]
    for fam, fn, cls, vt in members(p, p["names"]):
        tf, of = t.func(fn), o.func(fn)
        if not tf:
            bad.append(f"{fn}: not in the target object (symbols.txt)")
            continue
        if not same_as_template(t, tf[1], fam):
            bad.append(f"{fn}: target is not the {fam} template up to identifiers")
        if not of or not same_as_template(o, of[1], fam, ours=True):
            bad.append(f"{fn}: our compiled function is not the template's up to identifiers")
        if diff.get(fn) != 100:
            bad.append(f"{fn}: objdiff {diff.get(fn)}")
        if fam == "sinit":
            bad += bss_problems(u, t, o, diff)
        else:
            bad += vtable_problems(u, t, o, tf[1], of[1] if of else None, diff)
    return bad


def bss_problems(u, t, o, diff):
    """The unit's .bss in dtk's split: exactly our 9 @NNN (0xC, 100 %), then 9
    unnamed lbl_<addr> of 0x10 at +0x6C..+0xEC, then 4 unsplit bytes."""
    tb, ob = t.sec(".bss"), o.sec(".bss")
    if tb is None or t.e.sh[tb][5] != BSS_SIZE:
        return [f".bss is not 0x{BSS_SIZE:X}"]
    base = bss_base(u)
    ours = sorted((s for _, s in o.defined(".bss") if re.fullmatch(r"@\d+", s[0])), key=lambda s: s[1])
    want = [(s[0], s[1], 0xC) for s in ours]
    want += [(f"lbl_{base + 0xC * N_POOLS + 0x10 * k:08X}", 0xC * N_POOLS + 0x10 * k, 0x10) for k in range(N_POOLS)]
    pad = BSS_SIZE - 4
    # the 4 unsplit bytes: dtk's split names them gap_<align>_<addr>_bss
    want.append((f"gap_08_{base + pad:08X}_bss", pad, 4))
    have = sorted(((s[0], s[1], s[2]) for _, s in t.defined(".bss")), key=lambda x: x[1])
    bad = []
    if [w[1:] for w in want[:N_POOLS]] != [(0xC * k, 0xC) for k in range(N_POOLS)] or have != want:
        bad.append(".bss split is not our 9 @NNN + 9 lbl_ 0x10 copies + 4 pad bytes: "
                   + ", ".join(f"{n}@+0x{a:X}/0x{z:X}" for n, a, z in have if (n, a, z) not in want)[:200])
    bad += [f"{n}: objdiff {diff.get(n)}" for n, _, _ in want[:N_POOLS] if diff.get(n) != 100]
    return bad


def vtable_problems(u, t, o, tf, of, diff):
    """The vtable the dtor stores: same name in dtk's split and ours, 0x54 in
    symbols.txt, every slot's relocation target named as ours, 100 %."""
    if of is None:
        return []
    def vt_of(x, f):
        r = [r for r in x.relocs_in(f[5], f[1], f[2]) if r[1] == 6][0]
        return x.syms[r[2]]
    tv, ov = vt_of(t, tf), vt_of(o, of)
    bad = []
    if tv[0] != ov[0]:
        return [f"vtable is {tv[0]} in symbols.txt, ours {ov[0]}"]
    line = sym_line(symbols_lines(), ".data", data_base(u) + tv[1], tv[0])
    if not line or f"size:0x{VT_SIZE:X}" not in line or tv[2] != VT_SIZE:
        bad.append(f"{tv[0]}: symbols.txt size is not 0x{VT_SIZE:X}")
    ts = {off - tv[1]: t.syms[si][0] for off, _, si, _ in t.relocs_in(t.sec(".data"), tv[1], VT_SIZE)}
    os_ = {off - ov[1]: o.syms[si][0] for off, _, si, _ in o.relocs_in(o.sec(".data"), ov[1], VT_SIZE)}
    for k in range(8, VT_SIZE, 4):
        if ts.get(k) != os_.get(k):
            bad.append(f"{tv[0]} slot 0x{k:02X}: symbols.txt names {ts.get(k)}, ours {os_.get(k)}")
    if diff.get(tv[0]) != 100:
        bad.append(f"{tv[0]}: objdiff {diff.get(tv[0])}")
    return bad


def bss_base(unit):
    for l in SPLITS.read_text().split("\n\n"):
        if l.startswith(f"game/{unit}.cpp:"):
            m = re.search(r"\.bss\s+start:0x([0-9A-F]+)", l)
            return int(m[1], 16) if m else 0
    return 0


def git_blob(h):
    out = subprocess.run(["git", "cat-file", "blob", h], capture_output=True, text=True)
    return out.stdout if out.returncode == 0 else None


def text_problems(u, p, src_before, hdr_before):
    """The unit's files are exactly what the generator writes: a created file
    equals its rendering, an edited one equals the edit re-run on the file as
    it was before generation (its blob)."""
    bad = []
    for path, before, render in ((src_path(u), src_before, edit_source),
                                 (hdr_path(u), hdr_before, edit_header)):
        if before == "-":
            continue
        old = None if before == "new" else git_blob(before)
        if before != "new" and old is None:
            bad.append(f"{path}: its pre-generation blob {before} is not in git")
            continue
        want, why = render(u, p, old)
        have = path.read_text() if path.exists() else None
        if why or have != want:
            bad.append(f"{path}: not exactly the generator's output" + (f" ({why})" if why else
                       first_diff(want or "", have or "")))
    return bad


def first_diff(a, b):
    al, bl = a.splitlines(), b.splitlines()
    for k in range(max(len(al), len(bl))):
        x = al[k] if k < len(al) else "<end>"
        y = bl[k] if k < len(bl) else "<end>"
        if x != y:
            return f" (line {k + 1}: expected {x[:60]!r}, found {y[:60]!r})"
    return ""


# ---------------------------------------------------------------- check


def plan_from_rows(u, rs):
    """Rebuild the unit's plan from its manifest rows and dtk's split."""
    t = obj_fresh(target_obj(u))
    p = {"unit": u, "sinit": None, "dtors": [], "names": {}}
    bad = []
    for _, fam, fn, cls, vt, sb, hb in rs:
        f = t.func(fn)
        if not f:
            bad.append(f"{fn}: not in the target object (symbols.txt)")
            continue
        addr = text_base(u) + f[1][1]
        p["names"][addr] = fn
        if fam == "sinit":
            p["sinit"] = {"addr": addr}
            continue
        d, why = dtor_info(t, u, f[1])
        dp = dtor_plan(u, d) if d else None
        if why or not dp or dp["cls"] != cls or f"0x{d['vt']:08X}" != vt:
            bad.append(f"{fn}: no longer derives as class {cls}, vtable {vt}" + (f" ({why})" if why else ""))
            continue
        p["dtors"].append({**d, **dp})
    p["dtors"].sort(key=lambda d: d["addr"])
    return p, bad


def check(units):
    if not MANIFEST.exists():
        print("check: ok (no config/G4SE01/gen_game.tsv)")
        return True
    lines = MANIFEST.read_text().splitlines()
    if not lines or lines[0] != MANIFEST_HEAD:
        print(f"malformed manifest header: {lines[:1]}")
        return False
    known = set(game_units())
    rows = [r.split("\t") for r in lines[1:] if r.strip()]
    for r in rows:
        if r[0] not in known:
            print(f"malformed manifest row (not a game unit): {r}")
            return False
    if units:
        rows = [r for r in rows if r[0] in units]
    by_unit = {}
    for r in rows:
        if len(r) != 7:
            print(f"malformed manifest row: {r}")
            return False
        by_unit.setdefault(r[0], []).append(r)
    cfg = Path("build/G4SE01/config.json")
    if cfg.exists() and any(p.stat().st_mtime > cfg.stat().st_mtime for p in (SYMBOLS, SPLITS)):
        cfg.unlink()
    out = ninja("build/G4SE01/config.json", *[str(our_obj(u)) for u in by_unit], keep_going=True)
    if not Path("build/G4SE01/config.json").exists():
        print("split failed:", (out.stdout + out.stderr)[-1000:])
        return False
    with ThreadPoolExecutor(16) as ex:
        diffs = dict(zip(by_unit, ex.map(objdiff, by_unit)))
    problems = 0
    for u, rs in sorted(by_unit.items()):
        fwd = [r for r in rs if r[1] == "fwd"]
        if fwd:
            bad = fwd_check(fwd, diffs[u] or {})
            print(f"{u}: {len(fwd)} forwarders (fwd): " + ("ok" if not bad else "FAIL"))
            for b in bad:
                print(f"  {b}")
            problems += len(bad)
            rs = [r for r in rs if r[1] != "fwd"]
            if not rs:
                continue
        sbs, hbs = {r[5] for r in rs}, {r[6] for r in rs if r[1] == "dtor"}
        p, bad = plan_from_rows(u, rs)
        if not bad:
            bad = member_problems(u, p, diffs[u] or {})
            if len(sbs) != 1 or len(hbs) > 1:
                bad.append("manifest rows disagree on the files' pre-generation versions")
            else:
                bad += text_problems(u, p, sbs.pop(), hbs.pop() if hbs else "-")
        print(f"{u}: {', '.join(r[2] for r in rs)}: " + ("ok" if not bad else "FAIL"))
        for b in bad:
            print(f"  {b}")
        problems += len(bad)
    print("check: ok" if not problems else f"check: {problems} problem(s)")
    return not problems


# ---------------------------------------------------------------- family fwd: per-player forwarders
#
# tu_8022AD64's per-player queries (include/game/Player.h): templates
# fn_8022B7C4 (returns the player's getter result, else a fallback) and
# fn_8022B860 (void, passes its argument on), both hand-matched there. A
# member is a placeholder function of that unit whose code is one of the two
# templates with only the registers (each extra integer argument moves the
# manager pointer up one register), the fallback constant and the getter
# changed. The getter must be a placeholder in the player's units
# (Player_804A6F38, 0x8023FBF0-0x8027E5BC); it becomes a member of
# Player_804A6F38 with the forwarder's return type and extra arguments.
#
# Types: a declaration Player.h already has is kept as written (reviewed);
# otherwise the return type comes from the callers (every use of the result
# a clrlwi to a byte, or a byte store -> bool, with a 0/1 fallback; every use
# a word -> s32) and each extra argument's from the getter's first use of it
# (stw/cmpw/cmpwi/subf/add -> s32; clrlwi. 24 -> bool). Anything else (an
# unused result, a pointer or float argument) is skipped and listed.
#
# Grouping for the scripted check: (void, extra arguments, fallback). The
# group (value, 0, 0) is checked against fn_8022B7C4 and (void, 1) against
# fn_8022B860; every other group against its lowest-address member (the
# variant template, reviewed by hand once). Members equal their group
# template up to the getter's name, in dtk's split and in our object.

FWD_UNIT = "tu_8022AD64"
FWD_TEMPLATES = {False: "fn_8022B7C4__Fl", True: "fn_8022B860__Fll"}  # void? -> template
PLAYER_H = Path("include/game/Player.h")
PLAYER_CLS = "Player_804A6F38"
PLAYER_TEXT = (0x8023FBF0, 0x8027E5BC)
FWD_MGR = "lbl_8053AC90"
ASM_DIR = Path("build/G4SE01/asm")

FWD_DEF = """\
{ret} {name}({params}) {{
    if ({mgr} == NULL) {{
        return {fb};
    }}
    if (isPlayerActive(player)) {{
        return {mgr}->mPlayers[player]->{callee}({args});
    }}
    return {fb2};
}}
"""
FWD_DEF_VOID = """\
void {name}({params}) {{
    if ({mgr} == NULL) {{
        return;
    }}
    if (isPlayerActive(player)) {{
        {mgr}->mPlayers[player]->{callee}({args});
    }}
}}
"""
FWD_DECL_HEAD = """\
// Generated by tools/gen_game.py (family fwd), by address. The return type
// comes from the callers (only clrlwi 24 / byte stores of the result -> bool;
// word uses -> s32), each extra argument's from the getter's first use of it.
"""
FWD_MEMBER_HEAD = "    // Getters and setters the generated queries forward to (tools/gen_game.py).\n"
FWD_DECL_ANCHOR = "JGeometry::TVec3f* fn_8023E724(s32 player);"
FWD_MEMBER_ANCHOR = "    void fn_8023FC10(s32 value);"
DECL_RE = re.compile(r"^((?:const )?[\w:<>]+\*?)\s+(fn_8[0-9A-F]{7})\(([^)]*)\);(.*)$")


def shape_of(o, s):
    """The fingerprint with register fields, `li` immediates and the call
    target masked: what makes a function a forwarder of either shape."""
    code, rel = fingerprint(o, s, role_of)
    out = bytearray()
    for k in range(0, len(code), 4):
        w, = struct.unpack_from(">I", code, k)
        op = w >> 26
        if op not in (16, 18, 19):
            if op == 14 and (w >> 16) & 31 == 0:  # li
                w &= ~0xFFFF
            w &= ~0x03FF0000
            if op == 31:
                w &= ~0x0000F800
        out += struct.pack(">I", w)
    return bytes(out), [(a, t, "<getter>" if t == 10 else n, d) for a, t, n, d in rel]


def fwd_identity(o, s):
    """Identifier-only form with the getter as a role."""
    code, rel = fingerprint(o, s, role_of)
    return code, [(a, t, "<getter>" if t == 10 else n, d) for a, t, n, d in rel]


_fwd_tmpl = {}


def fwd_template(void, ours=False):
    key = (void, ours)
    if key not in _fwd_tmpl:
        o = Obj(our_obj(FWD_UNIT) if ours else target_obj(FWD_UNIT))
        f = o.func(FWD_TEMPLATES[void])
        if not f:
            die(f"template {FWD_TEMPLATES[void]} not in {FWD_UNIT}'s {'compiled' if ours else 'target'} object")
        _fwd_tmpl[key] = (o, f[1])
    return _fwd_tmpl[key]


def fwd_shape(o, s):
    """(void, extra arguments, fallback, getter symbol) or None."""
    for void in (False, True):
        to, ts = fwd_template(void)
        if s[2] != ts[2] or shape_of(o, s) != shape_of(to, ts):
            continue
        rels = o.relocs_in(s[5], s[1], s[2])
        code = o.bytes_of(s)
        mgr = [r for r in rels if r[1] == 109 and o.syms[r[2]][0] == FWD_MGR]
        calls = [r for r in rels if r[1] == 10]
        if len(mgr) != 1 or len(calls) != 1:
            return None
        w, = struct.unpack_from(">I", code, mgr[0][0] - s[1])
        nargs = ((w >> 21) & 31) - 5
        fb = None
        if not void:  # (without a manager, for an inactive player)
            li = [struct.unpack(">h", struct.pack(">H", w & 0xFFFF))[0]
                  for k in range(0, len(code), 4) for w in struct.unpack_from(">I", code, k)
                  if w >> 26 == 14 and (w >> 16) & 31 == 0 and (w >> 21) & 31 == 3]
            if len(li) != 2:
                return None
            fb = tuple(li)
        return void, nargs, fb, o.syms[calls[0][2]]
    return None


_asm = None


def asm_bodies():
    """function name -> [(mnemonic, operands)] from dtk's disassembly, and
    callee name -> [(body, index of the bl)] over all units."""
    global _asm
    if _asm is None:
        ins = re.compile(r"^/\* [0-9A-F]{8} [0-9A-F]{8}  (?:[0-9A-F]{2} ){4}\*/\t(\S+)\s*(.*)$")
        bodies, calls = {}, {}
        for p in ASM_DIR.rglob("*.s"):
            for m in re.finditer(r"^\.fn (\S+), \w+\n(.*?)^\.endfn", p.read_text(errors="replace"), re.M | re.S):
                b = [(x[1], x[2]) for l in m[2].split("\n") for x in [ins.match(l)] if x]
                bodies[m[1].strip('"')] = b
                for i, (mn, op) in enumerate(b):
                    if mn == "bl":
                        calls.setdefault(op.strip('"'), []).append((b, i))
        _asm = bodies, calls
    return _asm


ASM_STORE = ("stw", "stb", "sth", "stfs", "stfd", "cmp", "fcmp", "psq_st", "stmw")


def asm_regs(op):
    return re.findall(r"\b(r\d+|f\d+)\b", op)


def result_use(b, start, r, depth=0):
    """How a caller first uses register r (a call result) after b[start]."""
    for k in range(start, min(len(b), start + 40)):
        mn, op = b[k]
        rs = asm_regs(op)
        if mn == "bl":
            return "arg" if r == "r3" else "clobbered"
        if mn.startswith("b"):
            if mn in ("blr", "b"):  # returned (b: to the epilogue or a shared return)
                return "returned" if r == "r3" else "dead"
            continue
        if not rs:
            continue
        dst, srcs = (None, rs) if mn.startswith(ASM_STORE) else (rs[0], rs[1:])
        if r in srcs:
            if mn == "mr" and depth < 3:
                return result_use(b, k + 1, dst, depth + 1)
            if mn.startswith("clrlwi") and op.endswith(", 24"):
                return "byte"
            if mn in ("stb", "stbx"):
                return "bytestore"
            if re.match(r"(lfs|lwz|lbz|lhz|lha|psq_l|stfs|lfd)\b", mn) and f"({r})" in op:
                return "ptr"
            if mn.startswith(("clrlwi", "extsb", "extsh")):
                return "narrow"
            return "word"
        if dst == r:
            return "dead"
    return "?"


def param_use(b, r, depth=0):
    """The getter's first use of argument register r."""
    for k, (mn, op) in enumerate(b):
        rs = asm_regs(op)
        if mn == "bl":
            return "call"
        if mn.startswith("b"):
            if r in rs:
                return mn
            continue
        if not rs:
            continue
        dst, srcs = (None, rs) if mn.startswith(ASM_STORE) else (rs[0], rs[1:])
        if r in srcs:
            if mn == "mr" and depth < 2:
                return param_use(b[k + 1:], dst, depth + 1)
            return f"{mn} {op}"
        if dst == r:
            return "unused"
    return "unused"


def player_decls(text):
    """{base name: (ret, params, line, in extern "C")} of Player.h's free
    function declarations."""
    out, in_c = {}, False
    for l in text.splitlines():
        if l.startswith('extern "C" {'):
            in_c = True
        elif in_c and l.startswith("}"):
            in_c = False
        m = DECL_RE.match(l)
        if m:
            out.setdefault(m[2], []).append((m[1], m[3].strip(), l, in_c))
    return out


def split_params(params):
    """'s32 player, const T* box' -> [('s32', 'player'), ('const T*', 'box')]."""
    out = []
    for p in [x.strip() for x in params.split(",") if x.strip()]:
        m = re.match(r"^(.*?[\w*&>])\s*\b(\w+)$", p)
        if not m:
            return None
        out.append((m[1].strip(), m[2]))
    return out


def fallback_literal(ret, v):
    if ret == "bool":
        return "true" if v else "false"
    if v < 0 or v < 10:
        return str(v)
    return f"0x{v:X}"


def fwd_infer(name, void, nargs, fb, callee):
    """(ret, params) from the evidence, or (None, reason)."""
    bodies, calls = asm_bodies()
    if void:
        ret = "void"
    else:
        uses = {result_use(b, i + 1, "r3") for b, i in calls.get(name, [])}
        if uses and uses <= {"byte", "bytestore", "dead", "clobbered", "returned"} and "byte" in uses and set(fb) <= {0, 1}:
            ret = "bool"
        elif uses and uses <= {"word", "arg", "dead", "clobbered", "returned"} and uses & {"word", "arg"}:
            ret = "s32"
        else:
            return None, f"return type: the callers' uses of the result are {sorted(uses) or 'none'}"
    params = [("s32", "player")]
    names = ["arg"] if nargs == 1 else ["a", "b", "c", "d", "e", "f"]
    cb = bodies.get(callee, [])
    for k in range(nargs):
        u = param_use(cb, f"r{4 + k}")
        if re.match(r"(stw|cmpw|cmpwi|subf|add|subis)\b", u):
            t = "s32"
        elif re.match(r"clrlwi\. r\d+, r%d, 24$" % (4 + k), u):
            t = "bool"
        else:
            return None, f"argument {k + 1}: the getter's first use of r{4 + k} is {u!r}"
        params.append((t, names[k]))
    f1 = param_use(cb, "f1")
    if f1 not in ("unused", "call"):
        return None, f"the getter reads f1 first ({f1!r}): a float argument?"
    return (ret, ", ".join(f"{t} {n}" for t, n in params)), None


def fwd_plan(header_text=None):
    """The members to generate: [{name, addr, callee, callee_addr, void,
    nargs, fb, ret, params, declared}], and the skipped variants."""
    o = obj(FWD_UNIT)
    decls = player_decls(header_text if header_text is not None else PLAYER_H.read_text())
    callee_addr = {}
    for l in symbols_lines():
        m = SYMLINE.match(l.rstrip("\n"))
        if m and m[2] == ".text":
            callee_addr[m[1]] = int(m[3], 16)
    plan, skipped = [], []
    seen_callees = set()
    for s in o.funcs():
        if not re.fullmatch(r"fn_8[0-9A-F]{7}", s[0]):
            continue
        sh = fwd_shape(o, s)
        if not sh:
            continue
        void, nargs, fb, cs = sh
        callee = cs[0]
        why = None
        ca = callee_addr.get(callee)
        if not re.fullmatch(r"fn_8[0-9A-F]{7}", callee):
            why = f"getter {callee} is not a placeholder"
        elif ca is None or not PLAYER_TEXT[0] <= ca < PLAYER_TEXT[1]:
            why = f"getter {callee} is outside the player's units"
        elif callee in seen_callees:
            why = f"getter {callee} has two forwarders"
        elif not 0 <= nargs <= 6:
            why = f"{nargs} extra arguments"
        if why:
            skipped.append(f"{s[0]}: {why}")
            continue
        d = decls.get(s[0], [])
        if len(d) > 1:
            skipped.append(f"{s[0]}: declared {len(d)} times in Player.h")
            continue
        if d:
            ret, params, line, in_c = d[0]
            sp = split_params(params)
            ints = [t for t, n in (sp or [])[1:] if t not in ("f32", "f64")]
            if not sp or sp[0] != ("s32", "player"):
                why = f"declared as ({params}), not (s32 player, ...)"
            elif (ret == "void") != void:
                why = f"declared {ret}, but its code is the {'void' if void else 'value'} shape"
            elif len(ints) != nargs:
                why = f"declared with {len(ints)} extra integer arguments, its code passes {nargs}"
            elif not void and ret == "bool" and not set(fb) <= {0, 1}:
                why = f"declared bool with fallback {fb}"
            if why:
                skipped.append(f"{s[0]}: {why}")
                continue
            declared = True
        else:
            (rp, why) = fwd_infer(s[0], void, nargs, fb, callee)
            if why:
                skipped.append(f"{s[0]}: {why}")
                continue
            ret, params = rp
            declared = False
        names = (s[0], callee)
        decl_re = re.compile(r"^(?!\s*//)[^/]*\b(%s|%s)\s*\(" % names, re.M)
        # in sources only declarations and definitions count (a return type, then the name), not calls
        src_re = re.compile(r"^\s*(?!return\b|else\b|case\b)(?:extern\s+\"C\"\s+)?(?:static\s+)?(?:const\s+)?"
                            r"[\w:<>]+\s*\**\s+(?:\w+::)?(%s|%s)\s*\(" % names, re.M)
        others = [p for p in Path("include").rglob("*.h") if p != PLAYER_H
                  and decl_re.search(p.read_text(errors="replace"))]
        others += [p for p in [*Path("src").rglob("*.cpp"), *Path("src").rglob("*.h")] if p != src_path(FWD_UNIT)
                   and src_re.search(p.read_text(errors="replace"))]
        if others:
            skipped.append(f"{s[0]}: declared or defined in {others[0]}")
            continue
        seen_callees.add(callee)
        plan.append({"name": s[0], "addr": text_base(FWD_UNIT) + s[1], "callee": callee, "callee_addr": ca,
                     "void": void, "nargs": nargs, "fb": fb, "ret": ret, "params": params,
                     "declared": declared})
    return plan, skipped


def fwd_render_def(m):
    sp = split_params(m["params"])
    args = ", ".join(n for t, n in sp[1:])
    if m["void"]:
        return FWD_DEF_VOID.format(name=m["name"], params=m["params"], mgr=FWD_MGR, callee=m["callee"], args=args)
    return FWD_DEF.format(ret=m["ret"], name=m["name"], params=m["params"], mgr=FWD_MGR, callee=m["callee"],
                          args=args, fb=fallback_literal(m["ret"], m["fb"][0]),
                          fb2=fallback_literal(m["ret"], m["fb"][1]))


def fwd_edit_source(plan, text):
    """Insert every member's definition in address order (deterministic, as
    edit_source)."""
    for m in sorted(plan, key=lambda m: m["addr"]):
        if re.search(r"^[^\n]*\b%s\s*\([^;{]*\)\s*\{" % m["name"], text, re.M):
            return None, f"{m['name']} already defined"
        text = insert_def(text, FWD_UNIT, m["addr"], fwd_render_def(m))
    return text, None


def fwd_decl_line(m):
    fb = "" if m["void"] else f"; else {fallback_literal(m['ret'], m['fb'][1])}"
    if not m["void"] and m["fb"][0] != m["fb"][1]:
        fb += f" ({fallback_literal(m['ret'], m['fb'][0])} without a manager)"
    return f"{m['ret']} {m['name']}({m['params']}); // -> {m['callee']}{fb}"


def fwd_member_line(m):
    sp = split_params(m["params"])
    return f"    {m['ret']} {m['callee']}({', '.join(f'{t} {n}' for t, n in sp[1:])}); // via {m['name']}"


def fwd_edit_header(plan, text):
    """Player.h: declared forwarders move out of the extern "C" block (with
    their comment when it describes only generated ones), new ones are
    declared after fn_8023E724, and each getter is declared in the class."""
    lines = text.split("\n")
    names = {m["name"] for m in plan}
    # groups of the extern "C" block: comment lines, then declaration lines
    try:
        c0 = lines.index('extern "C" {')
    except ValueError:
        return None, 'no extern "C" block'
    c1 = next(k for k in range(c0 + 1, len(lines)) if lines[k].startswith("}"))
    groups, cur = [], None
    for k in range(c0 + 1, c1):
        l = lines[k]
        is_c = l.lstrip().startswith("//")
        if not l.strip() or (is_c and cur and cur["decls"]) or cur is None:
            cur = {"comment": [], "decls": []}
            groups.append(cur)
        if not l.strip():
            continue
        (cur["comment"] if is_c and not cur["decls"] else cur["decls"]).append(k)
    moved_groups, moved_lines, drop = [], [], set()
    for g in groups:
        gen = [k for k in g["decls"] if (DECL_RE.match(lines[k]) or [None, None, ""])[2] in names]
        if not gen:
            continue
        if gen[0] == g["decls"][0]:
            # the comment goes with the run's leading generated declarations
            lead = [k for j, k in enumerate(g["decls"]) if all(x in gen for x in g["decls"][:j + 1])]
            unit = g["comment"] + lead
            moved_groups.append([scrub(lines[k]) for k in unit])
            drop |= set(unit)
            gen = [k for k in gen if k not in unit]
        moved_lines += [lines[k] for k in gen]
        drop |= set(gen)
    declared = {m["name"] for m in plan if m["declared"]}
    in_c = {DECL_RE.match(lines[k])[2] for k in drop if DECL_RE.match(lines[k])}
    outside = declared - in_c
    if outside:
        return None, f"declared outside the extern \"C\" block: {sorted(outside)[:3]}"
    new = [fwd_decl_line(m) for m in sorted(plan, key=lambda m: m["addr"]) if not m["declared"]]
    block = [FWD_DECL_HEAD.rstrip("\n")]
    block += sorted(new + moved_lines, key=lambda l: DECL_RE.match(l)[2])
    for g in moved_groups:
        block += [""] + g
    out = [l for k, l in enumerate(lines) if k not in drop]
    a = next((k for k, l in enumerate(out) if l.startswith(FWD_DECL_ANCHOR)), None)
    b = next((k for k, l in enumerate(out) if l.startswith(FWD_MEMBER_ANCHOR)), None)
    if a is None or b is None:
        return None, "Player.h anchors not found"
    members = [FWD_MEMBER_HEAD.rstrip("\n")] + [fwd_member_line(m) for m in
                                                sorted(plan, key=lambda m: m["callee_addr"])]
    out = out[:a + 1] + [""] + block + out[a + 1:]
    out = out[:b + 1] + members + out[b + 1:]
    return "\n".join(out), None


def scrub(line):
    """A moved comment no longer says its function is unwritten."""
    if not line.lstrip().startswith("//"):
        return line
    return re.sub(r" \(not written yet\)|, not written yet", "", line)


def fwd_header_problems(rows_plan, text):
    """Each generated forwarder declared once in Player.h, outside the
    extern "C" block, as the manifest's types; each getter declared once in
    Player_804A6F38 with the forwarder's return type and extra arguments."""
    bad = []
    decls = player_decls(text)
    for m in rows_plan:
        d = decls.get(m["name"], [])
        if len(d) != 1 or d[0][3]:
            bad.append(f"Player.h: {m['name']} is not declared exactly once outside extern \"C\"")
            continue
        sp = split_params(m["params"])
        want = re.escape(m["ret"]) + r"\s+" + m["callee"] + r"\(" + re.escape(
            ", ".join(f"{t} {n}" for t, n in sp[1:])) + r"\);"
        if len(re.findall(r"^    " + want, text, re.M)) != 1:
            bad.append(f"Player.h: {PLAYER_CLS}::{m['callee']} not declared once as {m['ret']}({m['params']} minus player)")
    return bad


def fwd_names(o):
    """{base name: mangled name} of our object's functions."""
    return {s[0].split("__")[0]: s[0] for s in o.funcs() if "__" in s[0]}


def fwd_member_problems(plan, diff):
    """Template identity per group, in dtk's split and in our object, and
    objdiff 100 %. plan members carry their mangled names (mname)."""
    bad = []
    t = obj_fresh(target_obj(FWD_UNIT))
    o = obj_fresh(our_obj(FWD_UNIT)) if our_obj(FWD_UNIT).exists() else None
    if o is None:
        return ["no compiled object"]
    groups = {}
    for m in sorted(plan, key=lambda m: m["addr"]):
        groups.setdefault((m["void"], m["nargs"], m["fb"]), []).append(m)
    for key, ms in groups.items():
        void, nargs, fb = key
        if (not void and nargs == 0 and fb == (0, 0)) or (void and nargs == 1):
            ref_t = fwd_template(void)[1]
            ref_t = (t, t.func(ref_t[0])[1])
            ref_o = o.func(FWD_TEMPLATES[void])
            ref_o = (o, ref_o[1]) if ref_o else None
        else:
            first = ms[0]["mname"]
            ref_t = (t, t.func(first)[1]) if t.func(first) else None
            ref_o = (o, o.func(first)[1]) if o.func(first) else None
        if not ref_t or not ref_o:
            bad.append(f"group {key}: template function missing")
            continue
        for m in ms:
            tf, of = t.func(m["mname"]), o.func(m["mname"])
            if not tf or not of:
                bad.append(f"{m['mname']}: missing in the {'target' if not tf else 'compiled'} object")
                continue
            sh = fwd_shape(t, tf[1])
            if not sh or sh[:3] != key or sh[3][0] != m["cname"]:
                bad.append(f"{m['mname']}: target is not the forwarder shape {key} calling {m['cname']}")
            if fwd_identity(t, tf[1]) != fwd_identity(*ref_t):
                bad.append(f"{m['mname']}: target differs from its group template beyond the getter")
            if fwd_identity(o, of[1]) != fwd_identity(*ref_o):
                bad.append(f"{m['mname']}: our compiled function differs from its group template beyond the getter")
            calls = [o.syms[r[2]][0] for r in o.relocs_in(of[1][5], of[1][1], of[1][2]) if r[1] == 10]
            if calls != [m["cname"]]:
                bad.append(f"{m['mname']}: calls {calls}, not {m['cname']}")
            if diff.get(m["mname"]) != 100:
                bad.append(f"{m['mname']}: objdiff {diff.get(m['mname'])}")
    return bad


def generate_fwd():
    global _objs
    ninja(str(our_obj(FWD_UNIT)))
    if FWD_UNIT in claimed():
        return f"{FWD_UNIT}: skipped: claimed in progress/claims.json", []
    plan, skipped = fwd_plan()
    if not plan:
        return f"{FWD_UNIT}: nothing to generate", skipped
    src0, hdr0 = src_path(FWD_UNIT).read_text(), PLAYER_H.read_text()
    src_before = blob(src_path(FWD_UNIT))
    before = matched(objdiff(FWD_UNIT))
    edits = []
    while plan:
        src, why = fwd_edit_source(plan, src0)
        hdr, why2 = fwd_edit_header(plan, hdr0) if not why else (None, None)
        if why or why2:
            return f"{FWD_UNIT}: refused: {why or why2}", skipped
        src_path(FWD_UNIT).write_text(src)
        PLAYER_H.write_text(hdr)
        out = ninja(str(our_obj(FWD_UNIT)))
        if out.returncode:
            src_path(FWD_UNIT).write_text(src0)
            PLAYER_H.write_text(hdr0)
            return f"{FWD_UNIT}: does not compile:\n" + (out.stdout + out.stderr)[-1500:], skipped
        o = Obj(our_obj(FWD_UNIT))
        mn = fwd_names(o)
        lines = symbols_lines()
        edits, drop = [], []
        for m in plan:
            m["mname"] = mn.get(m["name"])
            f = o.func(m["mname"]) if m["mname"] else None
            calls = [o.syms[r[2]][0] for r in o.relocs_in(f[1][5], f[1][1], f[1][2]) if r[1] == 10] if f else []
            if not f or len(calls) != 1 or f"__{len(PLAYER_CLS)}{PLAYER_CLS}F" not in calls[0]:
                drop.append((m, "our object lacks it or it does not call a Player member"))
                continue
            m["cname"] = calls[0]
            old = sym_line(lines, ".text", m["addr"], m["name"])
            oldc = sym_line(lines, ".text", m["callee_addr"], m["callee"])
            if not old or not oldc:
                drop.append((m, "symbols.txt line not found"))
                continue
            edits += [(".text", m["addr"], [old], [rename_line(old, m["mname"])]),
                      (".text", m["callee_addr"], [oldc], [rename_line(oldc, m["cname"])])]
        if drop:
            for m, why in drop:
                skipped.append(f"{m['name']}: backed out: {why}")
            plan = [m for m in plan if m not in [d[0] for d in drop]]
            continue
        apply_edits(edits)
        resplit()
        _objs = {}
        ninja(str(our_obj(FWD_UNIT)))
        diff = objdiff(FWD_UNIT) or {}
        probs = fwd_member_problems(plan, diff)
        lost = sorted(before - matched(diff))
        bad = {m["name"] for m in plan for p in probs if p.startswith(m["mname"] + ":")}
        if lost or (probs and not bad):
            apply_edits(edits, revert=True)
            src_path(FWD_UNIT).write_text(src0)
            PLAYER_H.write_text(hdr0)
            resplit()
            return f"{FWD_UNIT}: backed out: " + "; ".join(probs[:5] + [f"lost {', '.join(lost)}"] if lost else probs[:5]), skipped
        if not bad:
            break
        apply_edits(edits, revert=True)
        for p in probs:
            skipped.append(f"backed out: {p}")
        plan = [m for m in plan if m["name"] not in bad]
        resplit()
        _objs = {}
    if not plan:
        src_path(FWD_UNIT).write_text(src0)
        PLAYER_H.write_text(hdr0)
        return f"{FWD_UNIT}: nothing generated", skipped
    rows = MANIFEST.read_text().splitlines()[1:]
    for m in plan:
        rows.append(f"{FWD_UNIT}\tfwd\t{m['mname']}\t{m['cname']}\t-\t{src_before}\t-")
    MANIFEST.write_text("\n".join([MANIFEST_HEAD] + sorted(set(rows))) + "\n")
    return f"{FWD_UNIT}: ok: {len(before)} -> {len(matched(diff))} matched symbols, {len(plan)} forwarders", skipped


def fwd_check(rs, diff):
    """--check for the fwd rows: rebuild each member from dtk's split and
    Player.h, then the group checks, Player.h and the source text."""
    bad = []
    t = obj_fresh(target_obj(FWD_UNIT))
    text = PLAYER_H.read_text()
    decls = player_decls(text)
    plan = []
    for r in rs:
        mname, cname = r[2], r[3]
        f = t.func(mname)
        if not f:
            bad.append(f"{mname}: not in the target object (symbols.txt)")
            continue
        sh = fwd_shape(t, f[1])
        if not sh:
            bad.append(f"{mname}: target is not a forwarder")
            continue
        base = mname.split("__")[0]
        d = decls.get(base, [])
        if len(d) != 1:
            bad.append(f"Player.h: {base} declared {len(d)} times")
            continue
        callee = cname.split("__")[0]
        plan.append({"name": base, "mname": mname, "cname": cname, "callee": callee,
                     "addr": text_base(FWD_UNIT) + f[1][1], "void": sh[0], "nargs": sh[1], "fb": sh[2],
                     "ret": d[0][0], "params": d[0][1], "declared": True})
    if bad:
        return bad
    bad += fwd_member_problems(plan, diff)
    bad += fwd_header_problems(plan, text)
    befores = {r[5] for r in rs}
    if len(befores) != 1:
        return bad + ["manifest rows disagree on the source's pre-generation version"]
    old = git_blob(befores.pop())
    if old is None:
        return bad + [f"{src_path(FWD_UNIT)}: its pre-generation blob is not in git"]
    want, why = fwd_edit_source(plan, old)
    have = src_path(FWD_UNIT).read_text()
    if why or want != have:
        bad.append(f"{src_path(FWD_UNIT)}: not exactly the generator's output"
                   + (f" ({why})" if why else first_diff(want or "", have)))
    return bad


def main():
    argv = sys.argv[1:]
    if not argv or "-h" in argv or "--help" in argv:
        print(__doc__)
        return
    if "--scan" in argv:
        scan(argv)
        return
    units = [unit_slug(a) for a in argv if not a.startswith("-")]
    if "--check" in argv:
        sys.exit(0 if check(units) else 1)
    known = set(game_units())
    for u in units:
        if u not in known:
            die(f"{u}: not a game unit")
    if FWD_UNIT in units:
        status, skipped = generate_fwd()
        print(status)
        for sk in skipped:
            print(f"  skipped variant: {sk}")
        units = [u for u in units if u != FWD_UNIT]
        if not units:
            return
    report = generate(units)
    for u in units:
        status, skipped = report.get(u, ("?", []))
        print(f"{u}: {status}")
        for s in skipped:
            print(f"  skipped variant: {s}")


if __name__ == "__main__":
    main()

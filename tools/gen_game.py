#!/usr/bin/env python3
"""Template generator for the two biggest game clone families (GAME_PLAN G2/G3).

Families (templates: game/act_vaati_buster.cpp, hand-matched and reviewed):

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

Generation: writes the source/header, compiles each unit, reads the @NNN,
sinit and mangled names back from our object, edits each unit's own lines
of symbols.txt in place (never re-sorts it), re-splits, recompiles and
diffs every unit with objdiff-cli (functionRelocDiffs=all, as the honest
report). A unit whose generated functions are not all 100 %, or where a
function that matched before drops, is restored (source, header,
symbols.txt lines) and the split redone. Generated members are recorded in
config/G4SE01/gen_game.tsv; that file is what --check checks.

--check (for the reviewer and the gate), per recorded member:
  1. the target function (dtk's split of the original) is the template's
     target up to identifiers;
  2. our compiled function is the template's compiled function up to
     identifiers;
  3. the generated source text is the template's text with only the
     identifiers (class name, vtable address, overridden slots, size)
     substituted, and the unit has nothing else in its place: the dtor
     definition is present verbatim, the header class block is exactly the
     rendered block, and a sinit unit includes game/GameAudio.h before
     anything else;
  4. objdiff gives the member 100 %.
It prints one line per unit and `check: ok` or the problems; exit 1 on any.
"""

import json
import re
import struct
import subprocess
import textwrap
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
    return sizes.pop() if len(sizes) == 1 else None


def class_name(unit, vt, ctors):
    """`<EnglishName>_<vtable>` when game_units.tsv ties one of this class's
    constructors to exactly one actor ID whose actors.tsv name is plain words
    (high/med confidence); `cls_<vtable>` otherwise. Returns (name, why)."""
    ev = evidence_for(unit)
    addrs = {c[3:].upper() for c in ctors if re.fullmatch(r"fn_[0-9A-Fa-f]{8}", c)}
    hits = [(i, c) for i, c in re.findall(r"([A-Z0-9_]{4}) \((?:vtable [0-9A-F]{8} set by )?ctor ([0-9A-F]{8})\)", ev)
            if c in addrs]
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


def render_class(cls, naming, overrides, size):
    slots = "".join(SLOT_LINE.format(off=k, decl=SLOT_DECLS[k]) for k in sorted(overrides))
    fields = ""
    if size and size > ACTOR_BASE_SIZE:
        fields = PAD_LINE.format(off=ACTOR_BASE_SIZE, n=size - ACTOR_BASE_SIZE)
    return CLASS_BLOCK.format(naming=comment(naming), cls=cls, slots=slots, fields=fields,
                              size=f"0x{size:X}" if size else "unknown, no `new` of it found")


def dtor_plan(unit, d):
    ctors = ctors_of(unit, d["vt_name"], d["name"])
    cls, naming = class_name(unit, d["vt"], ctors)
    size = alloc_size(ctors)
    if size is not None and size < ACTOR_BASE_SIZE:
        return None
    return {"cls": cls, "naming": naming, "size": size, "block": render_class(cls, naming, d["overrides"], size),
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
    """{target function name: match % (None when we lack it)}."""
    cmd = [OBJDIFF, "diff", "-p", ".", "-u", f"fsa_game_code/game/{unit}", "-o", "-",
           "-c", "functionRelocDiffs=all"]
    out = subprocess.run(cmd, capture_output=True, text=True)
    if out.returncode:
        return None
    d = json.loads(out.stdout)
    return {s["name"]: (float(s.get("match_percent") or 0) if s.get("target_symbol") is not None else None)
            for s in d["left"]["symbols"] if s.get("kind") == "SYMBOL_FUNCTION"}


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


def new_source(unit, plan):
    parts = ["the JAudio pools' __sinit (game/GameAudio.h)"] if plan["sinit"] else []
    if plan["dtors"]:
        parts.append("the destructor")
    out = (f"// So far only {' and '.join(parts)}, from tools/gen_game.py;\n"
           f"// the rest of this unit is still to be written.\n\n{SINIT_INCLUDE}\n")
    if plan["dtors"]:
        out += f'#include "game/act/{unit}.h"\n'
        for d in plan["dtors"]:
            out += "\n" + d["def"]
    return out


def edit_source(unit, plan, text):
    """Returns (new text, problem)."""
    if text is None:
        return new_source(unit, plan), None
    incs = INCLUDE.findall(text)
    if "game/GameAudio.h" not in incs:
        if any(i.startswith(("JSystem/JAudio", "game/GameAudioBase.h")) for i in incs):
            return None, "has JAudio includes but not game/GameAudio.h"
        m = INCLUDE.search(text)
        pos = m.start() if m else len(text)
        text = text[:pos] + SINIT_INCLUDE + "\n" + text[pos:]
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


def members(plan, edits_names):
    out = []
    if plan["sinit"]:
        out.append(("sinit", edits_names.get(plan["sinit"]["addr"]), "", ""))
    for d in plan["dtors"]:
        out.append(("dtor", f"__dt__{len(d['cls'])}{d['cls']}Fv", d["cls"], f"0x{d['vt']:08X}"))
    return out


def generate(units):
    ninja(str(our_obj(TEMPLATE_UNIT)))
    plans, report = {}, {}
    for u in units:
        p = plan_unit(u)
        if not p["sinit"] and not p["dtors"]:
            report[u] = ("nothing to generate", p["skipped"])
            continue
        plans[u] = p
    snaps, before = {}, {}
    for u, p in list(plans.items()):
        snaps[u] = snapshot(u)
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
    while plans:
        resplit()
        ninja(*[str(our_obj(u)) for u in plans], keep_going=True)
        bad = []
        for u, p in plans.items():
            diff = objdiff(u) or {}
            new_names = {e[1]: e[3][0].split(" = ")[0] for e in all_edits[u] if e[0] == ".text"}
            gen = [n for n in new_names.values() if n.startswith(("__sinit_", "__dt__"))]
            lost = sorted(before[u] - matched(diff))
            low = [f"{n} {diff.get(n)}" for n in gen if diff.get(n) != 100]
            if low or lost:
                bad.append(u)
                report[u] = (f"backed out: {'; '.join(low)}{' lost ' + ', '.join(lost) if lost else ''}",
                             p["skipped"])
            else:
                after = matched(diff)
                report[u] = (f"ok: {len(before[u])} -> {len(after)} functions match ("
                             + ", ".join(sorted(gen)) + ")", p["skipped"])
                p["names"] = new_names
        for u in bad:
            apply_edits(all_edits.pop(u), revert=True)
            restore(snaps[u])
            del plans[u]
        if not bad:
            break
    if not plans and all_edits == {}:
        resplit()  # everything was backed out after a split: split the restored symbols again
    rows = MANIFEST.read_text().splitlines() if MANIFEST.exists() else \
        ["unit\tfamily\tfunction\tclass\tvtable"]
    for u, p in plans.items():
        for fam, fn, cls, vt in members(p, p["names"]):
            row = f"{u}\t{fam}\t{fn}\t{cls}\t{vt}"
            if row not in rows:
                rows.append(row)
    MANIFEST.write_text("\n".join([rows[0]] + sorted(rows[1:])) + "\n")
    return report


# ---------------------------------------------------------------- check


def check(units):
    rows = [r.split("\t") for r in MANIFEST.read_text().splitlines()[1:] if r.strip()]
    if units:
        rows = [r for r in rows if r[0] in units]
    problems = []
    by_unit = {}
    for r in rows:
        by_unit.setdefault(r[0], []).append(r)
    cfg = Path("build/G4SE01/config.json")
    if cfg.exists() and any(p.stat().st_mtime > cfg.stat().st_mtime for p in (SYMBOLS, SPLITS)):
        cfg.unlink()
    ninja("build/G4SE01/config.json", *[str(our_obj(u)) for u in by_unit], keep_going=True)
    for u, rs in sorted(by_unit.items()):
        bad = []
        t, o = Obj(target_obj(u)), Obj(our_obj(u)) if our_obj(u).exists() else None
        diff = objdiff(u) or {}
        src = src_path(u).read_text() if src_path(u).exists() else ""
        hdr = hdr_path(u).read_text() if hdr_path(u).exists() else ""
        for _, fam, fn, cls, vt in rs:
            tf, of = t.func(fn), (o.func(fn) if o else None)
            if not tf:
                bad.append(f"{fn}: not in the target object (symbols.txt)")
                continue
            if not same_as_template(t, tf[1], fam):
                bad.append(f"{fn}: target is not the {fam} template up to identifiers")
            if not of or not same_as_template(o, of[1], fam, ours=True):
                bad.append(f"{fn}: our compiled function is not the template's up to identifiers")
            if diff.get(fn) != 100:
                bad.append(f"{fn}: objdiff {diff.get(fn)}")
            incs = INCLUDE.findall(src)
            if not incs or incs[0] != "game/GameAudio.h":
                bad.append("game/GameAudio.h is not the first include")
            if fam == "dtor":
                d, why = dtor_info(t, u, tf[1])
                if why:
                    bad.append(f"{fn}: {why}")
                    continue
                p = dtor_plan(u, d)
                if not p or p["cls"] != cls or f"0x{d['vt']:08X}" != vt:
                    bad.append(f"{fn}: class/vtable no longer derive as {cls} {vt}")
                    continue
                if hdr.count(p["block"]) != 1 or len(re.findall(r"\bclass %s\b" % cls, hdr)) != 1:
                    bad.append(f"{fn}: the header's class block is not the template's rendering")
                if src.count(p["def"]) != 1:
                    bad.append(f"{fn}: the definition is not the template's `{p['def'].strip()}`")
        print(f"{u}: {', '.join(r[2] for r in rs)}: " + ("ok" if not bad else "FAIL"))
        for b in bad:
            print(f"  {b}")
        problems += bad
    print("check: ok" if not problems else f"check: {len(problems)} problem(s)")
    return not problems


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
    report = generate(units)
    for u in units:
        status, skipped = report.get(u, ("?", []))
        print(f"{u}: {status}")
        for s in skipped:
            print(f"  skipped variant: {s}")


if __name__ == "__main__":
    main()

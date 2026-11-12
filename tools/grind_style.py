#!/usr/bin/env python3
"""Script-checked style rules for grind workers (tools/grind.py imports this).

Ported unchanged from the first grinder (branch `grind`, tools/grind.py), whose rules
came out of three review rounds of Haiku output: named literals, hex tags, prototypes
in a .cpp, extern "C" stand-ins for members, overlay structs, hand-written static
guards, byte-offset access, register-named locals, matching-only notes, empty tables,
vtable slots on the wrong class, and definitions out of address order.
"""

import re
import subprocess
from pathlib import Path


def sh(cmd, cwd, check=False, timeout=None, **kw):
    r = subprocess.run(cmd, cwd=cwd, shell=isinstance(cmd, str), capture_output=True,
                       text=True, timeout=timeout, **kw)
    if check and r.returncode:
        raise RuntimeError(f"{cmd}: {r.stdout[-2000:]}{r.stderr[-2000:]}")
    return r


BANNED = re.compile(r"\basm\b|__asm|fakematch|force_order|\bgoto\b|NO_INLINE|\bvolatile\b|dummy", re.I)
PRAGMA = re.compile(r"#\s*pragma\s+(?!auto_inline\s+(?:off|reset)\b)")
def code_part(line):
    """A diff line without its comments: "read off the asm" in a comment isn't an asm block."""
    t = line[1:] if line[:1] in "+-" else line
    if t.lstrip().startswith(("*", "/*")):
        return ""
    return re.sub(r"/\*.*?\*/", "", t).split("//")[0]
# A placeholder free function whose first parameter is a class pointer is a member function.
C_TAKES_CLASS = re.compile(r'extern\s+"C"\s+[\w\s\*]+\bfn_\w+\s*\(\s*(const\s+)?[A-Z]\w*\s*\*')


# Review 2026-11-03 (F1, F5, F7): byte-identical but not the original source.
NAMED_LITERAL = re.compile(  # F1: a unit's own .sdata2 float or string behind an lbl_ name
    r"\b(f32|f64|float|double)\b[^;=(]*\blbl_[0-9A-Fa-f]{8}\s*\[\s*1?\s*\]\s*[;=]"
    r"|\bchar\s+lbl_[0-9A-Fa-f]{8}\s*\[\s*\d*\s*\]\s*=\s*\"")
OWN_EXTERN = re.compile(r"\bextern\s+(\"C\"\s+)?const\s+(f32|f64|float|double)\s+lbl_")  # F1, in a .cpp
ONE = re.compile(r"\b(?:f32|f64|float|double)\b[^;=(]*\b(lbl_[0-9A-Fa-f]{8})\s*\[\s*1?\s*\]")
PROTO = re.compile(r"^\s*(extern\s+(\"C\"\s+)?)?(const\s+)?(?!return\b|else\b|delete\b|new\b)\w[\w:<>]*[\s\*&]+"
                   r"(\w+::)?fn_8[0-9A-Fa-f]{7}\w*\s*\([^)]*\)\s*;")  # F5
HEX = re.compile(r"\b0x([0-9A-Fa-f]{16}|[0-9A-Fa-f]{8})\b")  # F7


def fourcc(h):
    b = bytes.fromhex(h)
    return all(0x41 <= c <= 0x5A or 0x30 <= c <= 0x39 or c in (0x20, 0x5F) for c in b) and any(0x41 <= c <= 0x5A for c in b)


def added_lines(wt):
    """{path: set of added line numbers (1-based) in the new file}; new files: every line."""
    out, path = {}, None
    for l in sh(["git", "diff", "-U0", "--", "src", "include"], wt).stdout.splitlines():
        if l.startswith("+++ "):
            path = l[6:] if l.startswith("+++ b/") else None
        elif l.startswith("@@") and path:
            m = re.search(r"\+(\d+)(?:,(\d+))?", l)
            start, n = int(m.group(1)), int(m.group(2) or 1)
            out.setdefault(path, set()).update(range(start, start + n))
    for f in sh(["git", "ls-files", "--others", "--exclude-standard", "src", "include"], wt).stdout.split():
        out[f] = None
    return out


def unit_ranges(wt, unit):
    """{section: (start, end)} of a unit in splits.txt."""
    out, cur = {}, False
    for l in (wt / "config/G4SE01/splits.txt").read_text().splitlines():
        if l and not l[0].isspace():
            cur = l.rstrip(":") == unit
        elif cur:
            m = re.match(r"\s+(\S+)\s+start:0x([0-9A-Fa-f]+)\s+end:0x([0-9A-Fa-f]+)", l)
            if m:
                out[m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16))
    return out


GAME_CLASS = r"(?:cls_[0-9A-Fa-f]{8}|\w+_80[0-9A-Fa-f]{6})"
# Review 2: a placeholder of this unit declared C-linkage with a game-class (or void* self) first parameter.
C_MEMBER = re.compile(rf"\b(fn_(8[0-9A-Fa-f]{{7}}))\w*\s*\(\s*(?:const\s+)?(?:{GAME_CLASS}\s*\*|void\s*\*\s*(?:self|obj|this_?)\b)")
OVERLAY = re.compile(r"^\s*(?:struct\s+\w+\s*\{\s*)?u8\s+(?:field|pad|unk)_0x0\s*\[\s*0x[0-9A-Fa-f]+\s*\]\s*;")
EXTERN_LBL = re.compile(r"\bextern\b[^;(]*?\blbl_([0-9A-Fa-f]{8})\b")
# Port review 2026-11-04: what the Opus port of grind into matching had to fix.
FUNC_HEAD = re.compile(r"^\s*(?!return\b|else\b|if\b|while\b|for\b|switch\b)[\w:<>]+[\s\*&]+[\w:~]+\s*\([^;]*\)\s*(\{.*)?$")
C_DECL = re.compile(rf"\b\w*__\w*\s*\(|\bfn_8[0-9A-Fa-f]{{7}}\w*\s*\(\s*(?:const\s+)?(?:{GAME_CLASS}\s*\*|void\s*\*\s*(?:self|obj|this_?)\b)")
BYTE_PTR = re.compile(r"\(\s*(?:const\s+)?(?:u8|s8|char)\s*\*\s*\)\s*[\w.>\-]+\s*\)?\s*\+\s*(?:0x[0-9A-Fa-f]+|\d+)")
REG_LOCAL = re.compile(r"\b(?:[su](?:8|16|32)|int|f32|f64|float|double|bool|BOOL|\w+\s*\*)\s+([rf](?:[0-9]|[12][0-9]|3[01]))\s*(?:[;=]|,(?=[^()]*;))")  # locals, not parameters
MATCH_NOTE = re.compile(r"matching only|matching-only|what matches|stands in for|to match (?:the )?reg", re.I)
EMPTY_TABLE = re.compile(r"\]\s*=\s*\{\s*\}\s*;")


def style_violation(path, text, lines=None, ranges=None):
    """First F1/F5/F7 violation among `lines` (None: all) of a C/C++ file, or None. With the
    unit's split `ranges`, also review 2's unit-aware rules."""
    depth, extern_c, cpp = 0, [], path.endswith(".cpp")
    ones = set(ONE.findall(text))  # one-element literal arrays, read as name[0]
    # Hand-written guard of a function-local static: if (!lbl_X) { ...; lbl_X = 1; }
    guards = {n for n in re.findall(r"if\s*\(\s*!\s*(lbl_[0-9A-Fa-f]{8})\s*\)", text)
              if re.search(rf"\b{n}\s*=\s*1\s*;", text)}
    text_r = (ranges or {}).get(".text")
    in_unit = lambda a: any(lo <= a < hi for lo, hi in (ranges or {}).values())
    for i, raw in enumerate(text.splitlines(), 1):
        code = code_part(raw)
        if (lines is None or i in lines) and MATCH_NOTE.search(raw):
            return f"matching-only construct (write the natural code): {path}:{i}: {raw.strip()[:70]}"
        if (lines is None or i in lines) and code.strip():
            where = f"{path}:{i}: {raw.strip()[:70]}"
            if NAMED_LITERAL.search(code) or (cpp and OWN_EXTERN.search(code)) or any(
                    re.search(rf"\b{n}\s*\[\s*0\s*\]", code) for n in ones):
                return f"named literal (write 1.0f / \"text\", not lbl_): {where}"
            if HEX.search(code) and any(fourcc(h) for h in HEX.findall(code)):
                return f"tag as hex (write 'NULL'): {where}"
            if cpp and depth == len(extern_c) and PROTO.search(code):
                return f"prototype in a .cpp (declare it in the owner's shared header): {where}"
            if path.startswith("include/game/act/") and 'extern "C"' in code:
                return f"extern C in an act header (use the owner's shared header): {where}"
            if cpp and OVERLAY.search(code):
                return f"overlay struct (declare the real member in the class's header): {where}"
            if any(re.search(rf"\b{n}\b", code) for n in guards):
                return f"hand-written static guard (write a function-local static): {where}"
            m = C_MEMBER.search(code)
            if m and text_r and (bool(extern_c) or 'extern "C"' in code) and text_r[0] <= int(m.group(2), 16) < text_r[1]:
                return f"this unit's function declared extern C taking a class (write a member): {where}"
            m = EXTERN_LBL.search(code)
            if (m and cpp and ranges and "=" not in code and in_unit(int(m.group(1), 16))
                    and not re.search(rf"\blbl_{m.group(1)}\b[^;(]*=", text)):
                return f"extern of this unit's own data (define it here): {where}"
            c_line = 'extern "C"' in code or bool(extern_c)
            if cpp and c_line and FUNC_HEAD.search(code.replace('extern "C"', '')) and not code.rstrip().endswith(";"):
                return f"extern C definition in a .cpp (write it as C++ or a member): {where}"
            if c_line and C_DECL.search(code):
                return f"extern C for a mangled name or a class's function (write a member): {where}"
            if "reinterpret_cast" in code or BYTE_PTR.search(code):
                return f"byte-offset pointer access (declare the field in the class): {where}"
            if REG_LOCAL.search(code):
                return f"local named like a register (name it for what it holds): {where}"
            if EMPTY_TABLE.search(code):
                return f"empty table initializer (write the real entries): {where}"
        if re.search(r'extern\s+"C"\s*\{', code):
            extern_c.append(depth)
        depth += code.count("{") - code.count("}")
        while extern_c and depth <= extern_c[-1]:
            extern_c.pop()
    return None


def style_check(wt, unit=None):
    ranges = unit_ranges(wt, unit) if unit else None
    for path, lines in added_lines(wt).items():
        if path.endswith((".cpp", ".h", ".c")) and (wt / path).exists():
            v = style_violation(path, (wt / path).read_text(errors="replace"), lines, ranges)
            if v:
                return v
    return None


def class_of(name):
    """Class name from a CodeWarrior-mangled member (fn_X__15Player_804A6F38Fv -> Player_804A6F38)."""
    m = re.match(r".+?__(\d+)([A-Za-z_]\w*)", name)
    return m.group(2)[:int(m.group(1))] if m else None



def vtables_of(wt, name):
    """The asm __vt__ tables that list `name`."""
    asm = [str(a) for a in (wt / "build/G4SE01/asm/game").glob("*.s")]
    tables = set()
    for f in sh(["grep", "-l", "-w", name] + asm, wt).stdout.split():
        cur = None
        for l in Path(f).read_text(errors="replace").splitlines():
            if l.startswith(".obj "):
                cur = l.split()[1].rstrip(",")
            elif l.startswith(".endobj"):
                cur = None
            elif cur and cur.startswith("__vt__") and re.search(rf"\b{re.escape(name)}\b", l):
                tables.add(cur)
    return tables


def vtable_owner_problem(wt, ren, written=()):
    """A member named onto class X whose function sits only in other classes' vtables (asm), or a
    vtable slot (renamed or newly written) that the source doesn't declare virtual."""
    for new in set(ren.values()) | set(written):
        tables = vtables_of(wt, new)
        if not tables:
            continue
        cls = class_of(new)
        if new in ren.values() and cls and not any(t == f"__vt__{len(cls)}{cls}" for t in tables):
            return f"{new} is in {sorted(tables)[0]}, not {cls}'s vtable (wrong class)"
        base = new.split("__")[0] if not new.startswith("__") else None
        if base and not sh(["grep", "-rqE", rf"\bvirtual\b[^;]*\b{re.escape(base)}\s*\(", "include/game", "src/game"], wt).returncode == 0:
            return f"{new} is a slot of {sorted(tables)[0]}: declare it virtual on its class (vfunc_0xNN, update, draw)"
    return None


def order_problem(wt, unit, changed):
    """A changed function compiled out of the original's address order (game code is -sym off:
    the link needs the original order)."""
    lo, hi = unit_ranges(wt, unit).get(".text", (0, 0))
    addr = {}
    for l in (wt / "config/G4SE01/symbols.txt").read_text().splitlines():
        m = re.match(r"(\S+) = \.text:0x([0-9A-Fa-f]+);", l)
        if m and lo <= int(m.group(2), 16) < hi:
            addr[m.group(1)] = int(m.group(2), 16)
    obj = wt / "build/G4SE01/src" / unit.replace(".cpp", ".o")
    nm = sh(["build/binutils/powerpc-eabi-nm", "-n", "--defined-only", str(obj)], wt).stdout.splitlines()
    seq = [p[2] for p in (l.split(None, 2) for l in nm) if len(p) == 3 and p[1] in "tT" and p[2] in addr]  # weak inlines go elsewhere
    for a, b in zip(seq, seq[1:]):
        if addr[a] > addr[b] and (a in changed or b in changed):
            return f"{a} is compiled before {b} but sits after it in the original: keep definitions in address order"
    return None

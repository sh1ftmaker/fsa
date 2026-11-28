#!/usr/bin/env python3
"""Source permuter for the game's C++: search rewrites of ONE function for the form MWCC compiles
to the original code.

  python3 tools/permute.py <unit> <function> [--tier a|b] [--kinds decl,commute,...] [--depth 4]
                                             [--beam 6] [--budget 1500] [--apply] [--json FILE] [-v]
  python3 tools/permute.py --bulk [--class regalloc-only,...] [--units part,...] [--parallel N] [--apply] [--log FILE]
  python3 tools/permute.py <unit> <function> --deep [--tier b]     # minutes, not seconds: for a stubborn function
  python3 tools/permute.py --callees [--units part,...] [--parallel N] [--apply] [--log FILE]
  python3 tools/permute.py --callees --chain --kinds nest,decl [--units part,...]   # an inlining chain's helpers together
  python3 tools/permute.py --list-kinds

What nobody can read off the assembly, a search can find: which of two locals was declared first,
which operand of a `+` came first, whether an `if` tested the condition or its negation. MWCC's
register allocation, instruction order and branch layout follow such choices, so a function whose
instructions are right but sit in other registers or another order is mostly a search problem.

How it works: the source is parsed with libclang (the real C++ grammar and types); each rewrite is
a text edit of the function (or of a function of this file inlined into it); every candidate is
compiled and scored by the native farm (tools/fnscore.cpp, ~100 candidates a second; only the slice
of the file the function needs is compiled); candidates that lower the score seed the next round
(beam search, with a few sideways moves kept; improvements in different places of the function
are also tried together, since they mostly add up). The result is then checked with objdiff on the
whole unit: it is reported only if the function got closer and NOTHING else in the unit got worse.

Tier a rewrites keep the function's behaviour by construction and read as code a programmer wrote
(they never add a statement, a cast or dead code; docs/RULES_CARD.md "No tricks"):
  decl      order of local declarations: swap or move them, try every order of a short run, hoist
            one to the top (`T x = e;` -> `T x;` ... `x = e;`), sink a top one into its first assignment
  commute   operand order of + * & | ^ == != and < > <= >= (mirrored), both sides free of side effects
  compound  `x += e`  <->  `x = x + e`  <->  `x = e + x`
  if        `if (c) A else B`  <->  `if (!c) B else A`
  andif     `if (a && b) X`  <->  `if (a) { if (b) X }`
  early     `if (c) { ... }` at the end of a void function or loop body  <->  `if (!c) return;` / `continue;`
  ternary   `if (c) x = a; else x = b;`  <->  `x = c ? a : b;`  (and the same with return)
  stmt      order of two adjacent independent stores of plain values
  loop      a run of `a[0] = v; a[1] = v; ...`  ->  a for loop (MWCC unrolls short loops)
  incr      x++ / ++x / x += 1 as a statement
  cond      `x != 0` <-> `x`, `x == 0` <-> `!x` in conditions
  untemp    a local used once, in the next statement, replaced by its value
  scope     an uninitialised declaration moved into the block that uses it, or out of it
  boolret   `if (c) return true; return false;`  <->  `return c;`
  const     const put on, or taken off, a local that is never written
  intlong   a local declared `int` instead of `s32` or the other way round (same size, but another type
            to MWCC: handed to an inline function's parameter of the other type it is copied first)

Tier b rewrites may change behaviour, so a result that used one is accepted ONLY at 100 %: then
the compiled code is the original's, which proves the rewrite right (the reviewer still judges
whether it reads naturally; a new local needs a real name):
  type      an integer or float local declared as another width or signedness
  temp      a value read several times held in a local (declared before its first use)
  inline    a local with one assignment replaced by its value at every use
  swap      order of any two adjacent expression statements
  cast      an explicit cast removed
  loopform  a for loop written as a while loop
  retvar    a returned expression held in a local first
  outline   a loop moved into a small inline helper; what it reads of the function's locals becomes the
            helper's parameters (its counter is then created at inlining and coloured before the
            optimiser's loop temporaries)
  outblock  the same for an `if`, a switch or a braced block that declares locals
  outrun    the same for a run of statements: a declaration up to the last statement that uses it
  nest      a member read kept in a local, read through a small inline with a local of its own (changes
            when MWCC creates the value, so in which order it is coloured: docs/REGALLOC.md)

Functions of this file that MWCC inlined into the target are rewritten too (the diff's lines sit
at the call, so the cause is often in the callee); the whole-unit check covers their other users.

--callees works the other way round: it takes each function of a file that MWCC inlined into
functions that are still open (most inliners first), rewrites THAT function and scores every
function of the unit. A callee can match on its own while its inlined copies do not: one moved
declaration in Player's 0x80248970 made five of its callers match. Such a result is kept when
more functions match, none that matched breaks and no data section gets worse; functions that
were open may move either way.

--shared looks for a loop or block that one file writes out several times, word for word, puts it
into ONE inline helper and scores the unit the same way (all its copies, or only those in functions
that are still open). In the original such a statement was an inline function: FileSelect's three
pane loops, written out 40 times, only got the original's registers as helpers.

Exit status 0 when an improvement was found, 1 when not. --apply writes the result into the source
(otherwise the diff is printed and saved under build/permute/). A result below 100 % that used a
tier b rewrite is printed as a lead and never applied. --bulk saves every result's diff under
build/permute/bulk/ and, with --apply, writes a result at 100 % into the source unless it adds or
removes a local (kinds temp, inline, retvar: saved as <unit>__<function>.hand.diff). Those are
proven right by their 100 % but need a hand first: a real name for the local, or the macro or
inline the original had where a value is now written out twice.
"""

import argparse
import difflib
import hashlib
import itertools
import json
import re
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import scratch  # noqa: E402

try:
    import clang.cindex as C
except ImportError:  # pragma: no cover
    sys.exit("tools/permute.py needs the libclang Python bindings (python-clang / `pip install libclang`)")

K, TK = C.CursorKind, C.TypeKind
FUNC_KINDS = (K.FUNCTION_DECL, K.CXX_METHOD, K.CONSTRUCTOR, K.DESTRUCTOR, K.CONVERSION_FUNCTION)
SCALAR = {TK.BOOL, TK.CHAR_U, TK.UCHAR, TK.CHAR_S, TK.SCHAR, TK.USHORT, TK.SHORT, TK.UINT, TK.INT, TK.ULONG, TK.LONG,
          TK.ULONGLONG, TK.LONGLONG, TK.FLOAT, TK.DOUBLE, TK.POINTER, TK.ENUM, TK.WCHAR}
FLOATS = {TK.FLOAT, TK.DOUBLE}
INTEGRAL = SCALAR - FLOATS
IMPURE = {K.CALL_EXPR, K.CXX_NEW_EXPR, K.CXX_DELETE_EXPR, K.CXX_THROW_EXPR, K.COMPOUND_ASSIGNMENT_OPERATOR, K.LAMBDA_EXPR}
PRIMARY = {K.DECL_REF_EXPR, K.MEMBER_REF_EXPR, K.INTEGER_LITERAL, K.FLOATING_LITERAL, K.CALL_EXPR, K.PAREN_EXPR,
           K.ARRAY_SUBSCRIPT_EXPR, K.CXX_BOOL_LITERAL_EXPR, K.CHARACTER_LITERAL, K.CXX_THIS_EXPR, K.STRING_LITERAL,
           K.CXX_NULL_PTR_LITERAL_EXPR, K.GNU_NULL_EXPR}
COMMUTE = {b"+": b"+", b"*": b"*", b"&": b"&", b"|": b"|", b"^": b"^", b"==": b"==", b"!=": b"!=",
           b"<": b">", b">": b"<", b"<=": b">=", b">=": b"<="}
NEGATE = {b"==": b"!=", b"!=": b"==", b"<": b">=", b">=": b"<", b">": b"<=", b"<=": b">"}
ARITH = {b"+", b"-", b"*", b"/", b"%", b"&", b"|", b"^", b"<<", b">>"}
PREC = {b"*": 5, b"/": 5, b"%": 5, b"+": 6, b"-": 6, b"<<": 7, b">>": 7, b"<": 9, b">": 9, b"<=": 9, b">=": 9,
        b"==": 10, b"!=": 10, b"&": 11, b"^": 12, b"|": 13, b"&&": 14, b"||": 15, b"=": 16, b",": 17}
INT_TYPES = [b"s32", b"u32", b"s16", b"u16", b"s8", b"u8", b"int"]
FLOAT_TYPES = [b"f32", b"f64"]
TYPE_FAMILY = {b"s32": INT_TYPES, b"u32": INT_TYPES, b"s16": INT_TYPES, b"u16": INT_TYPES, b"s8": INT_TYPES,
               b"u8": INT_TYPES, b"int": INT_TYPES, b"long": INT_TYPES, b"short": INT_TYPES, b"char": INT_TYPES,
               b"unsigned int": INT_TYPES, b"unsigned long": INT_TYPES, b"unsigned short": INT_TYPES,
               b"unsigned char": INT_TYPES, b"signed char": INT_TYPES, b"bool": INT_TYPES, b"BOOL": INT_TYPES,
               b"f32": FLOAT_TYPES, b"f64": FLOAT_TYPES, b"float": FLOAT_TYPES, b"double": FLOAT_TYPES}
TIER = {"decl": "a", "commute": "a", "scope": "a", "compound": "a", "if": "a", "boolret": "a", "andif": "a", "early": "a",
        "ternary": "a", "stmt": "a", "const": "a", "loop": "a", "incr": "a", "cond": "a", "untemp": "a", "intlong": "a",
        "type": "b", "temp": "b", "inline": "b", "swap": "b", "cast": "b", "loopform": "b", "retvar": "b", "nest": "b", "outline": "b", "outblock": "b", "outrun": "b", "callfirst": "b"}
# results a person or a session has to finish before they go into the source: a local that needs a
# real name, or a value written out twice that was probably a macro or an inline in the original
NEEDS_HAND = {"callfirst", "temp", "inline", "retvar", "nest", "outline", "outblock", "outrun"}
ABOVE_FN = ("nest", "outline", "outblock", "outrun")  # kinds that also write a helper above the function
KINDS_A = [k for k, t in TIER.items() if t == "a"]
KINDS_B = [k for k, t in TIER.items() if t == "b"]
_INDEX = None


def D(b, n=40):
    s = b.decode("utf-8", "replace")
    s = " ".join(s.split())
    return s if len(s) <= n else s[:n - 1] + "~"


class Edit:
    __slots__ = ("kind", "desc", "reps", "line")

    def __init__(self, kind, desc, reps, line=None):
        self.kind, self.desc, self.reps, self.line = kind, desc, reps, line

    def apply(self, data):
        reps = sorted(self.reps, key=lambda r: (r[0], r[1]))
        for (s1, e1, _), (s2, _, _) in zip(reps, reps[1:]):
            if e1 > s2:
                return None
        out, pos = [], 0
        for s, e, new in reps:
            out += [data[pos:s], new]
            pos = e
        out.append(data[pos:])
        return b"".join(out)


# ---------------------------------------------------------------- parsing

def clang_args(unit):
    argv, out, k = unit.command(), [], 0
    while k < len(argv):
        a = argv[k]
        if a in ("-i", "-I") and k + 1 < len(argv):
            out.append("-I" + str((unit.root / argv[k + 1]).resolve()))
            k += 1
        elif a.startswith("-D"):
            out.append(a)
        k += 1
    return ["-x", "c++", "-std=c++98", "-fsyntax-only", "-nostdinc", "-nostdinc++", "-ferror-limit=0", "-Wno-everything",
            "-fms-extensions", "-target", "powerpc-unknown-eabi", "-fsigned-char", "-I" + str((unit.root / unit.src).parent)] + out


# libclang's Python cursors cost a ctypes call per property, and a search reads the same few thousand
# nodes hundreds of thousands of times per pass. Node wraps a cursor and remembers what it was asked
# (kind, extent, children, type, what it refers to); nodes are interned per translation unit, so two
# references to one declaration are the same object. Anything not cached falls through to the cursor.
_NONE = object()
_IS_EXPR = {}


def is_expr(kind):
    r = _IS_EXPR.get(kind)
    if r is None:
        r = _IS_EXPR[kind] = kind.is_expression()
    return r


class _File:
    __slots__ = ("name",)

    def __init__(self, name):
        self.name = name


class _Pos:
    __slots__ = ("line", "offset", "file")

    def __init__(self, loc):
        f = loc.file
        self.line, self.offset, self.file = loc.line, loc.offset, (_File(f.name) if f is not None else None)


class _Ext:
    __slots__ = ("start", "end")

    def __init__(self, e):
        self.start, self.end = _Pos(e.start), _Pos(e.end)


class TypeW:
    __slots__ = ("t", "_kind", "_canon", "_spelling", "_const")

    def __init__(self, t):
        self.t, self._kind, self._canon, self._spelling, self._const = t, None, None, None, None

    @property
    def kind(self):
        if self._kind is None:
            self._kind = self.t.kind
        return self._kind

    @property
    def spelling(self):
        if self._spelling is None:
            self._spelling = self.t.spelling
        return self._spelling

    def get_canonical(self):
        if self._canon is None:
            self._canon = TypeW(self.t.get_canonical())
        return self._canon

    def is_const_qualified(self):
        if self._const is None:
            self._const = self.t.is_const_qualified()
        return self._const

    def __getattr__(self, name):
        return getattr(self.t, name)


class Node:
    __slots__ = ("c", "tab", "hash", "_kind", "_spelling", "_extent", "_location", "_children", "_type", "_ref", "_sem",
                 "_lex", "_args", "_rtype", "_def", "_storage", "_isdef")

    def __init__(self, c, tab, h):
        self.c, self.tab, self.hash = c, tab, h
        self._kind = self._spelling = self._extent = self._location = self._children = self._type = None
        self._args = self._rtype = self._storage = self._isdef = None
        self._ref = self._sem = self._lex = self._def = _NONE

    def __eq__(self, other):
        if other is self:
            return True
        if isinstance(other, Node):
            return other.hash == self.hash and other.c == self.c
        return other is not None and self.c == other

    def __ne__(self, other):
        return not self.__eq__(other)

    def __hash__(self):
        return self.hash

    @property
    def kind(self):
        if self._kind is None:
            self._kind = self.c.kind
        return self._kind

    @property
    def spelling(self):
        if self._spelling is None:
            self._spelling = self.c.spelling
        return self._spelling

    @property
    def extent(self):
        if self._extent is None:
            self._extent = _Ext(self.c.extent)
        return self._extent

    @property
    def location(self):
        if self._location is None:
            self._location = _Pos(self.c.location)
        return self._location

    def get_children(self):
        if self._children is None:
            self._children = [wrap(x, self.tab) for x in self.c.get_children()]
        return self._children

    @property
    def type(self):
        if self._type is None:
            self._type = TypeW(self.c.type)
        return self._type

    @property
    def result_type(self):
        if self._rtype is None:
            self._rtype = TypeW(self.c.result_type)
        return self._rtype

    @property
    def referenced(self):
        if self._ref is _NONE:
            self._ref = wrap(self.c.referenced, self.tab)
        return self._ref

    @property
    def semantic_parent(self):
        if self._sem is _NONE:
            self._sem = wrap(self.c.semantic_parent, self.tab)
        return self._sem

    @property
    def lexical_parent(self):
        if self._lex is _NONE:
            self._lex = wrap(self.c.lexical_parent, self.tab)
        return self._lex

    def get_definition(self):
        if self._def is _NONE:
            self._def = wrap(self.c.get_definition(), self.tab)
        return self._def

    def get_arguments(self):
        if self._args is None:
            self._args = [wrap(x, self.tab) for x in self.c.get_arguments()]
        return self._args

    @property
    def storage_class(self):
        if self._storage is None:
            self._storage = self.c.storage_class
        return self._storage

    def is_definition(self):
        if self._isdef is None:
            self._isdef = self.c.is_definition()
        return self._isdef

    def __getattr__(self, name):
        return getattr(self.c, name)


def wrap(c, tab):
    """The interned Node of a cursor (None for no cursor)."""
    if c is None:
        return None
    h = c.hash
    n = tab.get(h)
    if n is not None and n.c == c:
        return n
    n = Node(c, tab, h)
    tab.setdefault(h, n)
    return n


class Parsed:
    def __init__(self, unit, data):
        global _INDEX
        if _INDEX is None:
            _INDEX = C.Index.create()
        self.data, self.path = data, str(unit.root / unit.src)
        self.tu = _INDEX.parse(self.path, args=clang_args(unit), unsaved_files=[(self.path, data)])
        self._fns, self._tab = None, {}

    def functions(self):
        if self._fns is None:
            out = []

            def walk_top(c):
                for ch in c.get_children():
                    f = ch.location.file
                    if f is None or f.name != self.path:
                        continue
                    if ch.kind in FUNC_KINDS and ch.is_definition():
                        out.append(ch)
                    elif ch.kind in (K.NAMESPACE, K.LINKAGE_SPEC, K.UNEXPOSED_DECL, K.CLASS_DECL, K.STRUCT_DECL):
                        walk_top(ch)
            walk_top(wrap(self.tu.cursor, self._tab))
            self._fns = out
        return self._fns

    def locate(self, demangled, line=None):
        """The definition of `Class::method(args)`; `line` (a source line inside it) settles overloads."""
        head = demangled.split("(")[0]
        parts, depth, cur = [], 0, ""
        for ch in head:
            depth += (ch == "<") - (ch == ">")
            if ch == ":" and depth == 0:
                if cur:
                    parts.append(cur)
                cur = ""
            else:
                cur += ch
        parts.append(cur)
        name = parts[-1].strip()
        cls = re.sub(r"<.*", "", parts[-2]).strip() if len(parts) > 1 else None
        cands = [c for c in self.functions() if c.spelling == name
                 and (cls is None or re.sub(r"<.*", "", c.semantic_parent.spelling or "") == cls)]
        if not cands and cls is None:
            cands = [c for c in self.functions() if c.spelling == name]
        if len(cands) > 1 and line is not None:
            inside = [c for c in cands if c.extent.start.line <= line <= c.extent.end.line]
            cands = inside or cands
        if len(cands) > 1:
            nargs = 0 if re.search(r"\((void)?\)", demangled) else demangled.count(",") + 1
            same = [c for c in cands if len(list(c.get_arguments())) == nargs]
            cands = same or cands
        return cands[0] if len(cands) == 1 else None

    def at_line(self, line):
        return next((c for c in self.functions() if c.extent.start.line <= line <= c.extent.end.line), None)


def body_of(fn):
    return next((c for c in fn.get_children() if c.kind == K.COMPOUND_STMT), None)


def walk(n):
    yield n
    for c in n.get_children():
        yield from walk(c)


def ext(n):
    return n.extent.start.offset, n.extent.end.offset


def good(p, n):
    f = n.extent.start.file
    s, e = ext(n)
    return f is not None and f.name == p.path and 0 <= s < e <= len(p.data)


def txt(p, n):
    s, e = ext(n)
    return p.data[s:e]


def strip_implicit(n):
    while n.kind == K.UNEXPOSED_EXPR:
        ch = list(n.get_children())
        if len(ch) != 1:
            break
        n = ch[0]
    return n


def strip_expr(n):
    """Through the implicit-conversion and parenthesis wrappers."""
    while n.kind in (K.UNEXPOSED_EXPR, K.PAREN_EXPR):
        ch = list(n.get_children())
        if len(ch) != 1:
            break
        n = ch[0]
    return n


def binop(p, n):
    """(lhs, operator text, rhs) of a binary operator whose pieces lie in the file in order, else None."""
    ch = list(n.get_children())
    if len(ch) != 2 or not all(good(p, c) for c in ch):
        return None
    (ls, le), (rs, re_) = ext(ch[0]), ext(ch[1])
    s, e = ext(n)
    if not (s <= ls < le <= rs < re_ <= e):
        return None
    return ch[0], p.data[le:rs].strip(), ch[1]


def impure(p, n):
    for c in walk(n):
        if c.kind in IMPURE:
            return True
        if c.kind == K.UNARY_OPERATOR:
            t = txt(p, c).strip()
            if t.startswith((b"++", b"--")) or t.endswith((b"++", b"--")):
                return True
        if c.kind == K.BINARY_OPERATOR:
            b = binop(p, c)
            if b is None or b[1] == b"=" or b[1] == b",":
                return True
    return False


def names_used(n):
    return {c.spelling for c in walk(n) if c.kind == K.DECL_REF_EXPR}


def paren(p, n):
    """n's text, parenthesised unless it is a primary expression."""
    t = txt(p, n).strip()
    return t if strip_implicit(n).kind in PRIMARY else b"(" + t + b")"


def line_span(data, s, e):
    """[s, e) widened to whole lines when the statement stands alone on them (a `;` and a trailing
    comment may follow), else None."""
    ls = data.rfind(b"\n", 0, s) + 1
    le = data.find(b"\n", e)
    le = len(data) if le < 0 else le + 1
    if data[ls:s].strip():
        return None
    tail = data[e:le].strip()
    if tail.startswith(b";"):
        tail = tail[1:].strip()
    if tail and not tail.startswith(b"//"):
        return None
    return ls, le


def indent_of(data, s):
    ls = data.rfind(b"\n", 0, s) + 1
    m = re.match(rb"[ \t]*", data[ls:s + 1])
    return m.group(0) if m else b""


def semi_end(data, e):
    """The offset just past the `;` that ends the statement whose extent ends at e."""
    m = re.match(rb"\s*;", data[e:e + 40])
    return e + m.end() if m else e


def reindent(block, delta, step):
    """Lines of `block` after the first, one indentation step deeper (delta > 0) or shallower."""
    lines = block.split(b"\n")
    out = [lines[0]]
    for l in lines[1:]:
        if not l.strip():
            out.append(l)
        elif delta > 0:
            out.append(step + l)
        else:
            out.append(l[len(step):] if l.startswith(step) else l.lstrip() if not l.startswith((b" ", b"\t")) else l)
    return b"\n".join(out)


class Ctx:
    """One function to rewrite: its nodes, their parents, the names in use."""

    def __init__(self, p, fn):
        self.p, self.fn, self.data, self.body = p, fn, p.data, body_of(fn)
        self.nodes, self.parent = [], {}

        def rec(n, par):
            self.nodes.append(n)
            self.parent[n.hash] = par
            for c in n.get_children():
                rec(c, n)
        rec(self.body, None)
        self.vars = [n for n in self.nodes if n.kind == K.VAR_DECL]
        self.names = {v.spelling for v in self.vars} | {a.spelling for a in fn.get_arguments()}
        self.void = fn.kind in (K.CONSTRUCTOR, K.DESTRUCTOR) or fn.result_type.kind == TK.VOID
        first = next(iter(self.body.get_children()), None)
        self.step = b"\t" if first is not None and indent_of(self.data, ext(first)[0]).startswith(b"\t") else b"    "

    def compounds(self):
        return [n for n in self.nodes if n.kind == K.COMPOUND_STMT]

    def fresh(self, base):
        used = self.names | {n.spelling for n in self.nodes if n.kind in (K.DECL_REF_EXPR, K.MEMBER_REF_EXPR)}
        name, k = base, 2
        while name in used:
            name, k = f"{base}{k}", k + 1
        return name

    def is_stmt(self, n):
        par = self.parent.get(n.hash)
        return par is not None and par.kind == K.COMPOUND_STMT

    def uses(self, var):
        return [n for n in self.nodes if n.kind == K.DECL_REF_EXPR and n.spelling == var.spelling and n.referenced == var]

    def written(self, use):
        """Is this reference a write target, or is its address taken?"""
        n, par = use, self.parent.get(use.hash)
        while par is not None and par.kind in (K.UNEXPOSED_EXPR, K.PAREN_EXPR):
            n, par = par, self.parent.get(par.hash)
        if par is None:
            return False
        if par.kind == K.UNARY_OPERATOR:
            t = txt(self.p, par).strip()
            return t.startswith((b"&", b"++", b"--")) or t.endswith((b"++", b"--"))
        if par.kind in (K.BINARY_OPERATOR, K.COMPOUND_ASSIGNMENT_OPERATOR):
            ch = list(par.get_children())
            if ch and ext(ch[0]) == ext(n):
                if par.kind == K.COMPOUND_ASSIGNMENT_OPERATOR:
                    return True
                b = binop(self.p, par)
                return b is None or b[1] == b"="
        if par.kind in (K.MEMBER_REF_EXPR, K.ARRAY_SUBSCRIPT_EXPR, K.CALL_EXPR):
            return par.kind != K.CALL_EXPR and False
        return False


class Decl:
    """A declaration statement of plain local variables."""

    def __init__(self, p, n):
        self.n, self.ok = n, False
        vs = list(n.get_children())
        if not vs or any(v.kind != K.VAR_DECL for v in vs) or not good(p, n):
            return
        if any(v.storage_class in (C.StorageClass.STATIC, C.StorageClass.EXTERN) for v in vs):
            return
        self.vars = vs
        self.names = {v.spelling for v in vs}
        inits = [c for v in vs for c in v.get_children() if is_expr(c.kind)]
        self.inits = inits
        self.has_init = bool(inits)
        self.impure = any(impure(p, c) for c in inits)
        self.refs = set().union(*[names_used(c) for c in inits]) if inits else set()
        self.span = line_span(p.data, *ext(n))
        self.ok = True

    def commutes(self, o):
        if self.names & o.refs or o.names & self.refs:
            return False
        return not self.has_init or not o.has_init or (not self.impure and not o.impure)


def single_var(p, st):
    """(var, head text `T `, text after the name up to the declarator's end, statement extent) of a
    one-variable declaration statement, else None."""
    d = Decl(p, st)
    if not d.ok or len(d.vars) != 1:
        return None
    v = d.vars[0]
    s, e = ext(st)
    name_at, ve = v.location.offset, v.extent.end.offset
    if not (s <= name_at < ve <= e) or p.data[name_at:name_at + len(v.spelling)] != v.spelling.encode():
        return None
    if p.data[ve:e].strip() not in (b"", b";"):
        return None
    return d, v, p.data[s:name_at], p.data[name_at + len(v.spelling):ve]


# ---------------------------------------------------------------- tier a generators

def gen_decl(c):
    p, data, body = c.p, c.data, c.body
    count = {}
    for v in c.vars:
        count[v.spelling] = count.get(v.spelling, 0) + 1
    top = None
    for comp in c.compounds():
        kids = list(comp.get_children())
        run = []
        for st in kids + [None]:
            d = Decl(p, st) if st is not None and st.kind == K.DECL_STMT else None
            if d is not None and d.ok and d.span:
                run.append(d)
                continue
            if comp == body and top is None:
                top = list(run)
            n = len(run)
            for i in range(n):
                for j in range(i + 1, n):
                    a, b = run[i], run[j]
                    na, nb = "/".join(sorted(a.names)), "/".join(sorted(b.names))
                    fwd = all(b.commutes(x) for x in run[i:j])
                    back = all(a.commutes(x) for x in run[i + 1:j + 1])
                    (as_, ae), (bs, be) = a.span, b.span
                    if fwd:
                        yield Edit("decl", f"declare `{nb}` before `{na}`", [(bs, be, b""), (as_, as_, data[bs:be])])
                    if back and j > i + 1:
                        yield Edit("decl", f"declare `{na}` after `{nb}`", [(as_, ae, b""), (be, be, data[as_:ae])])
                    if fwd and back and j > i + 1:
                        yield Edit("decl", f"swap the declarations of `{na}` and `{nb}`",
                                   [(as_, ae, data[bs:be]), (bs, be, data[as_:ae])])
            # every order of a short run (legal when each pair that changes places commutes)
            if 3 <= n <= 6 and all(x.span[1] == y.span[0] for x, y in zip(run, run[1:])):
                texts = [data[d.span[0]:d.span[1]] for d in run]
                for perm in itertools.permutations(range(n)):
                    moved = sum(1 for k, x in enumerate(perm) if k != x)
                    if moved <= 2:
                        continue  # identity and the single swaps above
                    if any(perm.index(i) > perm.index(j) and not run[i].commutes(run[j])
                           for i in range(n) for j in range(i + 1, n)):
                        continue
                    order = ", ".join("/".join(sorted(run[k].names)) for k in perm)
                    yield Edit("decl", f"declare in the order {order}",
                               [(run[0].span[0], run[-1].span[1], b"".join(texts[k] for k in perm))])
            run = []
    top = top or []
    kids = list(body.get_children())
    if not kids:
        return
    first = kids[0]
    front = line_span(data, *ext(first))
    if front is None:
        return
    after_top = top[-1].span[1] if top else front[0]
    ind = indent_of(data, ext(first)[0])
    top_nodes = {d.n.hash for d in top}
    # hoist a later declaration to the top of the function
    for st in c.nodes:
        if st.kind != K.DECL_STMT or st.hash in top_nodes:
            continue
        sv = single_var(p, st)
        if sv is None:
            continue
        d, v, head, after_name = sv
        if count.get(v.spelling, 0) != 1 or v.type.get_canonical().kind not in SCALAR or v.type.is_const_qualified():
            continue
        # the name must mean this variable everywhere in the function (no member or outer variable of that name)
        if any(n.spelling == v.spelling and n.kind in (K.DECL_REF_EXPR, K.MEMBER_REF_EXPR) and n.referenced != v for n in c.nodes):
            continue
        if b"&" in head or b"const" in head.split() or after_name.strip().startswith(b"["):
            continue
        s, e = ext(st)
        decl_line = ind + head.rstrip() + (b" " if not head.rstrip().endswith(b"*") else b"") + v.spelling.encode() + b";\n"
        places = [("top", front[0])] + ([("end of the top declarations", after_top)] if after_top != front[0] else [])
        if d.has_init:
            if not after_name.strip().startswith(b"="):
                continue
            repl = v.spelling.encode() + b" " + after_name.strip() + data[v.extent.end.offset:e]
            for where, pos in places:
                yield Edit("decl", f"declare `{v.spelling}` at the {where}, assign it where it was initialised",
                           [(pos, pos, decl_line), (s, e, repl)])
        elif d.span:
            for where, pos in places:
                yield Edit("decl", f"move the declaration of `{v.spelling}` to the {where}",
                           [(pos, pos, decl_line), (d.span[0], d.span[1], b"")])
    # sink a top declaration into its first assignment
    for d in top:
        if len(d.vars) != 1 or d.has_init:
            continue
        v = d.vars[0]
        if v.type.get_canonical().kind not in SCALAR or count.get(v.spelling, 0) != 1:
            continue
        s, e = ext(d.n)
        head = data[s:v.location.offset]
        after = [k for k in kids if ext(k)[0] >= d.span[1]]
        use = next((k for k in after if v.spelling in names_used(k)), None)
        if use is None:
            continue
        tgt, in_for = use, False
        if use.kind == K.FOR_STMT:
            fk = list(use.get_children())
            others = [k for k in after if k != use and v.spelling in names_used(k)]
            if not fk or others or not good(p, fk[0]):
                continue
            tgt, in_for = fk[0], True
        b = binop(p, tgt) if tgt.kind == K.BINARY_OPERATOR else None
        if b is None or b[1] != b"=" or strip_expr(b[0]).kind != K.DECL_REF_EXPR or strip_expr(b[0]).spelling != v.spelling:
            continue
        if v.spelling in names_used(b[2]):
            continue
        ts = ext(tgt)[0]
        yield Edit("decl", f"declare `{v.spelling}` where it is first assigned" + (" (in the for statement)" if in_for else ""),
                   [(d.span[0], d.span[1], b""), (ts, ts, head)])


def gen_commute(c):
    p = c.p
    for n in c.nodes:
        if n.kind != K.BINARY_OPERATOR or not good(p, n):
            continue
        b = binop(p, n)
        if b is None or b[1] not in COMMUTE:
            continue
        l, op, r = b
        if impure(p, l) or impure(p, r):
            continue
        lt, rt = txt(p, l), txt(p, r)
        if lt == rt:
            continue
        li = strip_implicit(l)
        lb = binop(p, li) if li.kind == K.BINARY_OPERATOR else None
        new_r = b"(" + lt + b")" if lb is not None and PREC.get(lb[1], 0) >= PREC[op] else lt
        (ls, le), (rs, re_) = ext(l), ext(r)
        mid = p.data[le:rs].replace(op, COMMUTE[op], 1)
        yield Edit("commute", f"line {n.extent.start.line}: `{D(lt, 28)} {op.decode()} {D(rt, 28)}` with the operands the other way round",
                   [(ls, re_, rt + mid + new_r)], n.extent.start.line)


def gen_compound(c):
    p = c.p
    for n in c.nodes:
        if not good(p, n):
            continue
        par = c.parent.get(n.hash)
        if par is None or is_expr(par.kind):
            continue
        if n.kind == K.COMPOUND_ASSIGNMENT_OPERATOR:
            b = binop(p, n)
            if b is None or not b[1].endswith(b"=") or b[1][:-1] not in ARITH or impure(p, b[0]):
                continue
            l, op, r = b
            base, lt = op[:-1], txt(p, l).strip()
            rb = strip_implicit(r)
            rbin = binop(p, rb) if rb.kind == K.BINARY_OPERATOR else None
            rt = txt(p, r).strip()
            rr = b"(" + rt + b")" if (rbin is not None and PREC.get(rbin[1], 99) >= PREC[base]) or rb.kind == K.CONDITIONAL_OPERATOR else rt
            yield Edit("compound", f"line {n.extent.start.line}: `{D(lt, 24)} {op.decode()} ...` as `{D(lt, 24)} = {D(lt, 24)} {base.decode()} ...`",
                       [(*ext(n), lt + b" = " + lt + b" " + base + b" " + rr)], n.extent.start.line)
            if base in (b"+", b"*", b"&", b"|", b"^"):
                rl = b"(" + rt + b")" if (rbin is not None and PREC.get(rbin[1], 99) > PREC[base]) or rb.kind == K.CONDITIONAL_OPERATOR else rt
                yield Edit("compound", f"line {n.extent.start.line}: `{D(lt, 24)} {op.decode()} ...` as `{D(lt, 24)} = ... {base.decode()} {D(lt, 24)}`",
                           [(*ext(n), lt + b" = " + rl + b" " + base + b" " + lt)], n.extent.start.line)
        elif n.kind == K.BINARY_OPERATOR:
            b = binop(p, n)
            if b is None or b[1] != b"=" or impure(p, b[0]):
                continue
            lt = txt(p, b[0]).strip()
            rs = strip_expr(b[2])
            rb = binop(p, rs) if rs.kind == K.BINARY_OPERATOR else None
            if rb is None or rb[1] not in ARITH:
                continue
            x, op, y = rb
            if txt(p, x).strip() == lt:
                other = y
            elif op in (b"+", b"*", b"&", b"|", b"^") and txt(p, y).strip() == lt:
                other = x
            else:
                continue
            yield Edit("compound", f"line {n.extent.start.line}: `{D(lt, 24)} = ...` as `{D(lt, 24)} {op.decode()}= ...`",
                       [(*ext(n), lt + b" " + op + b"= " + txt(p, other).strip())], n.extent.start.line)


def negated(p, cond):
    c = strip_expr(cond)
    t = txt(p, cond).strip()
    if c.kind == K.UNARY_OPERATOR and txt(p, c).strip().startswith(b"!"):
        ch = list(c.get_children())
        if len(ch) == 1 and good(p, ch[0]):
            inner = txt(p, ch[0]).strip()
            if ch[0].kind == K.PAREN_EXPR or strip_implicit(ch[0]).kind == K.PAREN_EXPR:
                inner = inner[1:-1].strip()
            return inner
    if c.kind == K.BINARY_OPERATOR:
        b = binop(p, c)
        if b and b[1] in NEGATE:
            kinds = {b[0].type.get_canonical().kind, b[2].type.get_canonical().kind}
            if b[1] in (b"==", b"!=") or kinds <= INTEGRAL:
                (ls, le), (rs, re_) = ext(b[0]), ext(b[2])
                return p.data[ls:le] + p.data[le:rs].replace(b[1], NEGATE[b[1]], 1) + p.data[rs:re_]
    if c.kind in (K.DECL_REF_EXPR, K.MEMBER_REF_EXPR, K.CALL_EXPR, K.ARRAY_SUBSCRIPT_EXPR):
        return b"!" + t
    return b"!(" + t + b")"


def if_parts(p, n, want):
    """Children of an if statement when it has `want` of them, all in the file and in order."""
    ch = list(n.get_children())
    if len(ch) != want or not all(good(p, x) for x in ch):
        return None
    pos = ext(n)[0]
    for x in ch:
        s, e = ext(x)
        if s < pos:
            return None
        pos = e
    return ch


def gen_if(c):
    p, d = c.p, c.data
    for n in c.nodes:
        if n.kind != K.IF_STMT or not good(p, n):
            continue
        ch = if_parts(p, n, 3)
        if ch is None or ch[1].kind != K.COMPOUND_STMT or ch[2].kind != K.COMPOUND_STMT:
            continue
        (cs, ce), (ts, te), (es, ee) = (ext(x) for x in ch)
        new = negated(p, ch[0]) + d[ce:ts] + d[es:ee] + d[te:es] + d[ts:te]
        yield Edit("if", f"line {n.extent.start.line}: test the negated condition and swap the if / else blocks",
                   [(cs, ee, new)], n.extent.start.line)


def gen_andif(c):
    p, d = c.p, c.data
    for n in c.nodes:
        if n.kind != K.IF_STMT or not good(p, n):
            continue
        ch = if_parts(p, n, 2)
        if ch is None or ch[1].kind != K.COMPOUND_STMT:
            continue
        cond, then = ch
        (cs, ce), (ts, te) = ext(cond), ext(then)
        s, _ = ext(n)
        ind = indent_of(d, s)
        cb = strip_implicit(cond)
        b = binop(p, cb) if cb.kind == K.BINARY_OPERATOR else None
        if b is not None and b[1] == b"&&":
            a_t, b_t = txt(p, b[0]).strip(), txt(p, b[2]).strip()
            inner = b"if (" + b_t + b") " + reindent(d[ts:te], 1, c.step)
            new = d[s:cs] + a_t + d[ce:ts] + b"{\n" + ind + c.step + inner + b"\n" + ind + b"}"
            yield Edit("andif", f"line {n.extent.start.line}: `if (a && b)` as two nested ifs", [(s, te, new)], n.extent.start.line)
        kids = list(then.get_children())
        if len(kids) == 1 and kids[0].kind == K.IF_STMT:
            ich = if_parts(p, kids[0], 2)
            if ich is None or ich[1].kind != K.COMPOUND_STMT:
                continue

            def side(x):
                xb = strip_implicit(x)
                bb = binop(p, xb) if xb.kind == K.BINARY_OPERATOR else None
                t = txt(p, x).strip()
                return b"(" + t + b")" if (bb is not None and PREC.get(bb[1], 0) > PREC[b"&&"]) or xb.kind == K.CONDITIONAL_OPERATOR else t
            (its, ite) = ext(ich[1])
            new = d[s:cs] + side(cond) + b" && " + side(ich[0]) + d[ce:ts] + reindent(d[its:ite], -1, c.step)
            yield Edit("andif", f"line {n.extent.start.line}: two nested ifs as `if (a && b)`", [(s, te, new)], n.extent.start.line)


def gen_early(c):
    p, d = c.p, c.data
    places = [(c.body, b"return;")] if c.void else []
    for n in c.nodes:
        if n.kind in (K.FOR_STMT, K.WHILE_STMT, K.DO_STMT):
            kids = list(n.get_children())
            lb = next((k for k in (kids[-1:] if n.kind != K.DO_STMT else kids[:1]) if k.kind == K.COMPOUND_STMT), None)
            if lb is not None:
                places.append((lb, b"continue;"))
    for comp, word in places:
        kids = [k for k in comp.get_children()]
        if not kids or not all(good(p, k) for k in kids):
            continue
        last = kids[-1]
        # `if (c) { ... }` last  ->  `if (!c) return;` then the block's statements
        ch = if_parts(p, last, 2) if last.kind == K.IF_STMT else None
        if ch is not None and ch[1].kind == K.COMPOUND_STMT and list(ch[1].get_children()):
            cond, then = ch
            s, _ = ext(last)
            (cs, ce), (ts, te) = ext(cond), ext(then)
            span = line_span(d, s, te)
            inner = d[ts + 1:te - 1]
            if span and inner.startswith(b"\n") or (span and inner.lstrip(b" \t").startswith(b"\n")):
                ind = indent_of(d, s)
                inner = inner[inner.index(b"\n") + 1:].rstrip(b" \t")
                moved = reindent(b"\n" + inner, -1, c.step)[1:]
                if not moved.endswith(b"\n"):
                    moved += b"\n"
                new = ind + d[s:cs] + negated(p, cond) + d[ce:ts] + b"{\n" + ind + c.step + word + b"\n" + ind + b"}\n" + moved
                yield Edit("early", f"line {last.extent.start.line}: `if (c) {{ ... }}` as `if (!c) {word.decode()}` and the block after it",
                           [(span[0], span[1], new)], last.extent.start.line)
        # `if (c) return;` followed by the rest  ->  `if (!c) { rest }`
        for i, st in enumerate(kids[:-1]):
            ch = if_parts(p, st, 2) if st.kind == K.IF_STMT else None
            if ch is None:
                continue
            t = ch[1]
            only = list(t.get_children()) if t.kind == K.COMPOUND_STMT else [t]
            want = K.RETURN_STMT if word == b"return;" else K.CONTINUE_STMT
            if len(only) != 1 or only[0].kind != want or list(only[0].get_children()):
                continue
            rest = kids[i + 1:]
            if any(r.kind == K.DECL_STMT for r in rest):
                continue  # a declaration would end up in a narrower scope: fine for behaviour, but it reads oddly
            a = line_span(d, ext(st)[0], semi_end(d, ext(st)[1]))
            first = line_span(d, ext(rest[0])[0], semi_end(d, ext(rest[0])[1]))
            lastsp = line_span(d, ext(rest[-1])[0], semi_end(d, ext(rest[-1])[1]))
            if not a or not first or not lastsp or a[1] > first[0]:
                continue
            ind = indent_of(d, ext(st)[0])
            cond = ch[0]
            block = d[first[0]:lastsp[1]]
            moved = reindent(b"\n" + block.rstrip(b"\n"), 1, c.step)[1:] + b"\n"
            new = ind + b"if (" + negated(p, cond) + b") {\n" + moved + ind + b"}\n"
            yield Edit("early", f"line {st.extent.start.line}: `if (c) {word.decode()}` as `if (!c) {{ ... }}` around what follows",
                       [(a[0], lastsp[1], d[a[1]:first[0]] + new)], st.extent.start.line)


def one_stmt(n):
    if n.kind == K.COMPOUND_STMT:
        ch = list(n.get_children())
        return ch[0] if len(ch) == 1 else None
    return n


def gen_ternary(c):
    p, d = c.p, c.data
    for n in c.nodes:
        if not good(p, n):
            continue
        if n.kind == K.IF_STMT:
            ch = if_parts(p, n, 3)
            if ch is None:
                continue
            a, b = one_stmt(ch[1]), one_stmt(ch[2])
            if a is None or b is None or a.kind != b.kind:
                continue
            cb = strip_implicit(ch[0])
            cbin = binop(p, cb) if cb.kind == K.BINARY_OPERATOR else None
            ct = txt(p, ch[0]).strip()
            if (cbin is not None and PREC.get(cbin[1], 0) >= PREC[b"="]) or cb.kind == K.CONDITIONAL_OPERATOR:
                ct = b"(" + ct + b")"
            s, e = ext(n)
            e = semi_end(d, e) if d[e - 1:e] != b"}" else e
            if a.kind == K.BINARY_OPERATOR:
                ba, bb = binop(p, a), binop(p, b)
                if not ba or not bb or ba[1] != b"=" or bb[1] != b"=" or impure(p, ba[0]):
                    continue
                if txt(p, ba[0]).strip() != txt(p, bb[0]).strip():
                    continue
                new = txt(p, ba[0]).strip() + b" = " + ct + b" ? " + paren_if_low(p, ba[2]) + b" : " + paren_if_low(p, bb[2]) + b";"
            elif a.kind == K.RETURN_STMT:
                ra, rb = list(a.get_children()), list(b.get_children())
                if len(ra) != 1 or len(rb) != 1:
                    continue
                new = b"return " + ct + b" ? " + paren_if_low(p, ra[0]) + b" : " + paren_if_low(p, rb[0]) + b";"
            else:
                continue
            yield Edit("ternary", f"line {n.extent.start.line}: the if / else as `c ? a : b`", [(s, e, new)], n.extent.start.line)
        elif n.kind in (K.BINARY_OPERATOR, K.RETURN_STMT) and c.is_stmt(n):
            if n.kind == K.BINARY_OPERATOR:
                b = binop(p, n)
                if b is None or b[1] != b"=" or impure(p, b[0]):
                    continue
                lead, val = txt(p, b[0]).strip() + b" = ", strip_expr(b[2])
            else:
                ch = list(n.get_children())
                if len(ch) != 1:
                    continue
                lead, val = b"return ", strip_expr(ch[0])
            if val.kind != K.CONDITIONAL_OPERATOR:
                continue
            q = list(val.get_children())
            if len(q) != 3 or not all(good(p, x) for x in q):
                continue
            s, e = ext(n)
            span = line_span(d, s, e)
            if not span:
                continue
            ind = indent_of(d, s)
            new = (ind + b"if (" + txt(p, q[0]).strip() + b") {\n" + ind + c.step + lead + txt(p, q[1]).strip() + b";\n" + ind
                   + b"} else {\n" + ind + c.step + lead + txt(p, q[2]).strip() + b";\n" + ind + b"}\n")
            yield Edit("ternary", f"line {n.extent.start.line}: `c ? a : b` as an if / else", [(span[0], span[1], new)], n.extent.start.line)


def paren_if_low(p, n):
    x = strip_implicit(n)
    t = txt(p, n).strip()
    b = binop(p, x) if x.kind == K.BINARY_OPERATOR else None
    return b"(" + t + b")" if (b is not None and PREC.get(b[1], 0) >= PREC[b"="]) or x.kind == K.CONDITIONAL_OPERATOR else t


def own_place(c, n):
    """Is n a member of `this` (written plainly or through this->) or a local variable or its member?"""
    n = strip_expr(n)
    if n.kind == K.DECL_REF_EXPR:
        r = n.referenced
        return r is not None and r.kind in (K.VAR_DECL, K.PARM_DECL) and r.type.kind not in (TK.LVALUEREFERENCE, TK.POINTER) \
            and r.semantic_parent == c.fn
    if n.kind == K.MEMBER_REF_EXPR:
        ch = list(n.get_children())
        if not ch:
            return True
        b = strip_expr(ch[0])
        return b.kind == K.CXX_THIS_EXPR or (b.kind in (K.MEMBER_REF_EXPR, K.DECL_REF_EXPR) and b.type.kind != TK.POINTER and own_place(c, b))
    return False


def plain_value(c, n):
    """No side effect and no read through a pointer or an array."""
    if impure(c.p, n):
        return False
    for x in walk(n):
        if x.kind == K.ARRAY_SUBSCRIPT_EXPR:
            return False
        if x.kind == K.UNARY_OPERATOR and txt(c.p, x).strip().startswith(b"*"):
            return False
        if x.kind == K.MEMBER_REF_EXPR and not own_place(c, x):
            return False
    return True


def gen_stmt(c):
    p = c.p
    for comp in c.compounds():
        kids = list(comp.get_children())
        for a, b in zip(kids, kids[1:]):
            if not (good(p, a) and good(p, b)) or a.kind != K.BINARY_OPERATOR or b.kind != K.BINARY_OPERATOR:
                continue
            ba, bb = binop(p, a), binop(p, b)
            if not ba or not bb or ba[1] != b"=" or bb[1] != b"=":
                continue
            if not own_place(c, ba[0]) or not own_place(c, bb[0]) or not plain_value(c, ba[2]) or not plain_value(c, bb[2]):
                continue
            ta, tb = txt(p, ba[0]).strip(), txt(p, bb[0]).strip()
            if ta == tb or ta in txt(p, bb[2]) or tb in txt(p, ba[2]) or ta.startswith(tb) or tb.startswith(ta):
                continue
            la, lb = line_span(p.data, *ext(a)), line_span(p.data, *ext(b))
            if not la or not lb or la[1] != lb[0]:
                continue
            yield Edit("stmt", f"lines {a.extent.start.line}-{b.extent.start.line}: `{D(tb, 30)} = ...` before `{D(ta, 30)} = ...`",
                       [(la[0], lb[1], p.data[lb[0]:lb[1]] + p.data[la[0]:la[1]])], a.extent.start.line)


def gen_loop(c):
    p, d = c.p, c.data
    for comp in c.compounds():
        run = []

        def flush():
            if len(run) >= 3:
                (st0, base, k0, val, sp0), (_, _, _, _, sp1) = run[0], run[-1]
                ind = indent_of(d, ext(st0)[0])
                i = c.fresh("i").encode()
                for ty in (b"s32", b"u32"):
                    new = (ind + b"for (" + ty + b" " + i + b" = " + str(k0).encode() + b"; " + i + b" < " + str(k0 + len(run)).encode()
                           + b"; " + i + b"++) {\n" + ind + c.step + base + b"[" + i + b"] = " + val + b";\n" + ind + b"}\n")
                    yield Edit("loop", f"lines {st0.extent.start.line}-{run[-1][0].extent.start.line}: {len(run)} stores to "
                                       f"`{D(base, 24)}[...]` as a for loop ({ty.decode()} counter)", [(sp0[0], sp1[1], new)],
                               st0.extent.start.line)
        for st in list(comp.get_children()) + [None]:
            item = None
            if st is not None and good(p, st) and st.kind == K.BINARY_OPERATOR:
                b = binop(p, st)
                l = strip_expr(b[0]) if b and b[1] == b"=" else None
                if l is not None and l.kind == K.ARRAY_SUBSCRIPT_EXPR and not impure(p, b[2]):
                    ch = list(l.get_children())
                    if len(ch) == 2 and strip_expr(ch[1]).kind == K.INTEGER_LITERAL and not impure(p, ch[0]):
                        try:
                            k = int(txt(p, ch[1]).strip().rstrip(b"uUlL").decode(), 0)
                        except ValueError:
                            k = None
                        sp = line_span(d, *ext(st))
                        if k is not None and sp:
                            item = (st, txt(p, ch[0]).strip(), k, txt(p, b[2]).strip(), sp)
            if item and run and item[1] == run[-1][1] and item[3] == run[-1][3] and item[2] == run[-1][2] + 1 and item[4][0] == run[-1][4][1]:
                run.append(item)
                continue
            yield from flush()
            run = [item] if item else []


def gen_incr(c):
    p = c.p
    for st in c.nodes:
        if not good(p, st):
            continue
        par = c.parent.get(st.hash)
        if par is None or is_expr(par.kind):
            continue
        t = txt(p, st).strip()
        line = st.extent.start.line
        if st.kind == K.UNARY_OPERATOR:
            ch = list(st.get_children())
            if len(ch) != 1 or impure(p, ch[0]):
                continue
            x = txt(p, ch[0])
            for op in (b"++", b"--"):
                if t == x + op:
                    yield Edit("incr", f"line {line}: `{op.decode()}{D(x, 30)}`", [(*ext(st), op + x)], line)
                elif t == op + x:
                    yield Edit("incr", f"line {line}: `{D(x, 30)}{op.decode()}`", [(*ext(st), x + op)], line)
        elif st.kind == K.COMPOUND_ASSIGNMENT_OPERATOR:
            b = binop(p, st)
            if b and b[1] in (b"+=", b"-=") and txt(p, b[2]).strip() == b"1" and not impure(p, b[0]):
                x = txt(p, b[0])
                op = b"++" if b[1] == b"+=" else b"--"
                yield Edit("incr", f"line {line}: `{D(x, 30)}{op.decode()}`", [(*ext(st), x + op)], line)


def gen_cond(c):
    p = c.p
    for n in c.nodes:
        if n.kind not in (K.IF_STMT, K.WHILE_STMT, K.CONDITIONAL_OPERATOR) or not good(p, n):
            continue
        ch = list(n.get_children())
        if not ch or not good(p, ch[0]):
            continue
        tops, leaves = [ch[0]], []
        while tops:
            x = tops.pop()
            xx = strip_expr(x)
            b = binop(p, xx) if xx.kind == K.BINARY_OPERATOR else None
            if b and b[1] in (b"&&", b"||"):
                tops += [b[0], b[2]]
            else:
                leaves.append((x, xx, b))
        for x, xx, b in leaves:
            line = x.extent.start.line
            if b and b[1] in (b"==", b"!=") and txt(p, b[2]).strip() in (b"0", b"NULL", b"false") \
                    and b[0].type.get_canonical().kind in INTEGRAL:
                v = paren(p, b[0])
                new = v if b[1] == b"!=" else b"!" + v
                yield Edit("cond", f"line {line}: `{D(txt(p, xx))}` as `{D(new)}`", [(*ext(xx), new)], line)
            elif xx.kind in (K.DECL_REF_EXPR, K.MEMBER_REF_EXPR) and xx.type.get_canonical().kind in INTEGRAL - {TK.BOOL}:
                v = txt(p, xx)
                zero = b"NULL" if xx.type.get_canonical().kind == TK.POINTER else b"0"
                yield Edit("cond", f"line {line}: `{D(v)}` as `{D(v)} != {zero.decode()}`", [(*ext(xx), v + b" != " + zero)], line)
            elif xx.kind == K.UNARY_OPERATOR and txt(p, xx).strip().startswith(b"!"):
                k = list(xx.get_children())
                if len(k) == 1 and strip_expr(k[0]).kind in (K.DECL_REF_EXPR, K.MEMBER_REF_EXPR) \
                        and strip_expr(k[0]).type.get_canonical().kind in INTEGRAL - {TK.BOOL}:
                    v = txt(p, strip_expr(k[0]))
                    zero = b"NULL" if strip_expr(k[0]).type.get_canonical().kind == TK.POINTER else b"0"
                    yield Edit("cond", f"line {line}: `!{D(v)}` as `{D(v)} == {zero.decode()}`", [(*ext(xx), v + b" == " + zero)], line)


def local_only(c, n):
    """Does the expression read nothing but locals, parameters and literals?"""
    for x in walk(n):
        if x.kind in (K.MEMBER_REF_EXPR, K.ARRAY_SUBSCRIPT_EXPR, K.CXX_THIS_EXPR):
            return False
        if x.kind == K.UNARY_OPERATOR and txt(c.p, x).strip().startswith(b"*"):
            return False
        if x.kind == K.DECL_REF_EXPR:
            r = x.referenced
            if r is None or r.kind not in (K.VAR_DECL, K.PARM_DECL) or r.semantic_parent != c.fn:
                return False
    return True


def temps(c):
    """Locals declared alone with an initialiser and never written again: (decl, var, init, uses)."""
    for st in c.nodes:
        if st.kind != K.DECL_STMT or not c.is_stmt(st):
            continue
        sv = single_var(c.p, st)
        if sv is None:
            continue
        d, v, head, after_name = sv
        if not d.has_init or len(d.inits) != 1 or not d.span or not after_name.strip().startswith(b"="):
            continue
        if v.type.get_canonical().kind not in SCALAR:
            continue
        uses = c.uses(v)
        if not uses or any(c.written(u) for u in uses):
            continue
        yield d, v, d.inits[0], uses


def gen_untemp(c):
    p = c.p
    for d, v, init, uses in temps(c):
        if len(uses) != 1 or impure(p, init):
            continue
        if strip_implicit(init).type.get_canonical().spelling != v.type.get_canonical().spelling:
            continue  # the declaration converts the value
        use = uses[0]
        par = c.parent.get(d.n.hash)
        sibs = list(par.get_children())
        k = next((i for i, s in enumerate(sibs) if s == d.n), None)
        if k is None or k + 1 >= len(sibs):
            continue
        nxt = sibs[k + 1]
        chain, n = set(), use
        while n is not None and n != nxt:
            chain.add(n.hash)
            n = c.parent.get(n.hash)
        if n is None:
            continue  # the use is not in the next statement
        chain.add(nxt.hash)
        if not local_only(c, init):
            def bad(x):
                if x.hash in chain:
                    return any(bad(y) for y in x.get_children())
                return impure(p, x)
            if bad(nxt):
                continue
        yield Edit("untemp", f"line {d.n.extent.start.line}: use the value of `{v.spelling}` directly in the next statement",
                   [(d.span[0], d.span[1], b""), (*ext(use), paren(p, init))], d.n.extent.start.line)


def gen_boolret(c):
    """`if (c) return true; return false;`  <->  `return c;` in a function that returns bool."""
    p, d = c.p, c.data
    if c.fn.result_type.get_canonical().kind != TK.BOOL:
        return
    lit = {b"true": True, b"false": False}

    def ret_lit(n):
        n = one_stmt(n) if n is not None else None
        if n is None or n.kind != K.RETURN_STMT:
            return None
        ch = list(n.get_children())
        return lit.get(txt(p, ch[0]).strip()) if len(ch) == 1 else None
    for comp in c.compounds():
        kids = list(comp.get_children())
        for a, b in zip(kids, kids[1:]):
            ch = if_parts(p, a, 2) if a.kind == K.IF_STMT else None
            if ch is None or not good(p, b):
                continue
            va, vb = ret_lit(ch[1]), ret_lit(b)
            if va is None or vb is None or va == vb or ch[0].type.get_canonical().kind != TK.BOOL:
                continue
            sa = line_span(d, ext(a)[0], ext(a)[1])
            sb = line_span(d, ext(b)[0], semi_end(d, ext(b)[1]))
            if not sa or not sb or sa[1] > sb[0]:
                continue
            ind = indent_of(d, ext(a)[0])
            cond = txt(p, ch[0]).strip() if va else negated(p, ch[0])
            yield Edit("boolret", f"line {a.extent.start.line}: `if (c) return {str(va).lower()}; return {str(vb).lower()};` as one return",
                       [(sa[0], sb[1], ind + b"return " + cond + b";\n")], a.extent.start.line)
    for n in c.nodes:
        if n.kind != K.RETURN_STMT or not c.is_stmt(n) or not good(p, n):
            continue
        ch = list(n.get_children())
        if len(ch) != 1 or ch[0].type.get_canonical().kind != TK.BOOL:
            continue
        v = strip_expr(ch[0])
        vb = binop(p, v) if v.kind == K.BINARY_OPERATOR else None
        if not (vb and vb[1] in (b"&&", b"||", b"==", b"!=", b"<", b">", b"<=", b">=")) and \
                not (v.kind == K.UNARY_OPERATOR and txt(p, v).strip().startswith(b"!")):
            continue
        sp = line_span(d, ext(n)[0], semi_end(d, ext(n)[1]))
        if not sp:
            continue
        ind = indent_of(d, ext(n)[0])
        t = txt(p, ch[0]).strip()
        yield Edit("boolret", f"line {n.extent.start.line}: `return c;` as `if (c) return true; return false;`",
                   [(sp[0], sp[1], ind + b"if (" + t + b") {\n" + ind + c.step + b"return true;\n" + ind + b"}\n" + ind + b"return false;\n")],
                   n.extent.start.line)
        yield Edit("boolret", f"line {n.extent.start.line}: `return c;` as `if (!c) return false; return true;`",
                   [(sp[0], sp[1], ind + b"if (" + negated(p, ch[0]) + b") {\n" + ind + c.step + b"return false;\n" + ind + b"}\n" + ind + b"return true;\n")],
                   n.extent.start.line)


def gen_const(c):
    """`const` on a local that is initialised and never written, or taken off one."""
    p, d = c.p, c.data
    for st in c.nodes:
        if st.kind != K.DECL_STMT:
            continue
        sv = single_var(p, st)
        if sv is None:
            continue
        dd, v, head, after_name = sv
        if not dd.has_init or v.type.get_canonical().kind not in SCALAR - {TK.POINTER}:
            continue
        s = ext(st)[0]
        words = head.split()
        if b"const" in words:
            new = b" ".join(w for w in words if w != b"const") + b" "
            yield Edit("const", f"line {st.extent.start.line}: `{v.spelling}` without const", [(s, s + len(head), new)], st.extent.start.line)
        elif not any(c.written(u) for u in c.uses(v)):
            yield Edit("const", f"line {st.extent.start.line}: `{v.spelling}` declared const", [(s, s, b"const ")], st.extent.start.line)


def gen_scope(c):
    """An uninitialised declaration moved into the innermost block that holds all its uses, or out to
    the start of an enclosing block."""
    p, d = c.p, c.data
    count = {}
    for v in c.vars:
        count[v.spelling] = count.get(v.spelling, 0) + 1
    for st in c.nodes:
        if st.kind != K.DECL_STMT or not c.is_stmt(st):
            continue
        sv = single_var(p, st)
        if sv is None:
            continue
        dd, v, head, after_name = sv
        if dd.has_init or not dd.span or count.get(v.spelling, 0) != 1 or v.type.get_canonical().kind not in SCALAR:
            continue
        if any(n.spelling == v.spelling and n.kind in (K.DECL_REF_EXPR, K.MEMBER_REF_EXPR) and n.referenced != v for n in c.nodes):
            continue
        uses = c.uses(v)
        if not uses:
            continue
        home = c.parent.get(st.hash)
        # chains of enclosing compounds of every use, innermost first
        chains = []
        for u in uses:
            ch, n = [], c.parent.get(u.hash)
            while n is not None:
                if n.kind == K.COMPOUND_STMT:
                    ch.append(n)
                n = c.parent.get(n.hash)
            chains.append(ch)
        common = [x for x in chains[0] if all(any(x == y for y in ch) for ch in chains[1:])]
        if not common:
            continue
        inner = common[0]
        text = d[dd.span[0]:dd.span[1]].strip()
        if inner != home and good(p, inner):
            # only into a block that is not a loop body (a value kept across iterations would be lost)
            n, loop = inner, False
            while n is not None and n != home:
                par = c.parent.get(n.hash)
                if par is not None and par.kind in (K.FOR_STMT, K.WHILE_STMT, K.DO_STMT):
                    loop = True
                n = par
            kids = list(inner.get_children())
            if not loop and kids and good(p, kids[0]):
                fs = line_span(d, ext(kids[0])[0], ext(kids[0])[0])
                if fs:
                    ind = indent_of(d, ext(kids[0])[0])
                    yield Edit("scope", f"line {st.extent.start.line}: `{v.spelling}` declared in the block that uses it (line {inner.extent.start.line})",
                               [(dd.span[0], dd.span[1], b""), (fs[0], fs[0], ind + text + b"\n")], st.extent.start.line)
        # outwards: the start of each enclosing block
        n = c.parent.get(home.hash) if home is not None else None
        while n is not None:
            if n.kind == K.COMPOUND_STMT and good(p, n):
                kids = list(n.get_children())
                if kids and good(p, kids[0]):
                    fs = line_span(d, ext(kids[0])[0], ext(kids[0])[0])
                    if fs:
                        ind = indent_of(d, ext(kids[0])[0])
                        yield Edit("scope", f"line {st.extent.start.line}: `{v.spelling}` declared at the start of the block at line {n.extent.start.line}",
                                   [(dd.span[0], dd.span[1], b""), (fs[0], fs[0], ind + text + b"\n")], st.extent.start.line)
            n = c.parent.get(n.hash)


# ---------------------------------------------------------------- tier b generators

def gen_nest(c):
    """A member read that is kept in a local, read through a small inline with a local of its own
    (written before the function). MWCC creates an inlined function's locals when its call is
    expanded, one pass per nesting level, and colours values in the order they were created
    (docs/REGALLOC.md): a value that comes out of a nested inline is created, and coloured, after
    every local of the function it is used in. Only matters where this function is itself inlined."""
    p, d = c.p, c.data
    fn = c.fn
    cls = fn.semantic_parent
    if cls is None or cls.kind not in (K.CLASS_DECL, K.STRUCT_DECL) or not cls.spelling:
        return
    top = d.rfind(b"\n", 0, ext(fn)[0]) + 1
    if d[top:ext(fn)[0]].strip():
        return
    while top > 0:  # above the comment block that belongs to the function
        prev = d.rfind(b"\n", 0, top - 1) + 1
        if not d[prev:top].lstrip().startswith(b"//"):
            break
        top = prev
    seen = set()
    for n in c.nodes:
        rhs = None
        if n.kind == K.BINARY_OPERATOR and c.is_stmt(n):
            b = binop(p, n)
            if b and b[1] == b"=" and strip_expr(b[0]).kind == K.DECL_REF_EXPR:
                rhs = b[2]
        elif n.kind == K.VAR_DECL:
            init = [x for x in n.get_children() if is_expr(x.kind)]
            if len(init) == 1:
                rhs = init[0]
        if rhs is None or not good(p, rhs):
            continue
        m = strip_expr(rhs)
        if m.kind != K.MEMBER_REF_EXPR or m.type.get_canonical().kind not in SCALAR:
            continue
        kids = list(m.get_children())
        if kids and strip_expr(kids[0]).kind != K.CXX_THIS_EXPR:
            continue  # a member of this object only
        name = m.spelling
        ty = m.type.spelling
        if not name or not ty or "(" in ty or "[" in ty or name in seen:
            continue
        seen.add(name)
        helper = c.fresh("read_" + name + "_" + re.sub(r"\W", "", fn.spelling)).encode()
        t = ty.encode()
        text = (b"static inline " + t + (b"" if t.endswith(b"*") else b" ") + helper + b"(const " + cls.spelling.encode()
                + b"* self) {\n" + c.step + t + (b"" if t.endswith(b"*") else b" ") + b"value = self->" + name.encode()
                + b";\n" + c.step + b"return value;\n}\n\n")
        uses = [x for x in c.nodes if x.kind in (K.BINARY_OPERATOR, K.VAR_DECL)]
        reps = [(top, top, text), (*ext(m), helper + b"(this)")]
        yield Edit("nest", f"line {n.extent.start.line}: `{name}` read through a nested inline with its own local (`{helper.decode()}`)",
                   reps, n.extent.start.line)


def helper_top(c):
    """Where a helper of c.fn is written: above the function and the comment block that belongs to it."""
    d, fn = c.data, c.fn
    top = d.rfind(b"\n", 0, ext(fn)[0]) + 1
    if d[top:ext(fn)[0]].strip():
        return None
    while top > 0:
        prev = d.rfind(b"\n", 0, top - 1) + 1
        if not d[prev:top].lstrip().startswith(b"//"):
            break
        top = prev
    return top


def by_reference(c, use):
    """Is this use of a local handed to something that could change it through a reference?"""
    n, par = use, c.parent.get(use.hash)
    while par is not None and par.kind in (K.UNEXPOSED_EXPR, K.PAREN_EXPR):
        n, par = par, c.parent.get(par.hash)
    if par is None:
        return False
    if par.kind == K.VAR_DECL:
        t = par.type
        return t.kind == TK.LVALUEREFERENCE and not t.get_pointee().is_const_qualified()
    if par.kind != K.CALL_EXPR:
        return False
    args = list(par.get_arguments())
    idx = next((i for i, x in enumerate(args) if ext(x)[0] <= ext(n)[0] and ext(n)[1] <= ext(x)[1]), None)
    if idx is None:
        return False  # the object of a member call (a record goes to the helper by reference anyway)
    decl = par.referenced
    if decl is None:
        return True
    params = list(decl.get_arguments())
    if idx >= len(params):
        return False
    t = params[idx].type
    return t.kind == TK.LVALUEREFERENCE and not t.get_pointee().is_const_qualified()


def outline_site(c, run, top, need_local=False):
    """One statement, or a run of consecutive statements of one block, as an inline helper: a dict
    (span, ind, params, args, body, top, line, end, outer), or None when it cannot move as it stands."""
    p, d, fn = c.p, c.data, c.fn
    if not all(good(p, n) for n in run):
        return None
    s, e = ext(run[0])[0], semi_end(d, ext(run[-1])[1])
    span = line_span(d, s, e)
    if span is None:
        return None
    inner = [x for n in run for x in walk(n)]
    if any(x.kind in (K.RETURN_STMT, K.GOTO_STMT, K.LABEL_STMT, K.CXX_TRY_STMT, K.CASE_STMT, K.DEFAULT_STMT) and
           not any(y.kind == K.SWITCH_STMT and ext(y)[0] <= ext(x)[0] and ext(x)[1] <= ext(y)[1] for y in inner)
           for x in inner if x.kind in (K.CASE_STMT, K.DEFAULT_STMT)):
        return None
    if any(x.kind in (K.RETURN_STMT, K.GOTO_STMT, K.LABEL_STMT, K.CXX_TRY_STMT) for x in inner):
        return None
    if need_local and not any(x.kind == K.VAR_DECL for x in inner):
        return None

    def leaves(x, loop, sw):
        """a break or continue that belongs to a statement outside the run"""
        if x.kind == K.BREAK_STMT and not (loop or sw):
            return True
        if x.kind == K.CONTINUE_STMT and not loop:
            return True
        lp = loop or x.kind in (K.FOR_STMT, K.WHILE_STMT, K.DO_STMT)
        return any(leaves(k, lp, sw or x.kind == K.SWITCH_STMT) for k in x.get_children())
    if any(leaves(n, False, False) for n in run):
        return None
    cls = fn.semantic_parent
    member = cls is not None and cls.kind in (K.CLASS_DECL, K.STRUCT_DECL) and bool(cls.spelling)
    const = b"const " if txt(p, fn).split(b"{", 1)[0].rstrip().endswith(b"const") else b""
    outer = []
    for x in inner:
        if x.kind != K.DECL_REF_EXPR:
            continue
        r = x.referenced
        if r is None or r.kind not in (K.VAR_DECL, K.PARM_DECL) or r.semantic_parent != fn:
            continue
        if s <= r.extent.start.offset < e:
            continue
        if r.storage_class in (C.StorageClass.STATIC, C.StorageClass.EXTERN) or c.written(x) or by_reference(c, x):
            return None
        if all(r != o for o in outer):
            outer.append(r)
    if len(outer) > 6:
        return None
    params, args = [], []
    for r in outer:
        t = r.type
        ck = t.get_canonical().kind
        sp = t.spelling
        if not sp or "(" in sp or "unnamed" in sp or "anonymous" in sp:
            return None
        if ck in SCALAR:
            decl = sp + ("" if sp.endswith("*") else " ") + r.spelling
        elif ck == TK.LVALUEREFERENCE:
            decl = sp + ("" if sp.endswith("&") else " ") + r.spelling
        elif ck == TK.RECORD:
            decl = sp + "& " + r.spelling
        elif ck == TK.CONSTANTARRAY and "[" not in t.get_array_element_type().spelling:
            decl = t.get_array_element_type().spelling + "* " + r.spelling
        else:
            return None
        params.append(decl.encode())
        args.append(r.spelling.encode())
    # the text with every use of `this` spelled through the helper's parameter
    reps = []
    for x in inner:
        if not good(p, x):
            continue
        if x.kind == K.CXX_THIS_EXPR:
            xs, xe = ext(x)
            if d[xs:xe] == b"this":
                reps.append((xs - s, xe - s, b"self"))
        elif x.kind == K.MEMBER_REF_EXPR and not list(x.get_children()):
            xs = ext(x)[0]
            reps.append((xs - s, xs - s, b"self->"))
    if reps and not member:
        return None
    body = Edit("x", "", reps).apply(d[s:e]) if reps else d[s:e]
    if body is None:
        return None
    if reps:
        params.insert(0, const + cls.spelling.encode() + b"* self")
        args.insert(0, b"this")
    ind = indent_of(d, s)
    lines = body.split(b"\n")
    body = b"\n".join([c.step + lines[0]] + [(c.step + l[len(ind):] if l.startswith(ind) else l) for l in lines[1:]])
    return {"node": run[0], "span": span, "ind": ind, "params": b", ".join(params), "args": b", ".join(args), "body": body,
            "top": top, "line": run[0].extent.start.line, "end": run[-1].extent.end.line, "outer": len(outer)}


def outline_sites(c, kinds=(K.FOR_STMT, K.WHILE_STMT, K.DO_STMT), need_local=False):
    """Statements of c.fn that can move, as they stand, into an inline helper written above the
    function. The statement may read (never write) the function's own locals and arguments:
    scalars go to the helper by value, records and arrays by reference. It may not return, jump or
    leave an enclosing loop."""
    top = helper_top(c)
    if top is None:
        return []
    out = []
    for n in c.nodes:
        if n.kind in kinds and c.is_stmt(n):
            site = outline_site(c, [n], top, need_local)
            if site is not None:
                out.append(site)
    return out


def run_sites(c, cap=40):
    """Runs of statements that hold the whole life of the locals declared in them: from a
    declaration to the last statement of its block that uses what the run declares."""
    top = helper_top(c)
    if top is None:
        return []
    out, seen = [], set()
    for comp in c.compounds():
        kids = list(comp.get_children())
        for i, st in enumerate(kids):
            if st.kind != K.DECL_STMT or len(out) >= cap:
                continue
            j = i
            while True:
                lo, hi = ext(kids[i])[0], ext(kids[j])[1]
                last = max((ext(u)[0] for v in c.vars if lo <= ext(v)[0] < hi for u in c.uses(v)), default=lo)
                k = max((m for m in range(j, len(kids)) if ext(kids[m])[0] <= last), default=j)
                if k == j:
                    break
                j = k
            if j == i or (i == 0 and j == len(kids) - 1 and comp == c.body):
                continue  # nothing uses it, or the run is the whole function
            for stop in {j, len(kids) - 1} if not (i == 0 and comp == c.body) else {j}:
                key = (ext(kids[i])[0], ext(kids[stop])[1])
                if key in seen:
                    continue
                seen.add(key)
                site = outline_site(c, kids[i:stop + 1], top)
                if site is not None:
                    out.append(site)
    return out


def outline_edit(c, site, kind, what, prefix):
    name = c.fresh(f"{prefix}{site['line']}_" + re.sub(r"\W", "", c.fn.spelling)).encode()
    helper = b"static inline void " + name + b"(" + site["params"] + b") {\n" + site["body"] + b"\n}\n\n"
    return Edit(kind, f"lines {site['line']}-{site['end']}: {what} in an inline helper of its own (`{name.decode()}`)",
                [(site["top"], site["top"], helper), (site["span"][0], site["span"][1], site["ind"] + name + b"(" + site["args"] + b");\n")],
                site["line"])


def gen_outline(c):
    """A loop moved into a small inline helper written above the function (values of the function
    it only reads become the helper's parameters). A loop counter declared in the function is a
    named local and is coloured after the temporaries the optimiser makes for the loop (a
    strength-reduced table pointer); the counter of an inlined helper is created at inlining,
    before those, and is coloured first (docs/REGALLOC.md). In the original such loops were inline
    functions."""
    for site in sorted(outline_sites(c), key=lambda x: x["outer"]):
        yield outline_edit(c, site, "outline", "the loop", "loop")


def gen_outblock(c):
    """An `if` statement or a braced block that declares locals, moved into an inline helper the same
    way: its locals become the locals of an inlined function, created (and coloured) at inlining
    instead of with the function's own (docs/REGALLOC.md, rule 4)."""
    for site in sorted(outline_sites(c, (K.IF_STMT, K.COMPOUND_STMT, K.SWITCH_STMT), need_local=True), key=lambda x: x["outer"]):
        yield outline_edit(c, site, "outblock", "the block", "block")


def gen_outrun(c):
    """A run of statements that holds the whole life of its locals (a declaration up to the last
    statement that uses it), moved into an inline helper the same way."""
    for site in sorted(run_sites(c), key=lambda x: x["outer"]):
        yield outline_edit(c, site, "outrun", "the statements", "part")


def gen_loopform(c):
    """`for (init; cond; inc) { body }`  ->  `init; while (cond) { body inc; }` (bodies without continue)."""
    p, d = c.p, c.data
    for n in c.nodes:
        if n.kind != K.FOR_STMT or not c.is_stmt(n) or not good(p, n):
            continue
        ch = list(n.get_children())
        if len(ch) != 4 or not all(good(p, x) for x in ch) or ch[3].kind != K.COMPOUND_STMT:
            continue
        init, cond, inc, body = ch
        if any(x.kind == K.CONTINUE_STMT for x in walk(body)) or init.kind == K.DECL_STMT:
            continue
        sp = line_span(d, ext(n)[0], ext(n)[1])
        if not sp:
            continue
        ind = indent_of(d, ext(n)[0])
        bs, be = ext(body)
        inner = d[bs + 1:be - 1].rstrip(b" \t")
        if not inner.endswith(b"\n"):
            continue
        new = (ind + txt(p, init).strip().rstrip(b";") + b";\n" + ind + b"while (" + txt(p, cond).strip() + b") {" + inner
               + ind + c.step + txt(p, inc).strip() + b";\n" + ind + b"}\n")
        yield Edit("loopform", f"line {n.extent.start.line}: the for loop as a while loop", [(sp[0], sp[1], new)], n.extent.start.line)


def gen_retvar(c):
    """`return e;` -> `T result = e; return result;`"""
    p, d = c.p, c.data
    if c.void:
        return
    ty = c.fn.result_type.spelling
    if not ty or c.fn.result_type.get_canonical().kind not in SCALAR:
        return
    for n in c.nodes:
        if n.kind != K.RETURN_STMT or not c.is_stmt(n) or not good(p, n):
            continue
        ch = list(n.get_children())
        if len(ch) != 1 or strip_expr(ch[0]).kind in (K.DECL_REF_EXPR, K.INTEGER_LITERAL, K.CXX_BOOL_LITERAL_EXPR, K.FLOATING_LITERAL):
            continue
        sp = line_span(d, ext(n)[0], semi_end(d, ext(n)[1]))
        if not sp:
            continue
        ind = indent_of(d, ext(n)[0])
        name = c.fresh("result").encode()
        t = ty.encode()
        new = ind + t + (b"" if t.endswith(b"*") else b" ") + name + b" = " + txt(p, ch[0]).strip() + b";\n" + ind + b"return " + name + b";\n"
        yield Edit("retvar", f"line {n.extent.start.line}: the returned value held in a local `{name.decode()}` first",
                   [(sp[0], sp[1], new)], n.extent.start.line)


def gen_inline(c):
    p = c.p
    for d, v, init, uses in temps(c):
        if len(uses) > 3:
            continue
        reps = [(d.span[0], d.span[1], b"")] + [(*ext(u), paren(p, init)) for u in uses]
        yield Edit("inline", f"line {d.n.extent.start.line}: `{v.spelling}` replaced by its value at its {len(uses)} use(s)", reps,
                   d.n.extent.start.line)


def gen_type(c):
    p, d = c.p, c.data
    for st in c.nodes:
        if st.kind != K.DECL_STMT:
            continue
        sv = single_var(p, st)
        if sv is None:
            continue
        _, v, head, _ = sv
        words = head.split()
        quals = [w for w in words if w in (b"const", b"register")]
        base = b" ".join(w for w in words if w not in (b"const", b"register"))
        fam = TYPE_FAMILY.get(base)
        if fam is None:
            continue
        s = ext(st)[0]
        for ty in fam:
            if ty == base:
                continue
            new = b" ".join(quals + [ty]) + b" "
            yield Edit("type", f"line {st.extent.start.line}: `{v.spelling}` declared {ty.decode()} (was {base.decode()})",
                       [(s, s + len(head), new)], st.extent.start.line)


SAME_SIZE = {b"int": b"s32", b"s32": b"int", b"long": b"int", b"u32": b"unsigned int", b"unsigned int": b"u32",
             b"unsigned long": b"unsigned int"}


def gen_intlong(c):
    """`int` <-> `s32` (`long`), `unsigned int` <-> `u32`, on a local or a for-loop counter. Same size
    and signedness, so behaviour is kept; but they are different types to MWCC: a value of one handed
    to an inline function's parameter of the other is converted, and the converted copy is one more
    value to colour (docs/REGALLOC.md, "An argument of another type is a copy")."""
    p = c.p
    for st in c.nodes:
        if st.kind != K.DECL_STMT:
            continue
        sv = single_var(p, st)
        if sv is None:
            continue
        _, v, head, _ = sv
        words = head.split()
        quals = [w for w in words if w in (b"const", b"register")]
        base = b" ".join(w for w in words if w not in (b"const", b"register"))
        ty = SAME_SIZE.get(base)
        if ty is None:
            continue
        s = ext(st)[0]
        yield Edit("intlong", f"line {st.extent.start.line}: `{v.spelling}` declared {ty.decode()} (was {base.decode()}; same size, another type)",
                   [(s, s + len(head), b" ".join(quals + [ty]) + b" ")], st.extent.start.line)


def temp_name(text):
    ids = re.findall(rb"[A-Za-z_]\w*", text)
    ids = [i for i in ids if i not in (b"this", b"get", b"sizeof")]
    if not ids:
        return "value"
    w = ids[-1].decode()
    w = re.sub(r"_[0-9A-Fa-f]{8}$", "", w)
    m = re.match(r"field_0x([0-9A-Fa-f]+)$", w)
    if m:
        return "field" + m.group(1).upper()
    w = re.sub(r"^(get|is|fn_)", "", w) or w
    if len(w) > 1 and w[0] == "m" and w[1].isupper():
        w = w[1:]
    w = re.sub(r"^[gs]([A-Z])", r"\1", w)
    return (w[0].lower() + w[1:]) if w and w[0].isalpha() else "value"


def gen_temp(c):
    """Hold a value that is read several times in a local declared before its first use."""
    p, d = c.p, c.data
    groups = {}
    writes = set()
    for n in c.nodes:
        if n.kind in (K.BINARY_OPERATOR, K.COMPOUND_ASSIGNMENT_OPERATOR):
            b = binop(p, n)
            if b and (n.kind == K.COMPOUND_ASSIGNMENT_OPERATOR or b[1] == b"="):
                writes.add(txt(p, b[0]).strip())
        elif n.kind == K.UNARY_OPERATOR:
            t = txt(p, n).strip()
            if t.startswith((b"++", b"--", b"&")):
                writes.add(t.lstrip(b"+-&").strip())
            elif t.endswith((b"++", b"--")):
                writes.add(t.rstrip(b"+-").strip())
    for n in c.nodes:
        if n.kind not in (K.MEMBER_REF_EXPR, K.ARRAY_SUBSCRIPT_EXPR, K.CALL_EXPR, K.BINARY_OPERATOR, K.UNARY_OPERATOR) or not good(p, n):
            continue
        t = txt(p, n).strip()
        if len(t) < 4 or t in writes or any(w.startswith(t) for w in writes if w != t and w[len(t):len(t) + 1] in (b".", b"-", b"[")):
            continue
        ty = n.type
        if ty.get_canonical().kind not in SCALAR:
            continue
        if n.kind == K.CALL_EXPR:
            if any(impure(p, a) for a in n.get_arguments()):
                continue
        elif impure(p, n):
            continue
        if n.kind == K.MEMBER_REF_EXPR and c.parent.get(n.hash) is not None and c.parent[n.hash].kind == K.CALL_EXPR \
                and ext(c.parent[n.hash])[0] == ext(n)[0] and n.type.kind != TK.POINTER and not list(n.get_children()):
            continue  # the callee name of a method call
        groups.setdefault(t, []).append(n)
    ranked = sorted(((len(v) * len(k), k, v) for k, v in groups.items() if len(v) >= 2), key=lambda x: -x[0])[:14]
    for _, t, occ in ranked:
        # drop occurrences nested in another occurrence's extent (the same node seen through a wrapper)
        occ = sorted({ext(n): n for n in occ}.values(), key=lambda n: ext(n)[0])
        if len(occ) < 2:
            continue
        spelling = occ[0].type.spelling
        if not spelling or "(" in spelling or "<dependent" in spelling or "[" in spelling:
            continue
        if occ[0].type.get_canonical().kind != TK.POINTER:
            spelling = re.sub(r"^const\s+", "", spelling)
        # the statement (child of a compound) that holds the first occurrence, and that compound
        n, par = occ[0], c.parent.get(occ[0].hash)
        while par is not None and par.kind != K.COMPOUND_STMT:
            n, par = par, c.parent.get(par.hash)
        if par is None:
            continue
        cs, ce = ext(par)
        inside = [o for o in occ if cs <= ext(o)[0] and ext(o)[1] <= ce]
        if len(inside) < 2:
            continue
        sp = line_span(d, ext(n)[0], semi_end(d, ext(n)[1])) or line_span(d, ext(n)[0], ext(n)[0])
        if not sp:
            continue
        name = c.fresh(temp_name(t))
        ind = indent_of(d, ext(n)[0])
        ty = spelling.encode()
        decl = ind + ty + (b"" if ty.endswith(b"*") else b" ") + name.encode() + b" = " + t + b";\n"
        reps = [(sp[0], sp[0], decl)] + [(*ext(o), name.encode()) for o in inside]
        yield Edit("temp", f"line {n.extent.start.line}: `{D(t, 36)}` ({len(inside)} reads) held in a local `{name}`", reps,
                   n.extent.start.line)


CMP_OPS = (b"==", b"!=", b"<", b">", b"<=", b">=")


def gen_callfirst(c):
    """A call compared with another value named first: `T v = f(...); if (v == x)` (docs/RULES_CARD.md:
    `cmpw` operands swapped against a call result). MWCC puts a named call result first in the compare."""
    p, d = c.p, c.data
    calls = {ext(n): n for n in c.nodes if n.kind == K.CALL_EXPR and good(p, n)}
    seen = set()
    for n in c.nodes:
        if n.kind != K.BINARY_OPERATOR or not good(p, n):
            continue
        b = binop(p, n)
        if not b or b[1] not in CMP_OPS:
            continue
        for side in (b[0], b[2]):
            call = calls.get(ext(side))
            if call is None:
                # an implicit cast or parentheses around the call
                inner = [k for k in side.get_children()]
                while len(inner) == 1 and ext(inner[0]) not in calls and inner[0].kind in (K.UNEXPOSED_EXPR, K.PAREN_EXPR):
                    inner = list(inner[0].get_children())
                call = calls.get(ext(inner[0])) if len(inner) == 1 else None
            if call is None or ext(call) in seen:
                continue
            ty = call.type
            if ty.get_canonical().kind not in SCALAR:
                continue
            spelling = ty.spelling
            if not spelling or "(" in spelling or "<dependent" in spelling or "[" in spelling:
                continue
            if ty.get_canonical().kind != TK.POINTER:
                spelling = re.sub(r"^const\s+", "", spelling)
            st, par = n, c.parent.get(n.hash)
            while par is not None and par.kind != K.COMPOUND_STMT:
                st, par = par, c.parent.get(par.hash)
            if par is None:
                continue
            ls = d.rfind(b"\n", 0, ext(st)[0]) + 1
            if d[ls:ext(st)[0]].strip():
                continue  # the statement does not start its line
            sp = (ls, ls)
            seen.add(ext(call))
            t = txt(p, call).strip()
            name = c.fresh(temp_name(t))
            ind = indent_of(d, ext(st)[0])
            tyb = spelling.encode()
            decl = ind + tyb + (b"" if tyb.endswith(b"*") else b" ") + name.encode() + b" = " + t + b";\n"
            yield Edit("callfirst", f"line {n.extent.start.line}: `{D(t, 36)}` named before the compare (`{name}`)",
                       [(sp[0], sp[0], decl), (*ext(call), name.encode())], n.extent.start.line)


def gen_swap(c):
    p, d = c.p, c.data
    for comp in c.compounds():
        kids = list(comp.get_children())
        for a, b in zip(kids, kids[1:]):
            if not (good(p, a) and good(p, b)) or not is_expr(a.kind) or not is_expr(b.kind):
                continue
            la, lb = line_span(d, *ext(a)), line_span(d, *ext(b))
            if not la or not lb or la[1] != lb[0]:
                continue
            yield Edit("swap", f"lines {a.extent.start.line}-{b.extent.start.line}: the two statements in the other order",
                       [(la[0], lb[1], d[lb[0]:lb[1]] + d[la[0]:la[1]])], a.extent.start.line)


def gen_cast(c):
    p = c.p
    for n in c.nodes:
        if n.kind != K.CSTYLE_CAST_EXPR or not good(p, n):
            continue
        ch = [x for x in n.get_children() if is_expr(x.kind)]
        if len(ch) != 1 or not good(p, ch[0]):
            continue
        yield Edit("cast", f"line {n.extent.start.line}: `{D(txt(p, n))}` without the cast", [(*ext(n), txt(p, ch[0]))],
                   n.extent.start.line)


GENERATORS = {"decl": gen_decl, "commute": gen_commute, "scope": gen_scope, "compound": gen_compound, "if": gen_if,
              "boolret": gen_boolret, "andif": gen_andif, "early": gen_early, "ternary": gen_ternary, "stmt": gen_stmt,
              "const": gen_const, "loop": gen_loop, "incr": gen_incr, "cond": gen_cond, "untemp": gen_untemp,
              "type": gen_type, "temp": gen_temp, "inline": gen_inline, "swap": gen_swap, "cast": gen_cast,
              "loopform": gen_loopform, "retvar": gen_retvar, "nest": gen_nest, "outline": gen_outline, "outblock": gen_outblock, "outrun": gen_outrun,
              "intlong": gen_intlong, "callfirst": gen_callfirst}


# ---------------------------------------------------------------- the slice of the file a function needs

def closure(p, fn):
    """Function definitions of this file whose code can end up in fn's (it refers to them, transitively)."""
    by = {f.hash: f for f in p.functions()}
    keep, todo = {fn.hash}, [fn]
    while todo:
        f = todo.pop()
        for x in walk(f):
            if x.kind in (K.DECL_REF_EXPR, K.MEMBER_REF_EXPR, K.CALL_EXPR):
                r = x.referenced
                if r is not None and r.kind in FUNC_KINDS:
                    dd = r.get_definition()
                    if dd is not None and dd.hash in by and dd.hash not in keep:
                        keep.add(dd.hash)
                        todo.append(by[dd.hash])
    return keep


def inlined_callees(p, fn, called, depth=3):
    """Functions of this file inlined into fn, nearest first: referenced from fn (or from one of these)
    and not among `called`, the names fn's compiled code calls out of line."""
    by = {f.hash: f for f in p.functions()}
    out, level = [], [fn]
    for _ in range(depth):
        nxt = []
        for g in level:
            for x in walk(g):
                if x.kind not in (K.CALL_EXPR, K.MEMBER_REF_EXPR, K.DECL_REF_EXPR):
                    continue
                r = x.referenced
                dd = r.get_definition() if r is not None and r.kind in FUNC_KINDS else None
                if dd is None or dd.hash not in by or dd == fn or any(dd == o for o in out) or body_of(dd) is None:
                    continue
                if called is not None and any(re.match(re.escape(dd.spelling) + r"(__|$)", n) for n in called):
                    continue
                out.append(dd)
                nxt.append(dd)
        level = nxt
    return out


def slice_text(p, fn):
    """The file with the body of every function fn cannot inline replaced by an empty loop, byte for
    byte as long as the original (so edits and line numbers carry over). Compiles several times faster."""
    data = bytearray(p.data)
    keep = closure(p, fn)
    for f in p.functions():
        if f.hash in keep:
            continue
        lp = f.lexical_parent
        if lp is not None and lp.kind in (K.CLASS_DECL, K.STRUCT_DECL, K.CLASS_TEMPLATE):
            continue
        b = body_of(f)
        if b is None or not good(p, b):
            continue
        s, e = ext(b)
        if data[s:s + 1] != b"{" or data[e - 1:e] != b"}" or e - s < 14:
            continue
        for i in range(s + 1, e - 1):
            if data[i] != 0x0A:
                data[i] = 0x20
        data[s + 1:s + 10] = b"for(;;){}"
    return bytes(data)


# ---------------------------------------------------------------- search

def candidates(unit, data, demangled, kinds, line=None, lines=None, extra_sites=True, called=None, within=None):
    """[Edit] for one function of `data` (and for functions of this file whose lines the diff points
    at: code inlined into it), cheapest kinds first; edits at the diff's lines before the others."""
    p = Parsed(unit, data)
    fn = p.locate(demangled, line)
    if fn is None or body_of(fn) is None:
        return None
    sites = [fn]
    if extra_sites:
        keep = closure(p, fn)
        for l in lines or ():
            g = p.at_line(l)
            if g is not None and g.hash in keep and all(g != s for s in sites) and body_of(g) is not None:
                sites.append(g)
        # callees defined in this file that are not called out of line: MWCC inlined them (and theirs)
        for g in inlined_callees(p, fn, called):
            if all(g != s for s in sites):
                sites.append(g)
    out, seen = [], set()
    near = set()
    for l in lines or ():
        near.update((l - 1, l, l + 1))
    for site in sites[:8]:
        try:
            ctx = Ctx(p, site)
        except Exception:  # noqa: BLE001
            continue
        lo, hi = ext(site)
        for k in kinds:
            try:
                eds = list(GENERATORS[k](ctx))
            except Exception as e:  # noqa: BLE001  (a generator must not take the search down)
                print(f"    (generator {k} failed on {site.spelling}: {type(e).__name__}: {e})", file=sys.stderr)
                continue
            for ed in eds:
                if any(not (lo <= s <= e <= hi) for s, e, _ in ed.reps if not (s == e and s < lo and ed.kind in ABOVE_FN)):
                    continue
                if within is not None and not any(within[0] <= data.count(b"\n", 0, s) + 1 <= within[1] for s, _, _ in ed.reps):
                    continue  # --lines: one piece of the function at a time
                key = (ed.kind, tuple(ed.reps))
                if key in seen:
                    continue
                seen.add(key)
                if site is not fn:
                    ed.desc = f"in `{site.spelling}` (inlined): " + ed.desc
                out.append(ed)
    if near:
        out.sort(key=lambda ed: 0 if ed.kind == "decl" or ed.line in near else 1)
    return out


def diff_text(old, new, path):
    a, b = old.decode("utf-8", "replace").splitlines(True), new.decode("utf-8", "replace").splitlines(True)
    return "".join(difflib.unified_diff(a, b, f"a/{path}", f"b/{path}", n=2))


def fn_lines(f):
    return sorted({r[3] for r in f.rows if r[0] and r[3]})


def merge_edits(items, cap=12):
    """From [(gain, Edit)], the edits with the largest gains whose text ranges neither overlap nor touch:
    improvements in different places of a function mostly add up, so they can be taken together."""
    taken, spans = [], []
    for gain, ed in sorted(items, key=lambda x: -x[0]):
        mine = [(a, b) for a, b, _ in ed.reps]
        if any(not (b < c or d < a) for a, b in mine for c, d in spans):
            continue
        taken.append(ed)
        spans += mine
        if len(taken) >= cap:
            break
    return taken


def search(unit, name, kinds=None, depth=4, beam=6, budget=1500, verbose=False, log=print, data=None, sideways=3,
           uphill=0, per_round=0, seed=0, within=None):
    """Beam search from the unit's source. Returns a dict: fn, base/best (pct, score), trace, kinds,
    text (bytes or None), lead (a below-100 % result that used a tier b rewrite), tried, secs, note.
    uphill > 0 also keeps candidates up to that much WORSE than their parent when a round finds too
    few better ones (register allocation often needs two changes of which the first alone looks
    worse); per_round > 0 compiles a random sample of that size per round instead of everything."""
    import random
    rng = random.Random(seed)
    kinds = kinds or KINDS_A
    data = data if data is not None else unit.source().encode("utf-8", "surrogateescape")
    dec = lambda b: b.decode("utf-8", "surrogateescape")  # noqa: E731
    t0 = time.time()
    base = unit.build(dec(data), sym=True)
    f0 = base.find(name)
    res = {"fn": f0.name, "base": (f0.pct, f0.score), "best": (f0.pct, f0.score), "trace": [], "kinds": [], "text": None,
           "lead": None, "tried": 0, "cls": f0.cls, "note": "", "secs": 0.0}
    if f0.score == 0 or f0.cls == "missing":
        res["note"] = "already matches" if f0.score == 0 else "not in our object"
        return res
    first_line = next((r[3] for r in f0.rows if r[3]), None)
    called = {r[2][2] for r in f0.rows if r[2] is not None and r[2][0] in ("bl", "b") and r[2][2]}  # out-of-line callees
    p = Parsed(unit, data)
    fn = p.locate(f0.demangled, first_line)
    if fn is None or body_of(fn) is None:
        res["note"] = "function not found in the source (macro-generated, or a name libclang reads differently)"
        return res
    # the slice: use it only when it compiles to the same code for this function
    work = slice_text(p, fn)
    if work != data:
        try:
            o_full, o_red = unit.compile(dec(data), tag="slice-full"), unit.compile(dec(work), tag="slice-red")
            import subprocess
            out = subprocess.run([str(scratch.fnscore_bin(unit.root)), str(o_full), str(o_red), f0.name],
                                 capture_output=True, text=True).stdout.split("\t")
            if len(out) < 2 or out[1] != "0":
                work = data
        except scratch.CompileError:
            work = data
    sliced = work != data
    names = [f0.name]
    _, r0 = scratch.farm(unit, [work], names, hashes=True)
    if isinstance(r0[0], Exception):
        res["note"] = f"baseline does not compile in the farm: {r0[0]}"
        return res
    s0, h0 = r0[0][0]
    # a state: (score, code hash, work text, full text, trace, kinds used, diff lines)
    frontier = [(s0, h0, work, data, [], [], fn_lines(f0))]
    seen_text = {hashlib.sha1(work).digest()}
    seen_code = {h0}
    best = None      # verified: (pct, objdiff score, full text, trace, kinds)
    lead = None
    native_best = s0

    def verify(full, trace, used):
        """objdiff on the whole unit: (pct, score) if the function is closer and nothing else is worse."""
        try:
            b = unit.build(dec(full), want={f0.name})
        except scratch.CompileError:
            return None
        g = b.fn.get(f0.name)
        if g is None or not (g.pct > f0.pct + 1e-9 or (g.pct == f0.pct and g.score < f0.score)):
            return None
        worse = b.worse_than(base, skip={f0.name})
        if worse:
            if verbose:
                log(f"      (not kept: {worse[0]})")
            return None
        return g.pct, g.score

    for rnd in range(depth):
        pool = []
        for score, h, wtext, ftext, trace, used, lines in frontier:
            eds = candidates(unit, wtext, f0.demangled, kinds, first_line, lines, called=called, within=within)
            if eds is None:
                continue
            for ed in eds:
                nw = ed.apply(wtext)
                if nw is None or nw == wtext:
                    continue
                hh = hashlib.sha1(nw).digest()
                if hh in seen_text:
                    continue
                seen_text.add(hh)
                nf = ed.apply(ftext) if sliced else nw
                if nf is None:
                    continue
                pool.append((ed, nw, nf, trace, used, score, h, wtext, ftext))
        room = budget - res["tried"]
        if not pool or room <= 0:
            break
        if per_round and len(pool) > per_round:
            head = pool[:per_round // 3]                      # the ordered front (declarations, diff lines)
            pool = head + rng.sample(pool[per_round // 3:], per_round - len(head))
        pool = pool[:room]
        _, scores = scratch.farm(unit, [c[1] for c in pool], names, hashes=True)
        res["tried"] += len(pool)
        better, side, up, failed = [], [], [], 0
        gains = {}
        for (ed, nw, nf, trace, used, parent, ph, pw, pf), r in zip(pool, scores):
            if isinstance(r, Exception):
                failed += 1
                if verbose:
                    log(f"    failed [{ed.kind}] {ed.desc}: {str(r)[:90]}")
                continue
            s, h = r[0]
            if s < 0:
                continue
            if verbose and h != ph:
                log(f"    {parent} -> {s} [{ed.kind}] {ed.desc}")
            item = (s, h, nw, nf, trace + [ed.desc], used + [ed.kind], None)
            if s < parent:
                better.append(item)
                gains.setdefault(id(pw), (pw, pf, trace, used, parent, []))[5].append((parent - s, ed))
            elif s == parent and h != ph and h not in seen_code:
                side.append(item)
            elif uphill and parent < s <= parent + uphill and h not in seen_code:
                up.append(item)
        # piecemeal: improvements in different places of the function, taken together
        merged = []
        for pw, pf, trace, used, parent, items in gains.values():
            if len(items) < 2:
                continue
            taken = merge_edits(items)
            for part in ([taken] if len(taken) < 4 else [taken, taken[:len(taken) // 2]]):
                if len(part) < 2:
                    continue
                m = Edit("merge", "", [r for ed in part for r in ed.reps])
                nw, nf = m.apply(pw), (m.apply(pf) if sliced else None)
                if nw is None or (sliced and nf is None):
                    continue
                hh = hashlib.sha1(nw).digest()
                if hh in seen_text:
                    continue
                seen_text.add(hh)
                merged.append((nw, nf if sliced else nw, trace + [ed.desc for ed in part], used + [ed.kind for ed in part],
                               min(parent - g for g, _ in items)))
        if merged and budget - res["tried"] > 0:
            _, ms = scratch.farm(unit, [c[0] for c in merged], names, hashes=True)
            res["tried"] += len(merged)
            for (nw, nf, trace, used, best_single), r in zip(merged, ms):
                if isinstance(r, Exception) or r[0][0] < 0:
                    continue
                s, h = r[0]
                if verbose:
                    log(f"    merged {len(trace)} edits -> {s} (best single {best_single})")
                if s < best_single:
                    better.append((s, h, nw, nf, trace, used, None))
        better.sort(key=lambda x: (x[0], len(x[4])))
        uniq = []
        for it in better:
            if it[1] not in seen_code or it[0] == 0:
                seen_code.add(it[1])
                uniq.append(it)
        log(f"  round {rnd + 1}: {len(pool)} candidates ({failed} did not compile), {len(better)} better, "
            f"{len(side)} sideways, best {uniq[0][0] if uniq else '-'} (was {native_best}) [{time.time() - t0:.0f} s]")
        done = False
        for it in uniq[:beam]:
            if it[0] >= native_best and best is not None:
                continue
            v = verify(it[3], it[4], it[5])
            if v is None:
                continue
            tier_b = any(TIER[k] == "b" for k in it[5])
            if tier_b and v[0] < 100:
                if lead is None or v[1] < lead[1]:
                    lead = (v[0], v[1], it[3], it[4], it[5])
            elif best is None or v[1] < best[1]:
                best = (v[0], v[1], it[3], it[4], it[5])
            native_best = min(native_best, it[0])
            if v[0] == 100:
                best = (v[0], v[1], it[3], it[4], it[5])
                done = True
                break
        if done:
            break
        nxt = uniq[:beam]
        for it in side[:sideways]:
            seen_code.add(it[1])
            nxt.append(it)
        if uphill and len(nxt) < beam:
            up.sort(key=lambda x: x[0])
            for it in up:
                if len(nxt) >= beam:
                    break
                if it[1] not in seen_code:
                    seen_code.add(it[1])
                    nxt.append(it)
        if not nxt:
            break
        # diff lines of each new state, for the next round's ordering (one objdiff run each, on the slice)
        frontier = []
        for s, h, nw, nf, trace, used, _ in nxt:
            try:
                g = unit.build(dec(nw), sym=True, want={f0.name}).fn.get(f0.name)
                lines = fn_lines(g) if g is not None else []
            except scratch.CompileError:
                lines = []
            frontier.append((s, h, nw, nf, trace, used, lines))
    if best is not None:
        res.update(best=(best[0], best[1]), text=best[2], trace=best[3], kinds=best[4])
    if lead is not None and (best is None or lead[1] < best[1]):
        res["lead"] = {"pct": lead[0], "score": lead[1], "trace": lead[3], "kinds": lead[4],
                       "diff": diff_text(data, lead[2], unit.src)}
    res["secs"] = round(time.time() - t0, 1)
    return res


def site_key(f):
    par = f.semantic_parent
    return ((par.spelling if par is not None and par.kind in (K.CLASS_DECL, K.STRUCT_DECL, K.CLASS_TEMPLATE) else ""),
            f.spelling, len(list(f.get_arguments())))


def shared_callees(unit, data=None):
    """Functions of the unit's file that MWCC inlined into functions that are still open, most
    inliners first: [(site key, [open functions that inline it])]. A callee can match on its own
    and still be the reason its inlined copies differ."""
    data = data if data is not None else unit.source().encode("utf-8", "surrogateescape")
    d = unit.build(data.decode("utf-8", "surrogateescape"), sym=True)
    p = Parsed(unit, data)
    users = {}
    for f in d.fn.values():
        if f.pct >= 100 or f.cls == "missing":
            continue
        fn = p.locate(f.demangled, next((r[3] for r in f.rows if r[3]), None))
        if fn is None or body_of(fn) is None:
            continue
        called = {r[2][2] for r in f.rows if r[2] is not None and r[2][0] in ("bl", "b") and r[2][2]}
        for g in inlined_callees(p, fn, called):
            users.setdefault(site_key(g), []).append(f.name)
    return sorted(users.items(), key=lambda kv: -len(kv[1]))


def unitwide(unit, key, kinds=None, depth=2, budget=600, data=None, log=print):
    """Rewrite ONE function (usually a callee inlined into many) and score every function of the unit.
    A result is kept when more functions match than before, no function that matched stops matching
    and no data section gets worse; functions that were open may move either way (their remaining
    difference is their own). Returns a dict: text (or None), trace, kinds, fixed, worse, tried."""
    kinds = kinds or (KINDS_A + KINDS_B)
    keys = key if isinstance(key, list) else [key]   # several callees of one inlining chain can be rewritten together
    key = keys[0]
    data = data if data is not None else unit.source().encode("utf-8", "surrogateescape")
    dec = lambda b: b.decode("utf-8", "surrogateescape")  # noqa: E731
    t0 = time.time()
    res = {"site": " + ".join("::".join(x for x in k[:2] if x) for k in keys), "text": None, "trace": [], "kinds": [], "fixed": [], "worse": 0, "tried": 0,
           "secs": 0.0, "note": ""}
    names, r0 = scratch.farm(unit, [data], None)
    if isinstance(r0[0], Exception):
        res["note"] = f"does not compile: {r0[0]}"
        return res
    base = r0[0]
    n0 = sum(1 for x in base if x == 0)

    def edits(text):
        p = Parsed(unit, text)
        out = []
        for site in (f for f in p.functions() if site_key(f) in keys):
            if body_of(site) is None:
                continue
            ctx, (lo, hi) = Ctx(p, site), ext(site)
            for k in kinds:
                try:
                    eds = list(GENERATORS[k](ctx))
                except Exception:  # noqa: BLE001
                    continue
                for ed in eds:
                    if all(lo <= s <= e <= hi or (s == e and s < lo and ed.kind in ABOVE_FN) for s, e, _ in ed.reps):
                        if len(keys) > 1:
                            ed.desc = f"in `{site.spelling}`: " + ed.desc
                        out.append(ed)
        return out
    level, seen, best = [(data, [], [])], {hashlib.sha1(data).digest()}, None
    for _ in range(depth):
        pool = []
        for text, trace, used in level:
            for ed in edits(text):
                new = ed.apply(text)
                if new is None:
                    continue
                h = hashlib.sha1(new).digest()
                if h not in seen:
                    seen.add(h)
                    pool.append((new, trace + [ed.desc], used + [ed.kind]))
        pool = pool[:max(0, budget - res["tried"])]
        if not pool:
            break
        _, rs = scratch.farm(unit, [c[0] for c in pool], None)
        res["tried"] += len(pool)
        scored = []
        for (text, trace, used), r in zip(pool, rs):
            if not isinstance(r, list) or any(b == 0 and x != 0 for b, x in zip(base, r)):
                continue
            n = sum(1 for x in r if x == 0)
            scored.append((n, -sum(x for x in r if x > 0), len(trace), text, trace, used))
        scored.sort(key=lambda x: (-x[0], -x[1], x[2]))
        if scored and scored[0][0] > (best[0] if best else n0):
            best = scored[0]
        level = [(x[3], x[4], x[5]) for x in scored[:8] if x[0] >= n0]
        if not level:
            break
    if best is not None:
        try:
            a, b = unit.build(dec(data)), unit.build(dec(best[3]))
        except scratch.CompileError as e:
            res["note"] = str(e)[:100]
            return res
        fixed = sorted(n for n, f in b.fn.items() if f.pct == 100 and n in a.fn and a.fn[n].pct < 100)
        broke = [n for n, f in a.fn.items() if f.pct == 100 and (n not in b.fn or b.fn[n].pct < 100)]
        secs = [k for k, v in a.sections.items() if not k.startswith(".text") and b.sections.get(k, 0) < v - 0.005]
        extra = [n for n in b.extra if n not in a.extra]
        if fixed and not broke and not secs and not extra:
            res.update(text=best[3], trace=best[4], kinds=best[5], fixed=fixed,
                       fixed_bytes=sum(b.fn[n].size for n in fixed),
                       worse=sum(1 for n, f in b.fn.items() if n in a.fn and f.pct < a.fn[n].pct - 0.005))
        else:
            res["note"] = f"objdiff: fixes {len(fixed)}, breaks {len(broke)}, sections {secs}"
    res["secs"] = round(time.time() - t0, 1)
    return res


def callees(a, kinds):
    """--callees: for every unit, rewrite the functions inlined into open functions, most inliners
    first, each scored on the whole unit (unitwide). One JSON line per site."""
    allu = json.loads(Path("objdiff.json").read_text())["units"]
    names = [u["name"] for u in allu if "base_path" in u and (u.get("metadata") or {}).get("source_path")
             and (not a.units or any(x in u["name"] for x in a.units.split(",")))]
    if a.parallel > 1 and not a.only:
        import subprocess
        from concurrent.futures import ThreadPoolExecutor
        jobs = max(2, scratch.JOBS * 2 // a.parallel)
        argv0 = [sys.executable, __file__, "--callees", "--kinds", ",".join(kinds), "--depth", str(a.depth), "--budget", str(a.budget),
                 "--max-sites", str(a.max_sites), "--save", a.save] + (["--apply"] if a.apply else []) + (["--log", a.log] if a.log else []) \
            + (["--chain"] if a.chain else [])

        def run(name):
            env = dict(__import__("os").environ, SCRATCH_JOBS=str(jobs), GRIND_BROKER_SERVER="1")
            pr = subprocess.run(argv0 + ["--units", name, "--only", "1"], capture_output=True, text=True, env=env)
            return [l for l in pr.stdout.splitlines() if l.startswith(("  ", "["))] + ([pr.stderr[-300:]] if pr.returncode else [])
        with ThreadPoolExecutor(max_workers=a.parallel) as ex:
            for lines in ex.map(run, names):
                for l in lines:
                    print(l, flush=True)
        return 0
    logf = open(a.log, "a") if a.log else None
    for name in names:
        if a.only and name != a.units:
            continue
        try:
            unit = scratch.Unit(name)
            sites = shared_callees(unit)
        except Exception as e:  # noqa: BLE001
            print(f"[{name}] {type(e).__name__}: {str(e)[:120]}", flush=True)
            continue
        todo = sites[:a.max_sites]
        if a.chain and todo:
            # the helpers of an inlining chain together: a value may have to change in two of them at once
            todo = [([k for k, _ in todo], sorted({x for _, us in todo for x in us}))]
        for key, users in todo:
            try:
                r = unitwide(unit, key, kinds, min(a.depth, 3 if a.chain else 2), a.budget)
            except Exception as e:  # noqa: BLE001
                r = {"site": key[1], "text": None, "trace": [], "kinds": [], "fixed": [], "worse": 0, "tried": 0, "secs": 0,
                     "note": f"{type(e).__name__}: {str(e)[:150]}"}
            applied, saved = False, None
            if r["text"] is not None:
                old = (unit.root / unit.src).read_bytes()
                out = Path(a.save)
                out.mkdir(parents=True, exist_ok=True)
                hand = any(x in NEEDS_HAND for x in r["kinds"])
                saved = out / (re.sub(r"[^A-Za-z0-9_]", "_", name.split("/")[-1] + "__callee__" + ("chain" if a.chain else r["site"]))[:110]
                               + (".hand" if hand else ".auto") + ".diff")
                saved.write_text(diff_text(old, r["text"], unit.src))
                if a.apply and (not hand or a.apply_b):
                    (unit.root / unit.src).write_bytes(r["text"])
                    applied = True
            row = {"unit": name, "site": r["site"], "users": len(users), "fixed": r["fixed"], "fixed_bytes": r.get("fixed_bytes", 0),
                   "worse": r["worse"], "trace": r["trace"], "kinds": r["kinds"], "tried": r["tried"], "secs": r["secs"],
                   "note": r["note"], "applied": applied, "diff": str(saved) if saved else None}
            if logf:
                logf.write(json.dumps(row) + "\n")
                logf.flush()
            if r["text"] is not None or a.verbose:
                print(f"  {name.split('/')[-1]} {r['site']} (inlined into {len(users)} open): fixes {len(r['fixed'])} "
                      f"({r.get('fixed_bytes', 0)} B), {r['worse']} open ones worse, {r['tried']} tried, {r['secs']} s"
                      + ("  << " + "; ".join(r["trace"])[:140] if r["trace"] else "") + (f" ({r['note']})" if r["note"] else ""), flush=True)
        unit.clean()
    return 0


def shared_groups(unit, data, open_fns=None):
    """Statements written out identically in two or more places of the file, each group as the
    edits that put it into ONE inline helper: [(sites, lines, text of the helper, Edit for every
    site, Edit for the sites in open functions only or None)], most text saved first."""
    p = Parsed(unit, data)
    groups = {}
    for fn in p.functions():
        if body_of(fn) is None or not good(p, fn):
            continue
        try:
            c = Ctx(p, fn)
            sites = outline_sites(c) + outline_sites(c, (K.IF_STMT, K.COMPOUND_STMT, K.SWITCH_STMT), need_local=True) + run_sites(c)
        except Exception:  # noqa: BLE001
            continue
        for site in sites:
            site["fn"] = fn
            groups.setdefault((site["params"], site["body"]), []).append(site)
    out = []
    for (params, body), sites in groups.items():
        if len(sites) < 2:
            continue
        sites.sort(key=lambda x: x["span"][0])
        # copies that overlap (a run found from two starting points) count once
        kept = []
        for x in sites:
            if not kept or x["span"][0] >= kept[-1]["span"][1]:
                kept.append(x)
        sites = kept
        if len(sites) < 2:
            continue
        loop = sites[0]["node"].kind in (K.FOR_STMT, K.WHILE_STMT, K.DO_STMT)
        name = f"{'loop' if loop else 'block'}{sites[0]['line']}_x{len(sites)}".encode()
        while name in data:
            name += b"_"
        helper = b"static inline void " + name + b"(" + params + b") {\n" + body + b"\n}\n\n"
        top = min(x["top"] for x in sites)

        def edit(which):
            lines = ", ".join(str(x["line"]) for x in which)
            return Edit("outline" if loop else "outblock",
                        f"the same {'loop' if loop else 'block'} at lines {lines} in one inline helper (`{name.decode()}`)",
                        [(top, top, helper)] + [(x["span"][0], x["span"][1], x["ind"] + name + b"(" + x["args"] + b");\n") for x in which],
                        sites[0]["line"])
        part = None
        if open_fns is not None:
            sub = [x for x in sites if any(x["fn"] == f for f in open_fns)]
            if 0 < len(sub) < len(sites):
                part = edit(sub)
        out.append((len(sites), body.count(b"\n") + 1, helper, edit(sites), part))
    out.sort(key=lambda g: -(g[0] - 1) * g[1])
    return out


def shared_unit(unit, depth=4, cap=60, log=print):
    """Put statements that the file writes out several times into shared inline helpers, one group
    per round, each scored on every function of the unit (the rule of unitwide). FileSelect had
    three pane loops written out 40 times; in the original each was one inline function, and its
    copies only got the original's registers once ours was one too."""
    dec = lambda b: b.decode("utf-8", "surrogateescape")  # noqa: E731
    data = unit.source().encode("utf-8", "surrogateescape")
    t0 = time.time()
    res = {"text": None, "trace": [], "kinds": [], "fixed": [], "worse": 0, "tried": 0, "groups": 0, "secs": 0.0, "note": ""}
    try:
        d0 = unit.build(dec(data), sym=True)
    except scratch.CompileError as e:
        res["note"] = f"does not compile: {str(e)[:100]}"
        return res
    if all(f.pct >= 100 for f in d0.fn.values()):
        res["note"] = "nothing open"
        return res
    names, r0 = scratch.farm(unit, [data], None)
    if isinstance(r0[0], Exception):
        res["note"] = f"does not compile: {r0[0]}"
        return res
    base = r0[0]
    n0 = sum(1 for x in base if x == 0)
    text, trace, used, best_n = data, [], [], n0
    for _ in range(depth):
        p = Parsed(unit, text)
        open_fns = []
        if text is data:
            for f in d0.fn.values():
                if f.pct < 100 and f.cls != "missing":
                    fn = p.locate(f.demangled, next((r[3] for r in f.rows if r[3]), None))
                    if fn is not None:
                        open_fns.append(fn)
        groups = shared_groups(unit, text, open_fns or None)
        if _ == 0:
            res["groups"] = len(groups)
        pool = []
        for _, _, _, ed_all, ed_part in groups[:cap]:
            for ed in (ed_all, ed_part):
                if ed is not None:
                    new = ed.apply(text)
                    if new is not None:
                        pool.append((new, ed))
        if not pool:
            break
        _, rs = scratch.farm(unit, [c[0] for c in pool], None)
        res["tried"] += len(pool)
        scored = []
        for (new, ed), r in zip(pool, rs):
            if not isinstance(r, list) or any(b == 0 and x != 0 for b, x in zip(base, r)):
                continue
            scored.append((sum(1 for x in r if x == 0), -sum(x for x in r if x > 0), new, ed))
        scored.sort(key=lambda x: (-x[0], -x[1]))
        if not scored or scored[0][0] <= best_n:
            break
        best_n, text = scored[0][0], scored[0][2]
        trace.append(scored[0][3].desc)
        used.append(scored[0][3].kind)
    if text is not data:
        try:
            b = unit.build(dec(text))
            a = unit.build(dec(data))
        except scratch.CompileError as e:
            res["note"] = str(e)[:100]
            return res
        fixed = sorted(n for n, f in b.fn.items() if f.pct == 100 and n in a.fn and a.fn[n].pct < 100)
        broke = [n for n, f in a.fn.items() if f.pct == 100 and (n not in b.fn or b.fn[n].pct < 100)]
        secs = [k for k, v in a.sections.items() if not k.startswith(".text") and b.sections.get(k, 0) < v - 0.005]
        extra = [n for n in b.extra if n not in a.extra]
        if fixed and not broke and not secs and not extra:
            res.update(text=text, trace=trace, kinds=used, fixed=fixed, fixed_bytes=sum(b.fn[n].size for n in fixed),
                       worse=sum(1 for n, f in b.fn.items() if n in a.fn and f.pct < a.fn[n].pct - 0.005))
        else:
            res["note"] = f"objdiff: fixes {len(fixed)}, breaks {len(broke)}, sections {secs}"
    res["secs"] = round(time.time() - t0, 1)
    return res


def shared(a):
    """--shared: every unit (or --units) through shared_unit; one JSON line per unit with groups."""
    allu = json.loads(Path("objdiff.json").read_text())["units"]
    names = [u["name"] for u in allu if "base_path" in u and (u.get("metadata") or {}).get("source_path")
             and (not a.units or any(x in u["name"] for x in a.units.split(",")))]
    if a.parallel > 1 and not a.only:
        import subprocess
        from concurrent.futures import ThreadPoolExecutor
        jobs = max(2, scratch.JOBS * 2 // a.parallel)
        argv0 = [sys.executable, __file__, "--shared", "--depth", str(a.depth), "--save", a.save] + (["--log", a.log] if a.log else [])

        def run(name):
            env = dict(__import__("os").environ, SCRATCH_JOBS=str(jobs), GRIND_BROKER_SERVER="1")
            pr = subprocess.run(argv0 + ["--units", name, "--only", "1"], capture_output=True, text=True, env=env)
            return [l for l in pr.stdout.splitlines() if l.startswith(("  ", "["))] + ([pr.stderr[-300:]] if pr.returncode else [])
        with ThreadPoolExecutor(max_workers=a.parallel) as ex:
            for lines in ex.map(run, names):
                for l in lines:
                    print(l, flush=True)
        return 0
    logf = open(a.log, "a") if a.log else None
    for name in names:
        if a.only and name != a.units:
            continue
        unit = None
        try:
            unit = scratch.Unit(name)
            r = shared_unit(unit, min(a.depth, 6))
        except Exception as e:  # noqa: BLE001
            print(f"[{name}] {type(e).__name__}: {str(e)[:120]}", flush=True)
            continue
        finally:
            if unit is not None:
                unit.clean()
        saved = None
        if r["text"] is not None:
            out = Path(a.save)
            out.mkdir(parents=True, exist_ok=True)
            saved = out / (re.sub(r"[^A-Za-z0-9_]", "_", name.split("/")[-1]) + "__shared.hand.diff")
            saved.write_text(diff_text((unit.root / unit.src).read_bytes(), r["text"], unit.src))
        row = {"unit": name, "groups": r["groups"], "fixed": r["fixed"], "fixed_bytes": r.get("fixed_bytes", 0), "worse": r["worse"],
               "trace": r["trace"], "tried": r["tried"], "secs": r["secs"], "note": r["note"], "diff": str(saved) if saved else None}
        if logf and (r["groups"] or r["note"]):
            logf.write(json.dumps(row) + "\n")
            logf.flush()
        if r["text"] is not None or a.verbose:
            print(f"  {name.split('/')[-1]}: {r['groups']} repeated statements, fixes {len(r['fixed'])} ({r.get('fixed_bytes', 0)} B), "
                  f"{r['worse']} open ones worse, {r['tried']} tried, {r['secs']} s"
                  + ("  << " + "; ".join(r["trace"])[:160] if r["trace"] else "") + (f" ({r['note']})" if r["note"] else ""), flush=True)
    return 0


def bulk(a, kinds):
    """Run the search over many functions (from structural.json); one JSON line per function."""
    st = json.loads(Path(a.structural).read_text())["units"]
    classes = set(a.cls.split(","))
    only = set(a.only.split(",")) if a.only else None
    todo = []
    for uname, fns in st.items():
        if a.units and not any(x in uname for x in a.units.split(",")) or (only is not None and uname != a.units):
            continue
        for fn, (pct, size, cls) in fns.items():
            if only is not None and fn not in only:
                continue
            if (cls.split(" + ")[0] in classes or "all" in classes) and a.min_size <= size <= a.max_size and pct >= a.min_pct:
                todo.append((uname, fn, pct, size, cls))
    todo.sort(key=lambda t: (t[0], t[3]))
    todo = todo[a.offset::a.step][:a.limit] if a.limit else todo[a.offset::a.step]
    print(f"{len(todo)} functions", flush=True)
    logf = open(a.log, "a") if a.log else None
    won = gained = leads = 0
    if a.parallel > 1:
        # several functions at once: one child process per unit (a unit's functions stay in order, so
        # --apply never has two writers on one file), each with a share of the compile jobs
        import subprocess
        by_unit = {}
        for t in todo:
            by_unit.setdefault(t[0], []).append(t)
        jobs = max(2, scratch.JOBS * 2 // a.parallel)
        argv0 = [sys.executable, __file__, "--bulk", "--class", "all", "--structural", a.structural, "--kinds", ",".join(kinds),
                 "--depth", str(a.depth), "--beam", str(a.beam), "--budget", str(a.budget), "--uphill", str(a.uphill),
                 "--per-round", str(a.per_round)] + \
                (["--apply"] if a.apply else []) + (["--partial"] if a.partial else []) + (["--log", a.log] if a.log else []) + \
                (["--apply-b"] if a.apply_b else []) + ["--save", a.save]
        if logf:
            logf.close()

        def run(item):
            uname, rows = item
            want = ",".join(r[1] for r in rows)
            env = dict(__import__("os").environ, SCRATCH_JOBS=str(jobs), GRIND_BROKER_SERVER="1")
            pr = subprocess.run(argv0 + ["--units", uname, "--only", want], capture_output=True, text=True, env=env)
            return [l for l in pr.stdout.splitlines() if l.startswith("[")] + ([pr.stderr[-300:]] if pr.returncode else [])
        from concurrent.futures import ThreadPoolExecutor
        n = 0
        with ThreadPoolExecutor(max_workers=a.parallel) as ex:
            for lines in ex.map(run, sorted(by_unit.items(), key=lambda kv: -len(kv[1]))):
                for l in lines:
                    n += 1
                    print(f"({n}/{len(todo)}) " + l.split("] ", 1)[-1], flush=True)
                    gained += " << " in l
                    won += "-> 100.00" in l and " << " in l
                    leads += "(lead " in l
        print(f"improved {gained} of {len(todo)}, {won} to 100 %, {leads} leads")
        return 0
    units = {}
    for k, (uname, fn, pct, size, cls) in enumerate(todo, 1):
        unit = None
        try:
            unit = units.get(uname) or units.setdefault(uname, scratch.Unit(uname))
            r = search(unit, fn, kinds, a.depth, a.beam, a.budget, False, log=lambda *_: None, uphill=a.uphill,
                       per_round=a.per_round)
        except Exception as e:  # noqa: BLE001
            r = {"fn": fn, "base": (pct, -1), "best": (pct, -1), "trace": [], "kinds": [], "text": None, "lead": None, "tried": 0,
                 "cls": cls, "note": f"{type(e).__name__}: {str(e)[:200]}", "secs": 0}
        finally:
            if unit is not None:
                unit.clean()
        (p0, s0), (p1, s1) = r["base"], r["best"]
        applied, saved = False, None
        if r["text"] is not None:
            gained += 1
            won += p1 == 100
            tier_b = any(x in NEEDS_HAND for x in r["kinds"])
            old = (unit.root / unit.src).read_bytes()
            out = Path(a.save)
            out.mkdir(parents=True, exist_ok=True)
            saved = out / (re.sub(r"[^A-Za-z0-9_]", "_", uname.split("/")[-1] + "__" + fn)[:110] + (".hand" if tier_b else ".auto") + ".diff")
            saved.write_text(diff_text(old, r["text"], unit.src))
            # a result that adds or removes a local is right at 100 % but may not read naturally (a new name,
            # a duplicated expression that was a macro): it goes to a session or a person, not into the source
            if a.apply and (p1 == 100 or a.partial) and (not tier_b or a.apply_b):
                (unit.root / unit.src).write_bytes(r["text"])
                applied = True
        leads += r["lead"] is not None
        row = {"unit": uname, "fn": fn, "size": size, "cls": r["cls"], "base": [p0, s0], "best": [p1, s1], "trace": r["trace"],
               "kinds": r["kinds"], "tried": r["tried"], "note": r["note"], "secs": r["secs"], "applied": applied,
               "diff": str(saved) if saved else None,
               "lead": r["lead"] and {x: r["lead"][x] for x in ("pct", "score", "trace", "kinds")}}
        if logf:
            logf.write(json.dumps(row) + "\n")
            logf.flush()
        tail = ("  << " + "; ".join(r["trace"])[:150]) if r["trace"] else \
               (f"  (lead {r['lead']['pct']:.2f} %: " + "; ".join(r["lead"]["trace"])[:110] + ")") if r["lead"] else ""
        print(f"[{k}/{len(todo)}] {uname.split('/')[-1]} {fn[:42]:42} {size:5} B {p0:6.2f} -> {p1:6.2f} "
              f"({r['tried']} tried, {r['secs']} s){' ' + r['note'] if r['note'] else ''}{tail}", flush=True)
    print(f"improved {gained} of {len(todo)}, {won} to 100 %, {leads} leads")
    return 0


def main():
    import grind_broker
    grind_broker.maybe_delegate("permute", sys.argv[1:])
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0], formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("unit", nargs="?")
    ap.add_argument("function", nargs="?")
    ap.add_argument("--tier", choices=["a", "b"], default="a", help="b adds the rewrites that are accepted only at 100 %%")
    ap.add_argument("--kinds", help="comma-separated kinds instead of a tier")
    ap.add_argument("--depth", type=int, default=4)
    ap.add_argument("--beam", type=int, default=6)
    ap.add_argument("--budget", type=int, default=1500, help="candidates compiled at most")
    ap.add_argument("--deep", action="store_true",
                    help="a long search for a stubborn function: depth 14, 20000 candidates, worse-by-a-little steps allowed")
    ap.add_argument("--uphill", type=int, default=0, help="keep candidates up to this much worse than their parent")
    ap.add_argument("--per-round", type=int, default=0, help="compile a random sample of this size per round")
    ap.add_argument("--lines", metavar="A-B", help="rewrite only these source lines (one piece of a large function; tools/pieces.py)")
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--partial", action="store_true", help="with --bulk --apply: also apply results below 100 %%")
    ap.add_argument("--apply-b", action="store_true", help="with --bulk --apply: also apply results that add or remove a local")
    ap.add_argument("--save", default="build/permute/bulk", help="with --bulk: where each result's diff is saved")
    ap.add_argument("--json")
    ap.add_argument("--list-kinds", action="store_true")
    ap.add_argument("--bulk", action="store_true", help="every function of --class in build/G4SE01/structural.json")
    ap.add_argument("--class", dest="cls", default="regalloc-only,scheduling,scheduling+regalloc")
    ap.add_argument("--structural", default="build/G4SE01/structural.json")
    ap.add_argument("--units", help="comma-separated parts of unit names")
    ap.add_argument("--min-pct", type=float, default=0)
    ap.add_argument("--min-size", type=int, default=0)
    ap.add_argument("--max-size", type=int, default=1 << 30)
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--offset", type=int, default=0)
    ap.add_argument("--step", type=int, default=1)
    ap.add_argument("--log")
    ap.add_argument("--callees", action="store_true",
                    help="rewrite functions inlined into open functions, scored on the whole unit (all units, or --units)")
    ap.add_argument("--max-sites", type=int, default=10, help="with --callees: inlined functions tried per unit")
    ap.add_argument("--chain", action="store_true",
                    help="with --callees: rewrite the unit's inlined functions together in one search (use with --kinds nest)")
    ap.add_argument("--shared", action="store_true",
                    help="statements a file writes out several times, in one inline helper each, scored on the whole unit")
    ap.add_argument("--parallel", type=int, default=1, help="with --bulk, --callees, --shared: this many units at once")
    ap.add_argument("--only", help=argparse.SUPPRESS)
    ap.add_argument("-v", "--verbose", action="store_true")
    a = ap.parse_args()
    if a.deep:
        a.depth, a.budget = max(a.depth, 14), max(a.budget, 20000)
        a.uphill, a.per_round, a.beam = a.uphill or 40, a.per_round or 600, max(a.beam, 8)
    kinds = [k for k in a.kinds.split(",") if k] if a.kinds else (KINDS_A + KINDS_B if a.tier == "b" else KINDS_A)
    bad = [k for k in kinds if k not in GENERATORS]
    if bad:
        sys.exit(f"unknown kind {bad[0]!r}; kinds: {', '.join(GENERATORS)}")
    if a.shared:
        return shared(a)
    if a.callees:
        return callees(a, kinds if a.kinds or a.tier == "b" else KINDS_A + KINDS_B)
    if a.bulk:
        return bulk(a, kinds)
    if a.list_kinds or not a.unit or not a.function:
        print(__doc__)
        return 0
    unit = scratch.Unit(a.unit)
    try:
        within = None
        if a.lines:
            lo, _, hi = a.lines.partition("-")
            within = (int(lo), int(hi or lo))
        r = search(unit, a.function, kinds, a.depth, a.beam, a.budget, a.verbose, uphill=a.uphill, per_round=a.per_round,
                   within=within)
    except (KeyError, scratch.CompileError) as e:
        sys.exit(str(e))
    finally:
        unit.clean()
    (p0, s0), (p1, s1) = r["base"], r["best"]
    if a.json:
        Path(a.json).write_text(json.dumps({k: v for k, v in r.items() if k != "text"}, indent=1) + "\n")
    out = Path("build/permute")
    out.mkdir(parents=True, exist_ok=True)
    stem = re.sub(r"[^A-Za-z0-9_]", "_", r["fn"])[:80]
    if r["lead"]:
        ld = r["lead"]
        (out / (stem + ".lead.diff")).write_text(ld["diff"])
        print(f"lead (NOT applied: it uses a rewrite that may change behaviour and is below 100 %): {p0:.2f} % -> {ld['pct']:.2f} %")
        for step in ld["trace"]:
            print(f"  - {step}")
        print(f"  diff: {out / (stem + '.lead.diff')}")
    if r["text"] is None:
        print(f"{r['fn']}: {p0:.2f} % (score {s0}, {r['cls']}): no improvement in {r['tried']} candidates, {r['secs']} s"
              + (f" ({r['note']})" if r["note"] else ""))
        return 1
    old = unit.source().encode("utf-8", "surrogateescape")
    patch = diff_text(old, r["text"], unit.src)
    pf = out / (stem + ".diff")
    pf.write_text(patch)
    print(f"{r['fn']}: {p0:.2f} % -> {p1:.2f} % (score {s0} -> {s1}) after {r['tried']} candidates, {r['secs']} s; "
          "nothing else in the unit got worse")
    for step in r["trace"]:
        print(f"  - {step}")
    if any(TIER[k] == "b" for k in r["kinds"]):
        print("  (a tier b rewrite: right because the result is byte-identical; check that it reads naturally, name any new local)")
    if a.apply:
        (unit.root / unit.src).write_bytes(r["text"])
        print(f"applied to {unit.src} (the diff: {pf}); run tools/try.py to confirm")
    else:
        print(patch.rstrip())
        print(f"(not applied; rerun with --apply, or `git apply {pf}`)")
    return 0


if __name__ == "__main__":
    sys.exit(main())

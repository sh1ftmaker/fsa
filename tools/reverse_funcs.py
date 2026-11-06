#!/usr/bin/env python3
"""Reverse the order of top-level function definitions in a C++ source file.

For testing MWCC `-inline auto,deferred`: under deferred, function bodies are
compiled in reverse source order, so a file written in ascending address order
must list its emitted functions in descending order.

Layout produced:
  every non-function item (includes, preprocessor lines, file-scope data,
  declarations, class/struct/enum definitions, `inline`/template function
  definitions -- these emit nothing unless used, and keeping them early keeps
  declaration-before-use) in its original relative order, followed by all
  non-inline top-level function definitions in REVERSE order. Leading
  comments travel with their function. Before the reversed functions, a
  forward declaration is emitted for every free (non-member) function that
  is called before its definition in the new order (static ones keep
  `static`).

A preprocessor conditional group (#if..#endif) at top level is one item; if it
contains a function definition, the whole group moves as a function. Function
bodies inside `extern "C" {}` / namespace blocks are not moved (reported).

Usage: reverse_funcs.py [--check] file.cpp...   (rewrites in place; --check only reports)
"""
import re
import sys


def scan_items(src):
    """Split src into top-level items: (kind, text). kind in {'pp','code','ws'}."""
    i, n = 0, len(src)
    items = []
    cur_start = 0
    depth = 0  # brace depth
    pdepth = 0  # paren depth

    def flush(end, kind='code'):
        nonlocal cur_start
        if end > cur_start:
            items.append((kind, src[cur_start:end]))
        cur_start = end

    at_line_start = True
    while i < n:
        c = src[i]
        if c == '\n':
            at_line_start = True
            i += 1
            continue
        if at_line_start and c in ' \t':
            i += 1
            continue
        if at_line_start and c == '#' and depth == 0 and strip_comments(src[cur_start:i]).strip() == '':
            # preprocessor line (with continuations)
            j = i
            while True:
                e = src.find('\n', j)
                if e == -1:
                    e = n
                    break
                if src[e - 1] == '\\':
                    j = e + 1
                    continue
                break
            # keep leading whitespace/comments with pp line as separate ws item
            flush(i, 'ws')
            items.append(('pp', src[i:e + 1] if e < n else src[i:e]))
            i = e + 1
            cur_start = min(i, n)
            at_line_start = True
            continue
        at_line_start = False
        if src.startswith('//', i):
            e = src.find('\n', i)
            i = n if e == -1 else e
            continue
        if src.startswith('/*', i):
            e = src.find('*/', i + 2)
            i = n if e == -1 else e + 2
            continue
        if c == '"' or c == "'":
            j = i + 1
            while j < n and src[j] != c:
                if src[j] == '\\':
                    j += 1
                j += 1
            i = j + 1
            continue
        if c == '(':
            pdepth += 1
        elif c == ')':
            pdepth -= 1
        elif c == '{':
            depth += 1
        elif c == '}':
            depth -= 1
            if depth == 0 and pdepth == 0:
                # end of a brace block at top level; include a trailing ';' if this is a
                # class/struct/enum or initializer (handled when we see ';')
                k = i + 1
                while k < n and src[k] in ' \t':
                    k += 1
                if k < n and src[k] == ';':
                    i = k
                    flush(i + 1)
                    i += 1
                    continue
                head = src[cur_start:i]
                if not is_aggregate_head(head):
                    flush(i + 1)
        elif c == ';' and depth == 0 and pdepth == 0:
            flush(i + 1)
        i += 1
    flush(n, 'ws' if src[cur_start:].strip() == '' else 'code')
    return items


def strip_comments(s):
    s = re.sub(r'/\*.*?\*/', ' ', s, flags=re.S)
    s = re.sub(r'//[^\n]*', ' ', s)
    return s


def is_aggregate_head(text):
    """True if the block is a class/struct/enum/union/namespace/extern block or
    an initializer (needs to continue to a ';')."""
    t = strip_comments(text)
    pre = t.split('{', 1)[0]
    if '=' in pre and '(' not in pre.split('=')[0]:
        return True
    if re.search(r'\b(class|struct|enum|union)\b', pre) and '(' not in pre:
        return True
    return False


def classify(text):
    """Return 'func', 'inline', 'block' or 'other' for a code item."""
    t = strip_comments(text).strip()
    if not t.endswith('}'):
        return 'other'
    pre = t.split('{', 1)[0]
    if re.match(r'^(namespace\b|extern\s*"C"\s*$)', pre.strip()):
        return 'block'
    if re.search(r'\b(class|struct|enum|union)\b', pre) and '(' not in pre:
        return 'other'
    if '(' not in pre:
        return 'other'
    if re.search(r'\binline\b', pre) or pre.strip().startswith('template'):
        return 'inline'
    return 'func'


def group_items(items):
    """Merge #if..#endif groups and attach leading comments to code items."""
    out = []  # (kind, text) kind in func/other
    i = 0
    pending = ''
    while i < len(items):
        kind, text = items[i]
        if kind == 'pp' and re.match(r'\s*#\s*if', text):
            lvl = 0
            j = i
            buf = ''
            while j < len(items):
                k2, t2 = items[j]
                if k2 == 'pp' and re.match(r'\s*#\s*if', t2):
                    lvl += 1
                elif k2 == 'pp' and re.match(r'\s*#\s*endif', t2):
                    lvl -= 1
                buf += t2
                j += 1
                if lvl == 0:
                    break
            sub = [classify(t) for k, t in items[i:j] if k == 'code']
            gk = 'func' if 'func' in sub else 'other'
            out.append((gk, pending + buf))
            pending = ''
            i = j
            continue
        if kind == 'ws':
            pending += text
        elif kind == 'pp':
            out.append(('other', pending + text))
            pending = ''
        else:
            # a code item may start with comments/blank lines: keep them with it
            ck = classify(text)
            out.append((ck, pending + text))
            pending = ''
        i += 1
    if pending:
        out.append(('other', pending))
    return out


def func_name(text):
    pre = strip_comments(text).split('{', 1)[0]
    # strip ctor initializer list
    m = re.search(r'([~\w:<>,\s]+?)\s*\(', pre)
    if not m:
        return None, pre
    name = m.group(1).split()[-1] if m.group(1).split() else None
    return name, pre


def proto_for(text):
    pre = strip_comments(text).split('{', 1)[0].strip()
    # free functions have no ctor initializer list
    return ' '.join(pre.split()) + ';'


def reverse_source(src):
    items = group_items(scan_items(src))
    funcs = [t for k, t in items if k == 'func']
    others = [t for k, t in items if k != 'func']
    blocks = [t for k, t in items if k == 'block']
    if len(funcs) < 2:
        return None, funcs, blocks
    rev = list(reversed(funcs))
    # forward declarations for free functions used before their definition
    protos = []
    for idx, f in enumerate(rev):
        name, pre = func_name(f)
        if not name or '::' in name or name.startswith('operator'):
            continue
        if f.lstrip().startswith('#'):
            continue
        earlier = '\n'.join(strip_comments(x) for x in rev[:idx])
        if re.search(r'\b' + re.escape(name) + r'\b', earlier):
            protos.append(proto_for(f))
    body = ''.join(others).rstrip('\n') + '\n'
    if protos:
        body += '\n// forward declarations (functions reversed for -inline deferred)\n'
        body += '\n'.join(protos) + '\n'
    for f in rev:
        body += '\n' + f.strip('\n') + '\n'
    return body, funcs, blocks


def main():
    check = '--check' in sys.argv
    files = [a for a in sys.argv[1:] if not a.startswith('--')]
    for path in files:
        src = open(path).read()
        out, funcs, blocks = reverse_source(src)
        note = []
        if blocks:
            note.append(f'{len(blocks)} extern/namespace block(s) not reordered')
        print(f'{path}: {len(funcs)} functions' + (' (unchanged)' if out is None else '') +
              ('; ' + '; '.join(note) if note else ''))
        if out is not None and not check:
            open(path, 'w').write(out)


if __name__ == '__main__':
    main()

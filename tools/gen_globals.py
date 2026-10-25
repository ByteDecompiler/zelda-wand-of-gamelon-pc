import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
VERSION = sys.argv[sys.argv.index("--version") + 1] if "--version" in sys.argv else "en"
OUT = os.path.join(ROOT, "build", "gen", VERSION)
RENAME = {"errno": "game_errno"}

SCALAR = {"char": 1, "short": 2, "long": 4, "int": 4, "float": 4, "double": 8}

def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    return re.sub(r"//[^\n]*", " ", text)

def number(expr, consts):
    expr = expr.strip()
    for k, v in consts.items():
        expr = re.sub(r"\b%s\b" % k, str(v), expr)
    return int(eval(expr.replace("0x", "0x"), {}))

def load_consts():
    consts = {}
    for name in os.listdir(os.path.join(ROOT, "game", "include")):
        text = open(os.path.join(ROOT, "game", "include", name), encoding="utf-8").read()
        for m in re.finditer(r"#define\s+(\w+)\s+(0x[0-9a-fA-F]+|\d+)\b", text):
            consts.setdefault(m.group(1), int(m.group(2), 0))
    return consts

class Type:
    def __init__(self, size, align, scalars):
        self.size, self.align, self.scalars = size, align, scalars

def parse_decl(decl, structs, consts):
    decl = decl.strip()
    if not decl or "(" in decl and "(*" not in decl:
        return None
    decl = re.sub(r"\b(extern|register|static|volatile|const|signed)\b", " ", decl).strip()
    m = re.match(r"(.*?)\(\s*\*\s*(\w+)\s*((?:\[[^\]]*\])*)\s*\)\s*\(.*\)$", decl)
    if m:
        name, dims = m.group(2), m.group(3)
        base = Type(4, 2, [(0, 4)])
    else:
        m = re.match(r"(.*?)([\s*]+)(\w+)\s*((?:\[[^\]]*\])*)$", decl)
        if not m:
            return None
        tname, stars, name, dims = m.group(1).strip(), m.group(2), m.group(3), m.group(4)
        if "*" in stars:
            base = Type(4, 2, [(0, 4)])
        else:
            words = tname.split()
            if words and words[0] == "struct":
                if len(words) < 2 or words[1] not in structs:
                    return (name, None)
                base = structs[words[1]]
            else:
                words = [w for w in words if w != "unsigned"]
                if not words:
                    words = ["int"]
                w = words[-1] if words[-1] != "int" or len(words) == 1 else words[0]
                if w not in SCALAR:
                    return None
                n = SCALAR[w]
                base = Type(n, min(n, 2), [(0, n)] if n in (2, 4) else [])
    count = 1
    open_dim = False
    for d in re.findall(r"\[([^\]]*)\]", dims):
        if d.strip() == "":
            open_dim = True
        else:
            count *= number(d, consts)
    t = Type(base.size * count, base.align,
             [(i * base.size + o, s) for i in range(count) for (o, s) in base.scalars])
    t.elem = base
    t.open = open_dim
    return (name, t)

def parse_struct_body(body, structs, consts):
    off, align, scalars = 0, 1, []
    for decl in body.split(";"):
        decl = decl.strip()
        if not decl:
            continue
        first = parse_decl(decl.split(",")[0], structs, consts)
        parts = [decl.split(",")[0]]
        if "," in decl and "(" not in decl:
            head = re.match(r"((?:unsigned |signed )?(?:struct \w+|\w+)\s*)", decl).group(1)
            parts += [head + p for p in decl.split(",")[1:]]
        for p in parts:
            r = parse_decl(p, structs, consts)
            if r is None or r[1] is None:
                raise ValueError("can't parse field: " + p)
            t = r[1]
            a = t.align
            off = (off + a - 1) // a * a
            scalars += [(off + o, s) for (o, s) in t.scalars]
            off += t.size
            align = max(align, a)
    size = (off + 1) // 2 * 2 if off > 1 else off
    return Type(size, max(align, 2 if size > 1 else 1), scalars)

def main():
    consts = load_consts()
    files = [os.path.join(ROOT, "game", "include", f) for f in sorted(os.listdir(os.path.join(ROOT, "game", "include")))]
    files += [os.path.join(ROOT, "game", "src", f) for f in sorted(os.listdir(os.path.join(ROOT, "game", "src")))]
    texts = [strip_comments(open(f, encoding="utf-8").read()) for f in files]

    structs = {}
    pending = []
    for text in texts:
        for m in re.finditer(r"struct\s+(\w+)\s*\{([^{}]*)\}\s*;", text):
            pending.append((m.group(1), m.group(2)))
    for _ in range(10):
        left = []
        for name, body in pending:
            try:
                structs.setdefault(name, parse_struct_body(body, structs, consts))
            except ValueError:
                left.append((name, body))
        pending = left
    for name, body in pending:
        print("struct not understood:", name, file=sys.stderr)

    types = {}
    for text in texts:
        for line in text.split(";"):
            line = re.sub(r"^.*[{}]", "", line, flags=re.S)
            line = re.sub(r"#[^\n]*", "", line)
            line = re.sub(r"^.*\)\s*\n", "", line, flags=re.S)
            line = " ".join(line.split())
            if "=" in line or not re.match(r"(extern |register |static )?(unsigned |signed )?(char|short|long|int|void|struct)\b", line):
                continue
            try:
                r = parse_decl(line, structs, consts)
            except Exception:
                continue
            if r and r[1] is not None:
                name, t = r
                old = types.get(name)
                if old is None or (old.open and not t.open) or (not old.open and not t.open and t.size > old.size):
                    types[name] = t

    offs = {}
    for path in [os.path.join(ROOT, "config", "symbols.txt")] + \
            ([os.path.join(ROOT, "config", VERSION, "symbols.txt")] if VERSION != "en" else []):
        for line in open(path):
            p = line.split()
            if len(p) >= 3 and p[0] == "data":
                offs[p[1]] = int(p[2], 0)
    syms = list(offs.items())
    below = max(-off for _, off in syms)
    by_off = sorted(set(off for _, off in syms))

    swap = {}
    report = []
    for name, off in syms:
        start = below + off
        nxt = [o for o in by_off if o > off]
        extent = (nxt[0] if nxt else 0) - off
        t = types.get(name)
        if t is None:
            report.append("no type: %-28s -$%04x (%d bytes)" % (name, -off, extent))
            continue
        scal = list(t.scalars)
        if t.open:
            n = max(1, extent // t.elem.size)
            scal = [(i * t.elem.size + o, s) for i in range(n) for (o, s) in t.elem.scalars]
        if not t.open and t.size > extent:
            report.append("bigger than its room: %-18s -$%04x (%d bytes declared, %d before the next global)"
                          % (name, -off, t.size, extent))
        for o, s in scal:
            if o + s > extent:
                continue
            at = start + o
            if at in swap and swap[at] != s:
                report.append("clash at -$%04x: %s wants %d, already %d" % (below - at, name, s, swap[at]))
                continue
            swap[at] = s

    os.makedirs(OUT, exist_ok=True)
    with open(os.path.join(OUT, "a5globals.s"), "w") as f:
        f.write("\t.data\n\t.balign 16\n\t.globl _a5space\n_a5space:\n\t.space %d\n" % (below + 16))
        f.write("\t.globl _a5_below\n\t.section .rdata\n\t.balign 4\n_a5_below:\n\t.long %d\n" % below)
        for name, off in syms:
            name = RENAME.get(name, name)
            f.write("\t.globl _%s\n\t_%s = _a5space + %d\n" % (name, name, below + off))
    with open(os.path.join(OUT, "a5swap.c"), "w") as f:
        f.write("const unsigned short a5swap[][2] = {\n")
        for at in sorted(swap):
            f.write("    {%d, %d},\n" % (at, swap[at]))
        f.write("};\nconst int a5swap_count = %d;\n" % len(swap))
        f.write("const char game_version[] = \"%s\";\n" % VERSION)
    with open(os.path.join(OUT, "a5report.txt"), "w") as f:
        f.write("\n".join(report) + "\n")
    print("%d globals, %d bytes below A5, %d values to swap, %d notes (build/gen/%s/a5report.txt)"
          % (len(syms), below, len(swap), len(report), VERSION))

if __name__ == "__main__":
    main()

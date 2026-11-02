# Build refs.json: for each function (text) the list of referenced symbol addresses; for each data symbol the refs it contains.
import os,re,json
from common import *
syms=load_syms()
byname={s['name']:s for s in syms}
ASM=ROOT+'/build/G4SE01/asm'
files=[]
for dp,dn,fn in os.walk(ASM):
    for f in fn:
        if f.endswith('.s'): files.append(os.path.join(dp,f))
tok=re.compile(r'[A-Za-z_$@.?][\w$@.?<>,:\-]*')
fnrefs={}; datarefs={}
cur=None;kind=None
for path in files:
    for l in open(path,errors='replace'):
        if l.startswith('.fn '):
            cur=l.split()[1].rstrip(','); kind='fn'; fnrefs.setdefault(cur,[]); continue
        if l.startswith('.obj '):
            cur=l.split()[1].rstrip(','); kind='obj'; datarefs.setdefault(cur,[]); continue
        if l.startswith('.endfn') or l.startswith('.endobj'): cur=None; continue
        if cur is None or l.startswith('#') : continue
        body=l.split('*/',1)[-1] if '*/' in l else l
        for m in re.finditer(r'\b((?:lbl|fn|jumptable|gap)_[0-9A-Fa-f_]+|[A-Za-z_@][\w@$]*)(?=(@sda21|@ha|@l|@h|\s*$|,|\)|\s*\+))',body):
            n=m.group(1)
            if n in byname and n!=cur:
                (fnrefs if kind=='fn' else datarefs)[cur].append(byname[n]['addr'])
json.dump(dict(fn=fnrefs,data=datarefs),open('refs.json','w'))
print(len(fnrefs),len(datarefs),sum(len(v) for v in fnrefs.values()))

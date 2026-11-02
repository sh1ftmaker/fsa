import re, struct
import os
ROOT=os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
DOL=open(ROOT+'/orig/sys/main.dol','rb').read()
_off=struct.unpack('>18I',DOL[0:72]);_addr=struct.unpack('>18I',DOL[72:144]);_sz=struct.unpack('>18I',DOL[144:216])
SECS=[(_addr[i],_addr[i]+_sz[i],_off[i]) for i in range(18) if _sz[i]]
def rd(a,n):
    for s,e,o in SECS:
        if s<=a and a+n<=e: return DOL[o+a-s:o+a-s+n]
    return None
def u32(a): return struct.unpack('>I',rd(a,4))[0]
def load_syms():
    syms=[]
    for l in open(ROOT+'/config/G4SE01/symbols.txt'):
        m=re.match(r'(\S+) = (\S+):(0x\w+); // (.*)',l)
        if not m: continue
        attrs=dict(x.split(':',1) for x in m[4].split() if ':' in x)
        syms.append(dict(name=m[1],sec=m[2],addr=int(m[3],16),size=int(attrs.get('size','0'),16),type=attrs.get('type'),scope=attrs.get('scope'),attrs=m[4]))
    return syms
def load_splits():
    out=[];cur=None
    for l in open(ROOT+'/config/G4SE01/splits.txt'):
        if l and not l[0].isspace() and l.strip().endswith(':') and not l.startswith('Sections'):
            cur=l.strip()[:-1]; continue
        m=re.match(r'\s+(\S+)\s+start:(0x\w+) end:(0x\w+)',l)
        if m and cur: out.append((cur,m[1],int(m[2],16),int(m[3],16)))
    return out

from common import *
import json
syms=load_syms()
R=json.load(open('refs.json'))
fns=sorted([s for s in syms if s['sec']=='.text' and s['type']=='function'],key=lambda s:s['addr'])
SEC={'.rodata':(0x8045c660,0x804916a0),'.data':(0x804916a0,0x804b2140),'.bss':(0x804b2140,0x80539bc0),
     '.sdata':(0x80539bc0,0x8053a280),'.sbss':(0x8053a280,0x8053afa0),'.sdata2':(0x8053afa0,0x80543f00),'.sbss2':(0x80543f00,0x80544000),'.text':(0x80021840,0x8045bd00)}
def secof(a):
    for k,(s,e) in SEC.items():
        if s<=a<e: return k
symaddr={s['addr']:s for s in syms}
for f in fns: f['refs']=R['fn'].get(f['name'],[])
GAME0=0x800F39EC

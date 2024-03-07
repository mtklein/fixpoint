#include "fixpoint.h"
#include <stdlib.h>

union Val {
    float f;
    int   i;
};

struct Inst {
    void (*fn)(struct Inst const *ip, union Val *v, int i, void* ptr[]);
    int imm,x,y,z;
};

struct Builder {
    int insts, unused;
    struct Inst *inst;
};

static _Bool pow2_or_zero(int n) {
    return (n & (n-1)) == 0;
}

static int push(struct Builder *b,
                void (*fn)(struct Inst const *ip, union Val *v, int i, void* ptr[]),
                int imm, int x, int y, int z) {
    if (pow2_or_zero(b->insts)) {
        b->inst = realloc(b->inst, b->insts ? 2*(size_t)b->insts : 1);
    }
    b->inst[b->insts] = (struct Inst) {fn,imm,x,y,z};
    return  b->insts++;
}

#define next ip[1].fn(ip+1,v+1,i,ptr); return
#define defn(name) static void name##_(struct Inst const *ip, union Val *v, int i, void* ptr[])

defn(load) {
    float const *p = ptr[ip->imm];
    v->f = p[i];
    next;
}
int load(struct Builder *b, int ptr) { return push(b, load_, ptr,0,0,0); }

defn(splat) {
    v->i = ip->imm;
    next;
}
int splat(struct Builder *b, unsigned bits) { return push(b, splat_, (int)bits,0,0,0); }

defn(store) {
    float *p = ptr[ip->imm];
    p[i] = v[ip->x].f;
    next;
}
void store(struct Builder *b, int ptr, int val) { (void)push(b, store_, ptr,val,0,0); }

defn(fadd) { v->f = v[ip->x].f + v[ip->y].f; next; }
defn(fsub) { v->f = v[ip->x].f - v[ip->y].f; next; }
defn(fmul) { v->f = v[ip->x].f * v[ip->y].f; next; }
defn(fdiv) { v->f = v[ip->x].f / v[ip->y].f; next; }

defn(iadd) { v->i = v[ip->x].i + v[ip->y].i; next; }
defn(isub) { v->i = v[ip->x].i - v[ip->y].i; next; }
defn(imul) { v->i = v[ip->x].i * v[ip->y].i; next; }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
defn(feq) { v->i = v[ip->x].f == v[ip->y].f ? ~0 : 0; next; }
defn(fne) { v->i = v[ip->x].f != v[ip->y].f ? ~0 : 0; next; }
#pragma GCC diagnostic pop

defn(flt) { v->i = v[ip->x].f <  v[ip->y].f ? ~0 : 0; next; }
defn(fle) { v->i = v[ip->x].f <= v[ip->y].f ? ~0 : 0; next; }
defn(fgt) { v->i = v[ip->x].f >  v[ip->y].f ? ~0 : 0; next; }
defn(fge) { v->i = v[ip->x].f >= v[ip->y].f ? ~0 : 0; next; }

defn(ieq) { v->i = v[ip->x].i == v[ip->y].i ? ~0 : 0; next; }
defn(ine) { v->i = v[ip->x].i != v[ip->y].i ? ~0 : 0; next; }
defn(ilt) { v->i = v[ip->x].i <  v[ip->y].i ? ~0 : 0; next; }
defn(ile) { v->i = v[ip->x].i <= v[ip->y].i ? ~0 : 0; next; }
defn(igt) { v->i = v[ip->x].i >  v[ip->y].i ? ~0 : 0; next; }
defn(ige) { v->i = v[ip->x].i >= v[ip->y].i ? ~0 : 0; next; }

defn(band) { v->i = v[ip->x].i & v[ip->y].i; next; }
defn(bor ) { v->i = v[ip->x].i | v[ip->y].i; next; }
defn(bxor) { v->i = v[ip->x].i ^ v[ip->y].i; next; }

int fadd(struct Builder *b, int x, int y) { return push(b, fadd_, 0,x,y,0); }
int fsub(struct Builder *b, int x, int y) { return push(b, fsub_, 0,x,y,0); }
int fmul(struct Builder *b, int x, int y) { return push(b, fmul_, 0,x,y,0); }
int fdiv(struct Builder *b, int x, int y) { return push(b, fdiv_, 0,x,y,0); }

int iadd(struct Builder *b, int x, int y) { return push(b, iadd_, 0,x,y,0); }
int isub(struct Builder *b, int x, int y) { return push(b, isub_, 0,x,y,0); }
int imul(struct Builder *b, int x, int y) { return push(b, imul_, 0,x,y,0); }

int feq(struct Builder *b, int x, int y) { return push(b, feq_, 0,x,y,0); }
int fne(struct Builder *b, int x, int y) { return push(b, fne_, 0,x,y,0); }
int flt(struct Builder *b, int x, int y) { return push(b, flt_, 0,x,y,0); }
int fle(struct Builder *b, int x, int y) { return push(b, fle_, 0,x,y,0); }
int fgt(struct Builder *b, int x, int y) { return push(b, fgt_, 0,x,y,0); }
int fge(struct Builder *b, int x, int y) { return push(b, fge_, 0,x,y,0); }

int ieq(struct Builder *b, int x, int y) { return push(b, ieq_, 0,x,y,0); }
int ine(struct Builder *b, int x, int y) { return push(b, ine_, 0,x,y,0); }
int ilt(struct Builder *b, int x, int y) { return push(b, ilt_, 0,x,y,0); }
int ile(struct Builder *b, int x, int y) { return push(b, ile_, 0,x,y,0); }
int igt(struct Builder *b, int x, int y) { return push(b, igt_, 0,x,y,0); }
int ige(struct Builder *b, int x, int y) { return push(b, ige_, 0,x,y,0); }

int band(struct Builder *b, int x, int y) { return push(b, band_, 0,x,y,0); }
int bor (struct Builder *b, int x, int y) { return push(b, bor_ , 0,x,y,0); }
int bxor(struct Builder *b, int x, int y) { return push(b, bxor_, 0,x,y,0); }

struct Program {
    int insts, unused;
    struct Inst inst[];
};

static void done_(struct Inst const *ip, union Val *v, int i, void* ptr[]) {
    (void)ip;
    (void)v;
    (void)i;
    (void)ptr;
}

struct Program* compile(struct Builder *b) {
    push(b, done_, 0,0,0,0);
    struct Program *p = malloc(sizeof *p + (size_t)b->insts * sizeof *p->inst);
    for (int i = 0; i < b->insts; i++) {
        int id = p->insts++;
        p->inst[id] = (struct Inst) {
            .fn  = b->inst[i].fn,
            .imm = b->inst[i].imm,
            .x   = b->inst[i].x - id,
            .y   = b->inst[i].y - id,
            .z   = b->inst[i].z - id,
        };
    }
    free(b);
    return p;
}

void run(struct Program const *p, int const n, void* ptr[]) {
    struct Inst const *start = p->inst;

    union Val *v = calloc((size_t)p->insts, sizeof *v);
    for (int i = 0; i < n; i++) {
        start->fn(start,v,i,ptr);
    }
    free(v);
}

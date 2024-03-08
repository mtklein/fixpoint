#pragma once

struct Builder* builder(void);
struct Program* compile(struct Builder*);
void                run(struct Program const *, int n, void* ptr[]);

int   load(struct Builder*, int ptr);
int  splat(struct Builder*, unsigned bits);
void store(struct Builder*, int ptr, int val);

int fadd(struct Builder*, int,int);
int fsub(struct Builder*, int,int);
int fmul(struct Builder*, int,int);
int fdiv(struct Builder*, int,int);

int iadd(struct Builder*, int,int);
int isub(struct Builder*, int,int);
int imul(struct Builder*, int,int);

int feq(struct Builder*, int,int);
int fne(struct Builder*, int,int);
int flt(struct Builder*, int,int);
int fle(struct Builder*, int,int);
int fgt(struct Builder*, int,int);
int fge(struct Builder*, int,int);

int ieq(struct Builder*, int,int);
int ine(struct Builder*, int,int);
int ilt(struct Builder*, int,int);
int ile(struct Builder*, int,int);
int igt(struct Builder*, int,int);
int ige(struct Builder*, int,int);

int band(struct Builder*, int,int);
int bor (struct Builder*, int,int);
int bxor(struct Builder*, int,int);
int bsel(struct Builder*, int cond, int t, int f);

int loop(struct Builder*, int,int);

#include "fixpoint.h"
#include <stdio.h>
#include <stdlib.h>

#define len(arr) (int)(sizeof(arr) / sizeof(0[arr]))

#define expect(x) \
    if(!(x)) dprintf(2, "%s:%d expect(%s)\n failed", __FILE__, __LINE__, #x), __builtin_trap()

static _Bool equiv(float x, float y) {
    return (x <= y && y <= x)
        || (x != x && y != y);
}

static void test_square(void) {
    struct Builder *b = builder();
    {
        int x = load(b, 0);
        store(b, 1, fmul(b, x,x));
    }
    struct Program *p = compile(b);

    float src[] = {1,2,3,4,5,6,7,8},
          dst[len(src)] = {0};
    run(p, len(src), (void*[]){src,dst});

    for (int i = 0; i < len(src); i++) {
        expect(equiv(dst[i], src[i]*src[i]));
    }
    free(p);
}

static void test_fix(void) {
    struct Builder *b = builder();
    {
        int x = load(b,0);
        int y = bsel(b, ilt(b,x,splat(b,10))
                      , iadd(b,x,splat(b,2))
                      , x);
        store(b,0,fix(b,y));
    }
    struct Program *p = compile(b);

    int v[] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
    run(p, len(v), (void*[]){v});

    for (int i = 0; i < len(v); i++) {
        int want = (i >= 10) ? i
                 : (i & 1)   ? 11
                 : 10;
    #if 1
        printf("%2d %2d %2d %s\n", i, v[i], want, v[i] == want ? "✅" : "❌");
    #else
        expect(v[i] == want);
    #endif
    }
    free(p);
}


int main(void) {
    test_square();
    test_fix();
    return 0;
}

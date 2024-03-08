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

static void test_loop(void) {
    struct Builder *b = builder();
    {
        store(b,1, load(b,0));

        int x    = load(b,1),
            done = ige(b, x, splat(b,10)),
            y    = bsel(b, done
                         , x
                         , iadd(b, x, splat(b,2)));
        store(b,1,y);

        store(b,0, loop(b,x,done));
    }
    struct Program *p = compile(b);

    int v[] = {0,1,2,3,4,5,6,7,8,9,10,11,12},
        tmp[len(v)] = {0};
    run(p, len(v), (void*[]){v,tmp});

    for (int i = 0; i < len(v); i++) {
        int want = (i >= 10) ? i
                 : (i & 1)   ? 11
                 : 10;
        expect(v[i] == want);
    }
    free(p);
}


int main(void) {
    test_square();
    test_loop();
    return 0;
}

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


int main(void) {
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
    return 0;
}

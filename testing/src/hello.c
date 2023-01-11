#include <stdio.h>
#include "hello.h"

void cool_function(int i, char c, CoolStruct* cs) {
    printf("%i,%c,(%i, %i)", i, c, cs->x, cs->y);
}

void hello_from_c() {
    printf("Hello from C!\n");
}
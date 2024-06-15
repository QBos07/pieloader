#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
void _ATTRIBUTE ((__noreturn__)) __assert_func (const char * file, int line, const char * function, const char * expression) {
    printf("Assertion failed: %s, file %s, line %d, function %s\n", expression, file, line, function);
    fflush(stdout);
    abort();
}
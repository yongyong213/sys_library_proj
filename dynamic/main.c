#include <stdlib.h>
#include <stdio.h>
#include "mystring.h"

int main() {
    char* test = str_reverse("Hello, World!");

    printf("결과: %s\n", test);

    free(test);

    return 0;
}
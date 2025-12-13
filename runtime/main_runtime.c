#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    void *handle;
    char *(*str_reverse)(const char*);
    char *error;

    handle = dlopen("./libmystring.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    str_reverse = dlsym(handle, "str_reverse");

    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    char *result = str_reverse("Hello World!");
    printf("Reversed: %s\n", result);

    if (result != NULL) free(result);

    if (dlclose(handle) < 0) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    return 0;
}
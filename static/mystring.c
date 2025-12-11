#include "mystring.h"
#include <string.h>
#include <stdlib.h>

char* str_reverse(const char* str){
    if(str == NULL) return NULL;

    int length = strlen(str);

    if(length == 0){
        char* empty_str = (char*)malloc(1);
        empty_str[0] = '\0';
        return empty_str;
    }

    char* new_str = (char*)malloc(sizeof(char) * (length + 1));

    strcpy(new_str, str);

    for(int i = 0; i<length/2; i++){
        char tmp = new_str[i];
        new_str[i] = new_str[length - i - 1];
        new_str[length - i - 1] = tmp;
    }
    return new_str;
}
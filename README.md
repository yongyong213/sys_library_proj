# sys_library_proj

Str_reverse
본 문서는 Static Linking, Dynamic Linking, Runtime Linking에 관한 구현 내용을 포함합니다.

C언어로 스트링을 받으면 순서를 뒤집어 출력하는 함수를 디자인하고 링킹하는 과정을 설명합니다. ( "Hello World!" -> "!dlroW olleH" )

☘️ 리포지토리 구조
<img width="845" height="717" alt="image" src="https://github.com/user-attachments/assets/c5ca6fb2-f67a-4262-be6a-4099d4d7c0a8" />


🐿️ 코드 설명
mystring.h
헤더 파일은 char *str_reverse(); 함수 프로토타입을 선언했습니다.

#ifndef MYSTRING_H
#define MYSTRING_H

char *str_reverse(const char *str);

#endif
mystring.c
다음 c파일은 헤더 파일을 포함하고, 함수 본문을 구현합니다.
length에 받은 str의 길이를 체크하여 0일 때 예외 처리를 하였습니다.
0이 아니라면 복사한 뒤 i = 0번째부터 length/2번째까지 반복하며 반대편 length - i - 1과 swap 하여 결과 값을 구하고 해당 값을 반환해줍니다

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
main.c
라이브러리를 사용하는 메인 프로그램입니다.
"Hello World!"를 예시로 결과 값을 출력합니다.

#include <stdlib.h>
#include <stdio.h>
#include "mystring.h"

int main() {
    char* test = str_reverse("Hello, World!");

    printf("결과: %s\n", test);

    free(test);

    return 0;
}
main_runtime.c
Runtime Linking에서 사용되는 main 코드입니다.
dl 명령어를 사용해 런타임에 동적으로 라이브러리를 로드하고 함수를 호출합니다.

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
Static Linking
해당 과정 실행은 static 폴더에서 이루어집니다.

객체 파일 생성
mystring.c 파일을 gcc를 통해 컴파일하여 객체 파일 mystring.o를 생성합니다.
$ gcc -Og -c mystring.c -o mystring.o

아카이브 파일 생성
정적 라이브러리를 생성하는 ar -rcs 명령어를 입력합니다.
ar rcs libmystring.a mystring.o

컴파일
정적 라이브러리를 사용해서 실행 파일인 program_static을 생성합니다.
$ gcc -static -o program_static main.c -L. -lmystring

main_static 파일을 실행합니다.

<img width="1129" height="349" alt="image" src="https://github.com/user-attachments/assets/d6977a6f-efcd-4c1e-81f2-9f7fe97d1235" />




Dynamic Linking
해당 과정 실행은 dynamic 폴더에서 이루어집니다.

객체 파일 생성
mystring.c 파일을 위치독립코드(pic)로 컴파일하여 객체 파일 mystring.o를 생성합니다.
$ gcc -Og -c -fPIC mystring.c -o mystring.o

동적 라이브러리를 생성하는 코드를 입력합니다.
$ gcc -shared -o libstring.so mystring.o

현재 디렉토리의 동적 라이브러리를 링크하고, 메인 파일을 컴파일합니다.
$ gcc -o program_dynamic main.c -L. -lmystring

현재 디렉토리를 동적 라이브러리 검색 경로로 만들어 환경변수를 설정합니다.
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

main_dynamic 파일을 실행합니다.

ldd로 동적 컴파일 제대로 실행됐는지 확인합니다.
ldd program_dynamic

<img width="958" height="451" alt="image" src="https://github.com/user-attachments/assets/cc483e1a-5bdc-449d-8525-6be09de60e67" />



Runtime Linking
해당 과정 실행은 runtime 폴더에서 이루어집니다.

객체 파일 생성
mystring.c 파일을 위치독립코드(pic)로 컴파일하여 객체 파일 mystring.o를 생성합니다.
$ gcc -Og -c -fPIC mystring.c -o mystring.o

동적 라이브러리를 생성하는 코드를 입력합니다.
$ gcc -shared -o libstring.so mystring.o

런타임 링크를 사용하도록 수정된 main_runtime.c를 컴파일합니다.
$ gcc -o program_runtime main_runtime.c -ldl

main_runtime 파일을 실행합니다.

<img width="952" height="291" alt="image" src="https://github.com/user-attachments/assets/cb8d6b21-ccac-4942-911e-d36e55fce1fd" />



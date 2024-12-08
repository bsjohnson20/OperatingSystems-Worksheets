#include <stdio.h>

char test(){
    return 'c';
}

void main(){
    char t;
    t = test();
    printf("%c\n", t);
}
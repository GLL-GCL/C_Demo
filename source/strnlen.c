#include<stdio.h>
#include<string.h>

void strCharArr(){
    char str1[8] = "opqrstuv";
    char str[5] = {'a','b','c','d','e','f'};
    printf("str = %s\n", str);

    int len = strnlen(str, 10);
    
    printf("strlen(%s) = %d\n", str,strlen(str));
    printf("len = %d\n", len);
    int i = 0;
    for (; i < len; i++){
        printf("str[%d] = %c\n", i,str[i]);
    }
    printf("str[%d] = %c\n", i,str[i]);

}

void strString(){
    printf("=====================string=======================\n");
    char str1[8] = "ghijklmn";
    char str[5] = "abcdef";
    printf("str = %s\n", str);

    int len = strnlen(str, 10);
    
    printf("strlen(%s) = %d\n", str,strlen(str));
    printf("len = %d\n", len);
    int i = 0;
    for (; i < len; i++){
        printf("str[%d] = %c\n", i,str[i]);
    }
    printf("str[%d] = %c\n", i,str[i]);
}


void main(){
    strCharArr();
    strString();
}
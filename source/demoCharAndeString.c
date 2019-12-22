#include<stdio.h>
#include<string.h>

int charPointerAndString(){
    // char* ip = "192.254.1.16";
    char* ip[10] = {'192','.','168','.','50','.','130'};

    printf("ip = %s\n", ip);
    int len = strlen(ip);
    printf("len = %d\n", len);
    for(int i = 0; i < len; i++){
        printf("%c\n", ip[i]);
    }

    return 0;
}

int main(){
    if(charPointerAndString() != 0){
        printf("%s at %d : charPointerAndString fail !!!\n", __FUNCTION__,__LINE__);
        return -1;
    }
    return 0;
}
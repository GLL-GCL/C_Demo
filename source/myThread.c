#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void* myfunc1(void* args){
    int i = 0;
    while(1){
        printf("%s : %d\n", (char*)args,i++);
    }
    return NULL;
}
void* myfunc2(void* args){
    int i = 0;
    while(1){
        printf("%s : %d\n", (char*)args,i++);
    }
    return NULL;
}

void main(){

    // 表示th是一条新的线程，此时还没有创建线程，
    // 只是定义了一个变量，如同int a，还有没给a复制。
    pthread_t th1;
    pthread_create(&th1, NULL, myfunc1, "th1     ");
    pthread_t th2;
    pthread_create(&th2, NULL, myfunc2, "th2");

    printf("at main\n");
    
    // 注意，这里th不需要带&
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
}
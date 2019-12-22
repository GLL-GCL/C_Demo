#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 


pthread_t tid;
int creatTask(){
    pthread_create(&tid, NULL, tfn, (void *)i);
}


int main(int argc, char *argv[])
{
	
	
	return 0;
}

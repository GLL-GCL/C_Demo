#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// #include <loglib.h>
#include <stddef.h>
#include <time.h>
// #include <tasklib.h>
 
LOCAL int tidThis;
LOCAL int tidThat;
 
#define STACK_SIZE 1000
 
void* thisRoutine(void* args)
{
	int ii = 0;
	for (ii = 0; ii < 100; ii++)
	{
		printf("task 1\n");
		sleep(1);
	}
}
 
void* thatRoutine(void* args)
{
	int ii = 0;
	for (ii = 0; ii < 100; ii++)
	{
		printf("task 2\n");
		sleep(1);
	}
}
 
STATUS func(void)
{
	tidThis = taskSpawn("taskThis", 200, 0, STACK_SIZE, (FUNCPTR)thisRoutine, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	tidThat = taskSpawn("taskThat", 220, 0, STACK_SIZE, (FUNCPTR)thatRoutine, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	if (tidThis == ERROR || tidThat == ERROR)
	{
		return ERROR;
	}
	
	return OK;
}
 
 
STATUS main(int argc, char* argv[])
{
	int iRet;
	int ii = 0;
 
	printf("welcome to workbench for vxworks6.6\n");
	
	iRet = func();
	if (iRet == ERROR)
	{
		return ERROR;
	}
	
	for (ii = 0; ii < 10; ii++)
	{
		printf("main task\n");
		sleep(1);
	}
	
	taskDelete(tidThis);
	taskDelete(tidThat);
 
	return 0;
}

#include "includes.h"
#include "system_stm32f10x.h"
#include "string.h"
#define SIZE 100

OS_STK task1_STK[SIZE] ;
OS_STK task2_STK[SIZE] ;
OS_STK task3_STK[SIZE] ;

void task1(void *arg);
void task2(void *arg);
void task3(void *arg);

OS_EVENT *mutex1;

int main(void)
{
	INT8U ret, err;

	OSInit();



	ret = OSTaskCreate(&task3, (void *)0, &task3_STK[SIZE-1], 1);
				if(ret != OS_ERR_NONE)
				printf("error in creating task3 \n\r");

	ret = OSTaskCreate(&task1, (void *)0, &task1_STK[SIZE-1], 3);
		if(ret != OS_ERR_NONE)
		printf("error in creating task1 \n\r");

	ret = OSTaskCreate(&task2, (void *)0, &task2_STK[SIZE-1], 2);
			if(ret != OS_ERR_NONE)
			printf("error in creating task2 \n\r");
	mutex1 = OSMutexCreate(10, &err) ;
	if(mutex1 == (void *)0)
		printf("Error creating Mutex \n\r");

	OSStart();
	while(1)
	{
		printf("hello in while \n\r");
	}
}

void task3(void* arg)
{
	INT8U err ;
	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC) ;
	printf("suspending prio 1 \n\r") ;
	OSTaskSuspend(OS_PRIO_SELF) ;
	printf("prio1 resumed \n\r");
	for(; ;)
	{
		printf("highest prio:: \n\r");
		OSMutexPend(mutex1, 1000, &err) ;
		printf("semaphore acquired by highest \n\r");
		OSTimeDly(10*OS_TICKS_PER_SEC);
		printf("Highest prio released semaphore\n\r");

		OSMutexPost(mutex1);

		OSTimeDly(0.1*OS_TICKS_PER_SEC);
	}
}


void task1(void* arg)
{
	INT8U err;
	while(1)
	{
		printf("In lowest prio: \n\r");
		OSMutexPend(mutex1, 100000, &err) ;
		printf("sema acq by lowest \n\r") ;
		OSTimeDly(10*OS_TICKS_PER_SEC) ;
		printf("Lowest released semaphore\n\r");
		OSMutexPost(mutex1);

	}
}


void task2(void* arg)
{
	while(1)
	{
			OSTimeDly(0.1*OS_TICKS_PER_SEC) ;
			printf("In medium prio task: \n\r");
			OSTaskResume(1) ;
			printf("Deleting prio 2\n\r");
			OSTaskDel(OS_PRIO_SELF) ;
	}
}

#include "includes.h"
#include "system_stm32f10x.h"

#define SIZE 100

OS_STK highest_prio_stk[SIZE];
OS_STK medium_prio_stk[SIZE];
OS_STK lowest_prio_stk[SIZE];
OS_EVENT *sem;

void highest_prio(void *);
void medium_prio(void *);
void lowest_prio(void *);

int main(void)
{
	INT8U ret;
	OSInit();

	ret = OSTaskCreate(&highest_prio, (void *)0, &highest_prio_stk[SIZE-1], 0);
	if(ret)
		printf("Highest priority task creation failed\n\r");
	printf("Highest prio task created\n\r");

	ret = OSTaskCreate(&medium_prio, (void *)0, &medium_prio_stk[SIZE-1], 1);
	if(ret)
		printf("Medium priority task creation failed\n\r");
	printf("Medium prio task created\n\r");

	ret = OSTaskCreate(&lowest_prio, (void *)0, &lowest_prio_stk[SIZE-1], 2);
	if(ret)
		printf("Lowest priority task creation failed\n\r");
	printf("Lowest prio task created\n\r");

	sem = OSSemCreate(1);
	if(sem == (void *)0)
		printf("Semaphore cannot be created\n\r");
	printf("Semaphore created\n\r");

	OSStart();

	while(1)
    {
    }
}

void highest_prio(void *arg){
	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
	INT8U err;
	printf("suspending highest prio task\n\r");
	OSTaskSuspend(OS_PRIO_SELF);
	printf("High Prio waiting for semaphore\n\r");
	OSSemPend(sem, 1000, &err);
	printf("High prio got semaphore\n\r");
	for(;;){
		printf("highest prio task for loop\n\r");
		OSTimeDly(1*OS_TICKS_PER_SEC);
	}
}
void medium_prio(void *arg){
	OSTimeDly(0.1*OS_TICKS_PER_SEC);
	printf("Medium task resumed high prio task\n\r");
	OSTaskResume(0);

	for(;;){
		printf("In medium priority task for loop\n\r");
		OSTaskDel(OS_PRIO_SELF);
	}
}
void lowest_prio(void *arg){
	INT8U err, i = 0;
	OSSemPend(sem, 200, &err);
	printf("Lowest prio task got semaphore\n\r");
	for(;;){
		printf("Lowest prio task for loop\n\r");
		OSTimeDly(1*OS_TICKS_PER_SEC);
		i++;
		if(i == 10){
			printf("Lowest prio released semaphore\n\r");
			OSSemPost(sem);
			OSTaskDel(OS_PRIO_SELF);
		}
	}
}

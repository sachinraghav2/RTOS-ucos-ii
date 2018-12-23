#include "includes.h"
#include "system_stm32f10x.h"
//#include "stdio.h"
#define STACK_SIZE 100

OS_STK task_stk1[STACK_SIZE];
OS_STK task_stk2[STACK_SIZE];
OS_STK task_stk3[STACK_SIZE];
OS_STK task_stk4[STACK_SIZE];
OS_STK task_stk5[STACK_SIZE];


void task1(void *arg);
void task2(void *arg);
void task3(void *arg);
void task4(void *arg);
void task5(void *arg);
void *tasks[6] = {NULL, NULL, &task2, &task3, &task4, &task5};
void *stacks[6] = {NULL, NULL, &task_stk2[STACK_SIZE-1], &task_stk3[STACK_SIZE-1], &task_stk4[STACK_SIZE-1], &task_stk5[STACK_SIZE-1]};
int main(void)
{
	INT8U ret;//,ret1,ret2,ret3,ret4;
	OSInit();
	ret=OSTaskCreate(&task1,(void *)0,&task_stk1[STACK_SIZE-1],0);
	if(ret!=OS_ERR_NONE)
		printf("task1 creation failed..\n\r");
	printf("First periodic task created.\n\r");

	OSStart();
	while(1){

	}

}

void task1(void *arg)
{
	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
	int i, ret;
	for(i=2;i<5;i++)
	{
		ret=OSTaskCreate(tasks[i],(void *)0,(OS_STK)stacks[i],i);
			if(ret!=OS_ERR_NONE)
				printf("task%d creation failed..\n\r", i);
		printf("In task1:task%d created \n\r",i);
		OSTimeDly(1*OS_TICKS_PER_SEC);
	}
	printf("All tasks created\n\r");
	for(;;)
	{
		printf("Task1 Idle\n\r");
		OSTimeDly(1*OS_TICKS_PER_SEC);
	}
}

void task2(void *arg)
{
//	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
		printf("in task2 \n\r");
		OSTimeDly(1*OS_TICKS_PER_SEC);
		OSTaskDelReq(OS_PRIO_SELF);
}

void task3(void *arg)
{
//	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
		printf("in task3 \n\r");
		OSTimeDly(1*OS_TICKS_PER_SEC);
		OSTaskDelReq(OS_PRIO_SELF);
}

void task4(void *arg)
{
//	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
		printf("in task4 \n\r");
		OSTimeDly(1*OS_TICKS_PER_SEC);
		OSTaskDelReq(OS_PRIO_SELF);
}

void task5(void *arg)
{
//	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
	for(;;)
	{
		printf("in task5:task \n\r");
		OSTimeDly(1*OS_TICKS_PER_SEC);
	}
}

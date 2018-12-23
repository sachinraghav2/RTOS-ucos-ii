#include "includes.h"
#include "system_stm32f10x.h"
//#include "stdio.h"
#define STACK_SIZE 100

OS_STK task_stk1[STACK_SIZE];
OS_STK task_stk2[STACK_SIZE];


void task1(void *arg);
void task2(void *arg);
int main(void)
{
	INT8U ret;//,ret1,ret2,ret3,ret4;
	OSInit();
	ret=OSTaskCreate(&task1,(void *)0,&task_stk1[STACK_SIZE-1],0);
	if(ret!=OS_ERR_NONE)
		printf("task1 creation failed..\n\r");
	printf("First periodic task created.\n\r");
	ret=OSTaskCreate(&task2,(void *)0,&task_stk2[STACK_SIZE-1],1);
	if(ret!=OS_ERR_NONE)
			printf("task1 creation failed..\n\r");
	printf("Second periodic task created.\n\r");
	OSStart();
	while(1){

	}

}

void task1(void *arg)
{
	int i;
	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
	for(i=0;i<3;i++){
		printf("In task1 for %d th time\n\r", i);
		OSTimeDly(1*OS_TICKS_PER_SEC);
	}
	OSTaskDel(OS_PRIO_SELF);
}

void task2(void *arg)
{
	int i;
//	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
	for(i=0;i<3;i++){
		printf("In task2 for %d th time\n\r", i);
		OSTimeDly(1*OS_TICKS_PER_SEC);
	}
	OSTaskDel(OS_PRIO_SELF);
}

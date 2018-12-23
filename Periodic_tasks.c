#include "includes.h"
#include "system_stm32f10x.h"
//#include "stdio.h"
#define STACK_SIZE 100

OS_STK task_stk1[STACK_SIZE];
OS_STK task_stk2[STACK_SIZE];
OS_STK task_stk3[STACK_SIZE];


void task1(void *arg);
void task2(void *arg);
void task3(void *arg);
int main(void)
{
	INT8U ret;//,ret1,ret2,ret3,ret4;
	OSInit();
	ret=OSTaskCreate(&task1,(void *)0,&task_stk1[STACK_SIZE-1],0);
	if(ret!=OS_ERR_NONE)
		printf("task1 creation failed..\n\r");
	printf("First Master periodic task created.\n\r");
	ret=OSTaskCreate(&task2,(void *)0,&task_stk2[STACK_SIZE-1],1);
	if(ret!=OS_ERR_NONE)
			printf("task2 creation failed..\n\r");
	printf("First Slave periodic task created.\n\r");
	ret=OSTaskCreate(&task3,(void *)0,&task_stk3[STACK_SIZE-1],2);
		if(ret!=OS_ERR_NONE)
				printf("task2 creation failed..\n\r");
		printf("Second Slave periodic task created.\n\r");
	OSStart();
	while(1){

	}

}

void task1(void *arg)
{
	int i=0, j=0;
	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
	for(;;){
		printf("In task1 \n\r");
		OSTimeDly(0.1*OS_TICKS_PER_SEC);
		i++;j++;
		if(i == 5){
			OSTaskResume(1);
			i=0;
		}

		if(j ==7){
			OSTaskResume(2);
			j=0;
		}

	}
//	OSTaskDel(OS_PRIO_SELF);
}

void task2(void *arg)
{
	for(;;){
		printf("In First Slave task.\n\r");
		OSTaskSuspend(OS_PRIO_SELF);
	}
//	OSTaskDel(OS_PRIO_SELF);
}

void task3(void *arg)
{
	for(;;){
		printf("In second slave task\n\r");
		OSTaskSuspend(OS_PRIO_SELF);
	}
//	OSTaskDel(OS_PRIO_SELF);
}

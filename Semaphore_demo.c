#include "includes.h"
#include "system_stm32f10x.h"
//#include "stdio.h"
#define STACK_SIZE 100
INT8U data[20];
OS_STK alpha_stk[STACK_SIZE];
//OS_STK producer2_stk[STACK_SIZE];
OS_STK num_stk[STACK_SIZE];
OS_EVENT *alpha_sem;
OS_EVENT *num_sem;
int i;

void alpha(void *arg);
//void producer2(void *arg);
void num(void *arg);
int main(void)
{
	INT8U ret;//,ret1,ret2,ret3,ret4;
	OSInit();
	ret=OSTaskCreate(&alpha,(void *)0,&alpha_stk[STACK_SIZE-1],0);
	if(ret!=OS_ERR_NONE)
		printf("alpha creation failed..\n\r");
	printf("alpha task created.\n\r");
/*	ret=OSTaskCreate(&producer2,(void *)0,&producer2_stk[STACK_SIZE-1],1);
	if(ret!=OS_ERR_NONE)
			printf("producer2 creation failed..\n\r");
	printf("Producer2 task created.\n\r");*/
	ret=OSTaskCreate(&num,(void *)0,&num_stk[STACK_SIZE-1],2);
	if(ret!=OS_ERR_NONE)
			printf("num creation failed..\n\r");
	printf("num task created.\n\r");
	alpha_sem = OSSemCreate(1);
	num_sem = OSSemCreate(0);
	OSStart();
	while(1){

	}

}

void alpha(void *arg)
{
	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
	INT8U err;
	int j=0;
	for(i=0;i<20;i++, j++){
		printf("In alpha_task for %d th time\n\r", i);
		OSSemPend(alpha_sem, 200, &err);
		OSTimeDly(1*OS_TICKS_PER_SEC);
		data[i] = 'a' + j;
		OSSemPost(num_sem);
		OSTimeDly(0.1*OS_TICKS_PER_SEC);
	}
	OSTaskDel(OS_PRIO_SELF);
}

void num(void *arg)
{
//	printf("%s",data);
	int j=0;
	INT8U err;
	for(;i<20;i++, j++){
		printf("In num_task for %d th time\n\r", i);
		OSSemPend(num_sem, 200, &err);
		OSTimeDly(1*OS_TICKS_PER_SEC);
		data[i] = '1' + j;
		OSSemPost(alpha_sem);
		OSTimeDly(0.1*OS_TICKS_PER_SEC);
	}
	printf("%s \n\r", data);
	OSTaskDel(OS_PRIO_SELF);
}

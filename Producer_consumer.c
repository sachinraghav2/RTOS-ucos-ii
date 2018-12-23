#include "includes.h"
#include "system_stm32f10x.h"
//#include "stdio.h"
#define STACK_SIZE 100
INT8U data[20];
OS_STK producer1_stk[STACK_SIZE];
OS_STK producer2_stk[STACK_SIZE];
OS_STK consumer_stk[STACK_SIZE];
int i;

void producer1(void *arg);
void producer2(void *arg);
void consumer(void *arg);
int main(void)
{
	INT8U ret;//,ret1,ret2,ret3,ret4;
	OSInit();
	ret=OSTaskCreate(&producer1,(void *)0,&producer1_stk[STACK_SIZE-1],0);
	if(ret!=OS_ERR_NONE)
		printf("producer1 creation failed..\n\r");
	printf("producer1 task created.\n\r");
	ret=OSTaskCreate(&producer2,(void *)0,&producer2_stk[STACK_SIZE-1],1);
	if(ret!=OS_ERR_NONE)
			printf("producer2 creation failed..\n\r");
	printf("Producer2 task created.\n\r");
	ret=OSTaskCreate(&consumer,(void *)0,&consumer_stk[STACK_SIZE-1],2);
	if(ret!=OS_ERR_NONE)
			printf("consumer creation failed..\n\r");
	printf("Consumer task created.\n\r");
	OSStart();
	while(1){

	}

}

void producer1(void *arg)
{
	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	for(i=0;i<3;i++){
		printf("In task1 for %d th time\n\r", i);
		data[i] = 'a' + i;
		OSTimeDly(0.1*OS_TICKS_PER_SEC);
	}
	OS_EXIT_CRITICAL();
	OSTaskDel(OS_PRIO_SELF);
}

void producer2(void *arg)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	for(;i<6;i++){
		printf("In task2 for %d th time\n\r", i);
		data[i] = 'a';
		OSTimeDly(0.1*OS_TICKS_PER_SEC);
	}
	OS_EXIT_CRITICAL();
	OSTaskDel(OS_PRIO_SELF);
}

void consumer(void *arg)
{
	printf("%s",data);
	for(;;){
		printf("In consumer \n\r");
		OSTimeDly(1*OS_TICKS_PER_SEC);
		printf("%s", data);
	}
}

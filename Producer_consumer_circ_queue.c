#include "includes.h"
#include "system_stm32f10x.h"

#define SIZE 100
#define QSIZE 5

OS_STK master_stk[SIZE];
OS_STK prod1_stk[SIZE];
OS_STK prod2_stk[SIZE];
OS_STK consumer_stk[SIZE];
OS_EVENT *sem;

void master(void *);
void prod1(void *);
void prod2(void *);
void consumer(void *);

INT8U queue[QSIZE];
int front = -1, rear = -1;

INT8U is_full(){
	if((rear == QSIZE-1 && front == 0) || (rear == front -1))
		return 1;
	return 0;
}

INT8U is_empty(){
	if(front == -1)
		return 1;
	return 0;
}

void enqueue(INT8U data){
	if(is_full()){
		printf("Queue is full\n\r");
		return;
	}
	INT8U err;
	OSSemPend(sem, 200, &err);
	if(front == -1){
		front=rear=0;
		queue[rear] = data;
	}
	else if((rear == QSIZE-1) && (front !=0)){
		rear = 0;
		queue[rear] = data;
	}
	else{
		rear++;
		queue[rear] = data;
	}
	printf("Inserted %c\n\r", data);
}

void dequeue(){
	if(is_empty()){
		printf("Queue is empty.\n\r");
		return;
	}
	INT8U data = queue[front];
	if(front == rear)
		front = rear = -1;
	else if(front == QSIZE-1)
		front = 0;
	else
		front++;
	printf("%c deleted\n\r", data);
	OSSemPost(sem);
}

int main(void)
{
	INT8U ret;
	OSInit();

	ret = OSTaskCreate(&master, (void *)0, &master_stk[SIZE-1], 0);
	if(ret){
		printf("Master task creation failed.\n\r");
	}
	printf("Master task created.\n\r");

	ret = OSTaskCreate(&prod1, (void *)0, &prod1_stk[SIZE-1], 1);
	if(ret){
		printf("Producer1 task creation failed.\n\r");
	}
	printf("Producer1 task created.\n\r");

	ret = OSTaskCreate(&prod2, (void *)0, &prod2_stk[SIZE-1], 2);
	if(ret){
		printf("Producer2 task creation failed.\n\r");
	}
	printf("Producer2 task created.\n\r");

	ret = OSTaskCreate(&consumer, (void *)0, &consumer_stk[SIZE-1], 3);
	if(ret){
		printf("Consumer task creation failed.\n\r");
	}
	printf("Consumer task created.\n\r");

	sem = OSSemCreate(QSIZE);
	if(sem == (void *)0)
		printf("Cannot create semaphore\n\r");

	OSStart();
	while(1)
    {
    }
}


void master(void *arg){
	OS_CPU_SysTickInit(SystemCoreClock/OS_TICKS_PER_SEC);
	INT8U i=0, j=0, k=0;
	for(;;){
		OSTimeDly(0.1*OS_TICKS_PER_SEC);
		i++;j++;k++;
		if(i == 5){
			OSTaskResume(1);
			i=0;
		}
		if(j == 7){
			OSTaskResume(2);
			j=0;
		}
		if(k == 10){
			OSTaskResume(3);
			k=0;
		}
		printf("In master task\n\r");
	}
}

void prod1(void *arg){
	INT8U data = 'a';
	for(;;){
		enqueue(data);
		OSTaskSuspend(OS_PRIO_SELF);
	}
}

void prod2(void *arg){
	INT8U data = 'z';
	for(;;){
		enqueue(data);
		OSTaskSuspend(OS_PRIO_SELF);
	}
}

void consumer(void *arg){
	for(;;){
		dequeue();
		OSTaskSuspend(OS_PRIO_SELF);
	}
}

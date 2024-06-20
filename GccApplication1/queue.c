//#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define TRUE 1
#define FALSE 0


int front_fire=-1;   // read index
int rear_fire=-1;    // insert index


int queue_fire_full(void)
{
	// queue에서 rear+1 % QUEUE_MAX의 값이 front와 같으면 queue full
	int tmp=(rear_fire+1) % fir_b_size;
	if (tmp == front_fire)  // front와 같으면 queue full
	return TRUE;
	else return FALSE;
}

int queue_empty()
{
	if (rear_fire == front_fire)  // front와 같으면 queue empty
	return TRUE;
	else return FALSE;
}

short read_queue_fire()
{
	if (queue_empty()) return(0);
	//printf("Queue is empty !!!\n");
	else
	{
		front_fire = (front_fire+1) % fir_b_size;
		return (queue_fire[front_fire]);
	}
}

void queue_fire_init()  // queue가 텅 빈경우 fron와 rear가 동일한 위치를 가리틴다.
{
	front_fire=-1;   // read index
	rear_fire=-1;    // insert index
}

void insert_queue_fire(short value)
{
	if (queue_fire_full())
	{
		//printf("queue full!!!!\n");
		return;
	}
	else   // save
	{
		rear_fire = (rear_fire+1) % fir_b_size;
		queue_fire[rear_fire]=value;
	}
}
#ifndef QUEUE_H_
#define QUEUE_H_

#define TRUE 1
#define FALSE 0
#define QUEUE_MAX 100

int queue_fire_full(void);
int queue_fire_empty();
short read_queue_fire();
void queue_fire_init();
void insert_queue_fire(short value);

#define fir_b_size 22
short queue_fire[fir_b_size];

#endif /* QUEUE_H_ */
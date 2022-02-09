#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef int buffer_item; //buffer_item is just another name for a buffer containing an integer
#define BUFFER_SIZE 5

int insert_item(buffer_item item);
int remove_item(buffer_item *item);

void *producer(void *params);
void *consumer(void *params);

buffer_item buffer[BUFFER_SIZE]; //Create our buffer containing 5 empty slots
pthread_mutex_t mutex;
sem_t full;
sem_t empty;

int in;
int out;

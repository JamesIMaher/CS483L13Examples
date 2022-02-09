#include "main.h"

int main(int argc, char *argv[]){
	if(argc !=4){
		printf("Usage: pc <length of run in seconds> <number of producers> <number of consumers>\n");
		exit(1); //End the main program
	}

	int how_long = atoi(argv[1]);
	int num_prods = atoi(argv[2]);
	int num_cons = atoi(argv[3]);

	in = 0;
	out = 0;

	sem_init(&full,0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	pthread_mutex_init(&mutex,NULL);

	//Clear the buffer by setting everything to -1
	for(int i = 0; i<BUFFER_SIZE; i++){
		buffer[i] = -1;
	}

	//Create the pthreads
	pthread_t prod_tids[num_prods];
	for(int i = 0; i<num_prods; i++){
		pthread_create(&prod_tids[i],NULL,producer,NULL);
	}
	//Create the consumer threads
	pthread_t cons_tids[num_cons];
	for(int i = 0; i<num_cons; i++){
		pthread_create(&cons_tids[i],NULL,consumer,NULL);
	}
	
	sleep(how_long);

	return 1;
}

void print_buffer(){
	int i;
	printf("Buffer:");
	for (int i = 0; i<BUFFER_SIZE; i++){
		printf("%10d",buffer[i]);
	}
	printf("\n");
}

void *producer(void *params){
	buffer_item item;

	while(true){
		sleep(rand()%10);
		item = rand();
		if(insert_item(item)){
			printf("Error, can't insert an item\n");
			exit(1);}
		else{
			printf("Producer produced %d\n",(int) item);
		}
	}
}

void *consumer(void *params){
	buffer_item item;

	while(true){
		sleep(rand()%10);
		if(remove_item(&item)){
			printf("Error could not remove and item\n");
			exit(1);
		}else{
			printf("Consumer consumed %d\n",(int) item);
		}
	}
}

int insert_item(buffer_item item){
	sem_wait(&empty);

	pthread_mutex_lock(&mutex);

	buffer[in]=item;
	in = (in + 1)%BUFFER_SIZE;
	print_buffer();
	
	pthread_mutex_unlock(&mutex);
	sem_post(&full);

	return 0;
}

int remove_item(buffer_item *item){
	sem_wait(&full);

	pthread_mutex_lock(&mutex);

	*item = buffer[out];
	buffer[out] = -1;
	out = (out + 1) % BUFFER_SIZE;
	print_buffer();

	pthread_mutex_unlock(&mutex);
	sem_post(&empty);

	return 0;
}



/*
Written by: Adam Stillman
            11/4/14
            CSC 139
            program  assignment 3
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>

//defines
#define SLOTSIZE 14
#define SLOTCOUNT 7

void *producer_t();
void *consumer_t();

//global variables
char buffer[SLOTCOUNT][SLOTSIZE];
int in=0, out=0, count=0;

//semaphores
sem_t buf_lock,
      slot_avail,
      item_avail;

//files to read and write
FILE *fp0, *fp1;

//to create threads
pthread_t producer, consumer;



//main program
void main(int argc, char *argv[]){

//initialize semaphores
if(sem_init(&buf_lock,0,1)) printf("initializing buffer semaphore error error");
if(sem_init(&slot_avail,0,7))printf("initializing buffer semaphore error error");
if(sem_init(&item_avail,0,0))printf("initializing buffer semaphore error error");

//check if empty and if two entries
if( (argv[1]== NULL) || argc <3 || argc >3){
	printf("Correct usage: prog3 infile outfile\n");
	return;  }
//open file to read from
fp0 = fopen(argv[1],"rb");

//open/create file to write to
fp1 = fopen(argv[2],"wb");

//create threads in the if statement and if it fails let the user know
//it it succeeds
int r0, r1;
printf("Creating thread producer:\n");
if( (r0= pthread_create(&producer, NULL, producer_t, (void*) 0)) ){
	printf("thread creation failed: %d\n", r0); }
	else{
		printf("thread producer created with tid: %ld\n", pthread_self()); }

printf("Creating thread consumer:\n");
if( (r1= pthread_create(&consumer, NULL, consumer_t, (void*) 1)) ){
	printf("thread creation failed: %d\n", r1);  }
	else{
		printf("thread consumer created with tid: %ld\n", pthread_self()); }

////wait to close the threads
pthread_join(producer, NULL);
printf("Closing producer..\n");
pthread_join(consumer, NULL);
printf("Closing consumer..\n");

////close the files
fclose(fp0);
fclose(fp1);

printf("end of programm :P\n");
}

//thread calls
void *producer_t(){
//needed for strncpy
char tbuff[SLOTSIZE];

strncpy(tbuff, "tmpbuff", sizeof(tbuff));
printf("in the producer thread\n");

while(tbuff != NULL){
	if(fgets(tbuff,SLOTSIZE, fp0) != NULL){//while not empty file
		
		sem_wait(&slot_avail); //wait fo. a slot to be available
		sem_wait(&buf_lock);//wait to sue the buffer
		strncpy(buffer[in], tbuff, SLOTSIZE);//captuing from the in stream
		count++;//next spot
		in = (in+1)% SLOTCOUNT;//keep it within slot count
		sem_post(&buf_lock);//release the  buffer
		sem_post(&item_avail); //unlock the semaphore
	}
	else pthread_exit(NULL);
}

pthread_exit(NULL);
}

void *consumer_t(){
printf("in the consumer thread\n");

char tbuff[SLOTSIZE];
strncpy(tbuff, "tmpbuff", sizeof(tbuff));
	while(tbuff != NULL){
		sem_wait(&item_avail); //wait for a slot to be available
		sem_wait(&buf_lock);//wait to sue the buffer
		strncpy(tbuff, buffer[out], sizeof(tbuff));
		out = (out+1) % SLOTCOUNT;
		count--;
		sem_post(&buf_lock);//release the  buffer
		sem_post(&slot_avail); //unlock the semaphore

		fputs(tbuff, fp1);
		if ((feof(fp0) !=0) && (count==0)) pthread_exit(NULL);
	}
pthread_exit(NULL);
}







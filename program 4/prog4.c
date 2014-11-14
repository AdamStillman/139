/*
Written by: Adam Stillman
            11/4/14
            CSC 139
            program  assignment 4
	    same as prog3 but with condition mutex
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
//threads to be called
void *producer_t();
void *consumer_t();
//global variables
char buffer[SLOTCOUNT][SLOTSIZE];
int in=0, out=0, count=0;
//mutexs
pthread_mutex_t buf_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty_slot = PTHREAD_COND_INITIALIZER;
pthread_cond_t item_avail = PTHREAD_COND_INITIALIZER;
//files fp0 read and fp1 write
FILE *fp0, *fp1;
//to create threads
pthread_t producer, consumer;
//main program
void main(int argc, char *argv[]){
//check if empty and if two entries
if( (argv[1]== NULL)||argc<3||argc>3){printf("Correct usage: prog3 infile outfile\n");return;}
//open file stream fp0 to read from fp1 to write to
fp0 = fopen(argv[1],"rb"); fp1 = fopen(argv[2],"wb");
//create threads in the if statement and if it fails let the user know
//it it succeeds
int r0, r1;
if( (r0= pthread_create(&producer, NULL, producer_t, (void*) 0)) ) printf("thread creation failed: %d\n", r0);
if( (r1= pthread_create(&consumer, NULL, consumer_t, (void*) 1)) ) printf("thread creation failed: %d\n", r1);
////wait to close the threads and file streams
pthread_join(producer, NULL); pthread_join(consumer, NULL);
fclose(fp0); fclose(fp1);
}//end of program
//thread calls
void *producer_t(){
//needed for strncpy
char tbuff[SLOTSIZE];
strncpy(tbuff, "tmpbuff", sizeof(tbuff));//populate the char array
while(tbuff != NULL){
	if(fgets(tbuff,SLOTSIZE, fp0) != NULL){//while not empty file
		pthread_mutex_lock(&buf_lock);//lock the buffer
		if(SLOTCOUNT==count)pthread_cond_wait(&empty_slot, &buf_lock);
		strncpy(buffer[in], tbuff, SLOTSIZE);//captuing from the in stream
		count++;//next spot
		in = (in+1)% SLOTCOUNT;//keep it within slot count
		pthread_cond_signal(&item_avail);
		pthread_mutex_unlock(&buf_lock);//release buffer
		//release of old locks
	}
	else pthread_exit(NULL);
}pthread_exit(NULL); }

void *consumer_t(){
char tbuff[SLOTSIZE];
strncpy(tbuff, "tmpbuff", sizeof(tbuff));	
while(tbuff != NULL){
		pthread_mutex_lock(&buf_lock);//lock buffer
		if(count==0) pthread_cond_wait(&item_avail, &buf_lock);
		strncpy(tbuff, buffer[out], sizeof(tbuff));
		out = (out+1) % SLOTCOUNT;
		count--;
		pthread_cond_signal(&empty_slot);
		pthread_mutex_unlock(&buf_lock);//lock the buffer
		fputs(tbuff, fp1);//write to file
		if ((feof(fp0) !=0) && (count==0)) pthread_exit(NULL);//check end of file
		}
pthread_exit(NULL);
}

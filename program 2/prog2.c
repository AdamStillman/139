/*
Written by: Adam Stillman
            10/14/14
            CSC 139
            program  assignment 2
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

void *mc0();
void *mc1();
void *mc2();
void *mc3();
void *display();
//global variables
int c0, c1, c2, c3;
// create and initialize the mutexs for maintaining the critical section
pthread_mutex_t mutex0 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
//global storage for entries
int entries[14];


void main(int argc, char *argv[]){
//variables
int i, a, b, length;
//incase the threads fail to create
int r0,r1,r2,r3,r4;
char *temp_s, *p;
long input;//for string to long conversion

//to create threads
pthread_t thread0, thread1, thread2, thread3, thread4;
//tids
int tid0, tid1, tid2, tid3;

//validation
if(argv[1]==0){// check tos ee if the first entry is populated
printf("no entries, please enter 12 entries\n");
return;
}

for(i=1; i<argc; i++){//check the first digit
if(argv[i][0] != '+' && argv[i][0] != '-' && isdigit(argv[i][0]) ==0){
	printf("Entry not accepted: %s\n", argv[i]);
	return ; }//allow + and - through 
//use the string to long function to capture ints and take out strings
input = strtol(argv[i], &p, 0);//string to long
//strtlong takes the input takes the digits in cluding sign and seperates any
//chars and puts the chars in p. if p is empty then there are no characters
if(*p!=0){//any non int's put in p
	printf("Entry not allowed: %s\n", argv[i]);
	return ; }
if(argc <13 || argc >13){//check for 12 entries
	printf("Number of inputs != 12. Enter 12 nputs please\n");
	return ; }
//store the inputs from string to long as ints
//pthread_mutex_lock(&mutexe);
entries[i] = (int)input;
//pthread_mutex_unlock(&mutexe);

}
//create threads in the if statement and if it fails let the user know
//it it succeeds, lock a mutex to that thread.
printf("Creating thread 0:\n");
if( (r0= pthread_create(&thread0, NULL, (void *) mc0, NULL)) ){
printf("thread creation failed: %d\n", r0);                            
}
else{
pthread_mutex_lock(&mutex0);
printf("thread 0 created with tid: %ld\n", pthread_self());
}
printf("Creating thread 1:\n");
if( (r1= pthread_create(&thread1, NULL, (void *) mc1, NULL)) ){
printf("thread creation failed: %d\n", r1);                            
}
else{
pthread_mutex_lock(&mutex1);
printf("thread 1 created with tid: %ld\n", pthread_self());
}
printf("Creating thread 2:\n");
if( (r2= pthread_create(&thread2, NULL, (void *) mc2, NULL)) ){
printf("thread creation failed: %d\n", r2);                            
}
else{
pthread_mutex_lock(&mutex2);
printf("thread 2 created with tid: %ld\n", pthread_self());
}
printf("Creating thread 3:\n");
if( (r3= pthread_create(&thread3, NULL, (void *) mc3, NULL)) ){
printf("thread creation failed: %d\n", r3);                            
}
else{ 
printf("Creating thread 3 \n");
pthread_mutex_lock(&mutex3);
}
printf("Creating thread 4:\n");
if( (r4= pthread_create(&thread4, NULL, (void *) display, NULL) )){
printf("thread creation failed: %d\n", r4);                            
}
else printf("thread 4(display) created with tid: %ld\n", pthread_self());
//waiut for threads to close
pthread_join(thread0, NULL);
printf("Closing thread 0:\n");
pthread_join(thread1, NULL);
printf("Closing thread 1:\n");
pthread_join(thread2, NULL);
printf("Closing thread 2:\n");
pthread_join(thread3, NULL);
printf("Closing thread 3:\n");
pthread_join(thread4, NULL);
printf("Closing thread 4:\n");
//clean up the mutexes
pthread_mutex_destroy(&mutex0);
pthread_mutex_destroy(&mutex1);
pthread_mutex_destroy(&mutex2);
pthread_mutex_destroy(&mutex3);

printf("end of programm :P\n");
}
//thread calls
void *mc0(){
printf("I'm calculating c0. my thread is: %ld\n", pthread_self());
c0= ( entries[1] * entries[7] ) + (entries[2] * entries[9] ) + (entries[3] * entries[11]);
pthread_mutex_unlock( &mutex0 );//unlock the critical sections
printf("c0 = %d\n", c0);
printf("leaving process mc0: %ld...\n", pthread_self() );    
pthread_exit;
return NULL;
}
void *mc1(){
printf("I'm calculating c1. my thread is: %ld\n", pthread_self());
c1 = entries[1] * entries[8] + entries[2] * entries[10] + entries[3] * entries[12];
pthread_mutex_unlock( &mutex1 );
printf("c1 = %d\n", c1);
printf("leaving process mc1: %ld...\n", pthread_self() );       
pthread_exit;
return NULL;
}
void *mc2(){
printf("I'm calculating c2. my thread is: %d\n", pthread_self());
c2= entries[4] * entries[7] + entries[5] * entries[9] + entries[6] * entries[11];
pthread_mutex_unlock( &mutex2 );
printf("c2 = %d\n", c2);
printf("leaving process mc2: %ld...\n", pthread_self());            
pthread_exit;
return NULL;
}
void *mc3(){
printf("I'm calculating c3. my thread is: %d\n", pthread_self());
c3= entries[4] * entries[8] + entries[5] * entries[10] + entries[6] * entries[12];
pthread_mutex_unlock( &mutex3 );
printf("c3 = %d\n", c3);
printf("leaving process mc3: %ld...\n", pthread_self());                 
pthread_exit;
return NULL;
}
//display process
void *display(){
printf("im in the display\n");
//lock the mutexes. this makes the display process wait until it can lock the mutex
//which is after the 'MC' vaues have been calculated
pthread_mutex_lock(&mutex0);
printf("locked mutex0\n");
pthread_mutex_lock(&mutex1);
printf("locked mutex1\n");
pthread_mutex_lock(&mutex2);
printf("locked mutex2\n");
pthread_mutex_lock(&mutex3);
printf("locked mutex3\n");


printf("--------------------------\n");
printf("|  (%d)--(%d)  |\n", c0, c1);
printf("|  (%d)--(%d)  |\n", c2, c3);
printf("--------------------------\n");
printf("About to terminate display process...\n");   
pthread_exit;
return NULL;
}


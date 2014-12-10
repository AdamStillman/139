/******************* 

 *******************/
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>

typedef struct {		
    long    msg_type;
    long    msg_fm;
    char    buffer[100];
} MESSAGE;
//variables 
int mid;
key_t key;
struct msqid_ds buf;
MESSAGE msg;
FILE *fp0;
char line[20];

int main(int argc, char *argv[]) {

//get id
key = ftok(".", 'z');
mid = msgget(key, 0);

//get pid
//set up up MESSAGE to be a client
long iD = (long)getpid();
msg.msg_type = (long)1;
msg.msg_fm = (long)getpid();

//poll the server
if(msgsnd(mid, &msg, sizeof(msg.buffer), 0)==-1){
	printf("msgsnd error");
	exit(-1);
}
int i = 0;
//server says ok
if(msgrcv(mid, &msg, sizeof(msg.buffer), iD, 0)<0){
	printf("msgrcv error");
	exit(-1);
}
//open fp0
    while(i<2){
        if(i == 0){
              fp0 = fopen(argv[1], "rb");
         }
         else{
              fp0 = fopen(argv[2], "rb");
         }
         
         if(fp0 == NULL){
             printf("file open error\n");
             exit(EXIT_FAILURE);
     }
     //Write contents of file to buffer    
         while(fgets(line, 20, fp0) != NULL){
              strcat(msg.buffer, line);
     }  
         i++;
         //Sent info to server
         msg.msg_type = msg.msg_fm;     
         msg.msg_fm = (long)getpid();
         if(msgsnd(mid, &msg, sizeof(msg.buffer), 0)==-1){
	     printf("msgsnd error\n");
	     exit(-1);
     }
     //get back modified chars
     if(msgrcv(mid, &msg, sizeof(msg.buffer), iD, 0)<0){
	     printf("msgrcv error\n");
	     exit(-1);
     } 
     //Print to stdout
     printf("%s", msg.buffer);
     strcpy(msg.buffer, "");
     fclose(fp0);
     }
return(0);	
}

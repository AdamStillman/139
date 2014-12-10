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

//get pid so server client can sync
long iD = (long)getpid();
msg.msg_type = (long)1;
msg.msg_fm = (long)getpid();

//holla at the server homeboy
if(msgsnd(mid, &msg, sizeof(msg.buffer), 0)==-1){
	printf("msgsnd");
	exit(-1);
}
if(msgrcv(mid, &msg, sizeof(msg.buffer), iD, 0)<0){
	printf("msgrcv");
	exit(-1);
}
//open text file 
fp0 = fopen(argv[1], "r");
if(fp0 == NULL){
         printf("Error while opening the file.\n");
         exit(EXIT_FAILURE);
}   
//fill buffer
while(fgets(line, 20, fp0) != NULL){
         strcat(msg.buffer, line);
}
fclose(fp0);  
//send to server
msg.msg_type = msg.msg_fm;
msg.msg_fm = (long)getpid();	
if(msgsnd(mid, &msg, sizeof(msg.buffer), 0)==-1){
	printf("msgsnd");
	exit(-1);
}
//got back from server
if(msgrcv(mid, &msg, sizeof(msg.buffer), iD, 0)<0){
	printf("msgrcv");
	exit(-1);
}
//Print to stdout
printf("%s", msg.buffer);
msgsnd(mid, &msg, 0, 0);
return(0);
}

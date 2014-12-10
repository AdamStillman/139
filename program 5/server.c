/*
Written by: Adam Stillman
            11/25/14
            CSC 139
            program  assignment 5
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#define SERVER 1L
tpyedef struct{
	long msg_to;
	long msg_fm;
	char buffer[100];
} MESSAGE;

int mid, status, i;
key_t key;
struct msqid_ds buff;
MESSAGE msg;
pid_t process[2];
pid_t pid, wait;
void main(int argc, char *argv[]){

	//grab identifiers of correct que
	key = ftok(".",'z');
	if(mid = msgget(key,IPC_CREAT | 0666) <0 ) {printf("error createing que\n"); exit();
i = 0;

do{
	//waiting for clients
	if(msgrcv(mid,&msg, sizeof(msg.buffer),1, 0) <0){ printf("couldnt recieve\n"); exit;}

	//make a process for clients
	pid=fork();
	process[i] = pid;
	i++;

	if(pid <-1){printf("Could not fork\n"); exit;}
	else if(pid==0){

		//send msg back to 
		long client = msg.msg_fm;
		msg.msg_fm = (long)getppid();
	        msg.msg_type = client;
		long childType = (long)getppid();

		if(msgsnd(mid, (struct MESSAGE)&msg, sizeof(msg.buffer),0) ==-1){printf("msgsend error\n"); exit;}

		int l = 0;
		do{
			l++;
			//recieve txt from client

			if(msgrcv(mid,&msg, sizeof(msg.buffer),childType, 0) <0){ printf("couldnt recieve\n"); exit;}
			if(strlen(msg.buffer) ==0 ) break;

			//to upper
			int a = 0;
			for( int a = 0; a<strlen(msg.buffer); a++){
				msg.buffer[a] = toupper(msg.buffer[a]);
			}
	 		msg.msg_type = msg.msg_fm;
			msg.msg_fm = (long)getppid();
			
			//send back
			if(msgsnd(mid, (struct MESSAGE)&msg, sizeof(msg.buffer),0) ==-1){printf("msgsend error\n"); exit;}
				
		}while(l<2);
		exit(0);

	}
} while(x<2);


int k	
//wait for child proccess t0 finish
for(int k=0; k<2; k++){
wait = waitpid(process[k], &status,WUNTRSCED | WCONTINUED);
if(wait ==-1){printf("wait pid errpr\n"); exit;}
}

msgctl(mid, IPC_RMID, (struct msqid_ds *) 0);
return(0);

}



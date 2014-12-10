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
#include <sys/shm.h>
#include <sys/sem.h>
typedef struct {	
    long    msg_type;
    long    msg_fm;
    char    buffer[100];
} MESSAGE;
int mid;
key_t key;
struct msqid_ds buf;
MESSAGE msg;
pid_t pid, wait, wait1;
int mid, status;
//pids for each file
pid_t process[2];

int main(int argc, char** argv) { 
     
    //make the msg que
    key = ftok(".", 'z');
    if((mid = msgget(key, IPC_CREAT | 0666))<0){
        printf("Error Creating Message Queue\n");
        exit(-1);
    }
    int x = 0;
    while(x < 2)
    {
          //need clients, wait for them
          if(msgrcv(mid, &msg, sizeof(msg.buffer), 1, 0)<0){
		printf("msgrcv");
		exit(-1);
		}
		//first process to handle the client
		pid=fork();
		process[x] = pid;
		x++;
		if(pid==-1){
		     printf("fork error\n"); exit(-1);
		}
		else if(pid==0)
		{    
		//make sure que is going to send back to the right client
	    	long client = msg.msg_fm;
		msg.msg_fm = (long)getppid();
	    	msg.msg_type = client; 
	    	long childType=(long)getppid();
		if(msgsnd(mid, (struct MESSAGE*)&msg, sizeof(msg.buffer), 0)==-1){
		 printf("msgsnd");
		 exit(-1);
		}
		int j = 0;
		//get text from clients
		while(j<2){
			j++;
		    	if(msgrcv(mid, &msg, sizeof(msg.buffer), childType, 0)<0) {
		       		printf("msgrcv error");
		       		exit(-1);
	          } 
	         if(strlen(msg.buffer) == 0){
	               break;
	         }
	         //capitalize the recieved txt
	         int i = 0;
		         for(i = 0; i<strlen(msg.buffer); i++) {
           	  msg.buffer[i] = toupper(msg.buffer[i]);
	         }
		//and pput back in buffer to be sent back
		////make sure its going to the right person
       		  msg.msg_type = msg.msg_fm;
	         msg.msg_fm = (long)getppid();
	         //Sent the text back to the client
	         if(msgsnd(mid, (struct MESSAGE*)&msg, sizeof(msg.buffer), 0)==-1) {    
		    printf("msgsnd");
	            exit(-1);
		    }
		}
		exit(0);
		}
    } 
    //let child finish before closing
     wait = waitpid(process[0], &status, WUNTRACED | WCONTINUED);
        if (wait == -1) {
	   printf("waitpiderror");
	   exit(EXIT_FAILURE);
        }
   //let the other child finish
     wait1 = waitpid(process[1], &status, WUNTRACED | WCONTINUED);
     if (wait1 == -1) {
	 printf("waitpid error");
         exit(EXIT_FAILURE);
        }
     //rm that que 
     msgctl(mid, IPC_RMID, (struct msqid_ds *) 0);
     return(0);                   
}

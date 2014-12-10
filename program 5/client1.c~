/*
Written by: Adam Stillman
            12/10/14
            CSC 139
            program  assignment 5: client 1
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

int mid;
key_t key;
struct msqid_ds buff;
MESSAGE msg;


void main(int argc, char *argv[]){
	//check for correct number of inputs
	if( (argv[1]== NULL) || argc <3 || argc >3){printf("Correct usage: prog5 infile infile\n"); return;  }

	FILE *fp0;
	int i = 0;

	//grab identifiers of correct que
	key = ftok(".",'z');
	mid = msgget(key,0);

	long id = (long)getppid();
	msg.msg_t0 = (long)1;
	msg.msg_fm = (long)getpid();

	//find the que
	if(msgsnd(mid,&msg, sizeof(msg.buffer), 0)==-1){ printf("couldnt find que\n"); exit;}
	
	do{
		if(i==0){ if(fp0 = fopen(argv[1],"rb")==null){ printf("file open error\n"); exit;} }//open file 1
				else {if(fp0 = fopen(argv[2],"rb")==null){ printf("file open error\n"); exit;} }//open file 2
		
		char temp[20];
		
		//loop through file stream until EOF found and concatonate the file to msge buffer to be sent to server
		while( fgets(temp, 20, fp0)!= NULL){ strcat(msg.buffer, temp); }
		
		i++;

		//msgsend
		msg.type = msg.msg.fm;
		msg.fm = (long)getpid();
		if(msgsnd(mid,&msg, sizeof(msg.buffer), 0)==-1){ printf("couldnt send msg\n"); exit;}

		//msg recieve
		if(msgrcv(mid,&msg, sizeof(msg.buffer),id,0) <0){ printf("couldnt recieve\n"); exit;}

		//print stdout
		print("%s ", msg.buffer);
		strcpy(msg.buffer, "");

		//close the file stream
		fclose(fp0);
	}while(i<2);
	return(0);
}

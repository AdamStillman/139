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
	char buffer[BUFSIZ];
} MESSAGE;

int mid;
key_t key;
struct msqid_ds buff;
MESSAGE msg;

Struct msqid_ds {
struct ipc_perm msg_perm; // ipc permissions
struct msg *msg_first; // ptr to first msg
struct msg *msg_last; // ptr to last msg
ulong msg_cbytes; // current # bytes on Q
ulong msg_qnum; // # of msg in Q
ulong msg_qbytes; // max # of bytes in Q
pid_t msg_lspid; // pid of last msgsnd
pid_t msg_lrpid; // pid of last msgrcv
};

void main(int argc, char *argv[]){
	if( (argv[1]== NULL) || argc <3 || argc >3){printf("Correct usage: prog5 infile infile\n"); return;  }

	FILE *fp0, *fp1

	//open file streams to read from fp0 and fp1
	fp0 = fopen(argv[1],"rb");
	fp1 = fopen(argv[2],"rb");

	char tbuff[SLOTSIZE];
	strncpy(tbuff, "tmpbuff", sizeof(tbuff));
	
	while(tbuff != NULL){
		strncpy(tbuff, buffer[out], sizeof(tbuff));
		fputs(tbuff, fp1);
		if ((feof(fp0) !=0) && (count==0)) pthread_exit(NULL);



	//close the file streams
	fclose(fp0);
	fclose(fp1);

}

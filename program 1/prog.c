/*
Written by: Adam Stillman
            9/24/14
            CSC 139
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <ctype.h>


void main(int argc, char *argv[]){
//variables
int i, a, b, length, entries[14];
char *temp_s, *p;
long input;//for string to long conversion
//to get the process ids
pid_t ppid, childpid0, childpid1, childpid2, childpid3, displaypid;
//for the pipes
int fd0[2], fd1[2], fd2[2], fd3[2], fd4[2];

int c0, c1, c2, c3, display_c0, display_c1, display_c2, display_c3;
int status0, status1, status2, status3, status4;
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
entries[i] = (int)input;


}

//create all the pipes
if(pipe(fd0)==-1){
printf("Pipe making error");
return;
}
if(pipe(fd1)==-1){
printf("pipe making error");
return;
}
if(pipe(fd2)==-1){
printf("Pipe making error");
return;
}
if(pipe(fd3)==-1){
printf("Pipe making error");
return;
}
if(pipe(fd4)==-1){
printf("Pipe making error");
return;
}

/////////// c0 fork to calc c0
childpid0 = fork();//calculate c0
if(childpid0<0){//if the child is not zero or more then there was an error
printf("Fork failed\n");
return;
}
else if(childpid0==0){//if the child pid is 0 then it is the child
printf("I'm child process c0 and my pid is: %d\n",getpid());//get own pids
printf("my parent pid is: %d\n", getppid());//get parent pid
//calculate c0
c0= ( entries[1] * entries[7] ) + (entries[2] * entries[9] ) + (entries[3] * entries[11]);
printf("C0 = %d \n", c0);//debugging
//send c0 through the pipe
close(fd0[0]);//close read end
write(fd0[1], &c0, sizeof(c0) );//write to write end
//preparing to close process
printf("About to terminate process c0: %d\n", getpid());
return; } 
//if not 0 and not below 0 then we are the parent process and 
//we need to wait for child to finish
else{//wait for child to finish
if(waitpid(childpid0, &status0,0)==childpid0){//wait for a change in the child pid
printf("child 1 exit success! \n");
;
}
else{//if no change happens there there is something wrong
printf("child 1 exit error\n");
return;
}
}


/////////////////////c1 fork
//repeat 3 more tiems
childpid1 = fork();
if(childpid1 < 0){
printf("fork error\n");
return;
}
else if(childpid1==0){
printf("I'm calculating c1. my pid is: %d\n", getpid());
printf("my parent process is %d \n", getppid() );
c1= entries[1] * entries[8] + entries[2] * entries[10] + entries[3] * entries[12];
printf("c1 = %d\n", c1);
close(fd1[0]);
write(fd1[1], &c1, sizeof(c1) );

printf("About to terminate process c1: %d...\n", getpid());
return;
}
else{//wait for child 2 ro finish
if(waitpid(childpid1, &status1,0)==childpid1){
printf("child 2 exit success! \n");
//return;
}
else{
printf("child 2 exit error\n");
return;
}


}
//fork c2
childpid2 = fork();
if(childpid2<0){
printf("fork error\n");
return; }
else if(childpid2 ==0){
printf("I'm calculating c3 and my pid is %d\n", getpid());
printf("my parent prcoess was %d\n", getppid());
c2= entries[4] * entries[7] + entries[5] * entries[9] + entries[6] * entries[11];
printf("c2 = %d\n", c2);
//wrtie to display
close(fd2[0]);
write(fd2[1], &c2, sizeof(c2) );
printf("About to close process c2: %d\n", getpid());
return;
}
else{//wait for child 3 to stop
if(waitpid(childpid2, &status2,0)==childpid2){
printf("child 2 exit success! \n");
}
else{
printf("child 2 exit error\n");
return;
}



};
//child 3
childpid3 = fork();
if(childpid3<0){
printf("fork error\n");
return;
}
else if(childpid3 == 0){
printf("child process to calculate c3: %d \n", getpid());
printf("my parent process is: %d\n", getppid());
c3= entries[4] * entries[8] + entries[5] * entries[10] + entries[6] * entries[12];
printf("c3 is %d\n ", c3);
close(fd3[0]);
write(fd3[1], &c3, sizeof(c3));
printf("about to exit child %d\n", getpid());
return;
}
else{
if(waitpid(childpid3, &status3,0)==childpid3){
printf("child 3 exit success! \n");
}
else{
printf("child 3 exit error\n");
return;
}

}

///////////display fork
//same pid checking
displaypid = fork();
if(displaypid < 0){
printf("fork error\n");
return;
}
else if(displaypid==0){
printf("I am in the display process, pid : %d\n", getpid());
printf("my parents pid is: %d\n", getppid());
//now read from the pipes that the other child processes have written to
//read co pipe
close(fd0[1]);//close write end
read(fd0[0], &display_c0,sizeof(display_c0) );//read from c1 and store variable in display_c0
//read c1 pipie
close(fd1[1]);
read(fd1[0], &display_c1, sizeof(display_c1));
//read c2 pipie
close(fd2[1]);
read(fd2[0], &display_c2, sizeof(display_c2) );
//read c3 pipe
close(fd3[1]);
read(fd3[0], &display_c3, sizeof(display_c3) );
//display the resulting matix
printf("--------------------------\n");
printf("|  (%d)--(%d)  |\n", display_c0, display_c1);
printf("|  (%d)--(%d)  |\n", display_c2, display_c3);
printf("--------------------------\n");
printf("About to terminate display process...\n");
return;
}
else{
if(waitpid(displaypid, &status4,0)==displaypid){
printf("child display exit success! \n");
}
else{
printf("child display exit error\n");
return;
}

}

printf("end of programm :P\n");
}


#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include <bits/stdc++.h>
#include<sys/types.h>

using namespace std;

bool bg = false;

int tokenizeCommand(char * ar [], char str []){
    char *token = strtok(str, " ");
    int i=0;
    while(token != NULL){
        ar[i++]=token;
        token = strtok(NULL, " ");
    }
    ar[i]=NULL;
    return i;
}

void sig_handler(int iSignal)
{
    //signal(SIGCHLD,sig_handler); //reset handler to catch SIGCHLD for next time;
    int status;
    pid_t pid;
    //if(!bg)
    pid = wait(&status); //After wait, child is definitely freed.
    printf("pid = %d , status = %d\n", pid, status);
    if(WIFEXITED(status)) printf("child exited with = %d\n",WEXITSTATUS(status));
}

void write_to_file(){
}

void executeCommand(char * ar[], bool bg){

    //signal(SIGCHLD,sig_handler);
    pid_t process = fork();

    if (process== 0){
        printf("hello from child: %d\n", getpid());
        execvp(ar[0],ar);
        exit(errno);
    }
    else
    {
        printf("hello from parent: %d\n", getpid());
        int status;
        if(!bg) waitpid(process,&status,0);
        //wait(NULL);
        if(WIFEXITED(status)) {
            printf("child exited with = %d and %d\n",WEXITSTATUS(status), process);
            write_to_file(process, WEXITSTATUS(status));
        }

        printf("child has terminated\n");
    }

    printf("Bye\n");
}

int main()
{


    while(true){
        char str[256];
        scanf("%[^\n]%*c", str);

        int N = 256;
        char * ar[N];
        int i = tokenizeCommand(ar, str);

        if(strcmp(ar[0], "exit") == 0) exit(0);
        if(strcmp(ar[i-1], "&") == 0){
            ar[i-1] = NULL;
            bg = true;
        }
        executeCommand(ar,bg);

    }

    return 0;
}

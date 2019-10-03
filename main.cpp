#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include <bits/stdc++.h>

using namespace std;


void tokenizeCommand(char * ar [], char str []){
    char *token = strtok(str, " ");
    int i=0;
    while(token != NULL){
        ar[i++]=token;
        token = strtok(NULL, " ");
    }
    ar[i]=NULL;
}

void executeCommand(char * ar[]){
    int process = fork();

    if (process== 0){
        printf("hello from child\n");

        execvp(ar[0],ar);
        exit(errno);
    }
    else
    {
        printf("hello from parent\n");
        int status;
        wait(&status);
        if(WIFEXITED(status)) printf("child exited with = %d\n",WEXITSTATUS(status));
        //if (WIFSIGNALED(status)) psignal(WTERMSIG(status), "Exit signal");
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
        tokenizeCommand(ar, str);

        if(strcmp(ar[0], "exit") == 0) exit(0);
        executeCommand(ar);

    }

    return 0;
}

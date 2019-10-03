#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include <bits/stdc++.h>

using namespace std;


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

void executeCommand(char * ar[], bool bg){
    pid_t process = fork();

    if (process== 0){
        printf("hello from child\n");
        execvp(ar[0],ar);
        exit(errno);
    }
    else
    {
        printf("hello from parent\n");
        int status;
        if(!bg) waitpid(process,&status,0);
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
        int i = tokenizeCommand(ar, str);
        bool bg = false;

        if(strcmp(ar[0], "exit") == 0) exit(0);
        if(strcmp(ar[i-1], "&") == 0){
            ar[i-1] = NULL;
            bg = true;
        }
        executeCommand(ar,bg);

    }

    return 0;
}

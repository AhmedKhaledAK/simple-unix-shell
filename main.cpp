#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include <bits/stdc++.h>

using namespace std;


void tokenize(char * ar [], char str []){
    char *token = strtok(str, " ");
    int i=0;
    while(token != NULL){
        ar[i++]=token;
        token = strtok(NULL, " ");
    }
    ar[i]=NULL;
}

int main()
{
    int process = fork();

    if (process== 0){
        printf("hello from child\n");

        char str[256];

        scanf("%[^\n]%*c", str);

        int N = 256;

        char * ar[N];

        tokenize(ar, str);

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
    return 0;
}

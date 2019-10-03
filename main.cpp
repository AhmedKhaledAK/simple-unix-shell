#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include <bits/stdc++.h>

using namespace std;


/*char * tokenize(char ar [], int len){
    char *token = strtok(str, " ");
    char * ar[N] = tokenize(str, len);
    int i=0;
    while(token != NULL){
        ar[i++]=token;
        token = strtok(NULL, " ");
    }
    ar[i]=NULL;
    return ar;
}*/

int main()
{
    int N = 3;

    int process = fork();

    if (process== 0){
        printf("hello from child\n");

        char str[20];
        scanf("%[^\n]%*c", str);

        char *token = strtok(str, " ");
        char * ar[N];
        int i=0;
        while(token != NULL){
            ar[i++]=token;
            token = strtok(NULL, " ");
        }
        ar[i]=NULL;

        int status = execvp(ar[0],ar);
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

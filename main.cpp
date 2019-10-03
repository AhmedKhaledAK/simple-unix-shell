#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>

using namespace std;

int main()
{

    int process = fork();
    if (process== 0)
        printf("hello from child\n");
    else
    {
        printf("hello from parent\n");
        wait(NULL);
        printf("child has terminated\n");
    }

    printf("Bye\n");
    return 0;
}

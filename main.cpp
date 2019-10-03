#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main()
{
    int x = 1;
    int process = fork();

    if (process == 0){
        cout << "Child ID: " << getpid() << "\n";
        cout << "Parent ID: " << getppid() << "\n";
        printf("Child has x = %d\n", ++x);
    }
    else {
        cout << "Process ID: " << getpid() << "\n";
        cout << "Parent ID: " << getppid() << "\n";
        printf("Parent has x = %d\n", --x);
    }
    return 0;
}

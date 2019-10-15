#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include <bits/stdc++.h>
#include<sys/types.h>
#include <fstream>

using namespace std;

char * org_log_loc;
char log_file[100];

int tokenize_command(char * ar [], char str []){
    char *token = strtok(str, " ");
    int i=0;
    while(token != NULL){
        ar[i++]=token;
        token = strtok(NULL, " ");
    }
    ar[i]=NULL;
    return i;
}

void write_to_file(pid_t pid, int status){
    ofstream myfile;
    cout << "log loc " << log_file << "\n";
    cout << "getcwd " << org_log_loc << "\n";
    myfile.open(log_file, ios_base::app);
    myfile << "Child process with id: " << "" << " terminated with status: " << "" << "\n";
    myfile.close();
}

void sig_handler(int iSignal)
{
    //int status;
    //pid_t pid = wait(&status);
    write_to_file(0,0);

    /*if(WIFEXITED(status)) {
        printf("child exited with = %d and %d\n",WEXITSTATUS(status), pid);
        write_to_file(pid, WEXITSTATUS(status));
    }*/
    /*int status;
    pid_t pid;
    cout << "pid: " << getpid() << "\n";
    pid = wait(&status); //After wait, child is definitely freed.
    printf("pid = %d , status = %d\n", pid, status);
    if(WIFEXITED(status)) {
        printf("child exited with = %d and %d\n",WEXITSTATUS(status), pid);
        write_to_file(pid, WEXITSTATUS(status));
    }*/
}

void execute_command(char * ar[], bool bg){

    signal(SIGCHLD,sig_handler);
    pid_t process = fork();

    if (process== 0){
        //printf("hello from child: %d\n", getpid());
        execvp(ar[0],ar);
        exit(errno);
    }
    else
    {
        //printf("hello from parent: %d\n", getpid());
        int status;
        if(!bg) waitpid(process,&status,0);
        //printf("child has terminated\n");
    }

    //printf("Bye\n");
}

int main()
{
    org_log_loc = get_current_dir_name();
    strcat(log_file, org_log_loc);
    strcat(log_file, "/logs.txt");
    int N = 256;
    /* this loop keeps the terminal running to keep taking inputs from the user */
    while(true){
        bool bg = false;
        char str[N];
        printf("ushell:%s~$",get_current_dir_name());
        /* this scanf takes the whole command with spaces, and it igonres the "enter" character made by the user at the end of the line */
        scanf("%[^\n]%*c", str);

        /* an arbitary size of the command and arguments array which will be the parameter for the execvp system call */
        char * ar[N];
        /* after reading the line from the user, I tokenize the command into strings to insert them into the command and arguments array.
            This function returns the index of the last element in the array, to check if there's & or no later on */
        int i = tokenize_command(ar, str);
        /* after tokenizing and inserting into the array, I check if the first element in the array (i.e. the command without the arguments) is equal to exit, the program stops */
        if(strcmp(ar[0], "exit") == 0) exit(0);

        else if(strcmp(ar[0], "cd")==0){
            int isChanged = chdir(ar[1]);
            if(isChanged < 0)
                printf("No such file or directory\n");
        }

        else{
            /* this is to check if the user wants to execute the entered line as a background process (i.e. the parent process doesn't wait for its child to finish executing) */
            if(strcmp(ar[i-1], "&") == 0){
                /* if it meant to run as a backgroung process, we put NULL in place of & in the commands and arguments array. That's the only reason the user writes &, it's for me
                 to know if I should make the parent process wait or no. I also created a flag to help me determnine whether I should wait or no in the parent process */
                ar[i-1] = NULL;
                bg = true;
            }

            /* finally, after tokenizing and parsing, it is time for executing the command */
            execute_command(ar,bg);
        }

    }

    return 0;
}

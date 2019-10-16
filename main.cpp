#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include <bits/stdc++.h>
#include<sys/types.h>
#include <fstream>

using namespace std;

/* these two lines are used for saving the orignial logs.txt file path for later use if the user changed directories */
char * org_log_loc;
char log_file[100];

/* this method tokenizes the input line taken from the user */
int tokenize_command(char * ar [], char str []){
    /* first, we take the first token... so for example if str is equal to "ls -a", token here will be equal to "ls" */
    char *token = strtok(str, " ");
    int i=0;
    /* loop on str, and every time put it in the token... if end of str is reached, token will be NULL */
    while(token != NULL){
        /* every time we insert the string (the token) into the array at index i and then we increment i */
        ar[i++]=token;
        /* we get the other token from str, according to our example... token in the second loop will be equal to "-a", and in the third loop it is finally equals to NULL */
        token = strtok(NULL, " ");
    }
    /* at last, the array of the command and arguments must have a NULL in it as the last element, this is required by the execvp() system call */
    ar[i]=NULL;
    /* the array is as this: ar = {"ls", "-a", NULL} */
    /* this method returns the index of the last element of the array, so in the caller we can get this index and check for "&" to determine if this will be a background process */
    return i;
}

/* this function writes to the file. I created it with two parameters, the process id and the status of termination to print them in the file... although they are not used here
    because I didn't send the process id and the status of the terminated process, so they are useless here */
void write_to_file(pid_t pid, int status){
    ofstream myfile;
    myfile.open(log_file, ios_base::app);
    myfile << "Child process with id: " << "" << " terminated with status: " << "" << "\n";
    myfile.close();
}

/* this is the signal handler, this interrupt handler is called and the parent process executes it when the child process is terminated */
void sig_handler(int iSignal)
{
    //int status;
    //pid_t pid = wait(&status);
    /* I do nothing here except that I am writing to the file */
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

/* this function executes the command sent by the user */
void execute_command(char * ar[], bool bg){

    /* at first, we initialize a signal handler using the signal() function... Actually, this line of code can be written anywhere before the fork() function is called */
    signal(SIGCHLD,sig_handler);

    /* we fork() here, which means we create a child process here... this child process will run concurrently with the parent process. The parent and child processes are two
        totally different processes, and the local variables are for each one only and not seen by the other */
    /* if there is a global variable, both processes will see it normally... but if a process changes its value, the other process will be affected at all */
    pid_t process = fork();

    /* if the fork() function returned 0, then the child process is running now */
    if (process== 0){
        //printf("hello from child: %d\n", getpid());
        int is_executed = execvp(ar[0],ar);
        if(is_executed < 0)
            cout << "error: " << errno <<" this is not a command!\n";
        exit(errno);
    }
    /* else if the fork() returned a number which is greater than 0, this means that the parent process is running now and this returned integer is the id of the child process */
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

    /* here, we set the org_log_loc file vairable to the current directory and concatenate it with the file name which is logs.txt */
    org_log_loc = get_current_dir_name();
    strcat(log_file, org_log_loc);
    strcat(log_file, "/logs.txt");

    /* this is the size of the arrays and lines entered by the user, can also be done using malloc(which would be better) but this will need to be freed */
    int N = 256;

    /* this loop keeps the terminal running to keep taking inputs from the user */
    while(true){
        bool bg = false;
        char str[N];
        printf("ushell:%s~$",get_current_dir_name());
        /* this scanf takes the whole command with spaces, and it igonres the "enter" character made by the user at the end of the line */
        fgets(str, N, stdin);
        str[strlen(str)-1] = '\0';
        if(strlen(str) == 0) continue;
        /* an arbitary size of the command and arguments array which will be the parameter for the execvp system call */
        char * ar[N];
        /* after reading the line from the user, I tokenize the command into strings to insert them into the command and arguments array.
            This function returns the index of the last element in the array, to check if there's & or no later on */
        int i = tokenize_command(ar, str);
        /* after tokenizing and inserting into the array, I check if the first element in the array (i.e. the command without the arguments) is equal to exit, the program stops */
        if(strcmp(ar[0], "exit") == 0) exit(0);

        /* this is for executing the change directory "cd" command, as the execvp doesn't execute it */
        else if(strcmp(ar[0], "cd")==0){
            /* the chdir is a built-in function that changes the working directory. It returns a positive integer if operation is successfull, else, it returns a negative number */
            int is_changed = chdir(ar[1]);
            if(is_changed < 0)
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

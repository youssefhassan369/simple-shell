#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/types.h>


int TakeInput(char* array[10]){
    char command[100];
    printf("Enter command\n");
    fgets(command,sizeof(command),stdin);
    char *flag=strchr(command,'\n');
    if (flag) {
        *flag=0;
    }
    
    
    char *temp;
    temp=strtok(command," ");
    
    int i=0;
    while (temp) {
        array[i]=strdup(temp);
        temp=strtok(NULL," ");
        i++;
    }
    array[i]=NULL;
    return i-1;
}

int waitFlag(char** array, int count){
    
    return !(strcmp(array[count],"&"));
}
void DiedChild(int state){
    pid_t pid;
    
    pid=waitpid(-1,&state,WNOHANG);
    FILE *fp;
    fp=fopen("logFile.txt", "a");
    if (!fp)
        {
        printf("Error opening file\n");
        }
    else{
    
    fprintf(fp,"Child terminated, ID = %d \n",pid);
    fclose(fp);
    }
}
void printDirectory(){
    char cwd[PATH_MAX];
    getcwd(cwd,PATH_MAX);
    printf(" dir: %s\n", cwd);
}

int CheckExit(char* command){
    
    return !(strcmp(command,"exit")); //checking if command is exit
}

int CheckCD(char* command){
    
    return !(strcmp(command,"cd"));
    
}

int main(int argc, const char * argv[]) {
    signal(SIGCHLD,DiedChild); //check the change of child statues if yes function DiedChild will be called

    while (1) {
        printDirectory();

    
    char* array[10];
    int count = TakeInput(array); //taking command from user and returns index of last element
    
    if (CheckExit(array[0])) break;
        
    else if (CheckCD(array[0])) chdir(array[1]);

    
        
    else{    //other commands
    
    int backgroundFlag = waitFlag(array, count);   //checking & in command


    pid_t pid =fork();
    
    if (pid==0) {//if child
    
        int ErrorFlag=1;
        ErrorFlag=execvp(array[0],array);
        
        if(ErrorFlag<0){
        
            printf("Invaild Command\n");
        }
    }
    else{  //if parent
        
        
        if(!backgroundFlag)   //if there is no & in command will wait
            waitpid(pid, NULL, 0);
        }
        }
    }

    return 0;
}

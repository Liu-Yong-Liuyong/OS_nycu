#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void show(int fork_id) {
    printf("Fork %d. I'm the child %d, my parent is %d.\n", fork_id, getpid(), getppid());
}

int main() {
    printf("Main Process ID: %d\n\n", getpid());

    pid_t pid;
    pid = fork();//1
    if(pid == 0){
        show(1);
    }
    else if(pid > 0){
        wait(NULL);
    }
    pid = fork();//2
    if(pid == 0){
        show(2);
    }
    else if(pid > 0){
        wait(NULL);
    }
    if (pid > 0) {
        pid = fork();//3
        if(pid == 0){
            show(3);
        }else if(pid > 0){
            wait(NULL);
        }
        pid=fork();//4
        if(pid == 0){
            show(4);
        }else if(pid > 0){
            wait(NULL);
        }
    }
    return 0;
}

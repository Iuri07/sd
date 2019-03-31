#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h>
#include<math.h>
#include <time.h>
#define MSG_SIZE 11

int isPrime(int number){
    for(int i = 2; i <= sqrt(number); i ++){
        if(number % i == 0)
            return 0;
    }
    return 1;
}

void consumer(int fd_r){
    char buf[MSG_SIZE];
    int i = 1;
    while(read(fd_r, &buf, MSG_SIZE) > 0){
        int result = atoi(buf);
        if(*buf == '0'){
            printf("Closing reading pipe\n");
            close(fd_r);
            return;
        }
        printf("%d - %d%s\n", i,result, isPrime(result) ? " *" : "");
        i++;
    }
}

void producer(int fd_w, int max){
    char msg[MSG_SIZE];
    int i=0;
    srand (time(NULL));
    for(int n = 1; i < max; n = n + rand()%100){
        sprintf(msg, "%d", n);
        write(fd_w, msg, sizeof msg);
        i++;
    }

    write(fd_w, "0", sizeof msg);
    printf("Closing writing pipe\n");
    close(fd_w);
}

int main(int argc, char *argv[]){
    int fd[2], max;
    pid_t pid;

    if(argc != 2){
        printf("Wrong number of args\n  pipe $n\n");
        exit(1);
    }
    max = atoi(argv[1]);

    if(pipe(fd) == -1){
        printf("Error creating pipe\n");
        exit(1);
    }

    pid = fork();
    if(pid == -1){
        printf("Error creating fork\n");
        exit(1);
    }else if(pid == 0){ //child
        consumer(fd[0]);
    }else{ // parent
        producer(fd[1], max);
    }

}
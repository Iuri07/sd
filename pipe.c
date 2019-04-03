#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h>
#include<math.h>
#include <time.h>
#include <string.h> 

#define MSG_SIZE 10

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
    while(1){
        read(fd_r, &buf, MSG_SIZE);
        int result = atoi(buf);
        if(!strcmp(buf, "0")){
            printf("Closing reading pipe\n");
            close(fd_r);
            return;
        }
        printf("%d - %d%s\n", i,result, isPrime(result) ? " *" : "");
        i++;
    }
}

void producer(int fd_w, int iterations){
    char msg[MSG_SIZE];
    int i=0;
    srand (time(NULL));
    for(int n = 1; i < iterations; n = n + rand()%99 + 1){
        sprintf(msg, "%d", n);
        write(fd_w, msg, sizeof msg);
        i++;
    }

    write(fd_w, "0", sizeof msg);
    printf("Closing writing pipe\n");
    close(fd_w);
}

int main(int argc, char *argv[]){
    int fd[2], iterations;
    pid_t pid;

    if(argc != 2){
        printf("Usage: %s $iterations\n", argv[0]);
        exit(1);
    }
    iterations = atoi(argv[1]);

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
        producer(fd[1], iterations);
    }

}
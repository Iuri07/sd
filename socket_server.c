#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <time.h> 
#define PORT 8080 
#define MSG_SIZE 11

int main(int argc, char *argv[]) 
{ 
    int socket_fd, sock; 
    int i=0, lim; 
    struct sockaddr_in address; 
    int addrlen = sizeof(address); 
    char buffer[MSG_SIZE]; 
    char msg[MSG_SIZE];

    if(argc != 2){
        printf("Wrong number of args\n");
        exit(1);
    }
       
    lim = atoi(argv[1]);
    // Creating socket file descriptor 
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        exit(1); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address))<0){ 
        printf("Binding error\n"); 
        exit(1); 
    } 

    if (listen(socket_fd, 5) < 0){ 
        printf("Listening error\n"); 
        exit(1); 
    } 

    if ((sock = accept(socket_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){ 
        printf("Accept error\n"); 
        exit(1); 
    }
    
    srand(time(NULL));
    for(int n = 1; i < lim; n = n + rand()%100){
        sprintf(msg,"%d",n);
        send(sock , msg , MSG_SIZE, 0); 
        recv( sock , buffer, MSG_SIZE, 0); 
        printf("%d - %s\n", i+1,buffer );
        i++;
    }
    sprintf(msg,"%d", 0);
    send(sock , msg , MSG_SIZE, 0);   
    printf("Server SENT: %s\n", msg);  

  

    return 0; 
} 
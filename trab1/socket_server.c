#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <time.h> 
#define PORT 8080 
#define MSG_SIZE 10

int main(int argc, char *argv[]) 
{ 
    int socket_fd, sock; 
    int i=0, lim; 
    struct sockaddr_in server_address, client_address; 
    int client_addrlen = sizeof(client_address); 
    char buffer[MSG_SIZE]; 
    char msg[MSG_SIZE];

    if(argc != 2){
        printf("Usage: %s $iterations\n", argv[0]);
        exit(1);
    }
       
    lim = atoi(argv[1]);
    // Create socket file descriptor 
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        printf("Socket failed"); 
        exit(1); 
    } 

    //Reuse address and port; Helps with binding problem
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(int){ 1 }, sizeof(int)) < 0){
        printf("Setsockopt failed");
        exit(1);
    }
    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = INADDR_ANY; 
    server_address.sin_port = htons( PORT ); 

    if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address))<0){ 
        printf("Binding error\n"); 
        exit(1); 
    } 

    if (listen(socket_fd, 5) < 0){ 
        printf("Listening error\n"); 
        exit(1); 
    } 

    if ((sock = accept(socket_fd, (struct sockaddr *)&client_address, (socklen_t*)&client_addrlen))<0){ 
        printf("Accept error\n");   
        exit(1); 
    }

    //Producer
    
    srand(time(NULL));
    for(int n = 1; i < lim; n = n + rand()%99 + 1){
        sprintf(msg,"%d",n);
        send(sock , msg , MSG_SIZE, 0);
        recv(sock , buffer, MSG_SIZE, 0); 
        printf("%d - %d%s\n", i+1,n,buffer );
        i++;
    }
    sprintf(msg,"%d", 0);
    send(sock , msg , MSG_SIZE, 0);   
    printf("Server SENT: %s\n", msg);  

    return 0; 
} 
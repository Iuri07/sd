#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <math.h> 
#include <arpa/inet.h>
#define PORT 8080 
#define MSG_SIZE 10
   
int isPrime(int number){
    for(int i = 2; i <= sqrt(number); i ++){
        if(number % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) 
{ 
    int sock = 0, result; 
    struct sockaddr_in serv_addr;  
    char buffer[MSG_SIZE]; 
    char msg[MSG_SIZE];
    char *server_host;
    //Create socket file descriptor 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 

    if(argc != 2){
        printf("Usage: %s $server_ip\n", argv[0]);
        exit(1);
    }

    server_host = argv[1];
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons( PORT ); 
    
    //convert IPv4 address from text to binary form; Also validates IP
    if(inet_pton(AF_INET, server_host, &serv_addr.sin_addr)<=0){ 
       printf("\nInvalid address\n"); 
       return -1; 
    } 
    
    //connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
        printf("\nConnection error\n"); 
        return -1; 
    } 

    while(1){
        recv(sock , buffer, MSG_SIZE, 0); 
        printf("Client RECEIVED: %s\n", buffer);
        if(*buffer == '0'){
            printf("Closing socket.\n");  
            break;
        }
        result  = atoi(buffer);
        sprintf(msg,"%d%s", result,isPrime(result) ? " *":"");
        send(sock , msg, MSG_SIZE, 0);
    } 
} 
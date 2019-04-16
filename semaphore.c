#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define BUFFER_SIZE 32
#define M 100000

sem_t mutex, empty, full, counter, producer_counter;
int list[BUFFER_SIZE] = {0}, count = 0;

int isPrime(int number){
    for(int i = 2; i <= sqrt(number); i ++){
        if(number % i == 0)
            return 0;
    }
    return 1;
}


int get_empty(int *list){
    for(int i = 0; i < BUFFER_SIZE; i++){
        if(list[i] == 0) return i;
    }
    return -1;
}

int get_full(int *list){
    for(int i = 0; i < BUFFER_SIZE; i++){
        if(list[i] != 0) return i;
    }
    return -1;
}

void* producer(void* arg) {
    srand(time(NULL));
    while(1){
        sem_wait(&empty); 
        sem_wait(&mutex); 
        int errno = sem_trywait(&producer_counter); 
        if(errno < 0){
            sem_post(&mutex);
            sem_post(&empty);
            sem_post(&full);
            break;
        }
        int empty = get_empty(list);
        if(empty >= 0)
            list[empty] = rand() % 10000000 + 1;
        sem_post(&mutex);
        sem_post(&full);
    }
    // printf("Exiting producer\n");
} 

void* consumer(void* arg){
    while(1){ 
        char result[20]; 
        int number; 
        sem_wait(&full); 
        sem_wait(&mutex); 
        int errno = sem_trywait(&counter); 
        if(errno < 0){
            sem_post(&mutex); 
            sem_post(&empty); 
            sem_post(&full);
            break;
        }
        int full = get_full(list);
        if(full >= 0){
            number = list[full];
            list[full] = 0;
            count++;
        }
        sem_post(&mutex); 
        sem_post(&empty);
        // sprintf(result,"%d%s\n", number, isPrime(number) ? " é primo" : "");
        // write(1,result,strlen(result));
    }
    // printf("Exiting consumer\n");
} 

int main(int argc, char *argv[]){
    int nthreads_consumer[9] = {1,2,4,8,16,1,1,1,1};
    int nthreads_producer[9] = {1,1,1,1,1,2,4,8,16};
    
    for(int i=0; i < 9; i++){
        char file[30];
        sprintf(file, "results/4n%d-np%d-nc%d.txt" ,BUFFER_SIZE,nthreads_producer[i], nthreads_consumer[i]);
        
        FILE *fptr = fopen(file, "w");
        if (fptr == NULL){ 
            printf("Error opening file"); 
            return 0; 
        } 


    
        for(int j = 0; j < 10; j++){
            printf("%d\n", j);
            sem_init(&mutex, 0, 1); 
            sem_init(&empty, 0, BUFFER_SIZE); 
            sem_init(&counter, 0, M); 
            sem_init(&producer_counter, 0, M); 
            sem_init(&full, 0, 0); 
            pthread_t producers[nthreads_producer[i]], consumers[nthreads_consumer[i]]; 

            clock_t begin = clock();

            for(int k = 0; k < nthreads_consumer[i]; k++)
                pthread_create(&consumers[k],NULL,consumer,NULL);
            for(int k = 0; k < nthreads_producer[i]; k++)
                pthread_create(&producers[k],NULL,producer,NULL);  
            for(int k = 0; k < nthreads_producer[i]; k++)
                pthread_join(producers[k],NULL); 
            for(int k = 0; k < nthreads_consumer[i]; k++)
                pthread_join(consumers[k],NULL); 

            sem_destroy(&mutex); 
            sem_destroy(&full); 
            sem_destroy(&counter); 
            sem_destroy(&empty); 
            clock_t end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            fprintf(fptr,"%f\n", time_spent);
            printf("tempo: %f\n", time_spent);
        }
        fclose(fptr);
    }
    return 0; 
}
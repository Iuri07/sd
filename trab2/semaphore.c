#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define BUFFER_SIZE 2
#define M 100000

#define BILLION  1E9

sem_t mutex, empty, full, consumer_count, producer_count;
int list[BUFFER_SIZE] = {0};

int isPrime(int number){
    if (number <= 1) 
        return 0;
    if (number % 2 == 0 && number > 2) 
        return 0;
    for(int i = 3; i < sqrt(number); i +=2){
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
    while(1){
        int errno = sem_trywait(&producer_count); 
        if(errno < 0){
            break;
        }else{
            int random = rand() % 10000000 + 1;
            sem_wait(&empty); 
            sem_wait(&mutex); 
            int empty = get_empty(list);
            if(empty >= 0)
                list[empty] = random;
            sem_post(&mutex);
            sem_post(&full);
        }
    }
    return NULL;
} 

void* consumer(void* arg){
    int number = 0; 
    while(1){ 
        int errno = sem_trywait(&consumer_count); 
        if(errno < 0){
            break;
        }else {
            sem_wait(&full); 
            sem_wait(&mutex); 
            int full = get_full(list);
            if(full >= 0){
                number = list[full];
                list[full] = 0;
            }
            sem_post(&mutex); 
            sem_post(&empty);
            if(number > 0){
                int prime = isPrime(number);
                // if(prime) printf("primo: %d\n", number);
                number = 0;
            }
        }
    }
    return NULL;
} 

int main(int argc, char *argv[]){
    int nthreads_consumer[9] = {1,2,4,8,16,1,1,1,1};
    int nthreads_producer[9] = {1,1,1,1,1,2,4,8,16};
    srand(time(NULL));
    
    for(int i=0; i < 9; i++){
        double total_time = 0.0;
        
        for(int j = 0; j < 10; j++){
            sem_init(&mutex, 0, 1); 
            sem_init(&full, 0, 0); 
            sem_init(&empty, 0, BUFFER_SIZE); 
            sem_init(&consumer_count, 0, M); 
            sem_init(&producer_count, 0, M); 


            pthread_t producers[nthreads_producer[i]], consumers[nthreads_consumer[i]]; 

            struct timespec start, stop;
            // clock_t begin = clock();
            clock_gettime( CLOCK_MONOTONIC, &start);

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
            sem_destroy(&consumer_count); 
            sem_destroy(&producer_count); 
            sem_destroy(&empty);
            // clock_t end = clock();
            clock_gettime( CLOCK_MONOTONIC, &stop);
            double time_spent = ( stop.tv_sec - start.tv_sec ) + (( stop.tv_nsec - start.tv_nsec ) / BILLION);
            total_time += time_spent;
        }
        printf("%lf\n", total_time/10);
    }
    return 0; 
}
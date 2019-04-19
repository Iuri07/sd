/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: luisfernandolins
 *
 * Created on April 9, 2019, 11:44 AM
 */

#include <cstdlib>
#include <ctime>    // For time()
#include <iostream>       // std::cout
#include <atomic>         // std::atomic_flag
#include <thread>         // std::thread
#include <vector>         // std::vector
#include <ctime>        // std::clock

using namespace std;

#define N 10000000 // tamanho do vetor
//#define K 16 // número de threads

//bool flag = false;
//
//bool test_and_set(bool flag) {
//   bool anterior = flag;
//   flag = true;
//   return anterior;
//}
//
//void acquire(bool flag) {
//   while(test_and_set(flag)){};
//}
//void release(bool flag) {
//   flag = false;
//}

atomic_flag lock_stream = ATOMIC_FLAG_INIT;

void acquire(){
    while (lock_stream.test_and_set()) {}
}

void release(){
    lock_stream.clear();
}

void accumulator(const vector<int> &v, int &acm, 
                            int beginIndex, int endIndex)
    { 
    
   int sum = 0;
   
    for (unsigned int i = beginIndex; i < endIndex; i++)
    {
        
        sum += v[i];
        
    }
   
   acquire();
   acm += sum;
   release();    
    
    
    }

void print(std::vector<int> const &input)
{
	for (int i = 0; i < input.size(); i++) {
		std::cout << input.at(i) << ' ';
	}
        
        cout<< "\n";
}

/*
 * 
 */
int main(int argc, char** argv) {

for (int K = 1; K<=256; K=K*2){    
float avgtime = 0; 
vector<int> v;
v.reserve(N); // reserva um tamanho N para o vetor
srand(time(NULL)); // inicializa o gerador de número aleatório

for (int i = 0; i<10; i++){     
    
    v = {};
//    vector<int> test = {1,2,3,4,5};
    signed char r; // números com 1 byte só
    int acm = 0;
    thread myThreads[K+1];
 
    for (int i=0; i<N; i++){
        r = (rand() % 201) - 100;
        v.push_back(r);
    }

const clock_t begin_time = clock();

    for (int i=0; i<K; i++){
        myThreads[i] = thread(accumulator, ref(v), ref(acm), (N/K)*i, (N/K)*(i+1));
    }
    if (N%K != 0){
        myThreads[K] = thread(accumulator, ref(v), ref(acm), (N/K)*K, N);
    }

    
    for (int i=0; i<K; i++){
        myThreads[i].join();
    }
if (N%K != 0){
        myThreads[K].join();
    }
    
//    print(v);
    avgtime += float( clock () - begin_time ) /  CLOCKS_PER_SEC;
    
    cout << acm << "\n"; 
    acm = 0;
//    accumulator(ref(v), ref(acm), 0, N); // teste sem usar multithreading
//    cout << acm;
}

cout << "Tempo médio de execução para " << K << " thread(s): " << avgtime/10 << "\n";
}
    
    return 0;
}


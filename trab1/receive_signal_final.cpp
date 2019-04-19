/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: luisfernandolins
 *
 * Created on March 20, 2019, 10:23 AM
 */

#include <cstdlib>
#include <iostream>

using namespace std;

/*
 * 
 */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
 
    void signal_handler1(int); 
    void signal_handler2(int); 
    void signal_handler3(int); 



int main(int argc, char** argv) {
    
    void (*hand_pt1)(int);
    void (*hand_pt2)(int);
    void (*hand_pt3)(int);
    hand_pt1 = &signal_handler1;
    hand_pt2 = &signal_handler2;
    hand_pt3 = &signal_handler3;
    int waittype;

//    char* siginput = argv[1];
//    int sig = atoi( siginput );
    int release = 0;
        cout << "What type of waiting do you want?\nType 1 for busy waiting and 2 for blocking waiting: ";
        cin >> waittype;
        while(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Bad entry.  Enter a NUMBER: ";
            cin >> waittype;
        }    
        cout << "\n";
    
    if (waittype==1){ //busy waiting
       while(release==0){
            signal(SIGHUP, hand_pt1); 
            signal(SIGINT, hand_pt2); 
            signal(SIGQUIT, hand_pt3); 
       }
       }  
        
    
    
    if (waittype==2){ //blocking waiting
        while(release==0){
            signal(SIGHUP, hand_pt1); 
            signal(SIGINT, hand_pt2); 
            signal(SIGQUIT, hand_pt3); 
            
            sleep(100000);
       }
        
    }
        
    else{
        cout << "Input not recognized.\n";
    }
    
    return 0;
}

void signal_handler1(int signal)
{
    cout << "I have received a SIGHUP\n"; 
}

void signal_handler2(int signal)
{
    cout << "I have received a SIGINT\n"; 
}

void signal_handler3(int signal)
{
    cout << "I am being killed.\n"; 
    exit(0);
}

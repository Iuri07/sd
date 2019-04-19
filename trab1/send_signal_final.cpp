/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: luisfernandolins
 *
 * Created on March 20, 2019, 9:51 AM
 */

#include <cstdlib>
#include <iostream>

using namespace std;
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits>

/*
 * 
 */

    int send_signal (int pid, int sig=SIGHUP)
    {
        int ret;
        ret = kill(pid,sig);
        cout << "Signal sent successfully.\n";
        return ret;
    }
    
int main(int argc, char** argv) {
    
    int pid;
    int sig;
    int release = 0;
    string keepgoing;
    
    while (release==0){
        cout << "What process do you want to send a signal to?: ";
        cin >> pid;
        cout << "\nWhat signal do you want to send?\nType 1 for SIGHUP, 2 for SIGINT or 3 for SIGQUIT.\n";
        cin >> sig;
        while(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Bad entry.  Enter a NUMBER: ";
            cin >> sig;
        }
        
    //    if (argc>2){
    //        char* siginput = argv[2];
    //        sig = atoi( siginput );
    //    }

        if (kill(pid, 0) < 0)
        {
            cout << "\nProcess does not exist.\n";
            exit(EXIT_FAILURE);
        }
//
        if (sig==1 || sig==2 || sig==3){
            send_signal(pid,sig);
            cout << "\nDo you want to send another one? If so, type y: ";
            cin >> keepgoing;
            cout << "\n";
            if (keepgoing == "y" || keepgoing == "Y"){
                release = 0;
            }
            else{
                release = 1;
            }
            
        }
        
        else{
            cout << "Incorrect input. Try Again.\n\n";
            release=0;
        }
    }

    
}
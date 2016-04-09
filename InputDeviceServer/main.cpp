/* 
 * File:   main.cpp
 * Author: takeshi
 *
 * Created on April 6, 2016, 4:49 PM
 */

#include <stdint.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Application.h"

using namespace std;

int main(int argc, char** argv) {
    cout << "Hello" << endl;
    
    Application *application = Application::getInstance();
    application->start();
           
    // wait until any key input
    char x;
    cin >> x;   
    //while(1);
    
    // exit threads    
    application->stopRendezvous();
    
    cout << "Bye" << endl;
    return 0;
}


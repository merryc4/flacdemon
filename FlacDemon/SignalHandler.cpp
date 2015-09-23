//
//  SignalHandler.cpp
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "SignalHandler.h"

using namespace std;

void SignalHandler::setSignalHandler(){
    if(signal(SIGHUP, SignalHandler::signalHandler) == SIG_ERR){
        //handle error
        cout << "Error setting signal handler";
    }
}
void SignalHandler::signalHandler(int signum){
    cout << "received signal " << signum << endl;
    exit(signum);
}
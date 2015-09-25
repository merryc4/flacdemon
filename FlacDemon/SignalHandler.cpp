//
//  SignalHandler.cpp
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "SignalHandler.h"

using namespace std;

SignalHandler::SignalHandler() {
    this->signalMap = new std::map<const char *, FD_SIGNAL*>;
}
SignalHandler::~SignalHandler() {
    
}

void SignalHandler::setSystemSignalHandler(){
    if(signal(SIGHUP, SignalHandler::systemSignalHandler) == SIG_ERR){
        //handle error
        cout << "Error setting signal handler";
    }
}
void SignalHandler::systemSignalHandler(int signum){
    cout << "received signal " << signum << endl;
    exit(signum);
}
FD_SIGNAL * SignalHandler::signals(const char *signalName, bool returnNull) const {
    FD_SIGNAL * sig = NULL;
    if(this->signalMap->count(signalName) == 0){
        if(!returnNull){
            sig = new FD_SIGNAL;
            this->signalMap->insert(std::pair<const char *, FD_SIGNAL*>(signalName, sig));
        }
    } else {
        sig = this->signalMap->find(signalName)->second;
    }
    return sig;
}
void SignalHandler::call(const char * signalName, void * arg) const{
    FD_SIGNAL * sig = this->signals(signalName, true);
    if(sig){
        (*sig)(signalName, arg);
    }
}
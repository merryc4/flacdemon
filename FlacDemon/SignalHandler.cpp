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
    this->signalMap = new std::map<const char *, fd_signal*>;
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
fd_signal * SignalHandler::signals(const char *signalName, bool returnNull) const {
    fd_signal * sig = nullptr;
    if(this->signalMap->count(signalName) == 0){
        if(!returnNull){
            sig = new fd_signal;
            this->signalMap->insert(std::pair<const char *, fd_signal*>(signalName, sig));
        }
    } else {
        sig = this->signalMap->find(signalName)->second;
    }
    return sig;
}
void SignalHandler::call(const char * signalName, void * arg) const{
    fd_signal * sig = this->signals(signalName, true);
    if(sig){
        (*sig)(signalName, arg);
    }
}
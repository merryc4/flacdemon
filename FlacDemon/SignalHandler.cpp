/***********************************************************************
 * SignalHandler.cpp : Boost signals management
 * part of FlacDemon
 ************************************************************************
 *  Copyright (c) 2016 Meriadoc Clarke.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

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
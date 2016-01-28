//
//  SessionManager.cpp
//  FlacDemon
//
//  Created by merryclarke on 27/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "SessionManager.h"
SessionManager::SessionManager(){
    
}
SessionManager::~SessionManager(){
    
}
void SessionManager::setSession(std::thread::id threadid, FlacDemon::Session * session){
    std::lock_guard<std::mutex> lock(this->sessionMutex);
    this->sessions.insert(std::pair<std::thread::id, FlacDemon::Session * >(threadid, session));
}
FlacDemon::Session * SessionManager::getSession(std::thread::id threadid){
    std::lock_guard<std::mutex> lock(this->sessionMutex);
    FlacDemon::Session * session = this->sessions.at(threadid);
    return session;
}
void SessionManager::newSession(){
    FlacDemon::Session * session = new FlacDemon::Session();
    std::thread::id threadid = std::this_thread::get_id();
    this->setSession(threadid, session);
}
void SessionManager::setString(std::string * key, std::string * value){
    FlacDemon::Session * session = this->getSession(std::this_thread::get_id());
    if(session){
        
    }
}
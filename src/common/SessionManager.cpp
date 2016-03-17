/***********************************************************************
 * SessionManager.cpp : Session management
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

#include "SessionManager.h"
SessionManager::SessionManager(){
    
}
SessionManager::~SessionManager(){
    
}
void SessionManager::setSession(std::thread::id threadid, FlacDemon::Session * session){
    std::lock_guard<std::mutex> lock(this->sessionMutex);
    this->sessions.insert(std::pair<std::thread::id, FlacDemon::Session * >(threadid, session));
}
FlacDemon::Session * SessionManager::getSession(){
    return this->getSession(std::this_thread::get_id());
}
FlacDemon::Session * SessionManager::getSession(std::thread::id threadid){
    std::lock_guard<std::mutex> lock(this->sessionMutex);
    if(!(this->sessions.count(threadid))){
        this->sessionMutex.unlock();
        this->newSession();
    }
    FlacDemon::Session * session = this->sessions.at(threadid);
    return session;
}
void SessionManager::newSession(){
    FlacDemon::Session * session = new FlacDemon::Session();
    std::thread::id threadid = std::this_thread::get_id();
    this->setSession(threadid, session);
}
void SessionManager::destroySession(){
    FlacDemon::Session * session = this->getSession();
    if(session){
        for(std::map< std::thread::id, FlacDemon::Session * >::iterator it = this->sessions.begin(); it != this->sessions.end(); it++){
            if (it->second == session) {
                this->sessions.erase(it);
                break;
            }
        }
    }
    delete session;
}
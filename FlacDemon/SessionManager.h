//
//  SessionManager.h
//  FlacDemon
//
//  Created by merryclarke on 27/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef __FlacDemon__SessionManager__
#define __FlacDemon__SessionManager__

#include "includes.h"
#include "Session.h"
#include "FlacDemonNameSpace.h"

class SessionManager {
private:
    std::map< std::thread::id, FlacDemon::Session * > sessions;
    std::mutex sessionMutex;
    
    void setSession(std::thread::id threadid, FlacDemon::Session * session);
    
public:
    SessionManager();
    ~SessionManager();
    
    FlacDemon::Session * getSession();
    FlacDemon::Session * getSession(std::thread::id threadid);
    void newSession();
    void destroySession();
    
};

#endif /* defined(__FlacDemon__SessionManager__) */

/***********************************************************************
 * SessionManager.h : Session management
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

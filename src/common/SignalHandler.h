/***********************************************************************
 * SignalHandler.h : Boost signals management
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

#ifndef __FlacDemon__SignalHandler__
#define __FlacDemon__SignalHandler__

#include <iostream>
#include <csignal>
#include <boost/signals2.hpp>

typedef boost::signals2::signal<void (const char *, void *)> fd_signal;

class SignalHandler {
protected:
    
public:
    SignalHandler();
    ~SignalHandler();
    void setSystemSignalHandler();
    static void systemSignalHandler(int signum);
        
    std::map<const char *, fd_signal*> * signalMap;
    
    fd_signal * signals(const char * signalName, bool returnNull = false) const; // return signal for key
    void call(const char * signalName, void * ) const;
};

#endif /* defined(__FlacDemon__SignalHandler__) */

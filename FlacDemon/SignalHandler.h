//
//  SignalHandler.h
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__SignalHandler__
#define __FlacDemon__SignalHandler__

#include <iostream>
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

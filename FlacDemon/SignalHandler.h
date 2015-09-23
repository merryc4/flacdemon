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

class SignalHandler {
protected:
    
public:
    SignalHandler();
    ~SignalHandler();
    void setSignalHandler();
    static void signalHandler(int signum);
};

#endif /* defined(__FlacDemon__SignalHandler__) */

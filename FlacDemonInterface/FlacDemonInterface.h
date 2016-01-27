//
//  FlacDemonInterface.h
//  FlacDemon
//
//  Created by merryclarke on 02/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef __FlacDemon__FlacDemonInterface__
#define __FlacDemon__FlacDemonInterface__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <string>


#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "ncurses.h"

class FlacDemonInterface{
private:
    WINDOW *browser;
    
protected:
    
public:
    FlacDemonInterface();
    ~FlacDemonInterface();
    void initialize();
    void connect();
    void run();
    void printLibrary(int offset);
    void printLibraryHeaders();
};

#endif /* defined(__FlacDemon__FlacDemonInterface__) */

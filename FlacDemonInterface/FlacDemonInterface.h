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
#include <thread>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "ncurses.h"

#include "FlacDemonUtils.h"

using std::cout;
using std::endl;

class FlacDemonInterface{
private:
    int socketFileDescriptor;
    std::thread * readThread;
//    std::mutex socketMutex;
    
    WINDOW *browser;
    
    
    int maxColumns;
    int maxRows;
    int browserRows;
protected:
    
public:
    FlacDemonInterface();
    ~FlacDemonInterface();
    void initialize();
    void connect();
    void run();
    void printLibrary(int offset);
    void printLibraryHeaders();
    void printLibraryLine(std::vector< std:: string > * values);
    void sendCommand(const char * command);
    void readResponse();
    void parseResponse(std::string response);
};

#endif /* defined(__FlacDemon__FlacDemonInterface__) */

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
#include "TrackListing.h"

#define fd_interface_printlibrary 1 << 0
#define fd_interface_libraryupdate 1 << 1
#define fd_interface_printcommand 1 << 2

using std::cout;
using std::endl;

class FlacDemonInterface{
private:
    int socketFileDescriptor;
    std::thread * readThread;
    std::thread * retryConnectThread;
//    std::mutex socketMutex;
    std::mutex eventMutex;
    
    WINDOW *browser;
    WINDOW *search;
    
    unsigned long flags;
    
    int maxColumns;
    int maxRows;
    int browserRows;
    
    bool fetchedLibrary;
    
    std::map< std::string, FlacDemon::TrackListing * > tracks;
    std::string command;
protected:
    
public:
    FlacDemonInterface();
    ~FlacDemonInterface();
    void initialize();
    void connect();
    void retryConnect();
    void onConnect();
    void run();
    void event(unsigned long rlflags = 0);
    void setRunLoopFlags(unsigned long flags);
    void userInputLoop();
    void printLibrary(int offset);
    void printLibraryHeaders();
    void printLibraryLine(std::vector< std:: string > * values);
    void sendCommand(const char * command);
    void readResponse();
    void parseResponse(std::string response);
    std::string parseCommandFromResponse(std::string * response);
    const char * formatValue(std::string value, int max);
    void parseLibraryUpdate(std::string * response);
    void libraryUpdate(fd_keymap_vector * values);
    void updateTrackListing(fd_keymap * ikeymap);
    void printCommand();
};

#endif /* defined(__FlacDemon__FlacDemonInterface__) */

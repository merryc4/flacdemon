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
#include "Library.h"

#define fd_interface_printlibrary   1 << 0
#define fd_interface_libraryupdate  1 << 1
#define fd_interface_printcommand   1 << 2
#define fd_interface_playing        1 << 3
#define fd_interface_printplaying   1 << 4
#define fd_interface_playbackprogress 1 << 5
#define fd_interface_printprogress  1 << 6

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
    WINDOW *commandWindow;
    WINDOW* playbackWindow;
    
    FlacDemon::Library library;
    
    unsigned long flags;
    
    int maxColumns;
    int maxRows;
    int browserRows;

    size_t commandCursorPosition;
    size_t commandCursorDefault;
    
    std::string commandPrompt;
    
    bool fetchedLibrary;
    bool killResponseThread;
    bool isSearch;
    bool typeSearch;
    
    FlacDemon::TrackListing * nowPlaying;
    float progress;
    
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
    bool checkCommand(std::string * command);
    void parseCommand(std::string * iCommand);
    void sendCommand(const char * command);
    void readResponse();
    void parseResponse(std::string response);
    std::string parseCommandFromResponse(std::string * response);
    std::string removeCommandFromResponse(std::string * response);
    const char * formatValue(std::string value, int max);
    void parseLibraryUpdate(std::string * response);
    void setNowPlaying(std::string ID);
    void printNowPlaying();
    void printCommand();
    void setCommandCursor();
    void printProgress();
    void waitForSearch();
};

#endif /* defined(__FlacDemon__FlacDemonInterface__) */

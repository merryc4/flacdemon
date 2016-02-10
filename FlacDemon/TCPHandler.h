//
//  TCPHandler.h
//  FlacDemon
//
//  Created by merryclarke on 02/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef __FlacDemon__TCPHandler__
#define __FlacDemon__TCPHandler__

#include "FlacDemonAll.h"
#include "Track.h"

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


class FlacDemon::TCPHandler{
private:
    std::thread * acceptThread;
    bool socketClosedByRead;
    std::map < int , bool > openSockets;
protected:
public:
    //variables:
    std::vector<std::string> * commands;
    bool commandAvailable;
    bool threadSync;
    //methods:
    TCPHandler();
    ~TCPHandler();
    void initialize();
    void setSignals();
    void runAcceptLoop(int sockfd);
    void messageReceiverLoop(int sockfd);
    int writeResponseForCommand(int sockfd, const char * command, std::string * results);
    int write(int sockfd, const char * message);
    void addCommand(char * messageBuffer);
    void trackPlayingHandler(const char * signal, void * arg);
};

#endif /* defined(__FlacDemon__TCPHandler__) */

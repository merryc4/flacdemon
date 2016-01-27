//
//  TCPHandler.h
//  FlacDemon
//
//  Created by merryclarke on 02/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef __FlacDemon__TCPHandler__
#define __FlacDemon__TCPHandler__

#include "FlacDemonNameSpace.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


class FlacDemon::TCPHandler{
private:
    std::thread * acceptThread;
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
    void runAcceptLoop(int sockfd);
    void messageReceiverLoop(int sockfd);
    void addCommand(char * messageBuffer);
};

#endif /* defined(__FlacDemon__TCPHandler__) */

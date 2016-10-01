/***********************************************************************
 * TCPHandler.h : Network and socket management
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


#ifndef __FlacDemon__TCPHandler__
#define __FlacDemon__TCPHandler__

#include "FlacDemonAll.h"
#include "Track.h"

#include "netincludes.h"

#define FD_ALL_SOCKETS 0


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
    void join();
    void setSignals();
    void runAcceptLoop(int sockfd);
    void messageReceiverLoop(int sockfd);
    int writeResponseForCommand(int sockfd, const char * command, std::string * results);
    int write(int sockfd, const char * message);
    void addCommand(char * messageBuffer);
    void trackPlayingHandler(const char * signal, void * arg);
    void playbackUpdateHandler(const char  * signal, void * arg);
};

#endif /* defined(__FlacDemon__TCPHandler__) */

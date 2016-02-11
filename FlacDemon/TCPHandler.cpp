//
//  TCPHandler.cpp
//  FlacDemon
//
//  Created by merryclarke on 02/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "TCPHandler.h"

FlacDemon::TCPHandler::TCPHandler(){
    //init
    this->acceptThread = nullptr;
    this->commands = new std::vector<std::string>;
    this->commandAvailable = false;
    this->threadSync = false;
    this->socketClosedByRead = false;
    
    this->setSignals();
}
FlacDemon::TCPHandler::~TCPHandler(){
    //delete
}
void FlacDemon::TCPHandler::initialize(){
    //make lots of this configurable in settings
    struct sockaddr_in server_address;
    int port = 8995;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        std::cout << "Error opening socket for tcp" << std::endl;
        return;
    }
    int enable = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    bzero((char*) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd, (const struct sockaddr * )&server_address, sizeof(server_address)) < 0){
        std::cout << "Error binding socket:" << strerror(errno) << std::endl;
        //detail of error
        return;
    }
    listen(sockfd, 5);
    this->acceptThread = new std::thread(&TCPHandler::runAcceptLoop, this, sockfd);
    
    // some kind of accept thread loop?
}
void FlacDemon::TCPHandler::setSignals(){
    auto f = boost::bind(&FlacDemon::TCPHandler::trackPlayingHandler, this, _1, _2);
    signalHandler->signals("playingTrack")->connect(f);
    
    f = boost::bind(&FlacDemon::TCPHandler::playbackUpdateHandler, this, _1, _2);
    signalHandler->signals("playbackUpdate")->connect(f);
}
void FlacDemon::TCPHandler::trackPlayingHandler(const char * signal, void * arg){
    FlacDemon::Track * track = (FlacDemon::Track * )arg;
    std::string id = *track->valueForKey("id");
    this->writeResponseForCommand(FD_ALL_SOCKETS, "playing", &id);
}
void FlacDemon::TCPHandler::playbackUpdateHandler(const char *signal, void *arg){
    float * progress = (float * )arg;
    std::string strprogress = std::to_string(*progress);
    this->writeResponseForCommand(FD_ALL_SOCKETS, "playbackProgress", &strprogress);
}
void FlacDemon::TCPHandler::runAcceptLoop(int sockfd){
    struct sockaddr_in client_address;
    socklen_t client_length;
    int newsockfd;
    
    client_length = sizeof(client_address);
    std::cout << "waiting for tcp connection" << std::endl;
    do {
        if((newsockfd = accept(sockfd, (struct sockaddr * )&client_address, &client_length)) < 0){
            std::cout << "Error on tcp socket accept" << std::endl;
            return;
        }
        std::cout << "Socket connection accepted" << std::endl;
        this->openSockets.insert(std::pair < int , bool > (newsockfd, true));
        new std::thread(&TCPHandler::messageReceiverLoop, this, newsockfd);
    } while(1); //check server socket is still open
}
void FlacDemon::TCPHandler::messageReceiverLoop(int sockfd){
    ssize_t n;
    char buffer[256];
    
    sessionManager->newSession();
    
    do{ //check socket is still open
        bzero(buffer,256);
        
        if ((n = recv(sockfd, buffer, sizeof(buffer), 0)) <= 0){
            std::cout << "ERROR reading from socket" << std::endl;
            this->socketClosedByRead = true;
            continue;
        }
        
        std::cout << "Command from socket: " << buffer << std::endl;
        this->addCommand(buffer);
        std::string * results = sessionManager->getSession()->getString(buffer);
//        cout << *results << endl;

        if((n = this->writeResponseForCommand(sockfd, buffer, results)) < 0){
            continue;
        }
        //write response?
    } while (n > 0);
    std::cout << "socket disconnected" << std::endl;
    this->openSockets.erase(sockfd);
    sessionManager->destroySession();
}
int FlacDemon::TCPHandler::writeResponseForCommand(int sockfd, const char * command, std::string * results){
    std::string response;
    int n;
    response.assign("\"command\":\"");
    response.append(command);
    response.append("\"\n");
    if(results)
        response.append(*results);
    
    response.append("--data-end--");
    
    const char * cresponse = response.c_str();
    cout << cresponse << endl;
    if(sockfd == FD_ALL_SOCKETS){
        for(std::map < int , bool >::iterator it = this->openSockets.begin(); it != this->openSockets.end(); it++){
            sockfd = it->first;
            if((n = this->write(sockfd, cresponse)) < 0){
                cout << "Error writing response" << endl;
            }
        }
    } else {
        if((n = this->write(sockfd, cresponse)) < 0){
            cout << "Error writing response" << endl;
        }
    }
    return n;
}
int FlacDemon::TCPHandler::write(int sockfd, const char * message){
    int n;
    if ((n = send(sockfd,message,strlen(message), 0)) < 0){
        std::cout << "ERROR writing to socket" << std::endl;
    }
    return n;
}
void FlacDemon::TCPHandler::addCommand(char * messageBuffer){
    signalHandler->call("runCommand", messageBuffer);
}
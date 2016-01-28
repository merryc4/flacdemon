//
//  FlacDemonInterface.cpp
//  FlacDemon
//
//  Created by merryclarke on 02/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "FlacDemonInterface.h"

std::vector< std::string > * libraryTitles = new std::vector< std::string >{"Track", "Title", "Album", "Artist"};

FlacDemonInterface::FlacDemonInterface(){
    //init
//    this->initialize();
}
FlacDemonInterface::~FlacDemonInterface(){
    //de-init
}
void FlacDemonInterface::initialize(){
    
    
    //redirect cout to log file
    std::ofstream * out = new std::ofstream("fd_interface.log");
    std::cout.rdbuf(out->rdbuf());

    initscr();

    cbreak();
    noecho();
    keypad(stdscr, true);
    refresh(); //clears screen and sets scroll to correct position
    
    char msg[] = "FlacDemon NCURSES Interface";
    
    
    getmaxyx(stdscr, this->maxRows, this->maxColumns);
    
    WINDOW * titleWindow = newwin(1, this->maxColumns, 0, 0);
    WINDOW * searchWindow = newwin(1, this->maxColumns, 1, 0);
    
    mvwprintw(titleWindow, 0, (this->maxColumns - strlen(msg)) / 2, "%s", msg);
    wrefresh(titleWindow);
    
    mvwprintw(searchWindow, 0, 0, "Search:");
    wrefresh(searchWindow);
    
    this->browser = newwin(this->maxRows - 2, this->maxColumns, 2, 0);
}
void FlacDemonInterface::connect(){
    int sockfd, port;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    port = 8995;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERROR opening socket" << std::endl;
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);
    if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        std::cout << "ERROR connecting" << std::endl;
        return;
    }
    this->socketFileDescriptor = sockfd;
    std::cout << "Connected to server" << std::endl;

}
void FlacDemonInterface::sendCommand(const char * command){
    if(this->socketFileDescriptor < 0){
        return;
    }
    ssize_t n = send(this->socketFileDescriptor,command,strlen(command), 0);
    if (n < 0){
        std::cout << "ERROR writing to socket" << std::endl;
        //end socket
        close(this->socketFileDescriptor);
        this->socketFileDescriptor = -1;
    }
}
void FlacDemonInterface::readResponse(){
    ssize_t n;
    char buffer[256];
    std::string response="";
    std::string response2="";
    size_t pos;
    do{
        bzero(buffer,256);
        if ((n = recv(this->socketFileDescriptor, buffer, sizeof(buffer), 0)) < 0){
            cout << "ERROR reading from socket" << std::endl;
            continue;
        }
        response.append(buffer, sizeof(buffer));
        if((pos = response.find("--data-end--")) != std::string::npos){
            cout << response.length() << ":" << (pos + 12) << endl;
            if(response.length() > (pos + 12)){
                response2 = response.substr((pos + 12));
            } else
                response2 = "";
            
            response.erase(pos);
            this->parseResponse(response);
            response = response2;
        }
    }while(n>0);
}
void FlacDemonInterface::parseResponse(std::string response){
    cout << response << endl;
    fd_keymap_vector * results = fd_jsontokeymap_vector(&response);
    for(fd_keymap_vector::iterator it = results->begin(); it != results->end(); it++){
        FlacDemon::TrackListing * trackListing = new FlacDemon::TrackListing(*it);
        this->tracks.push_back(trackListing);
        cout << "added track to listings" << endl;
    }
}

void FlacDemonInterface::run(){
    this->readThread = new std::thread(&FlacDemonInterface::readResponse, this);
    this->sendCommand("get all");
    char c;
    char buf[256];
    do{
        this->printLibrary(0);
        c = getchar();
    }while(c != 'c');
}
void FlacDemonInterface::printLibrary(int offset = 0){
    cout << "printing library" << endl;
    this->browserRows=1; //title always first row
    this->printLibraryHeaders();
    std::string key;
    for(std::vector< FlacDemon::TrackListing * >::iterator it = this->tracks.begin(); it != this->tracks.end(); it++){
        std::vector < std::string > values;
        for(std::vector< std::string >::iterator it2 = libraryTitles->begin(); it2 != libraryTitles->end(); it2++){
            key = (*it2);
            fd_standardiseKey(&key);
            values.push_back(*(*it)->valueForKey(&key));
        }
        this->printLibraryLine(&values);
    }
    
    wrefresh(this->browser);
}
void FlacDemonInterface::printLibraryHeaders(){
    char title[] = "Library";
    mvwprintw(this->browser, 0, (this->maxColumns - strlen(title)) / 2, title);
    this->printLibraryLine(libraryTitles);
}
void FlacDemonInterface::printLibraryLine(std::vector<std::string> *values){
    int width = (this->maxColumns / values->size());
    int position = 0;
    this->browserRows++;
    
    cout << "printing line " << this->browserRows << endl;
    
    for(std::vector< std::string >::iterator it = values->begin(); it != values->end(); it++){
        const char * val = this->formatValue(*it, width);
        mvwprintw(this->browser, this->browserRows, position, val);
        position+=width;
        mvwprintw(this->browser, this->browserRows, position, "|");
        position++;
    }
}
const char * FlacDemonInterface::formatValue(std::string value, int max){
    if(value.length() > max){
        value = value.substr(0, max-2);
        value.append("..");
    }
    return value.c_str();
}
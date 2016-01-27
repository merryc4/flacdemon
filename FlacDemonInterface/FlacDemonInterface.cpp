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
    this->initialize();
}
FlacDemonInterface::~FlacDemonInterface(){
    //de-init
}
void FlacDemonInterface::initialize(){
    
    
    //redirect cout to log file
    std::ofstream * out = new std::ofstream("flacdemon.log");
    std::cout.rdbuf(out->rdbuf());

    initscr();

    raw();
    noecho();
    keypad(stdscr, true);
    refresh(); //clears screen and sets scroll to correct position
    
    char msg[] = "FlacDemon NCURSES Interface";
    
    
    int row,col;
    getmaxyx(stdscr, row, col);
    
    WINDOW * titleWindow = newwin(1, col, 0, 0);
    WINDOW * searchWindow = newwin(1, col, 1, 0);
    
    mvwprintw(titleWindow, 0, (col - strlen(msg)) / 2, "%s", msg);
    wrefresh(titleWindow);
    
    mvwprintw(searchWindow, 0, 0, "Search:");
    wrefresh(searchWindow);
    
    this->browser = newwin(row - 2, col, 2, 0);
}
void FlacDemonInterface::connect(){
    int sockfd, port;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[256];
    
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
    std::cout << "Connected to server" << std::endl;

//    while(1){
//        bzero(buffer,256);
//        fgets(buffer,255,stdin);
//        ssize_t n = send(sockfd,buffer,strlen(buffer), 0);
//        if (n < 0){
//            std::cout << "ERROR writing to socket" << std::endl;
//            break;
//        }
//    }
//    close(sockfd);
}
void FlacDemonInterface::run(){
    this->printLibrary(0);
    char c;
    do{
        c = getch();
    }while(c != 'c');
}
void FlacDemonInterface::printLibrary(int offset = 0){
    this->printLibraryHeaders();
}
void FlacDemonInterface::printLibraryHeaders(){
    mvwprintw(this->browser, 0, 0, "Library");
}
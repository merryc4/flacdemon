//
//  FlacDemonInterface.cpp
//  FlacDemon
//
//  Created by merryclarke on 02/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "FlacDemonInterface.h"

FlacDemonInterface::FlacDemonInterface(){
    //init
}
FlacDemonInterface::~FlacDemonInterface(){
    //de-init
}
void FlacDemonInterface::initialize(){
    
    //redirect cout to log file
    std::ofstream * out = new std::ofstream("flacdemon.log");
    std::cout.rdbuf(out->rdbuf());
        
    raw();
    noecho();
    keypad(stdscr, true);
    
    char msg[] = "FlacDemon NCURSES Interface";
    
    WINDOW * w = initscr();
    
    int row,col;
    getmaxyx(stdscr, row, col);
    
    WINDOW * titleWindow = newwin(1, col, 0, 0);
    WINDOW * searchWindow = newwin(1, col, 1, 0);
    
    mvwprintw(titleWindow, 0, (col - strlen(msg)) / 2, "%s", msg);
    wrefresh(titleWindow);
    
    mvwprintw(searchWindow, 0, 0, "Search:");
    wrefresh(searchWindow);
    
    

//    mvprintw(row / 2, (col - strlen(msg)) / 2, "%s", msg);

    int ch = getchar();
    waddch(w, ch);
    refresh();
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
    if (::connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        std::cout << "ERROR connecting" << std::endl;
    getchar(); //pauses program
//    bzero(buffer,256);
//    fgets(buffer,255,stdin);
//    n = write(sockfd,buffer,strlen(buffer));
//    if (n < 0)
//        error("ERROR writing to socket");
//    bzero(buffer,256);
//    n = read(sockfd,buffer,255);
//    if (n < 0)
//        error("ERROR reading from socket");
//    printf("%s\n",buffer);
//    close(sockfd);

}
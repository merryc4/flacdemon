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
//
//  ncursesHandler.cpp
//  FlacDemon
//
//  Created by merryclarke on 03/12/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "ncursesHandler.h"

FlacDemon::NCURSESHandler::NCURSESHandler(){
    //init
}
FlacDemon::NCURSESHandler::~NCURSESHandler(){
    //de-init
}
void FlacDemon::NCURSESHandler::initialize(){
    
    //redirect cout to log file
    std::ofstream * out = new std::ofstream("flacdemon.log");
    std::cout.rdbuf(out->rdbuf());
    
    demon->interfaceMode = interfaceModeCurses;
    
    raw();
    noecho();
    keypad(stdscr, true);
    
    WINDOW * w = initscr();
    wprintw(w, "Enter A Command Or Search:\n");
    refresh();
    int ch = getchar();
    waddch(w, ch);
    refresh();
}
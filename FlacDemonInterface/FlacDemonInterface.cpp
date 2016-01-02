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
    
    WINDOW * w = initscr();
    wprintw(w, "Enter A Command Or Search:\n");
    refresh();
    int ch = getchar();
    waddch(w, ch);
    refresh();
}
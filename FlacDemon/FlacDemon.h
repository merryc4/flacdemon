//
//  FlacDemon.h
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__FlacDemon__
#define __FlacDemon__FlacDemon__

#include "FlacDemonAll.h"
#include "CommandParser.h"
#include "ncursesHandler.h"
#include "FileImporter.h"
#include "Database.h"
#include "Player.h"
#include "TCPHandler.h"
#include "Session.h"

#include "tests.h"

enum flacdemonInterfaceMode {
    interfaceModeTerminal = 0,
    interfaceModeCurses,
    interfaceModeGUI
};

using namespace std;

class FlacDemon::Demon {
    friend FlacDemon::CommandParser;
private:

protected:
    vector<string>* commands;
    FlacDemon::TCPHandler * tcpHandler;
    FlacDemon::CommandParser* commandParser;
    FlacDemon::FileImporter* fileImporter;
    FlacDemon::Player * player;
public:
    int flags;
    int interfaceMode;
    
    FlacDemon::Database* database;

    Demon();
    ~Demon();
    
    void run();
    int add(vector<string>* args);
    int play(vector<string>* args);
    int stop(vector<string>* args);
    int set(vector<string>* args);
    int get(vector<string>* args);
};

#endif /* defined(__FlacDemon__FlacDemon__) */

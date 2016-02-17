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
#include "includes.h"
#include "tests.h"

#include "CommandMap.h"

enum flacdemonInterfaceMode {
    interfaceModeTerminal = 0,
    interfaceModeCurses,
    interfaceModeGUI
};

using namespace std;

class FlacDemon::Demon {
    friend FlacDemon::CommandParser;
private:
    
    std::map< std::string, demonCommandFunction > * commandMap;
    
    void callCommandHandler( const char * signal, void * arg );

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
    int add( fd_stringvector * args);
    int play( fd_stringvector * args);
    int stop( fd_stringvector * args);
    int set( fd_stringvector * args);
    int get( fd_stringvector * args);
};

#endif /* defined(__FlacDemon__FlacDemon__) */

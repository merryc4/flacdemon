//
//  FlacDemon.h
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__FlacDemon__
#define __FlacDemon__FlacDemon__

#include "FlacDemonNameSpace.h"
#include "CommandParser.h"
#include "FileImporter.h"
#include "Database.h"
#include "Player.h"

#include "tests.h"

using namespace std;

class FlacDemon::Demon {
protected:
    vector<string>* commands;
    FlacDemon::CommandParser* commandParser;
    FlacDemon::FileImporter* fileImporter;
    FlacDemon::Database* database;
    FlacDemon::Player * player;
    
public:
    Demon();
    ~Demon();
    
    void run();
    int add(vector<string>*);
    int play(vector<string>*);
    int stop(vector<string>*);
    int set(vector<string>*);
};

#endif /* defined(__FlacDemon__FlacDemon__) */

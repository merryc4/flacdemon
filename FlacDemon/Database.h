//
//  Database.h
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__Database__
#define __FlacDemon__Database__

#include <iostream>
#include <sqlite3.h>
#include <boost/format.hpp>

#include "FlacDemonNameSpace.h"
//#include "Group.h"
#include "File.h"
#include "Track.h"

typedef std::map<std::string, const unsigned char *> fd_sqlresults;

class FlacDemon::Database {
protected:
    
    struct SQLStatements {
        const char * createTableTracksFormat = "create table if not exists `tracks` (id INTEGER PRIMARY KEY AUTOINCREMENT, %s)";
        char * fields = NULL;
        char * addTrackFormat = (char *)"insert into tracks (%s) values(%s)";
    } sql_statements;
    
    std::vector<std::string> * metakeys;
    std::vector<std::string> * trackinfokeys;
    std::vector<std::string> * allkeys;
    
    std::vector<FlacDemon::Track *> * openTracks = new std::vector<FlacDemon::Track *>;
    
    sqlite3_stmt * sqlSelectStatment = NULL;
    
    sqlite3 * openDB();
    void closeDB(sqlite3 * db);
    void runSQL(const char * sql, int (*callback)(void*,int,char**,char**) = NULL, void * arg = NULL); //add return code
    fd_keymap * sqlSelect(std::string * sql);
    fd_keymap * sqlSelect(const char * sql);
    void clearSelect();
    
    void initDB();
    
public:
    Database();
    ~Database();
    
    void initSignals();
    void signalReceiver(const char * signalName, void * arg);
    void addAlbumDirectory(FlacDemon::File * albumDirectory);
    void add(FlacDemon::File * file);
    void add(FlacDemon::Track* track);
    
    FlacDemon::Track * trackForID(long ID);
    FlacDemon::Track * trackWithKeyMap(fd_keymap * keyMap);
};

#endif /* defined(__FlacDemon__Database__) */


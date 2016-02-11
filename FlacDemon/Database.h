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

#include "FlacDemonAll.h"
//#include "Group.h"
#include "File.h"
#include "TrackListing.h"
#include "Track.h"

typedef std::map<std::string, const unsigned char *> fd_sqlresults;

class FlacDemon::Database {
protected:
    
    std::vector<std::string> signalFuns {"addAlbumDirectory", "runSQL"};
    
    std::vector<std::string> * metakeys;
    std::vector<std::string> * trackinfokeys;
    std::vector<std::string> * allkeys;
    
    std::vector<FlacDemon::Track *> * openTracks = new std::vector<FlacDemon::Track *>;
    
    sqlite3_stmt * sqlSelectStatment = nullptr;
    
    std::string * currentWorkingDirectory = nullptr;
    
    sqlite3 * openDB();
    void closeDB(sqlite3 * db);
    void runSQL(std::string * sql, int (*callback)(void*,int,char**,char**) = nullptr, void * arg = nullptr); //add return code
    void runSQL(const char * sql, int (*callback)(void*,int,char**,char**) = nullptr, void * arg = nullptr); //add return code
    fd_keymap_vector * sqlSelect(std::string * sql);
    fd_keymap_vector * sqlSelect(const char * sql);
    fd_keymap * sqlSelectRow(std::string * sql);
    fd_keymap * sqlSelectRow(const char * sql);
    
    int sqlCount(std::string * sql);
    int sqlCount(const char * sql);
    void clearSelect();
    std::string * sqlSelectOne(std::string * isql);

    
    void initDB();
    
public:
    Database();
    ~Database();
    
    void initSignals();
    void signalReceiver(const char * signalName, void * arg);
    void addAlbumDirectory(FlacDemon::File * albumDirectory);
    void add(FlacDemon::File * file);
    void add(FlacDemon::Track* track);
    
    FlacDemon::Track * trackForID(unsigned long ID);
    FlacDemon::Track * trackWithKeyMap(fd_keymap * keyMap);
    std::string * albumDirectoryUUIDForPath(std::string * path);
    
    int hasEntryForFile(std::string * filepath, const char * table);
    
    std::string * getUUID();
    
    int setValue(unsigned long ID, std::string * key, std::string * value);
    std::string * getValue(unsigned long ID, std::string * key);
    std::string * getJSONForID(int uid);
    std::string * getAll();
    
    void fillDatabase(int entries);
};

#endif /* defined(__FlacDemon__Database__) */


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
        char * fields = nullptr;
        char * addTrackFormat = (char *)"insert into tracks (%s) values(%s)";
        const char * createTableFiles = "create table if not exists `associate_files` (id INTEGER PRIMARY KEY AUTOINCREMENT, filepath varchar(255), albumuuid varchar(255))";
        char * addFileFormat = (char *)"insert into associate_files (filepath, albumuuid) values(%s)";
        char * setValueFormat = (char *)"update tracks set %s=`%s` where id=%lu";
    } sql_statements;
    
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
    fd_keymap * sqlSelect(std::string * sql);
    fd_keymap * sqlSelect(const char * sql);
    
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
};

#endif /* defined(__FlacDemon__Database__) */


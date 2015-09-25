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

#include "FlacDemonNameSpace.h"
//#include "Group.h"
#include "File.h"
#include "Track.h"

class FlacDemon::Database {
protected:
    
    struct SQLStatements {
        const char * createTableTracks = "create table if not exists `tracks` (id int, Number int, Title varchar(255), AlbumArtist varchar(255), Artist varchar(255), Album varchar(255), Genre varchar(255), Composer varchar(255), Disc unsigned int, Time unsigned int, PlayCount unsigned int, DateAdded unsigned int)";
        
    } sql_statements;
    
    sqlite3 * openDB();
    void closeDB(sqlite3 * db);
    void runSQL(const char * sql, int (*callback)(void*,int,char**,char**) = NULL, void * arg = NULL); //add callback
    
    void initDB();
    
public:
    Database();
    ~Database();
    
    void initSignals();
    void signalReceiver(const char * signalName, void * arg);
    void addAlbumDirectory(FlacDemon::File * albumDirectory);
    void add(FlacDemon::File * file);
    void add(FlacDemon::Track* track);
};

#endif /* defined(__FlacDemon__Database__) */


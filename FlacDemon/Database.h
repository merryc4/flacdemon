/***********************************************************************
 * Database.h : SQLite Database management
 * part of FlacDemon
 ************************************************************************
 *  Copyright (c) 2016 Meriadoc Clarke.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

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
#include "Album.h"

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
    std::string sqlSelectOne(std::string * isql);

    
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
    FlacDemon::Album * albumForID( unsigned long ID );
    std::string albumDirectoryUUIDForPath(std::string * path);
    
    int hasEntryForFile(std::string * filepath, const char * table);
    
    std::string getUUID();
    
    int setValue(unsigned long ID, std::string * key, std::string * value);
    std::string getValue(unsigned long ID, std::string * key);
    std::string getJSONForID(int uid);
    std::string getAll();
    
    
    void fillDatabase(int entries);
};

#endif /* defined(__FlacDemon__Database__) */


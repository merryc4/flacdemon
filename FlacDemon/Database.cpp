//
//  Database.cpp
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "Database.h"

FlacDemon::Database::Database(){
    this->initDB();
}
FlacDemon::Database::~Database(){
    
}
void FlacDemon::Database::initDB(){
    sqlite3 * db = this->openDB();
    if(!db)
        return;
    char * err = NULL;
    
    sqlite3_exec(db, this->sql_statements.createTableTracks, NULL, NULL, &err);
    
    if(err){
        cout << err << endl;
        free(err);
    }
}

void FlacDemon::Database::addAlbumDirectory(FlacDemon::File *albumDirectory){
    
}
void FlacDemon::Database::add(FlacDemon::Track *track){
    
}
sqlite3 * FlacDemon::Database::openDB(){
    sqlite3 * db = NULL;
    if(sqlite3_open("flacdemon.db", &db) != SQLITE_OK){
        cout << "error opening sqlite3 database, err code: " << sqlite3_errcode(db) << ". err msg: " << sqlite3_errmsg(db);
    }
    return db;
}
void FlacDemon::Database::closeDB(sqlite3* db){
    sqlite3_close(db);
}
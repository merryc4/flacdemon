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
    this->initSignals();
}
FlacDemon::Database::~Database(){
    
}
void FlacDemon::Database::initSignals(){
    auto f = boost::bind(&FlacDemon::Database::signalReceiver, this, _1, _2);
    signalHandler->signals("addAlbumDirectory")->connect(f);
    
}
void FlacDemon::Database::signalReceiver(const char * signalName, void * arg){
    cout << "signal received: " << signalName << endl;
    if(strcmp(signalName, "addAlbumDirectory")){
        this->addAlbumDirectory(static_cast<FlacDemon::File *>(arg));
    }
}
void FlacDemon::Database::addAlbumDirectory(FlacDemon::File *albumDirectory){
    std::cout << "add album directory: " << *albumDirectory->path << endl;
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
void FlacDemon::Database::runSQL(const char *sql,int (*callback)(void*,int,char**,char**), void * arg){
    sqlite3 * db = this->openDB();
    if(!db)
        return;
    char * err = NULL;
    
    sqlite3_exec(db, sql, callback, arg, &err);
    
    if(err){
        cout << err << endl;
        free(err);
    }
    this->closeDB(db);
}
void FlacDemon::Database::initDB(){
    this->runSQL(this->sql_statements.createTableTracks, NULL);
}
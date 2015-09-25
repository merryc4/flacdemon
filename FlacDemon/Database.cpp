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
    if(strcmp(signalName, "addAlbumDirectory")==0){
        this->addAlbumDirectory(static_cast<FlacDemon::File *>(arg));
    }
}
void FlacDemon::Database::addAlbumDirectory(FlacDemon::File *albumDirectory){
    std::cout << "add album directory: " << *albumDirectory->path << endl;
    std::vector<FlacDemon::File*> * files = albumDirectory->getMediaFiles();
    for(std::vector<FlacDemon::File*>::iterator it = files->begin(); it != files->end(); it++){
        this->add(*it);
    }
}
void FlacDemon::Database::add(FlacDemon::File * file){
    this->add(file->track);
}
void FlacDemon::Database::add(FlacDemon::Track *track){
    cout << "adding track: " << *track->file->path << endl;
    const char * tracknum, *title, *artist, *albumartist, *album, *genre, *composer, *disc;
    uint time=0, playcount=0, dateadded=0;
    
    tracknum = track->file->getMetaDataEntry("track");
    title = track->file->getMetaDataEntry("title");
    artist = track->file->getMetaDataEntry("artist");
    albumartist = track->file->getMetaDataEntry("albumartist");
    album = track->file->getMetaDataEntry("album");
    genre = track->file->getMetaDataEntry("genre");
    composer = track->file->getMetaDataEntry("composer");
    disc = track->file->getMetaDataEntry("disc");
    
    int size = snprintf(NULL, 0, "%s", track->file->getMetaDataEntry("Title"));
    char * sql = (char *)malloc(size + 1);
    sprintf(sql, "%s", track->valueForKey<const char *>("Title"));
    cout << sql << endl;
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
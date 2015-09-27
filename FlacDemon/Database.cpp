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
    
    track->initInfo();
    
    std::string sql="'";
    for(std::vector<std::string>::iterator it = this->allkeys->begin(); it != this->allkeys->end(); it++){
        sql.append(track->valueForKey(&(*it)));
        sql.append("','");
    }
    sql.pop_back();
//    for(std::vector<std::string>::iterator it = this->trackinfokeys->begin(); it != this->trackinfokeys->end(); it++){
//        sql.append(track->valueForKey<const char*>((*it).c_str()));
//        sql.append(",");
//    }
    sql.pop_back();
    
    std::string sql2 = this->sql_statements.addTrackFormat;
    size_t pos = sql2.find("%s");
    sql2.erase(pos, 2);
    sql2.insert(pos, sql);
    
    cout << sql2.c_str() << endl;
    this->runSQL(sql2.c_str());
//    this->runSQL("insert into tracks (track,title,albumartist,artist,album,genre,composer,disc,tracktime,playcount,dateadded) values('01','The Fusion','Dusty Fingers','Jason Havelock','Dusty Fingers','','','00','','','')");
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
    this->runSQL("drop table `tracks`");
    
    std::string sql="";
    std::string fields="";
    for(std::vector<std::string>::iterator it = this->metakeys->begin(); it != this->metakeys->end(); it++){
        sql.append((*it));
        sql.append(" varchar(255),");
        
        fields.append((*it));
        fields.append(",");
        
        this->allkeys->push_back(*(new std::string((*it))));
    }
    for(std::vector<std::string>::iterator it = this->trackinfokeys->begin(); it != this->trackinfokeys->end(); it++){
        sql.append((*it));
//        sql.append(" unsigned int,");
        sql.append(" varchar(255),");

        
        fields.append((*it));
        fields.append(",");
        
        this->allkeys->push_back(*(new std::string((*it)))); //messy, use vector of string pointers intstead
    }
    
    sql.pop_back();
    fields.pop_back();
    
    std::string sql2 = this->sql_statements.createTableTracksFormat;
    size_t pos = sql2.find("%s");
    sql2.erase(pos, 2);
    sql2.insert(pos, sql);
    
    cout << sql2 << endl;
    this->runSQL(sql2.c_str(), NULL);
    
    std::string addTrackSQL = this->sql_statements.addTrackFormat;
    pos = addTrackSQL.find("%s");
    addTrackSQL.erase(pos, 2);
    addTrackSQL.insert(pos, fields);
    this->sql_statements.addTrackFormat = new char [addTrackSQL.length() + 1];
    std::strcpy(this->sql_statements.addTrackFormat, addTrackSQL.c_str());
    
    this->sql_statements.fields = new char [fields.length() + 1];
    std::strcpy(this->sql_statements.fields, fields.c_str());
    
}




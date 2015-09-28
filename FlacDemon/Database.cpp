//
//  Database.cpp
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "Database.h"

FlacDemon::Database::Database(){
//    this->sqlSelectStatment = NULL;

    this->allkeys = new std::vector<std::string>;

    this->metakeys = new std::vector<std::string>{
        "track",
        "title",
        "albumartist",
        "artist",
        "album",
        "genre",
        "composer",
        "disc"
    };
    this->trackinfokeys = new std::vector<std::string>{
        "tracktime",
        "playcount",
        "dateadded",
        "filepath"
    };
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
    if(sqlite3_open_v2("flacdemon.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK){
        cout << "error opening sqlite3 database, err code: " << sqlite3_errcode(db) << ". err msg: " << sqlite3_errmsg(db);
    }
    return db;
}
void FlacDemon::Database::closeDB(sqlite3* db){
    sqlite3_close_v2(db);
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
FD_SQLRESULTS * FlacDemon::Database::sqlSelect(std::string *sql){
    return this->sqlSelect(sql->c_str());
}
FD_SQLRESULTS * FlacDemon::Database::sqlSelect(const char *sql){
    sqlite3_stmt * stmt;
    if(this->sqlSelectStatment == NULL){
        if(sql == NULL)
            return NULL;
        sqlite3 * db = this->openDB();
        if(!db)
            return NULL;
        if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK){
            //error
            cout << "Error occured while preparing sql statement: " << sql << endl;
            return NULL;
        }
        this->sqlSelectStatment = stmt;
    } else {
        stmt = this->sqlSelectStatment;
    }
    FD_SQLRESULTS * results = new FD_SQLRESULTS;
    int stepCode, columns, i;
    const unsigned char * value = NULL;
    const char * key = NULL;
    if ((stepCode = sqlite3_step(stmt)) == SQLITE_ROW) {
        columns = sqlite3_column_count(stmt);
        for (i = 0; i < columns; i++) {
            if((value = sqlite3_column_text(stmt, i)) && (key = sqlite3_column_name(stmt, i))){
                results->insert(std::pair<std::string, const unsigned char *>(*new std::string(key), value)); //will need to free the key
            }
            
        }
    } else {
        if(stepCode != SQLITE_DONE){
            //error occurred, check result code
            cout << "Error stepping through sql select" << endl;
        }
        this->clearSelect();
    }

    return results;
}
void FlacDemon::Database::clearSelect(){
    if(this->sqlSelectStatment != NULL){
        sqlite3 * db = sqlite3_db_handle(this->sqlSelectStatment);
        sqlite3_finalize(this->sqlSelectStatment);
        this->closeDB(db);
        this->sqlSelectStatment = NULL;
    }
}
void FlacDemon::Database::initDB(){
//    this->runSQL("drop table if exists `tracks`");
    
    std::string sql="";
    std::string fields="";
    for(std::vector<std::string>::iterator it = this->metakeys->begin(); it != this->metakeys->end(); it++){
        sql.append((*it));
        sql.append(" varchar(255),");
        
        fields.append((*it));
        fields.append(",");
        
        this->allkeys->push_back(*new std::string((*it)));

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

FlacDemon::Track * FlacDemon::Database::trackForID(long ID){
    std::string sql = "select * from tracks where id=";
    sql.append(std::to_string(ID));
    FD_SQLRESULTS * results = this->sqlSelect(&sql);
    this->clearSelect();
    if(results->count("filepath")){
        cout << "found track for id " << ID << endl;
        return this->trackWithKeyMap(results);
    }
    return NULL;
}
FlacDemon::Track * FlacDemon::Database::trackWithKeyMap(FD_SQLRESULTS *keyMap){
    FlacDemon::Track * track = new FlacDemon::Track(keyMap);
    
    return NULL;
}



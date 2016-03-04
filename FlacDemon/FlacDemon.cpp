//
//  FlacDemon.cpp
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "FlacDemon.h"

FlacDemon::Demon::Demon() {
	cout << "New FlacDemon\n";
    
    this->flags = 0;
    av_register_all();
    
    this->commandParser = new FlacDemon::CommandParser();
    this->fileImporter = new FlacDemon::FileImporter();
    this->database = new FlacDemon::Database();
    this->player = new FlacDemon::Player();
    this->tcpHandler = new FlacDemon::TCPHandler();
    this->scraper = new FlacDemon::Scraper();
    
    this->player->setDatabase(this->database);
    
    this->setTargetMap( this , fd_commandmap < FlacDemon::Demon > {
        flacdemon_command( FlacDemon::Demon , "play" , play ),
        flacdemon_command( FlacDemon::Demon , "stop" , stop ),
        flacdemon_command( FlacDemon::Demon , "set" , set ),
        flacdemon_command( FlacDemon::Demon , "get" , get ),
        flacdemon_command( FlacDemon::Demon , "add" , add )
    });

}
FlacDemon::Demon::~Demon() {
	cout << "Deleting FlacDemon\n";
}

void FlacDemon::Demon::run() {
#ifdef DEBUG
    run_tests();
#endif
    this->tcpHandler->initialize();
	while (true) {
        this->commandParser->getCommand();
        
		usleep(100); //revise sleep length
	}
}

int FlacDemon::Demon::add( fd_stringvector * args ){
    cout << "add some files" << endl;
    if( args->size() < 3 ) return -1;
    std::string path;
    for(vector<string>::iterator it = (args->begin() + 2); it != args->end(); it++){
        path = *it;
        this->fileImporter->importFilesFromPath(&path);
    }
    return 0;
}
int FlacDemon::Demon::play(fd_stringvector * args){
    
    cout << "play some tunes" << endl;
    long ID = 1;
    if(args && args->size() > 2){
        ID = std::strtol((*args)[2].c_str(), nullptr, 0);
    }
    this->player->playTrackWithID(ID);
    return 0;
}
int FlacDemon::Demon::stop(fd_stringvector * args){
    cout << "stop playback" << endl;
    this->player->stop();
    return 0;
}
int FlacDemon::Demon::set(fd_stringvector * args){
    if(args->size() < 5){
        cout << "command error: incorrect arguments" << endl;
        return 1;
    }

    int id;
    if(fd_stringtoint(&(*args)[2], &id)){
        cout << "command error : unknown id: " << (*args)[0] << endl;
        return 1;
    }
    std::string metaTagName = (*args)[3];
    std::string metaTagValue = (*args)[4];
    
    this->database->setValue(id, &metaTagName, &metaTagValue);
    
    return 0;
}
int FlacDemon::Demon::get(fd_stringvector * args){
    std::string results;
    if (strcmp(args->back().c_str(), "all") == 0) {
        //get all
        results = this->database->getAll();
    }
    else {
        if(args->size() < 3){
            cout << "command error: incorrect arguments" << endl;
            return 1;
        }
        
        int id;
        if(fd_stringtoint(&(*args)[1], &id)){
            cout << "command error : unknown id: " << (*args)[0] << endl;
            return 1;
        }
        std::string metaTagName = (*args)[2];
        results = this->database->getValue(id, &metaTagName);
    }

    sessionManager->getSession()->setString(&((*args)[0]), &results);
    
    return 0;
}

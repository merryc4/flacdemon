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
    
    this->player->setDatabase(this->database);
    
    this->commandParser->setMapForDemon(this, new std::map<string, demonCommandFunction>{
        {"add", &FlacDemon::Demon::add},
        {"play", &FlacDemon::Demon::play},
        {"stop", &FlacDemon::Demon::stop},
        {"set", &FlacDemon::Demon::set},
        {"get", &FlacDemon::Demon::get}
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
int FlacDemon::Demon::add(vector<string> * args){
    cout << "add some files" << endl;
    for(vector<string>::iterator it = (args->begin() + 1); it != args->end(); it++){
        std::string * path = new std::string(*it);
        this->fileImporter->importFilesFromPath(path);
        delete path;
    }
    return 0;
}
int FlacDemon::Demon::play(vector<string> * args){
    
    cout << "play some tunes" << endl;
    long ID = 1;
    if(args && args->size() > 1){
        ID = std::strtol((*args)[1].c_str(), nullptr, 0);
    }
    this->player->playTrackWithID(ID);
    return 0;
}
int FlacDemon::Demon::stop(vector<string> * args){
    cout << "stop playback" << endl;
    this->player->stop();
    return 0;
}
int FlacDemon::Demon::set(vector<string> * args){
    if(args->size() < 4){
        cout << "command error: incorrect arguments" << endl;
        return 1;
    }

    int id;
    if(fd_stringtoint(&(*args)[1], &id)){
        cout << "command error : unknown id: " << (*args)[0] << endl;
        return 1;
    }
    std::string metaTagName = (*args)[2];
    std::string metaTagValue = (*args)[3];
    
    this->database->setValue(id, &metaTagName, &metaTagValue);
    
    return 0;
}
int FlacDemon::Demon::get(vector<string> * args){
    std::string * results;
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

    sessionManager->getSession()->setString(&((*args)[0]), results);
    
    return 0;
}

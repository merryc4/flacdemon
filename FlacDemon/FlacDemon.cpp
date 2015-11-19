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
    
    av_register_all();
    
    this->commandParser = new FlacDemon::CommandParser();
    this->fileImporter = new FlacDemon::FileImporter();
    this->database = new FlacDemon::Database();
    this->player = new FlacDemon::Player();
    
    this->player->setDatabase(this->database);
    
    this->commandParser->setMapForDemon(this, new std::map<string, demonCommandFunction>{
        {"add", &FlacDemon::Demon::add},
        {"play", &FlacDemon::Demon::play},
        {"stop", &FlacDemon::Demon::stop}
    });
}
FlacDemon::Demon::~Demon() {
	cout << "Deleting FlacDemon\n";
}

void FlacDemon::Demon::run() {
	while (true) {
        this->commandParser->getCommand();
        
		usleep(100); //revise sleep length
	}
}

int FlacDemon::Demon::add(vector<string> * args){
    cout << "add some files" << endl;
    for(vector<string>::iterator it = args->begin(); it != args->end(); it++){
        cout << *it << endl;
        string path = *it;
        this->fileImporter->importFilesFromPath(&path);
    }
    return 0;
}
int FlacDemon::Demon::play(vector<string> * args){
    cout << "play some tunes" << endl;
    long ID = 1;
    if(args && args->size()){
        ID = std::strtol((*args)[0].c_str(), NULL, 0);
    }
    this->player->playTrackWithID(ID);
    return 0;
}
int FlacDemon::Demon::stop(vector<string> * args){
    cout << "stop playback" << endl;
    this->player->stop();
    return 0;
}

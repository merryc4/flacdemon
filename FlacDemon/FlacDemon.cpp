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
    
    this->commandParser->setMapForDemon(this, new std::map<string, demonCommandFunction>{
        {"add", &FlacDemon::Demon::add},
        {"play", &FlacDemon::Demon::play}
    });
}
FlacDemon::Demon::~Demon() {
	cout << "Deleting FlacDemon\n";
}

void FlacDemon::Demon::run() {
	while (true) {
        this->commandParser->getCommand();
        
		sleep(1); //revise sleep length
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
    return 0;
}

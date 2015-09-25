//
//  FileImporter.cpp
//  FlacDemon
//
//  Created by merryclarke on 23/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "FileImporter.h"

FlacDemon::FileImporter::FileImporter(){
//    signalHandler->signals("sig1")->connect(boost::bind(&FlacDemon::FileImporter::sigtest, this, _1));
//    signalHandler->call("sig1");
}
FlacDemon::FileImporter::~FileImporter(){
    
}
void FlacDemon::FileImporter::sigtest(const char * signalName){
    cout << "hello world\n";
}
void FlacDemon::FileImporter::importFilesFromPath(string *path){
	cout << "Importing files from " << path << endl;
    FlacDemon::File * file = new FlacDemon::File(path);
    vector<FlacDemon::File*> * albumDirectories = file->getAlbumDirectories();
    
    cout << "------------------ ALBUM DIRECTORIES ------------------" << endl;
    for(vector<FlacDemon::File*>::iterator it = albumDirectories->begin(); it != albumDirectories->end(); it++){
        cout << *(*it)->path << endl;
    }
    cout << "-------------------------------------------------------" << endl;
    
    for(vector<FlacDemon::File*>::iterator it = albumDirectories->begin(); it != albumDirectories->end(); it++){
        signalHandler->call("addAlbumDirectory", (*it));
    }
}










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
    std::vector<FlacDemon::File*> * albumDirectories = file->getAlbumDirectories(-1);
    
    cout << "------------------ ALBUM DIRECTORIES ------------------" << endl;
    flacdemon_loop_all_files(albumDirectories){
        cout << *(*it)->filepath << endl;
    }
    cout << "-------------------------------------------------------" << endl;
    
    flacdemon_loop_all_files(albumDirectories){
        signalHandler->call("addAlbumDirectory", (*it));
    }
    
    std::vector<FlacDemon::File*> * noneAlbumFiles = file->getNoneAlbumFiles(-1);
    
    cout << "------------------ NONE ALBUM FILES ------------------" << endl;
    flacdemon_loop_all_files(noneAlbumFiles){
        cout << *(*it)->filepath << endl;
    }
    cout << "-------------------------------------------------------" << endl;

    
    //need to free memory allocated in getAlbumDirectories
    
}










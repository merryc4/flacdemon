//
//  FileImporter.cpp
//  FlacDemon
//
//  Created by merryclarke on 23/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "FileImporter.h"

FlacDemon::FileImporter::FileImporter(){
    auto f = boost::bind(&FlacDemon::FileImporter::addFileHandler, this, _1, _2);
    signalHandler->signals("addFile")->connect(f);
}
FlacDemon::FileImporter::~FileImporter(){
    
}
void FlacDemon::FileImporter::importFilesFromPath(string *path){
	cout << "Importing files from " << *path << endl;
    cout << "Searching..." << endl;
    FlacDemon::File * file = new FlacDemon::File(path);
    
    cout << "counts for path " << file->filepath << endl;
    cout << "immediate regular files: " << file->immediateChildFileCount << endl;
    cout << "immediate directories: " << file->immediateChildDirectoryCount << endl;
    cout << "immediate all files: " << file->immediateChildCount << endl;
    cout << "total regular files: " << file->totalChildFileCount << endl;
    cout << "total directories: " << file->totalChildDirectoryCount << endl;
    cout << "total all files: " << file->totalChildCount << endl;
    
    if( ( file = file->parse() )){ //could maybe just return a boolean to indicate whether it should be added
        this->addFile(file);
        delete file;
    }
}
void FlacDemon::FileImporter::addFileHandler(const char * signal, void * arg){
    this->addFile(( FlacDemon::File * )arg);
}
void FlacDemon::FileImporter::addFile(FlacDemon::File *file){
    fd_filevector * files = file->getAlbumDirectories(-1);
    cout << "------------------ ALBUM DIRECTORIES ------------------" << endl;
    flacdemon_loop_all_files(files){
        cout << *(*it)->filepath << endl;
    }
    cout << "-------------------------------------------------------" << endl;
    
    flacdemon_loop_all_files(files){
        signalHandler->call("addAlbumDirectory", (*it));
    }
    
    delete files;
    
    std::vector<FlacDemon::File*> * noneAlbumFiles = file->getNoneAlbumFiles(-1);
    
    cout << "------------------ NONE ALBUM FILES ------------------" << endl;
    flacdemon_loop_all_files(noneAlbumFiles){
        cout << *(*it)->filepath << endl;
    }
    cout << "-------------------------------------------------------" << endl;
    
    delete noneAlbumFiles;
}









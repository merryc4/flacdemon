/***********************************************************************
 * FileImporter.cpp : Import files into database
 * part of FlacDemon
 ************************************************************************
 *  Copyright (c) 2016 Meriadoc Clarke.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

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









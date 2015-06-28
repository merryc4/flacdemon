//
//  FileImporter.h
//  FlacDemon
//
//  Created by merryclarke on 23/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__FileImporter__
#define __FlacDemon__FileImporter__

#include <iostream>
#include "FlacDemonNameSpace.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

struct directory {
    string* path;
    AVCodecID codecID;
    bool isAlbumDirectory;
    bool hasMediaFiles;
};


class FlacDemon::FileImporter {
protected:
    
public:
    void importFilesFromPath(string*);
    directory* parseDirectory(string*);
    void addFile(string*);
    int isDirectory(string*);
    int isDirectory(const char*);
    
    int readMediaFile(string*, AVCodecID*);
    int readMediaFile(const char*, AVCodecID*);
};


#endif /* defined(__FlacDemon__FileImporter__) */

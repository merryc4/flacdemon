//
//  File.h
//  FlacDemon
//
//  Created by merryclarke on 28/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__File__
#define __FlacDemon__File__

#include <stdio.h>
#include "FlacDemonNameSpace.h"
#include "FlacDemonFileSystem.h"

#define FLACDEMON_FILE_IS_MEDIA         1
#define FLACDEMON_DIRECTORY_IS_ALBUM    2
#define FLACDEMON_FILE_IS_DIRECTORY     4

class FlacDemon::File {
private:
    string * path;
public:

    string * type;
    AVCodecID codecID;
    AVDictionary * metadata;
    vector<FlacDemon::File*> *files;
    int error;
    bool exists;
    
    unsigned int flags;
    
    File(string* path = NULL);
    ~File();
    
    string* getPath();
    void setPath(string*);
    
    void parse();
    void addFile(FlacDemon::File *);
    
    bool checkExists(struct stat * buffer = NULL);
    bool checkDirectory();
    
    bool isDirectory();
    bool isMediaFile();
    bool isAlbumDirectory();
    
    int readMediaInfo();
    
    void setToDirectory(bool setToDir);
    
    void printMetaDataDict(AVDictionary* dict);
    
};

#endif /* defined(__FlacDemon__File__) */


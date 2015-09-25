//
//  File.h
//  FlacDemon
//
//  Created by merryclarke on 28/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//


#ifndef _FlacDemonMetaDataMultipleValues
#define _FlacDemonMetaDataMultipleValues

extern const char * FlacDemonMetaDataMultipleValues;

#endif

#ifndef __FlacDemon__File__
#define __FlacDemon__File__

#include <stdio.h>
#include <iostream>
#include <regex>
#include "FlacDemonNameSpace.h"
#include "FlacDemonFileSystem.h"

//#define FLACDEMON_METADATA_MULTIPLE_VALUES "FlacDemonMetaDataMultipleValues"

#define FLACDEMON_FILE_IS_MEDIA         1
#define FLACDEMON_DIRECTORY_IS_ALBUM    2
#define FLACDEMON_FILE_IS_DIRECTORY     4
#define FLACDEMON_DIRECTORY_HAS_SUBDIRECTORIES 8
#define FLACDEMON_FILE_IS_NON_MEDIA     16
#define FLACDEMON_DIRECTORY_HAS_MULTIPLE_CODECS 32
#define FLACDEMON_SUBDIRECTORY_HAS_MEDIA 64

#define FLACDEMON_FILE_IS_MEDIA_DIRECTORY (FLACDEMON_FILE_IS_MEDIA & FLACDEMON_FILE_IS_DIRECTORY)

class FlacDemon::File {
private:
    vector<string*>* consistentMetadata;
    vector<string*>* inconsistentMetadata;
public:
    
    string * path;
    string * type;
    AVCodecID codecID;
    AVDictionary * metadata;
    vector<FlacDemon::File*> *files;
    int error;
    bool exists;
    
    unsigned int flags;
    
    FlacDemon::Track* track;
    MediaStreamInfo * mediaStreamInfo;
    unsigned long fileSize;
    
    File(string* path = NULL);
    ~File();
    
    string* getPath();
    void setPath(string*);
    
    void parse();
    void addFile(FlacDemon::File *);
    void addMetaDataFromFile(FlacDemon::File*);
    void checkFileStructure();
    
    bool checkExists(struct stat * buffer = NULL);
    bool checkDirectory();
    
    bool isDirectory();
    bool isMediaFile();
    bool isAlbumDirectory();
    
    int readMediaInfo();
    void setToMediaFile(AVFormatContext*);
    void makeTrack();
    void standardiseMetaTags();
    string * standardiseKey(string* key);
    
    void setToDirectory();
    
    void printMetaDataDict(AVDictionary* dict);
    string* getMetaDataEntry(const char* key);
    string* getMetaDataEntry(string* key);
    
    vector<FlacDemon::File*> * getAlbumDirectories();
    vector<FlacDemon::File*> * getNoneAlbumFiles();
};

#endif /* defined(__FlacDemon__File__) */


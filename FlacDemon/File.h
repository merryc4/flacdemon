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
#include <sstream>
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
#define FLACDEMON_FILE_IS_MEDIA_DIRECTORY 128

#define FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA (FLACDEMON_FILE_IS_MEDIA_DIRECTORY | FLACDEMON_SUBDIRECTORY_HAS_MEDIA)

class FlacDemon::File {
private:
    vector<string*>* consistentMetadata;
    vector<string*>* inconsistentMetadata;
public:
    AVFormatContext * formatContext = NULL;
    
    std::string * path = NULL;
    std::string * type = NULL;
    std::string * albumuuid = NULL;
    AVCodecID codecID;
    AVDictionary * metadata;
    vector<FlacDemon::File*> *files;
    int error;
    bool exists;
    bool readTags = true;
    
    unsigned int flags;
    
    FlacDemon::Track* track;
    MediaStreamInfo * mediaStreamInfo;
    unsigned long fileSize;
    int trackNumber;
    int trackCount;
    
    File(string* path = NULL, bool readTags = true);
    ~File();
    
    string* getPath();
    void setPath(string*);
    
    void setAlbumDirectoryUUID(std::string * uuid);
    
    void parse();
    void addFile(FlacDemon::File *);
    void addMetaDataFromFile(FlacDemon::File*);
    void checkFileStructure();
    
    bool checkExists(struct stat * buffer = NULL);
    bool checkDirectory();
    
    bool isDirectory();
    bool isMediaFile();
    bool isAlbumDirectory();
    void checkAlbumValues();
    void parseTrackNumber();
    int readMediaInfo();
    int openFormatContext(bool reset = false);
    void setToMediaFile(AVFormatContext*);
    void makeTrack();
    void standardiseMetaTags();
    string * standardiseKey(string* key);
    const char * standardiseKey(const char *key);
    
    void setToDirectory();
    
    void printMetaDataDict(AVDictionary* dict);
    std::string* getMetaDataEntry(const char* key);
    std::string* getMetaDataEntry(string* key);
    void setMetaDataEntry(std::string *key, std::string * value);
    void setMetaDataEntry(const char * key, std::string * value);
    void setMetaDataEntry(const char * key, const char * value);

    
    vector<FlacDemon::File*> * getAlbumDirectories(int depth = INT_MAX);
    vector<FlacDemon::File*> * getMediaFiles(int depth = INT_MAX);
    vector<FlacDemon::File*> * getNoneAlbumFiles(int depth = INT_MAX);
    
    void standardisePath(std::string * workingDirectory);
};

#endif /* defined(__FlacDemon__File__) */


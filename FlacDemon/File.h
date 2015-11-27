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
#define FLACDEMON_DIRECTORY_IS_DISC 256


/* Error Flags */
#define FLACDEMON_TRACKNUMBER_MISMATCH 1
#define FLACDEMON_NO_TRACKNUMBER 2
#define FLACDEMON_TRACKNUMBER_MISSING 4
#define FLACDEMON_TRACKCOUNT_INCONSISTENT 8

#define FLACDEMON_DISCNUMBER_MISMATCH 16
#define FLACDEMON_NO_DISCNUMBER 32
#define FLACDEMON_DISCNUMBER_MISSING 64
#define FLACDEMON_DISCCOUNT_INCONSISTENT 128

//files loop
#define FLACDEMON_LOOP_ALL_FILES fd_filevector::iterator it = this->files->begin(); it != this->files->end(); it++

enum checkDiscMethod {
    FLACDEMON_CHECK_DISC_METHOD_ALBUM,
    FLACDEMON_CHECK_DISC_METHOD_ARTST
};

enum setChildMetadata {
    FLACDEMON_DO_NOT_SET_CHILD_METADATA,
    FLACDEMON_SET_FIRST_CHILD_METADATA,
    FLACDEMON_SET_ALL_CHILD_METADATA
};

#define FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA (FLACDEMON_FILE_IS_MEDIA_DIRECTORY | FLACDEMON_SUBDIRECTORY_HAS_MEDIA)

class FlacDemon::File {
private:
    vector<string*>* consistentMetadata = nullptr;
    vector<string*>* inconsistentMetadata = nullptr;
public:
    
    unsigned long fileSize;
    
    int trackNumber = 0;
    int trackCount = 0;
    int discNumber = 0;
    int discCount = 0;
    
    int error;
    int verified;
    
    unsigned int flags;
    unsigned int errorFlags;
    
    bool exists;
    bool readTags = true;
    
    AVFormatContext * formatContext = nullptr;
    
    std::string * path = nullptr;
    std::string * name = nullptr;
    std::string * type = nullptr;
    std::string * albumuuid = nullptr;
    
    AVCodecID codecID;
    AVDictionary * metadata = nullptr;
    vector<FlacDemon::File*> *files = nullptr;
    
    FlacDemon::Track* track = nullptr;
    MediaStreamInfo * mediaStreamInfo = nullptr;

    
    File(string* path = nullptr, bool readTags = true);
    ~File();
    
    string* getPath();
    void setNameFromPath();
    void setPath(string*);
    
    void setAlbumDirectoryUUID(std::string * uuid);
    
    void parse();
    void addFile(FlacDemon::File *);
    void addMetaDataFromFile(FlacDemon::File*);
    void checkFileStructure();
    void checkDiscs(int method);
    void setDiscNumber(int discNumber);
    void reparseTags();
    
    bool checkExists(struct stat * buffer = nullptr);
    bool checkDirectory();
    
    bool isDirectory();
    bool isMediaFile();
    bool containsMedia();
    bool isAlbumDirectory();
    bool hasConsistantAlbumMetaData();
    void verifyAlbum();
    void setVerified(bool verified);
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
    std::string* getMetaDataEntry(const char* key, int flags = 0);
    std::string* getMetaDataEntry(string* key, int flags = 0);
    std::string* getMetaDataEntry(const char* key, AVDictionaryEntry * t, int flags = 0);
    std::string* getMetaDataEntry(string* key, AVDictionaryEntry *t, int flags = 0);
    
    void setMetaDataEntry(std::string *key, std::string * value, setChildMetadata setChildren = FLACDEMON_DO_NOT_SET_CHILD_METADATA);
    void setMetaDataEntry(const char * key, std::string * value, setChildMetadata setChildren = FLACDEMON_DO_NOT_SET_CHILD_METADATA);
    void setMetaDataEntry(const char * key, const char * value, setChildMetadata setChildren = FLACDEMON_DO_NOT_SET_CHILD_METADATA);

    
    vector<FlacDemon::File*> * getAlbumDirectories(int depth = INT_MAX);
    vector<FlacDemon::File*> * getAllFiles(int depth = INT_MAX);
    vector<FlacDemon::File*> * getMediaFiles(int depth = INT_MAX);
    vector<FlacDemon::File*> * getNoneAlbumFiles(int depth = INT_MAX);
    
    void standardisePath(std::string * workingDirectory);
};

#endif /* defined(__FlacDemon__File__) */


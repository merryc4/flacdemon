//
//  File.h
//  FlacDemon
//
//  Created by merryclarke on 28/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//


//#ifndef _FlacDemonMetaDataMultipleValues
//#define _FlacDemonMetaDataMultipleValues
//
//extern const char * FlacDemonMetaDataMultipleValues;
//
//#endif

#ifndef __FlacDemon__File__
#define __FlacDemon__File__

#include "FlacDemonAll.h"
#include "FlacDemonFileSystem.h"


//#define FLACDEMON_METADATA_MULTIPLE_VALUES "FlacDemonMetaDataMultipleValues"

#define FLACDEMON_FILE_IS_MEDIA                     1
#define FLACDEMON_DIRECTORY_IS_ALBUM                2
#define FLACDEMON_FILE_IS_DIRECTORY                 4
#define FLACDEMON_DIRECTORY_HAS_SUBDIRECTORIES      8
#define FLACDEMON_FILE_IS_NON_MEDIA                 16
#define FLACDEMON_DIRECTORY_HAS_MULTIPLE_CODECS     32
#define FLACDEMON_SUBDIRECTORY_HAS_MEDIA            64
#define FLACDEMON_FILE_IS_MEDIA_DIRECTORY           128
#define FLACDEMON_DIRECTORY_IS_DISC                 256
#define FLACDEMON_IS_TAG_SIMILARITY_ALBUM_DIRECTORY 512
#define FLACDEMON_FILECOUNT_IS_ABOVE_THRESHOLD      1024
#define FLACDEMON_ALBUM_HAS_MULTIPLE_ARTISTS        2048

/* Error Flags */
#define FLACDEMON_TRACKNUMBER_MISMATCH          1
#define FLACDEMON_NO_TRACKNUMBER                2
#define FLACDEMON_TRACKNUMBER_MISSING           4
#define FLACDEMON_TRACKCOUNT_INCONSISTENT       8

#define FLACDEMON_DISCNUMBER_MISMATCH           16
#define FLACDEMON_NO_DISCNUMBER                 32
#define FLACDEMON_DISCNUMBER_MISSING            64
#define FLACDEMON_DISCCOUNT_INCONSISTENT        128

#define FLACDEMON_METADATA_HAS_SIMILARITY       256
#define FLACDEMON_FILENAME_MATCHES_ALBUM        512
#define FLACDEMON_FILENAME_MATCHES_ARTIST       1024
#define FLACDEMON_FILENAME_MATCHES_ALBUMARTIST  2048
#define FLACDEMON_ALBUMARTIST_IS_INCORRECT      4096


/* filecount threshold */
#define FLACDEMON_FILECOUNT_THRESHOLD           100

// flag accessors
#define set_flag this->flags = this->flags |
#define unset_flag this->flags = this->flags & ~
#define has_flag this->flags &

#define set_eflag this->errorFlags = this->flags |
#define unset_eflag this->errorFlags = this->flags & ~
#define has_eflag this->errorFlags &
//files loop
#define flacdemon_loop_all_files(files) for(fd_filevector::iterator it = files->begin(); it != files->end(); it++)


/* other constants */
#define FLACDEMON_TAG_SIMILARITY_THRESHOLD 0.8 //80 percent

enum checkDiscMethod {
    FLACDEMON_CHECK_DISC_METHOD_ALBUM,
    FLACDEMON_CHECK_DISC_METHOD_ARTIST
};

enum setChildMetadata {
    FLACDEMON_DO_NOT_SET_CHILD_METADATA,
    FLACDEMON_SET_FIRST_CHILD_METADATA,
    FLACDEMON_SET_ALL_CHILD_METADATA
};

#define FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA (FLACDEMON_FILE_IS_MEDIA_DIRECTORY | FLACDEMON_SUBDIRECTORY_HAS_MEDIA)

class FlacDemon::File {
private:
    fd_stringvector * consistentMetadata;
    fd_stringvector * inconsistentMetadata;
    fd_stringvector * similarMetadata;
    AVDictionary * metadata;
public:
    
    unsigned long fileSize;
    
    int trackNumber;
    int trackCount;
    int discNumber;
    int discCount;
    
    int immediateChildDirectoryCount;
    int immediateChildFileCount;
    int immediateChildCount;
    int totalChildDirectoryCount;
    int totalChildFileCount;
    int totalChildCount;
    
    int error;
    int verified;
    
    unsigned int flags;
    unsigned int errorFlags;
    
    bool exists;
    bool readTags;
    
    AVFormatContext * formatContext;
    
    std::string * filepath;
    std::string * name;
    std::string * albumuuid;
    
    std::vector<FlacDemon::File*> *files;
    std::vector<FlacDemon::File*> *unparsedFiles;
    
    AVCodecID codecID;
    
    MediaStreamInfo * mediaStreamInfo;

    FlacDemon::Track* track;

    
    File(string* iPath = nullptr, bool readTags = true);
    ~File();
    
    string* getPath();
    void setNameFromPath();
    void setPath(std::string* iPath);
    
    void setAlbumDirectoryUUID(std::string * uuid);
    void setFlag(int flag);
    void unsetFlag(int flag);
    int hasFlag(int flag);
    
    void countChildren();
    
    FlacDemon::File * parse();
    void addFile(FlacDemon::File *);
    void addMetaDataFromFile(FlacDemon::File*);
    void checkFileStructure();
    void checkDiscs(int method);
    void setDiscNumber(int discNumber);
    void checkMetaData(bool albumConsistency, bool artistConsistency);
    void checkAlbumArtist( bool albumConsistency = false , bool artistConstitency = false );
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
    int checkTrackNumbers();
    int readMediaInfo();
    int openFormatContext(bool reset = false);
    void setToMediaFile(AVFormatContext*);
    void makeTrack();
    void standardiseMetaTags();
    
    void setToDirectory();
    
    void printMetaDataDict(AVDictionary* dict);
    std::string getMetaDataEntry(const char* key, int flags = 0);
    std::string getMetaDataEntry(string* key, int flags = 0);
    std::string getMetaDataEntry(const char* key, AVDictionaryEntry * t, int flags = 0);
    std::string getMetaDataEntry(string* key, AVDictionaryEntry *t, int flags = 0);
    
    void setMetaDataEntry(std::string *key, std::string * value, setChildMetadata setChildren = FLACDEMON_DO_NOT_SET_CHILD_METADATA);
    void setMetaDataEntry(const char * key, std::string * value, setChildMetadata setChildren = FLACDEMON_DO_NOT_SET_CHILD_METADATA);
    void setMetaDataEntry(const char * key, const char * value, setChildMetadata setChildren = FLACDEMON_DO_NOT_SET_CHILD_METADATA);

    
    std::vector<FlacDemon::File*> * getAlbumDirectories(int depth = INT_MAX);
    std::vector<FlacDemon::File*> * getAllFiles(int depth = INT_MAX);
    std::vector<FlacDemon::File*> * getMediaFiles(int depth = INT_MAX);
    std::vector<FlacDemon::File*> * getNoneAlbumFiles(int depth = INT_MAX);
    
    void standardisePath(std::string * workingDirectory);
};

#endif /* defined(__FlacDemon__File__) */


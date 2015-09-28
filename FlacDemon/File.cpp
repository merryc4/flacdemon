//
//  File.cpp
//  FlacDemon
//
//  Created by merryclarke on 28/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "TrackFile.h"

const char * FlacDemonMetaDataMultipleValues = "FlacDemonMetaDataMultipleValues";

FlacDemon::File::File(string* path, bool readTags){
    this->codecID = AV_CODEC_ID_NONE;
    this->flags = 0;
    this->metadata = NULL;
    this->files = NULL;
    this->path = new string;
    this->error = 0;
    this->fileSize = 0;
    this->readTags = readTags;
    
    if(path)
        this->setPath(path);
}
FlacDemon::File::~File(){
    if(this->files){
        this->files->clear();
        delete this->files;
    }
    if(this->consistentMetadata){
        this->consistentMetadata->clear();
        delete this->consistentMetadata;
    }
    if(this->inconsistentMetadata){
        this->inconsistentMetadata->clear();
        delete this->inconsistentMetadata;
    }
}
string* FlacDemon::File::getPath(){
    return this->path;
}
void FlacDemon::File::setPath(string* iPath){
    this->path->assign(*iPath);
    if(this->checkDirectory()){
        this->parse();
    } else {
        this->readMediaInfo();
    }
}

void FlacDemon::File::parse(){
    if(this)
    cout << "Checking directory " << *this->path << endl;
    struct dirent *ent;
    DIR* dir;
    string subpath;
    if(path->back() != '/'){
        path->append("/");
    }
    
    if ((dir = opendir (this->path->c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if(ent->d_name[0] == '.'){
                //hidden file
                cout << "skipping hidden file " << ent->d_name << endl;
                continue;
            }
            subpath = *path + ent->d_name;
            cout << subpath << endl;
            
            FlacDemon::File* tFile = new FlacDemon::File(&subpath);
            if(tFile->error) //handle separate error codes
                continue;
            
            this->addFile(tFile);

        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
    }
    this->checkFileStructure();
    this->printMetaDataDict(this->metadata);
}
void FlacDemon::File::addFile(FlacDemon::File * file){
    this->files->push_back(file);
    if(file->isDirectory()){
        this->flags = this->flags | FLACDEMON_DIRECTORY_HAS_SUBDIRECTORIES;
        if(file->flags & FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA)
            this->flags = this->flags | FLACDEMON_SUBDIRECTORY_HAS_MEDIA;
        
    } else if(file->isMediaFile()){
        this->flags = this->flags | FLACDEMON_FILE_IS_MEDIA;
        this->flags = this->flags | FLACDEMON_FILE_IS_MEDIA_DIRECTORY;
        if(this->codecID == AV_CODEC_ID_NONE){
            this->codecID = file->codecID;
        } else if(this->codecID != file->codecID){
            this->flags = this->flags | FLACDEMON_DIRECTORY_HAS_MULTIPLE_CODECS;
        }
    } else {
        this->flags = this->flags | FLACDEMON_FILE_IS_NON_MEDIA;
    }
    this->addMetaDataFromFile(file);
}
void FlacDemon::File::addMetaDataFromFile(FlacDemon::File * file){
    AVDictionaryEntry *copyFrom = NULL, *copyTo=NULL;
    const char * value=NULL;
    while ((copyFrom = av_dict_get(file->metadata, "", copyFrom, AV_DICT_IGNORE_SUFFIX))){
        if((copyTo = av_dict_get(this->metadata, copyFrom->key, copyTo, 0))
           && strcmp(copyTo->value, copyFrom->value) !=0 ){
            cout << "values for key '" << copyFrom->key << "' '" << copyTo->value << "' and '" << copyFrom->value << "' do not match" << endl;
            value = FlacDemonMetaDataMultipleValues;
        } else {
            value = copyFrom->value;
        }
        av_dict_set(&this->metadata, copyFrom->key, value, 0);
    }
}
void FlacDemon::File::checkFileStructure(){
    int lookForCDs = 0;
    if(!this->flags & FLACDEMON_FILE_IS_MEDIA){
        if(!(lookForCDs = (this->flags & FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA)))
            return;
    }
//    for(vector<FlacDemon::File*>::iterator it = files->begin(); it != files->end(); it++){
//        
//    }

    AVDictionaryEntry * t = NULL;
    
    while ((t = av_dict_get(this->metadata, "", t, AV_DICT_IGNORE_SUFFIX))){
        if(strcmp(t->value, FlacDemonMetaDataMultipleValues) == 0){
            this->inconsistentMetadata->push_back(new string(t->key));
        } else {
            this->consistentMetadata->push_back(new string(t->key));
        }
    }
    
    
}
bool FlacDemon::File::checkExists(struct stat * buffer){
    bool makeBuffer = false;
    if(!buffer){
        makeBuffer = true;
        buffer = new struct stat;
    }
    this->exists = false;
    if(stat(path->c_str(), buffer) == -1){
        cout << "error stat-ing file " << path << " errno: " << errno << endl;
        if(errno == ENOENT){
            cout << "File does not exist" << endl;
        }
        return false;
    }
    this->exists = true;
    if(makeBuffer)
        delete buffer;
    return true;
}
bool FlacDemon::File::checkDirectory(){
    struct stat buffer;
    bool isDir = false;
    if(this->checkExists(&buffer) && S_ISDIR(buffer.st_mode)){
        isDir = true;
        this->setToDirectory();
    } else {
        this->fileSize = buffer.st_size;
    }
    return isDir;
}
void FlacDemon::File::setToDirectory(){
    if (this->isDirectory())     //once set can not be unset
        return;
    
    this->flags = this->flags | FLACDEMON_FILE_IS_DIRECTORY;
    this->files = new vector<FlacDemon::File*>;
    this->consistentMetadata = new vector<string*>;
    this->inconsistentMetadata = new vector<string*>;
}
bool FlacDemon::File::isDirectory(){
    return this->flags & FLACDEMON_FILE_IS_DIRECTORY;
}
bool FlacDemon::File::isMediaFile(){
    return this->flags & FLACDEMON_FILE_IS_MEDIA && !(this->flags & FLACDEMON_FILE_IS_DIRECTORY);
}
bool FlacDemon::File::isAlbumDirectory(){

    if(!(this->flags & FLACDEMON_FILE_IS_MEDIA_DIRECTORY))
        return false;
    
    bool albumConsistency = false,
    artistConsistency = false;
    
    for(vector<string*>::iterator it = this->consistentMetadata->begin(); it != this->consistentMetadata->end(); it++){
        cout << **it << ", ";
        if(((*it)->compare("album"))==0 ) {
            albumConsistency = true;
        } else if ((*it)->compare("albumartist")==0) {
            artistConsistency = true;
        } else if(((*it)->compare("artist"))==0){
            artistConsistency = true;
        }
    }
    cout << endl;

    return albumConsistency && artistConsistency;
}
int FlacDemon::File::readMediaInfo(){    
    AVCodec* inputCodec;
    if(this->openFormatContext() < 0){
        return -1;
    }
    int averror;
    /** Get information on the input file (number of streams etc.). */
    if ((averror = avformat_find_stream_info(this->formatContext, NULL)) < 0) {
        cout << "Could not open find stream info" << endl;
        avformat_close_input(&this->formatContext);
        return averror;
    }
    
    //check streams
    
    /** Find a decoder for the audio stream. */
    if (!(inputCodec = avcodec_find_decoder((this->formatContext)->streams[0]->codec->codec_id))) {
        fprintf(stderr, "Could not find input codec\n");
        avformat_close_input(&this->formatContext);
        return AVERROR_EXIT;
    }
    
    if(inputCodec->type != AVMEDIA_TYPE_AUDIO){
        cout << "skipping none audio file " << *path << endl;
        this->flags = this->flags & ~ FLACDEMON_FILE_IS_MEDIA;
    } else {
        this->setToMediaFile(this->formatContext);
    }
    
    this->codecID = inputCodec->id;
    
    if(!this->readTags){
        return 0;
    }
    
    av_dict_copy(&this->metadata, this->formatContext->metadata, 0);
    
    this->standardiseMetaTags();
    
    this->printMetaDataDict(this->metadata);
    
    cout << "Found decoder " << inputCodec->name << endl;
    
    return inputCodec->id;
}
int FlacDemon::File::openFormatContext(){
    /** Open the input file to read from it. */
    if(this->formatContext)
        return 0;
    int averror;
    this->formatContext = avformat_alloc_context();

    if ((averror = avformat_open_input(&this->formatContext, this->path->c_str(), NULL,
                                       NULL)) < 0) {
        cout << "could not open input file" << endl;
        this->formatContext = NULL;
        return averror;
    }
    return 0;
}
void FlacDemon::File::setToMediaFile(AVFormatContext* formatContext){
    this->flags = this->flags | FLACDEMON_FILE_IS_MEDIA;
    
    AVCodecContext* codecContext = formatContext->streams[0]->codec;
    
    this->mediaStreamInfo = new struct MediaStreamInfo;
    this->mediaStreamInfo->bitRate = codecContext->bit_rate;
    this->mediaStreamInfo->sampleRate = codecContext->sample_rate;
    this->mediaStreamInfo->channels = codecContext->channels;
    this->mediaStreamInfo->codecID = codecContext->codec_id;
    this->mediaStreamInfo->duration = formatContext->streams[0]->duration;
    
    this->makeTrack();
}
void FlacDemon::File::makeTrack(){
    this->track = new FlacDemon::Track(this);
}
void FlacDemon::File::standardiseMetaTags(){
    // this function is not finished
    AVDictionaryEntry *copyFrom = NULL;
    string * key = NULL, * newKey = NULL;
    std::vector<std::pair<string *, const char *>> toAdd;
    while ((copyFrom = av_dict_get(this->metadata, "", copyFrom, AV_DICT_IGNORE_SUFFIX))){
        key = new string(copyFrom->key);
        newKey = this->standardiseKey(key);
        free(key);
        if(key != newKey)
            toAdd.push_back(*new std::pair<string *, const char*>{newKey, copyFrom->value});
    }
    for(std::vector<std::pair<string *, const char *>>::iterator it = toAdd.begin(); it != toAdd.end(); it++){
        av_dict_set(&this->metadata, (*it).first->c_str(), (*it).second, 0);
    }
}
string * FlacDemon::File::standardiseKey(string *key){
    key = new string(*key);
    transform(key->begin(), key->end(), key->begin(), ::tolower);
    regex e("album[^a-zA-Z]artist", regex_constants::icase);
    if(regex_match((*key), e)){
        key->assign("albumartist");
    }
    return key;
}
const char * FlacDemon::File::standardiseKey(const char *key){
    return this->standardiseKey(new string(key))->c_str();
}
void FlacDemon::File::printMetaDataDict(AVDictionary *dict){
    cout << "Metadata for " << *this->path << ":" <<endl;
    AVDictionaryEntry *t = NULL;
    while ((t = av_dict_get(dict, "", t, AV_DICT_IGNORE_SUFFIX))){
        cout << t->key << " : " << t->value << endl;
    }
    cout <<endl<<endl;
}
std::string * FlacDemon::File::getMetaDataEntry(string* key){
    return this->getMetaDataEntry(key->c_str());
}
std::string * FlacDemon::File::getMetaDataEntry(const char *key){
//    const char * tKey = this->standardiseKey(key);
    if(key == NULL)
        return NULL;
    AVDictionaryEntry *t = av_dict_get(this->metadata, key, NULL, AV_DICT_IGNORE_SUFFIX);
    if(!t)
        return NULL;
    return new std::string(t->value);
}
vector<FlacDemon::File*> * FlacDemon::File::getAlbumDirectories(int max){
    vector<FlacDemon::File*> * albumDirectories = new vector<FlacDemon::File*>;
    if(max > 0 && this->flags & FLACDEMON_SUBDIRECTORY_HAS_MEDIA){
        vector<FlacDemon::File*> * subdirectoryAlbumDirectories = NULL;
        for(vector<FlacDemon::File*>::iterator it = this->files->begin(); it != this->files->end(); it++){
            if(!((*it)->flags & FLACDEMON_FILE_IS_DIRECTORY))
                continue;
            if((*it)->isAlbumDirectory()){
                albumDirectories->push_back(*it);
            } else if((*it)->flags & FLACDEMON_FILE_IS_MEDIA_DIRECTORY){
                subdirectoryAlbumDirectories = (*it)->getAlbumDirectories(max - 1);
                albumDirectories->insert(albumDirectories->end(), subdirectoryAlbumDirectories->begin(), subdirectoryAlbumDirectories->end());
                free(subdirectoryAlbumDirectories);
            }
        }
    }
    if(this->isAlbumDirectory()){
        albumDirectories->push_back(this);
    }
    return albumDirectories;
}
vector<FlacDemon::File*> * FlacDemon::File::getMediaFiles(int max){
    vector<FlacDemon::File*> * mediaFiles = new vector<FlacDemon::File*>;
    if(max > 0 && this->flags & FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA){
        vector<FlacDemon::File*> * subdirectoryMediaFiles = NULL;
        for(vector<FlacDemon::File*>::iterator it = this->files->begin(); it != this->files->end(); it++){
            if((*it)->isMediaFile()){
                mediaFiles->push_back(*it);
            } else if((*it)->flags & FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA){
                subdirectoryMediaFiles = (*it)->getMediaFiles(max - 1);
                mediaFiles->insert(mediaFiles->end(), subdirectoryMediaFiles->begin(), subdirectoryMediaFiles->end());
                free(subdirectoryMediaFiles);
            }
        }
    }
    if(this->isMediaFile()){
        mediaFiles->push_back(this);
    }
    return mediaFiles;
}
vector<FlacDemon::File*> * FlacDemon::File::getNoneAlbumFiles(int max){
    
    return NULL;
}



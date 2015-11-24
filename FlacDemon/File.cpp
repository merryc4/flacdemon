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
    this->errorFlags = 0;
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
    
    this->setNameFromPath();
    
    if(this->checkDirectory()){
        this->parse();
    } else {
        this->readMediaInfo();
    }
}
void FlacDemon::File::setNameFromPath(){
    size_t pos = this->path->rfind("/");
    if(pos == string::npos){
        this->name = new std::string(*this->path);
    } else {
        this->name = new std::string(this->path->substr(pos+1));
    }
}
void FlacDemon::File::setAlbumDirectoryUUID(std::string * iuuid){
    this->albumuuid = new std::string(*iuuid);
    if(this->files){
        for (std::vector<FlacDemon::File *>::iterator it = this->files->begin(); it != this->files->end(); it++) {
            (*it)->setAlbumDirectoryUUID(iuuid);
        }
    }
}
void FlacDemon::File::parse(){
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
    cout << "Checking file structure for " << *this->path << endl;
    int lookForDiscs = 0;
    if(!(this->flags & FLACDEMON_FILE_IS_MEDIA_DIRECTORY) && (this->flags & FLACDEMON_SUBDIRECTORY_HAS_MEDIA)){
        lookForDiscs = 1;
    }

    AVDictionaryEntry * t = NULL;
    
    bool albumConsistency = false,
        artistConsistency = false;
    
    while ((t = av_dict_get(this->metadata, "", t, AV_DICT_IGNORE_SUFFIX))){
        if(strcmp(t->value, FlacDemonMetaDataMultipleValues) == 0){
            this->inconsistentMetadata->push_back(new string(t->key));
        } else {
            std::string * skey = new string(t->key);
            this->consistentMetadata->push_back(skey);
            
//            cout << *skey << ", ";
            
            if(!albumConsistency || !artistConsistency){

                if((skey->compare("album"))==0 ) {
                    albumConsistency = true;
                } else if (skey->compare("albumartist")==0) {
                    artistConsistency = true;
                } else if((skey->compare("artist"))==0){
                    artistConsistency = true;
                }
            }
        }
    }
//    cout << endl;
    
    if(artistConsistency){
        if(albumConsistency){
            this->flags = this->flags | FLACDEMON_DIRECTORY_IS_ALBUM;
            //only need to check 'disc' tag / file names to try and identify correct disc numbers
            if(lookForDiscs)
                this->checkDiscs(FLACDEMON_CHECK_DISC_METHOD_ALBUM);
        } else {
            //need to check "album" tag to see if they include disc numbers in album name
            if(lookForDiscs)
                this->checkDiscs(FLACDEMON_CHECK_DISC_METHOD_ARTST);
        }
    }
}
void FlacDemon::File::checkDiscs(int method){
    std::string * discStr = nullptr;
    int discNumber = 0;
    
    for(std::vector<FlacDemon::File * >::iterator it = this->files->begin(); it != this->files->end(); it++){
        switch (method) {
            case FLACDEMON_CHECK_DISC_METHOD_ALBUM:
                discStr = (*it)->getMetaDataEntry("disc", AV_DICT_IGNORE_SUFFIX);
                if(!discStr)
                    discStr = (*it)->getMetaDataEntry("cd", AV_DICT_IGNORE_SUFFIX);
                if(!discStr){
                    std::regex e("(?:disc|cd)\\s*(\\w+)", regex_constants::icase);
                    std::smatch ematch;
                    if(regex_search(*(*it)->name, ematch, e)){
                        if(ematch.size()){
                            std::ssub_match sub_match = ematch[ematch.size() - 1];
                            discStr = new std::string(sub_match.str());
                        }
                    } else {
                        //no disc number found
                    }

                }
                break;
            case FLACDEMON_CHECK_DISC_METHOD_ARTST:
                
                break;
        }
        if(fd_stringtoint(discStr, &discNumber)){
            //some error
            cout << "error getting disc number from string " << discStr << endl;
        } else if(discNumber){
            (*it)->setDiscNumber(discNumber);
        }
    }
    
}
void FlacDemon::File::setDiscNumber(int discNumber){
    this->discNumber = discNumber;
    if(this->isDirectory()){
        if(discNumber){
            this->flags = this->flags | FLACDEMON_DIRECTORY_IS_DISC;
        } else {
            this->flags = this->flags & ~ FLACDEMON_DIRECTORY_IS_DISC;
        }
        for(std::vector< FlacDemon::File * >::iterator it = this->files->begin(); it != this->files->end(); it++){
            (*it)->setDiscNumber(discNumber);
        }
    }
    std::string s = std::to_string(discNumber);
    this->setMetaDataEntry("disc", &s);
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
    if(this->flags & FLACDEMON_DIRECTORY_IS_ALBUM)
        return true;
    
    if(!(this->flags & FLACDEMON_FILE_IS_MEDIA_DIRECTORY))
        return false;
    
    return this->hasConsistantAlbumMetaData();
}
bool FlacDemon::File::hasConsistantAlbumMetaData(){ //deprecated
//    bool albumConsistency = false,
//    artistConsistency = false;
//    
//    for(vector<string*>::iterator it = this->consistentMetadata->begin(); it != this->consistentMetadata->end() && !(albumConsistency && artistConsistency); it++){
//        cout << **it << ", ";
//        if(((*it)->compare("album"))==0 ) {
//            albumConsistency = true;
//        } else if ((*it)->compare("albumartist")==0) {
//            artistConsistency = true;
//        } else if(((*it)->compare("artist"))==0){
//            artistConsistency = true;
//        }
//    }
//    cout << endl;
//    
//    if(albumConsistency && artistConsistency){
//        this->flags = this->flags | FLACDEMON_DIRECTORY_IS_ALBUM;
//        return true;
//    }
    return false;
}
void FlacDemon::File::checkAlbumValues(){
    if(!this->albumuuid) //set by database when adding an album directory
        return;
    
    vector < FlacDemon::File * > * files = this->getMediaFiles();
    int tTrackCount = 0,
        tTrackNumber = 0,
        maxTrackNumber = 0;
    std::vector<int> availableTracks;
    
    for(vector < FlacDemon::File * >::iterator it = files->begin(); it != files->end(); it++){
        (*it)->parseTrackNumber();
        if(tTrackCount){
            if((*it)->trackCount != tTrackCount){
                //track count incosistency, handle error
                cout << "track count incosistency" << endl;
                this->errorFlags = this->errorFlags | FLACDEMON_TRACKCOUNT_INCONSISTENT;
            }
        } else {
            tTrackCount = (*it)->trackCount;
        }
        if((tTrackNumber = (*it)->trackNumber)){
            if(maxTrackNumber < tTrackNumber){
                maxTrackNumber = tTrackNumber;
                availableTracks.resize(maxTrackNumber + 1);
            }
            availableTracks[tTrackNumber] = 1;
        } else {
            cout << "no track number for file" << endl;
            //no track number for file, handle error
        }
    }
    for(int i = 1; i <= maxTrackNumber; i++){
        if(!availableTracks[i]){
            cout << "Track Missing!";
            this->errorFlags = this->errorFlags | FLACDEMON_TRACKNUMBER_MISSING;
            //flag album as track missing
        }
        cout << i << ": " << availableTracks[i] << endl;
    }
}
void FlacDemon::File::parseTrackNumber(){
    std::string * trackNumStr = this->getMetaDataEntry("track"),
        * trackNumStr2 = NULL;
    int trackNum = 0,
        trackNum2 = 0,
        trackCnt = 0;
    int nonDigitFound = 0;
    
    if(!trackNumStr || fd_stringtoint(trackNumStr, &trackNum)){
        cout << "could not parse track number from metadata for" << *this->path << endl;
    }
    if(trackNumStr){
        //check value of trackNum
        for(std::string::iterator it = trackNumStr->begin(); it != trackNumStr->end(); it++){
            if(!isdigit((*it))){
                nonDigitFound = true;
            }
            else if(nonDigitFound){
                std::string tempStr(it, trackNumStr->end());
                trackCnt = std::stoi(tempStr);
                cout << trackCnt << endl;
                break;
            }
        }
    }
    
    std::regex e("^([0-9]{1,2})[^0-9]");
    std::smatch ematch;
    if(regex_search(*this->name, ematch, e)){
        if(ematch.size()){
            std::ssub_match sub_match = ematch[0];
            cout << "found track number " << sub_match.str() << endl;
            trackNumStr2 = new std::string(sub_match.str());
            if(fd_stringtoint(trackNumStr, &trackNum2)){
                cout << "Could not parse track number from file for " << *this->path << endl;
            }
            
        }
    }
    
    if(trackNum == trackNum2){
        if(trackNum == 0){ //no track number could be determined
            this->errorFlags = this->errorFlags | FLACDEMON_NO_TRACKNUMBER;
            return;
        }
    } else if(!trackNum){
        trackNum = trackNum2;
    } else {
        this->errorFlags = this->errorFlags | FLACDEMON_TRACKNUMBER_MISMATCH;
    }
    
    this->trackNumber = trackNum;
    this->trackCount = trackCnt;
    std::string track = std::to_string(trackNum);
    this->setMetaDataEntry("track", &track);
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
    
    /** Find a decoder for the audio stream. **/
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
int FlacDemon::File::openFormatContext(bool reset){
    /** Open the input file to read from it. */
    if(this->formatContext){
        if(reset){
            avformat_free_context(this->formatContext);
        } else {
           return 0;
        }
    }
    
    
    int averror;
    this->formatContext = avformat_alloc_context();

    if ((averror = avformat_open_input(&this->formatContext, this->path->c_str(), NULL,
                                       NULL)) < 0) {
        cout << "could not open input file" << endl;
        this->formatContext = NULL;
        return averror;
    }
    if(this->formatContext->probe_score <= 1){ //revise this number
        std::stringstream ss;
        ss << "\\." << this->formatContext->iformat->name << "$";
        std::regex e(ss.str(), regex_constants::icase);
        if(!regex_match(*this->path, e)){
            return -1;
        }
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
std::string * FlacDemon::File::getMetaDataEntry(string* key, int flags){
    return this->getMetaDataEntry(key->c_str(), NULL, flags);
}
std::string * FlacDemon::File::getMetaDataEntry(const char *key, int flags){
    return this->getMetaDataEntry(key, NULL, flags);
}
std::string* FlacDemon::File::getMetaDataEntry(string* key, AVDictionaryEntry *t, int flags){
    return this->getMetaDataEntry(key->c_str(), t, flags);
}
std::string* FlacDemon::File::getMetaDataEntry(const char* key, AVDictionaryEntry * t, int flags){
    if(key == NULL)
        return NULL;
    AVDictionaryEntry *newt = av_dict_get(this->metadata, key, t, flags);
    if(!newt)
        return NULL;
    return new std::string(newt->value);
}
void FlacDemon::File::setMetaDataEntry(std::string * key, std::string * value){

    if(key == NULL || value == NULL)
        return;
    this->setMetaDataEntry(key->c_str(), value->c_str());
}
void FlacDemon::File::setMetaDataEntry(const char * key, std::string * value){
    if(key == NULL || value == NULL)
        return;
    this->setMetaDataEntry(key, value->c_str());
}
void FlacDemon::File::setMetaDataEntry(const char * key, const char * value){
    if(key == NULL || value == NULL)
        return;
    av_dict_set(&this->metadata, key, value, 0);
}
vector<FlacDemon::File*> * FlacDemon::File::getAlbumDirectories(int max){
    vector<FlacDemon::File*> * albumDirectories = new vector<FlacDemon::File*>;
    if(max != 0 && this->flags & FLACDEMON_SUBDIRECTORY_HAS_MEDIA){
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
    vector<FlacDemon::File*> * noneAlbumFiles = new vector<FlacDemon::File*>;
    if(this->isAlbumDirectory()){
        return noneAlbumFiles;
    } else if(this->isMediaFile()){
        noneAlbumFiles->push_back(this);
        return noneAlbumFiles;
    }
    if(max != 0 && this->flags & FLACDEMON_FILE_IS_MEDIA_DIRECTORY){
        vector<FlacDemon::File*> * subdirectoryNoneAlbumFiles = NULL;
        for(vector<FlacDemon::File*>::iterator it = this->files->begin(); it != this->files->end(); it++){
            if((*it)->flags & FLACDEMON_FILE_IS_DIRECTORY){
                if((*it)->isAlbumDirectory()){
                    continue;
                } else if((*it)->flags & FLACDEMON_FILE_IS_MEDIA_DIRECTORY){
                    subdirectoryNoneAlbumFiles = (*it)->getNoneAlbumFiles(max - 1);
                    noneAlbumFiles->insert(noneAlbumFiles->end(), subdirectoryNoneAlbumFiles->begin(), subdirectoryNoneAlbumFiles->end());
                    free(subdirectoryNoneAlbumFiles);
                }
            } else if((*it)->flags && FLACDEMON_FILE_IS_MEDIA) {
                noneAlbumFiles->push_back((*it));
            }
        }
    }
    return noneAlbumFiles;
}
void FlacDemon::File::standardisePath(std::string * workingDirectory){
    if(this->path->at(0) == '/')
        return;
    boost::filesystem::path tpath = (*this->path);
    tpath = boost::filesystem::absolute(tpath);
    this->path = new std::string(tpath.string<std::string>()); //may need to free old this->path
    if(this->isMediaFile() && this->track){
        this->track->filepath = this->path;
    }
}


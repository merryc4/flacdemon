//
//  File.cpp
//  FlacDemon
//
//  Created by merryclarke on 28/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "TrackFile.h"

//const char * FlacDemonMetaDataMultipleValues = "FlacDemonMetaDataMultipleValues";

FlacDemon::File::File(string* iPath, bool readTags){
    
    this->fileSize = 0;
    
    this->trackNumber = 0;
    this->trackCount = 0;
    this->discNumber = 0;
    this->discCount = 0;
    
    this->immediateChildDirectoryCount = 0;
    this->immediateChildFileCount = 0;
    this->immediateChildCount = 0;
    this->totalChildDirectoryCount = 0;
    this->totalChildFileCount = 0;
    this->totalChildCount = 0;
    
    this->error = 0;
    this->verified = 0;
    
    this->flags = 0;
    this->errorFlags = 0;
    
    this->exists = 0;
    this->readTags = readTags;
    
    this->codecID = AV_CODEC_ID_NONE;

    
    this->formatContext = nullptr;
    
    this->filepath = nullptr;
    this->name = nullptr;
    this->albumuuid = nullptr;
    
    this->metadata = nullptr;
    this->files = nullptr;
    
    this->track = nullptr;
    this->mediaStreamInfo = nullptr;
    
    this->consistentMetadata = nullptr;
    this->inconsistentMetadata = nullptr;
    this->similarMetadata = nullptr;
    
    if(iPath)
        this->setPath(iPath);
}
FlacDemon::File::~File(){
    if(this->files){
        flacdemon_loop_all_files(this->files){
            delete (*it);
        }
        delete this->files;
    }
    if(this->consistentMetadata){
        delete this->consistentMetadata;
    }
    if(this->inconsistentMetadata){
        delete this->inconsistentMetadata;
    }
    if(this->similarMetadata){
        delete this->similarMetadata;
    }
    if(this->metadata){
        av_dict_free(&this->metadata);
    }
    delete this->filepath;
    delete this->name;
    delete this->albumuuid;
    
    if(this->track) //could cause issues if track is used elsewhere
        delete this->track;
    
    
}
string* FlacDemon::File::getPath(){
    return this->filepath;
}
void FlacDemon::File::setPath(std::string* iPath){
    if(this->filepath)
        this->filepath->assign(*iPath);
    else
        this->filepath = new std::string(*iPath);
    
    this->setNameFromPath();
    
    this->countChildren();
}
void FlacDemon::File::setNameFromPath(){
    if(this->filepath == nullptr)
        return;
    size_t pos = this->filepath->rfind("/");
    if(pos == string::npos){
        this->name = new std::string(*this->filepath);
    } else {
        this->name = new std::string(this->filepath->substr(pos+1));
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
void FlacDemon::File::setFlag(int flag){
    this->flags |= flag;
}
void FlacDemon::File::unsetFlag(int flag){
    unset_flag flag;
}
int FlacDemon::File::hasFlag(int flag){
    return has_flag flag;
}
void FlacDemon::File::countChildren(){
    if(!this->checkDirectory()){
        return;
    }
    struct dirent *ent;
    DIR* dir;
    string subpath;
    
    this->unparsedFiles = new std::vector < FlacDemon::File * >;
    
    if ((dir = opendir (this->filepath->c_str())) != nullptr) {
        while ((ent = readdir (dir)) != nullptr) {
            if(ent->d_name[0] == '.'){
                continue;
            }
            subpath = *this->filepath + ent->d_name;
            
            FlacDemon::File* tFile = new FlacDemon::File(&subpath);
            if( tFile->isDirectory() ){
                this->immediateChildDirectoryCount++;
                this->totalChildDirectoryCount += tFile->totalChildDirectoryCount;
                this->totalChildFileCount += tFile->totalChildFileCount;
            } else {
                this->immediateChildFileCount++;
            }
            this->unparsedFiles->push_back(tFile);
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("Error: File::countChildren: ");
    }
    this->totalChildFileCount += this->immediateChildFileCount;
    this->totalChildDirectoryCount += this->immediateChildDirectoryCount;
    this->immediateChildCount = this->immediateChildDirectoryCount + this->immediateChildFileCount;
    this->totalChildCount = this->totalChildDirectoryCount + this->totalChildFileCount;
    
    if(this->totalChildDirectoryCount > FLACDEMON_FILECOUNT_THRESHOLD){
        set_flag FLACDEMON_FILECOUNT_IS_ABOVE_THRESHOLD;
    }
}
FlacDemon::File * FlacDemon::File::parse(){
    if(!this->checkDirectory()){
        this->readMediaInfo();
        return this;
    }
    cout << "Checking directory " << *this->filepath << endl;
    FlacDemon::File * tFile;
    for( std::vector < FlacDemon::File * >::iterator it = this->unparsedFiles->begin(); it != this->unparsedFiles->end(); it++){
        tFile = (*it);
        tFile->parse();
        if(!tFile->error){
            if(has_flag FLACDEMON_FILECOUNT_IS_ABOVE_THRESHOLD && !( tFile->flags & FLACDEMON_FILECOUNT_IS_ABOVE_THRESHOLD ) ){
                signalHandler->call("addFile", tFile);
                delete tFile;
            } else {
                this->addFile(tFile);
            }
        }
    }
    delete this->unparsedFiles;
    this->unparsedFiles = nullptr;
    if(!this->files->size()){
        cout << "No files in directory " << *this->filepath << endl;
        return nullptr;
    }
    this->checkFileStructure();
    if(has_flag FLACDEMON_FILECOUNT_IS_ABOVE_THRESHOLD){
        return nullptr;
    }
    return this;
}
void FlacDemon::File::addFile(FlacDemon::File * file){
    this->files->push_back(file);
    if(file->isDirectory()){
        set_flag FLACDEMON_DIRECTORY_HAS_SUBDIRECTORIES;
        if(file->flags & FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA)
            set_flag FLACDEMON_SUBDIRECTORY_HAS_MEDIA;
        
    } else if(file->isMediaFile()){
        set_flag FLACDEMON_FILE_IS_MEDIA;
        set_flag FLACDEMON_FILE_IS_MEDIA_DIRECTORY;
        if(this->codecID == AV_CODEC_ID_NONE){
            this->codecID = file->codecID;
        } else if(this->codecID != file->codecID){
            set_flag FLACDEMON_DIRECTORY_HAS_MULTIPLE_CODECS;
        }
    } else {
        set_flag FLACDEMON_FILE_IS_NON_MEDIA;
    }
    this->addMetaDataFromFile(file);
}
void FlacDemon::File::addMetaDataFromFile(FlacDemon::File * file){
    if(this->metadata == nullptr){
        av_dict_copy(&this->metadata, file->metadata, 0);
        return;
    }
    AVDictionaryEntry *copyFrom = nullptr, *copyTo=nullptr;
    const char * value = nullptr;
    while ((copyFrom = av_dict_get(file->metadata, "", copyFrom, AV_DICT_IGNORE_SUFFIX))){
        if( av_dict_get( this->metadata, copyFrom->key , nullptr, 0 ) == nullptr ){
            av_dict_set( &this->metadata, copyFrom->key, FlacDemonMetaDataMultipleValues , 0 );
        }
    }

    copyFrom = nullptr;
    copyTo = nullptr;
    while ((copyTo = av_dict_get(this->metadata, "", copyTo, AV_DICT_IGNORE_SUFFIX))){
        if((copyFrom = av_dict_get(file->metadata, copyTo->key, nullptr, 0))
           && strcmp(copyFrom->value, copyTo->value) != 0 ){
            if( strcmp( copyTo->value , FlacDemonMetaDataMultipleValues ) == 0) continue;
            av_dict_set(&this->metadata, copyFrom->key, FlacDemonMetaDataMultipleValues, 0);
        }
    }
}
void FlacDemon::File::checkFileStructure(){
    if(!this->containsMedia())
        return;
    cout << "Checking file structure for " << *this->filepath << endl;
    int lookForDiscs = 0;
    if(!(has_flag FLACDEMON_FILE_IS_MEDIA_DIRECTORY) && (has_flag FLACDEMON_SUBDIRECTORY_HAS_MEDIA)){
        lookForDiscs = 1;
    }

    AVDictionaryEntry * t = nullptr;
    
    bool albumConsistency = false,
        artistConsistency = false,
        artistConsistencyIsArtist = false,
        artistConsistencyIsAlbumArtist = false;
    
    while ((t = av_dict_get(this->metadata, "", t, AV_DICT_IGNORE_SUFFIX))){
        if(strcmp(t->value, FlacDemonMetaDataMultipleValues) == 0){
            this->inconsistentMetadata->push_back(std::string(t->key));
        } else {
            std::string skey = t->key;
            this->consistentMetadata->push_back(skey);
            
            if(!albumConsistency || !artistConsistency){

                if(( skey.compare("album"))==0 ) {
                    albumConsistency = true;
                } else if ( skey.compare("albumartist") == 0 ) {
                    artistConsistencyIsAlbumArtist = true;
                } else if ( skey.compare("artist") == 0 ) {
                    artistConsistencyIsArtist = true;
                }
                artistConsistency = artistConsistencyIsAlbumArtist || artistConsistencyIsArtist;
            }
        }
    }
    
    if(artistConsistency){
        if(albumConsistency){
            if(lookForDiscs){
                this->checkDiscs(FLACDEMON_CHECK_DISC_METHOD_ALBUM);
                if(this->discCount > 1){
                    set_flag FLACDEMON_DIRECTORY_IS_ALBUM;
                }
            } else {
                set_flag FLACDEMON_DIRECTORY_IS_ALBUM;
            }
            
            //only need to check 'disc' tag / file names to try and identify correct disc numbers

        } else {
            if(lookForDiscs){
                this->checkDiscs(FLACDEMON_CHECK_DISC_METHOD_ARTIST);
                std::string albumTag = this->getMetaDataEntry("album");
                if(albumTag.compare(FlacDemonMetaDataMultipleValues) != 0){
                    this->consistentMetadata->push_back(std::string("album"));
                    for(fd_stringvector::iterator it = this->inconsistentMetadata->begin(); it != this->inconsistentMetadata->end(); it++){
                        if( it->compare("album") == 0 ){
                            this->inconsistentMetadata->erase(it);
                            it--; // resets to prevent iterator pointing to erased item
                        }
                    }
                    set_flag FLACDEMON_DIRECTORY_IS_ALBUM;
                    albumConsistency = true;
                }
            }
        }
    }
    if(!lookForDiscs && !this->isAlbumDirectory() && this->consistentMetadata->size()){
        //if there is any consistent metadata do a more thorough check for an album with mislabeled tags.
        this->checkTrackNumbers();
        this->checkMetaData(albumConsistency, artistConsistency);
    }
    
    if( this->isAlbumDirectory() ){
        if( ! artistConsistencyIsArtist ) {
            set_flag FLACDEMON_ALBUM_HAS_MULTIPLE_ARTISTS;
        }
        this->checkAlbumArtist();
    }
}
void FlacDemon::File::checkMetaData(bool albumConsistency, bool artistConsistency){
    FlacDemon::File * tFile;
    std::string value, key;
    int count;
    float filenameSimilarity;
    std::map< std::string, int > * valueCounts;
    std::map< std::string,  std::map<std::string, int> * > values;
    fd_stringvector missingValues;
    bool tagHasMissingValues;
    bool keyIsArtist, keyIsAlbum, keyIsAlbumArtist;
    bool tagSimilarityAlbumConsistency = false, tagSimilarityArtistConsitency = false;
    bool missingValuesAlbumConsistency = false , missingValuesArtistConsistency = false;
    for(fd_stringvector::iterator metaIterator = this->inconsistentMetadata->begin(); metaIterator != this->inconsistentMetadata->end(); metaIterator++){
        if(metaIterator->compare("track") == 0) //track numbers checked in checkTrackNumbers()
            continue;
        key = (*metaIterator);
        keyIsAlbum = false; keyIsArtist = false, keyIsAlbumArtist = false;
        tagHasMissingValues = false;
        
        if(key.compare("album") == 0)
            keyIsAlbum = true;
        else if(key.compare("artist") == 0)
            keyIsArtist = true;
        else if( key.compare("albumartist") == 0 )
            keyIsAlbumArtist = true;
        
        valueCounts = new std::map< std::string , int >;
        values.insert(std::pair<std::string, std::map< std::string , int> * >{(key), valueCounts});
        
        flacdemon_loop_all_files(this->files){
            count = 1;
            tFile = (*it);
            if((value = tFile->getMetaDataEntry(&(*metaIterator))) == "" ){
                //potentially use filename with internet scraper lookup, or filename with other such use
                tagHasMissingValues = true;
                continue;
            }
            
            if((filenameSimilarity = fd_comparetags(&value, this->name)) > FLACDEMON_TAG_SIMILARITY_THRESHOLD){
                //tag shares similarity / is the same as filename
                int flag = 0;
                if(keyIsAlbum)
                    flag = FLACDEMON_FILENAME_MATCHES_ALBUM;
                else if(keyIsArtist)
                    flag = FLACDEMON_FILENAME_MATCHES_ARTIST;
                else if(keyIsAlbumArtist)
                    flag = FLACDEMON_FILENAME_MATCHES_ALBUMARTIST;
                
                set_eflag flag;
            }
            if(valueCounts->count(value)){
                valueCounts->at(value) += 1;
            } else {
                valueCounts->insert(std::pair<std::string, int> { value, 1});
            }
        }
        std::string previous = "";
        
        for(std::map< std::string , int >::iterator valueIterator = valueCounts->begin(); valueIterator != valueCounts->end(); valueIterator++){
            value.assign((*valueIterator).first);
            if(previous.length() && fd_comparetags(&value, &previous) > FLACDEMON_TAG_SIMILARITY_THRESHOLD){
                cout << "Found high tag similarity for " << previous << " and " << value << endl;
                if(this->similarMetadata == nullptr)
                    this->similarMetadata = new fd_stringvector;
                this->similarMetadata->push_back(key);
                set_eflag FLACDEMON_METADATA_HAS_SIMILARITY;
                if(keyIsAlbum){
                    tagSimilarityAlbumConsistency = true;
                } else if(keyIsArtist || keyIsAlbumArtist) {
                    tagSimilarityArtistConsitency = true;
                }
                
            } else {
                previous = "";
            }
            previous = value;
        }
        
        if( tagHasMissingValues && valueCounts->size() == 1 ){ //potentially add conditionals for tag similarity with some missing values
            set_eflag FLACDEMON_ALBUM_HAS_PARTIALLY_MISSING_METADATA;
            if( keyIsArtist )
                set_eflag FLACDEMON_ALBUM_HAS_PARTIALLY_MISSING_ARTIST;
            else if( keyIsAlbumArtist )
                set_eflag FLACDEMON_ALBUM_HAS_PARTIALLY_MISSING_ALBUMARTIST;
            else continue;
            
            missingValuesArtistConsistency = true;
        }
    }
    albumConsistency = albumConsistency || tagSimilarityAlbumConsistency;
    artistConsistency = artistConsistency || tagSimilarityArtistConsitency || missingValuesArtistConsistency;
    if( albumConsistency && missingValuesArtistConsistency ){
        set_flag FLACDEMON_IS_MISSING_VALUES_ALBUM_DIRECTORY;
    }
    if(albumConsistency && artistConsistency && ( tagSimilarityAlbumConsistency || tagSimilarityArtistConsitency ))
        set_flag FLACDEMON_IS_TAG_SIMILARITY_ALBUM_DIRECTORY;
    
    //free maps
    for(std::map< std::string,  std::map<std::string, int> * >::iterator valuesIterator = values.begin(); valuesIterator != values.end(); valuesIterator++){
        delete  (*valuesIterator).second;
    }
}
void FlacDemon :: File :: checkAlbumArtist ( bool albumConsistency , bool artistConsistency ) {
    /* revise this function */
    std::string albumArtistTag = this->getMetaDataEntry( "albumartist" );
    size_t flag = 0;
    if( ! albumArtistTag.length() ){
        flag = FLACDEMON_ALBUMARTIST_IS_INCORRECT;
    }
    else if( albumArtistTag == FlacDemonMetaDataMultipleValues ) {
        flag = FLACDEMON_ALBUMARTIST_IS_INCORRECT;
    }
    else if( std::regex_search( albumArtistTag , std::regex( "(various|multiple)" , std::regex_constants::icase ) ) ) {
        flag = FLACDEMON_ALBUMARTIST_IS_INCORRECT;
    }
    if ( flag ) {
        flacdemon_loop_all_files( this->files ) {
            if( ( *it )->isMediaFile() ) {
                iset_flag( ( *it )->errorFlags , flag );
            }
        }
    }
}
void FlacDemon::File::checkDiscs(int method){
    
    std::regex e("(?:disc|cd)\\s*(\\w+)", std::regex_constants::icase);
    std::smatch ematch;
    std::ssub_match sub_match;

    this->discCount = 0;
    
    int maxDiscNumber = 0;
    
    bool reparseNeeded = false;

    
    for(std::vector<FlacDemon::File * >::iterator it = this->files->begin(); it != this->files->end(); it++){
        if(!(*it)->containsMedia())
            continue;
        std::string metaDiscStr = "",
                    fileNameDiscStr = "",
                    albumTagDiscStr = "";
        
        int metaDiscNumber = 0,
        filenameDiscNumber = 0,
        albumTagDiscNumber = 0;
        
        std::vector<int> numbers;
        
        metaDiscStr = (*it)->getMetaDataEntry("disc", 0);
        if( ! metaDiscStr.length() )
            metaDiscStr = (*it)->getMetaDataEntry("cd", 0);
        
        if(fd_stringtoint(&metaDiscStr, &metaDiscNumber) || metaDiscNumber == 0){
            cout << "could not get disc number from metadata for " << *(*it)->filepath << endl;
        } else {
            numbers.push_back(metaDiscNumber);
        }
        
        if(regex_search(*(*it)->name, ematch, e) && ematch.size()){
            sub_match = ematch[ematch.size() - 1];
            fileNameDiscStr = sub_match.str();
            fd_stringtoint(&fileNameDiscStr, &filenameDiscNumber);
        }
        if(!filenameDiscNumber){
            cout << "could not get disc number from filename for " << *(*it)->filepath << endl;
        } else {
            numbers.push_back(filenameDiscNumber);
        }

        
        if(method == FLACDEMON_CHECK_DISC_METHOD_ARTIST){
            std::string albumTag = (*it)->getMetaDataEntry("album", 0);
            if(regex_search(albumTag, ematch, e) && ematch.size()){
                sub_match = ematch[ematch.size() - 1];
                albumTagDiscStr = sub_match.str();
                if(!fd_stringtoint(&albumTagDiscStr, &albumTagDiscNumber)){
                    albumTag = regex_replace(albumTag, std::regex("\\s*(?:disc|cd)\\s*\\w+\\s*", std::regex_constants::icase), "");
                    (*it)->setMetaDataEntry("album", &albumTag, FLACDEMON_SET_ALL_CHILD_METADATA);
                    reparseNeeded = true;
                }
            }
            if(!albumTagDiscNumber){
                cout << "could not get disc number from album tag for " << *(*it)->filepath << endl;
            } else {
                numbers.push_back(albumTagDiscNumber);
            }
        }
        
        int discNumber = 0;
        for(std::vector<int>::iterator it2 = numbers.begin(); it2 != numbers.end(); it2++){
            if(discNumber && (*it2) != discNumber){
                this->errorFlags = this->errorFlags | FLACDEMON_DISCNUMBER_MISMATCH;
                discNumber = -1;
                break;
            } else {
                discNumber = (*it2);
            }
        }
        
        if(discNumber){
            (*it)->setDiscNumber(discNumber);
            if(maxDiscNumber < discNumber)
                maxDiscNumber = discNumber;
            this->discCount++;
            reparseNeeded = true;
        }
    }
    if(maxDiscNumber != this->discCount)
        this->errorFlags = this->errorFlags | FLACDEMON_DISCCOUNT_INCONSISTENT;
    
    if(!this->discCount)
        this->discCount = 1;
    
    if(reparseNeeded)
        this->reparseTags();
    
}
void FlacDemon::File::reparseTags(){
    if(this->metadata){
        av_dict_free(&this->metadata);
    }
    this->metadata = nullptr;
    flacdemon_loop_all_files(this->files){
        if((*it)->isDirectory())
            (*it)->reparseTags();
        this->addMetaDataFromFile((*it));
    }
    
}
void FlacDemon::File::setDiscNumber(int discNumber){
    this->discNumber = discNumber;
    if(this->isDirectory()){
        if(discNumber){
            set_flag FLACDEMON_DIRECTORY_IS_DISC;
        } else {
            unset_flag FLACDEMON_DIRECTORY_IS_DISC;
        }
        for(std::vector< FlacDemon::File * >::iterator it = this->files->begin(); it != this->files->end(); it++){
            (*it)->setDiscNumber(discNumber);
        }
    }
    std::string s = std::to_string(discNumber);
    this->setMetaDataEntry("disc", &s);
    if(this->track)
        this->track->setTrackInfoForKey("disc", discNumber);
}
bool FlacDemon::File::checkExists(struct stat * buffer){
    bool makeBuffer = false;
    if(!buffer){
        makeBuffer = true;
        buffer = new struct stat;
    }
    this->exists = false;
    if(stat(this->filepath->c_str(), buffer) == -1){
        cout << "error stat-ing file " << *this->filepath << " errno: " << errno << " " << strerror(errno) << endl;
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
    if(this->filepath->back() != '/'){
        this->filepath->append("/");
    }
    set_flag FLACDEMON_FILE_IS_DIRECTORY;
    this->files = new std::vector<FlacDemon::File*>;
    this->consistentMetadata = new fd_stringvector;
    this->inconsistentMetadata = new fd_stringvector;
}
bool FlacDemon::File::isDirectory(){
    return has_flag FLACDEMON_FILE_IS_DIRECTORY;
}
bool FlacDemon::File::isMediaFile(){
    return has_flag FLACDEMON_FILE_IS_MEDIA && !(has_flag FLACDEMON_FILE_IS_DIRECTORY);
}
bool FlacDemon::File::containsMedia(){
    return has_flag FLACDEMON_FILE_IS_MEDIA || has_flag FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA;
}
bool FlacDemon::File::isAlbumDirectory(){
    if((has_flag FLACDEMON_DIRECTORY_IS_ALBUM || has_flag FLACDEMON_IS_TAG_SIMILARITY_ALBUM_DIRECTORY || has_flag FLACDEMON_IS_MISSING_VALUES_ALBUM_DIRECTORY ) && !(has_flag FLACDEMON_DIRECTORY_IS_DISC))
        return true;
    
    if(!(has_flag FLACDEMON_FILE_IS_MEDIA_DIRECTORY))
        return false;
    
    return false;
}

void FlacDemon::File::verifyAlbum(){
    if(!this->albumuuid) //set by database when adding an album directory
        return;
    bool verified = true;
    
    if(this->checkTrackNumbers())
        verified = false;

    if(verified && this->errorFlags){
        verified = false;
    } else {
        this->setVerified(verified);
    }
}
void FlacDemon::File::setVerified(bool verified){
    this->verified = verified;
    if(this->track){
        this->track->setTrackInfoForKey("verified", verified);
    }
    if(has_flag FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA){
        for(std::vector < FlacDemon::File * >::iterator it = files->begin(); it != files->end(); it++){
            (*it)->setVerified(verified);
        }
    }
}
void FlacDemon::File::parseTrackNumber(){
    std::string trackNumStr = this->getMetaDataEntry("track"),
        trackNumStr2 = "";
    int trackNum = 0,
        trackNum2 = 0,
        trackCnt = 0;
    int nonDigitFound = 0;
    
    if(!trackNumStr.length() || fd_stringtoint(&trackNumStr, &trackNum)){
        cout << "could not parse track number from metadata for " << *this->filepath << endl;
    }
    for(std::string::iterator it = trackNumStr.begin(); it != trackNumStr.end(); it++){
        if(!isdigit((*it))){
            nonDigitFound = true;
        }
        else if(nonDigitFound){
            std::string tempStr(it, trackNumStr.end());
            trackCnt = std::stoi(tempStr);
            break;
        }
    }
    
    std::regex e("^([0-9]{1,2})[^0-9]");
    std::smatch ematch;
    if(regex_search(*this->name, ematch, e) && ematch.size()){
        std::ssub_match sub_match = ematch[0];
//            cout << "found track number " << sub_match.str() << endl;
        trackNumStr2 = sub_match.str();
        if(fd_stringtoint(&trackNumStr2, &trackNum2)){
            cout << "Could not parse track number from file for " << *this->filepath << endl;
        }
            
    }
    
    if(trackNum == trackNum2){
        if(trackNum == 0){ //no track number could be determined
            this->errorFlags = this->errorFlags | FLACDEMON_NO_TRACKNUMBER;
            cout << "no track number for file " << *this->filepath << endl;
            return;
        }
    } else if(!trackNum){
        trackNum = trackNum2;
    } else {
        this->errorFlags = this->errorFlags | FLACDEMON_TRACKNUMBER_MISMATCH;
        cout << "tracknumber mismatch for file " << * this->filepath << endl;
        trackNum = -1;
        
    }
    
    this->trackNumber = trackNum;
    this->trackCount = trackCnt;
    std::string track = std::to_string(trackNum);
//    this->setMetaDataEntry("track", &track);
    if(this->track)
        this->track->setTrackInfoForKey("track", trackNum);
}
int FlacDemon::File::checkTrackNumbers(){
    
    std::vector < FlacDemon::File * > * files = this->getMediaFiles();
    int tTrackCount = 0,
    tTrackNumber = 0,
    maxTrackNumber = 0;
    std::vector<std::vector<int> *> availableTracksPerDisc;
    std::vector<int> * availableTracks = nullptr;
    
    int error = 0;
    
    int disc = 0;
    for(std::vector < FlacDemon::File * >::iterator it = files->begin(); it != files->end(); it++){
        (*it)->parseTrackNumber();
        if(!disc || (*it)->discNumber != disc){
            disc = (*it)->discNumber;
            tTrackCount = 0;
            maxTrackNumber = 0;
            availableTracks = new std::vector<int>;
            availableTracksPerDisc.push_back(availableTracks);
        }
        
        if(tTrackCount){
            if((*it)->trackCount != tTrackCount){
                //track count incosistency, handle error
                cout << "track count incosistency" << endl;
                this->errorFlags = this->errorFlags | FLACDEMON_TRACKCOUNT_INCONSISTENT;
                error = 1;
            }
        } else {
            tTrackCount = (*it)->trackCount;
        }
        if((tTrackNumber = (*it)->trackNumber)){
            if(maxTrackNumber < tTrackNumber){
                maxTrackNumber = tTrackNumber;
                availableTracks->resize(maxTrackNumber + 1);
            }
            if(tTrackNumber > 0)
                availableTracks->at(tTrackNumber) = 1;
        } else {
            cout << "no track number for file" << endl;
            error = 1;
            //no track number for file, handle error
        }
        if(!error && (*it)->errorFlags)
            error = 1;
    }
    for(std::vector<std::vector<int> *>::iterator it = availableTracksPerDisc.begin(); it != availableTracksPerDisc.end(); it++){
        int count = 0; //not currently used
        if((*it)->size() == 0)
            continue;
        for(std::vector<int>::iterator it2 = (*it)->begin()+1; it2 != (*it)->end(); it2++) {
            if(!(*it2)){
                cout << "Track Missing!" << endl;
                this->errorFlags = this->errorFlags | FLACDEMON_TRACKNUMBER_MISSING;
                error = 1;
            }
            count ++;
        }
    }
    return error;
}
int FlacDemon::File::readMediaInfo(){
    AVCodec* inputCodec;
    if(this->openFormatContext() < 0){
        return -1;
    }
    int averror;
    /** Get information on the input file (number of streams etc.). */
    if ((averror = avformat_find_stream_info(this->formatContext, nullptr)) < 0) {
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
        cout << "skipping none audio file " << *this->filepath << endl;
        unset_flag FLACDEMON_FILE_IS_MEDIA;
    } else {
        this->setToMediaFile(this->formatContext);
    }
    
    this->codecID = inputCodec->id;
    
    if(!this->readTags){
        return 0;
    }
    
    av_dict_copy(&this->metadata, this->formatContext->metadata, 0);
    
    this->standardiseMetaTags();
    
    avformat_close_input(&this->formatContext);
    
//    this->printMetaDataDict(this->metadata);
    
//    cout << "Found decoder " << inputCodec->name << endl;
    
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

    if ((averror = avformat_open_input(&this->formatContext, this->filepath->c_str(), nullptr,
                                       nullptr)) < 0) {
        cout << "could not open input file" << endl;
        this->formatContext = nullptr;
        return averror;
    }
    if(this->formatContext->probe_score <= 1){ //revise this number
        std::stringstream ss;
        ss << "\\." << this->formatContext->iformat->name << "$";
        std::regex e(ss.str(), std::regex_constants::icase);
        if(!regex_match(*this->filepath, e)){
            return -1;
        }
    }
    return 0;
}
void FlacDemon::File::setToMediaFile(AVFormatContext* formatContext){
    set_flag FLACDEMON_FILE_IS_MEDIA;
    
    AVCodecContext* codecContext = formatContext->streams[0]->codec;
    
    this->mediaStreamInfo = new struct MediaStreamInfo;
    this->mediaStreamInfo->bitRate = codecContext->bit_rate;
    this->mediaStreamInfo->sampleRate = codecContext->sample_rate;
    this->mediaStreamInfo->channels = codecContext->channels;
    this->mediaStreamInfo->codecID = codecContext->codec_id;
    this->mediaStreamInfo->duration = formatContext->streams[0]->duration;
    
    this->discNumber = 1;
    this->discCount = 1;
    
    this->makeTrack();
}
void FlacDemon::File::makeTrack(){
    this->track = new FlacDemon::Track(this);
}
void FlacDemon::File::standardiseMetaTags(){
    // this function is not finished
    AVDictionaryEntry *copyFrom = nullptr;
    string * key = nullptr, * newKey = nullptr;
    std::vector<std::pair<string *, const char *>> toAdd;
    while ((copyFrom = av_dict_get(this->metadata, "", copyFrom, AV_DICT_IGNORE_SUFFIX))){
        key = new string(copyFrom->key);
        newKey = fd_standardiseKey(new std::string(*key));
        delete key;
        if(key != newKey)
            toAdd.push_back(*new std::pair<string *, const char*>{newKey, copyFrom->value});
    }
    for(std::vector<std::pair<string *, const char *>>::iterator it = toAdd.begin(); it != toAdd.end(); it++){
        av_dict_set(&this->metadata, (*it).first->c_str(), (*it).second, 0);
    }
}
void FlacDemon::File::printMetaDataDict(AVDictionary *dict){
    cout << "Metadata for " << *this->filepath << ":" <<endl;
    AVDictionaryEntry *t = nullptr;
    while ((t = av_dict_get(dict, "", t, AV_DICT_IGNORE_SUFFIX))){
        cout << t->key << " : " << t->value << endl;
    }
    cout <<endl<<endl;
}
std::string FlacDemon::File::getMetaDataEntry(string* key, int flags){
    return this->getMetaDataEntry(key->c_str(), nullptr, flags);
}
std::string FlacDemon::File::getMetaDataEntry(const char *key, int flags){
    return this->getMetaDataEntry(key, nullptr, flags);
}
std::string FlacDemon::File::getMetaDataEntry(string* key, AVDictionaryEntry *t, int flags){
    return this->getMetaDataEntry(key->c_str(), t, flags);
}
std::string FlacDemon::File::getMetaDataEntry(const char* key, AVDictionaryEntry * t, int flags){
    std::string value="";
    if(key == nullptr)
        return value;
    AVDictionaryEntry * newt = av_dict_get(this->metadata, key, t, flags);
    if(newt)
        value.assign(newt->value);
    return value;
}
void FlacDemon::File::setMetaDataEntry(std::string * key, std::string * value, setChildMetadata setChildren){

    if(key == nullptr || value == nullptr)
        return;
    this->setMetaDataEntry(key->c_str(), value->c_str(), setChildren);
}
void FlacDemon::File::setMetaDataEntry(const char * key, std::string * value, setChildMetadata setChildren){
    if(key == nullptr || value == nullptr)
        return;
    this->setMetaDataEntry(key, value->c_str(), setChildren);
}
void FlacDemon::File::setMetaDataEntry(const char * key, const char * value, setChildMetadata setChildren){
    if(key == nullptr || value == nullptr)
        return;
    av_dict_set(&this->metadata, key, value, 0);
    if(setChildren && this->isDirectory()){
        setChildren = (setChildren == FLACDEMON_SET_FIRST_CHILD_METADATA) ? FLACDEMON_DO_NOT_SET_CHILD_METADATA : setChildren;
        for(std::vector< FlacDemon::File * >::iterator it = this->files->begin(); it != this->files->end(); it++){
            (*it)->setMetaDataEntry(key, value, setChildren);
        }
    }
}
std::vector<FlacDemon::File*> * FlacDemon::File::getAlbumDirectories(int max){
    std::vector<FlacDemon::File*> * albumDirectories = new std::vector<FlacDemon::File*>;
    if(max != 0 && has_flag FLACDEMON_SUBDIRECTORY_HAS_MEDIA){
        std::vector<FlacDemon::File*> * subdirectoryAlbumDirectories = nullptr;
        for(std::vector<FlacDemon::File*>::iterator it = this->files->begin(); it != this->files->end(); it++){
            if(!((*it)->flags & FLACDEMON_FILE_IS_DIRECTORY))
                continue;
            if((*it)->isAlbumDirectory()){
                albumDirectories->push_back(*it);
            } else if((*it)->flags & FLACDEMON_FILE_IS_MEDIA_DIRECTORY){
                subdirectoryAlbumDirectories = (*it)->getAlbumDirectories(max - 1);
                albumDirectories->insert(albumDirectories->end(), subdirectoryAlbumDirectories->begin(), subdirectoryAlbumDirectories->end());
                delete subdirectoryAlbumDirectories;
            }
        }
    }
    if(this->isAlbumDirectory()){
        albumDirectories->push_back(this);
    }
    return albumDirectories;
}
std::vector<FlacDemon::File*> * FlacDemon::File::getAllFiles(int max){
    std::vector<FlacDemon::File*> * allFiles = new std::vector<FlacDemon::File*>;
    if(max > 0){
        std::vector<FlacDemon::File*> * subdirectoryFiles = nullptr;
        for(std::vector<FlacDemon::File*>::iterator it = this->files->begin(); it != this->files->end(); it++){
            if(!(*it)->isDirectory()){
                allFiles->push_back(*it);
            } else {
                subdirectoryFiles = (*it)->getAllFiles(max - 1);
                allFiles->insert(allFiles->end(), subdirectoryFiles->begin(), subdirectoryFiles->end());
                delete subdirectoryFiles;
            }
        }
    }
    if(!this->isDirectory()){
        allFiles->push_back(this);
    }
    return allFiles;
}
std::vector<FlacDemon::File*> * FlacDemon::File::getMediaFiles(int max){
    std::vector<FlacDemon::File*> * mediaFiles = new std::vector<FlacDemon::File*>;
    if(max > 0 && has_flag FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA){
        std::vector<FlacDemon::File*> * subdirectoryMediaFiles = nullptr;
        for(std::vector<FlacDemon::File*>::iterator it = this->files->begin(); it != this->files->end(); it++){
            if((*it)->isMediaFile()){
                mediaFiles->push_back(*it);
            } else if((*it)->flags & FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA){
                subdirectoryMediaFiles = (*it)->getMediaFiles(max - 1);
                mediaFiles->insert(mediaFiles->end(), subdirectoryMediaFiles->begin(), subdirectoryMediaFiles->end());
                delete subdirectoryMediaFiles;
            }
        }
    }
    if(this->isMediaFile()){
        mediaFiles->push_back(this);
    }
    return mediaFiles;
}
std::vector<FlacDemon::File*> * FlacDemon::File::getNoneAlbumFiles(int max){
    std::vector<FlacDemon::File*> * noneAlbumFiles = new std::vector<FlacDemon::File*>;
    if(this->isAlbumDirectory()){
        return noneAlbumFiles;
    } else if(this->isMediaFile()){
        noneAlbumFiles->push_back(this);
        return noneAlbumFiles;
    }
    if(max != 0 && has_flag FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA){
        std::vector<FlacDemon::File*> * subdirectoryNoneAlbumFiles = nullptr;
        for(std::vector<FlacDemon::File*>::iterator it = this->files->begin(); it != this->files->end(); it++){
            if((*it)->flags & FLACDEMON_FILE_IS_DIRECTORY){
                if((*it)->isAlbumDirectory()){
                    continue;
                } else if((*it)->flags & FLACDEMON_CHILD_OF_DIRECTORY_IS_MEDIA){
                    subdirectoryNoneAlbumFiles = (*it)->getNoneAlbumFiles(max - 1);
                    noneAlbumFiles->insert(noneAlbumFiles->end(), subdirectoryNoneAlbumFiles->begin(), subdirectoryNoneAlbumFiles->end());
                    delete subdirectoryNoneAlbumFiles;
                }
            } else if((*it)->flags && FLACDEMON_FILE_IS_MEDIA) {
                noneAlbumFiles->push_back((*it));
            }
        }
    }
    return noneAlbumFiles;
}
void FlacDemon::File::standardisePath(std::string * workingDirectory){
    if(this->filepath->at(0) == '/')
        return;
    boost::filesystem::path tpath = (*this->filepath);
    tpath = boost::filesystem::absolute(tpath);
    this->filepath = new std::string(tpath.string<std::string>()); //may need to free old this->filepath
    if(this->isMediaFile() && this->track){
        this->track->filepath = *this->filepath;
    }
}


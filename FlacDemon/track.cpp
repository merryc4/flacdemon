//
//  Track.cpp
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "TrackFile.h"

FlacDemon::Track::Track(FlacDemon::File* file){
    this->init();
    if(file)
        this->setFile(file);
}
FlacDemon::Track::Track(fd_keymap * ikeymap){
    Track();
    this->keymap = ikeymap;
    this->filepath = this->keymap->at("filepath");
//    for(FD_KEYMAP::iterator it = keymap->begin(); it != keymap->end(); it++){
//        this->setValueForKey(it->second, &(it->first));
//    }
}
FlacDemon::Track::~Track(){

}
void FlacDemon::Track::init(){
    this->playCount = 0;
    this->dateAdded = 0;
    this->trackTime = 0;
    
    this->trackinfo = new std::map<std::string, long>{
        {"playcount", 0},
        {"dateadded", 0},
        {"tracktime", 0}
    };
}
void FlacDemon::Track::setFile(FlacDemon::File * file){
    this->file = file;
    this->trackinfo->at("tracktime") = (long) this->file->mediaStreamInfo->duration / (0.001 * this->file->mediaStreamInfo->sampleRate);
    
    this->filepath = new std::string(*file->path);
}

std::string * FlacDemon::Track::valueForKey (const char * key){
    std::string tkey = key;
    return this->valueForKey(&tkey);
}
std::string * FlacDemon::Track::valueForKey(std::string* key){
    std::string * value = NULL;
    if(key->compare("filepath")==0){
        return this->filepath;
    }
    if(this->keymap)
        value = this->keymap->at(*key);
    else if(this->file)
        value = this->file->getMetaDataEntry(key);
    //query db if no file?
    if(value == NULL){
        value = new std::string(std::to_string(this->getTrackInfoForKey(key)));
    }
    return value;
}

//template <class KValue>
void FlacDemon::Track::setValueForKey(std::string * value, std::string *key){
    
}
//void FlacDemon::Track::setValueForKey(const unsigned char * value, const std::string *key){
//    
//}
long FlacDemon::Track::getTrackInfoForKey(const char * key){
    std::string tkey = key;
    return this->getTrackInfoForKey(&tkey);
}
long FlacDemon::Track::getTrackInfoForKey(std::string * key){
    if(this->trackinfo->count(*key)){
        return this->trackinfo->at(*key);
    }
    return -1;
}
void FlacDemon::Track::initInfo(){
    cout << time(NULL) << endl;
    this->trackinfo->at("dateadded") = time(NULL);
}
int FlacDemon::Track::openFilePath(){
    //will need to do more checks, path exists, relative pathname, file opened succesfully etc
    if(this->filepath && !this->file)
        this->file = new FlacDemon::File(this->filepath, false);
    return this->file==NULL ? 0 : 1;
}
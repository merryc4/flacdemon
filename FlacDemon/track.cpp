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
FlacDemon::Track::Track(FD_KEYMAP * ikeymap){
    Track();
    this->keymap = ikeymap;
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
//template <class KValue>
const char * FlacDemon::Track::valueForKey(std::string* key){
    const char * value = NULL;
    if(key->compare("filepath")==0){
        return this->filepath->c_str();
    }
    if(this->file)
        value = this->file->getMetaDataEntry(key);
    else if(this->keymap)
        value = this->keymap->at(*key);
    //query db if no file?
    if(value == NULL){
        char * tvalue = new char [11];
        sprintf(tvalue, "%ld", this->getTrackInfoForKey(key));
        value = (const char *)tvalue;
    }
    return value;
}

//template <class KValue>
void FlacDemon::Track::setValueForKey(std::string * value, std::string *key){
    
}
void FlacDemon::Track::setValueForKey(const unsigned char * value, const std::string *key){
    
}
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
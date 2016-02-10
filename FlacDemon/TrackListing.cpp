//
//  TrackListing.cpp
//  FlacDemon
//
//  Created by merryclarke on 28/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "TrackListing.h"

FlacDemon::TrackListing::TrackListing(){
    this->init();
}
FlacDemon::TrackListing::TrackListing(fd_keymap * ikeymap){
    TrackListing();
    this->keymap = ikeymap;
    if(this->keymap->count("filepath"))
        this->filepath = this->keymap->at("filepath");
}
FlacDemon::TrackListing::~TrackListing(){
    
}
void FlacDemon::TrackListing::init(){
    this->playCount = 0;
    this->dateAdded = 0;
    this->trackTime = 0;
    
    this->trackinfo = new std::map<std::string, long>{
        {"playcount", 0},
        {"dateadded", 0},
        {"tracktime", 0},
        {"verified", 0}
    };
}

std::string * FlacDemon::TrackListing::valueForKey (const char * key){
    std::string tkey = key;
    return this->valueForKey(&tkey);
}
std::string * FlacDemon::TrackListing::valueForKey(std::string* key){
    std::string * value = nullptr;
    if(key->compare("filepath")==0){
        return this->filepath;
    }
    
    if(this->keymap && this->keymap->count(*key))
        value = this->keymap->at(*key);
//    if(key->compare("track")==0){
//        
//    }
    //query db if no file?
    if(value == nullptr){
        value = new std::string(std::to_string(this->getTrackInfoForKey(key)));
    }
    return value;
}
std::string * FlacDemon::TrackListing::standardiseMetaValue(std::string *value, std::string *key){
    if(key->compare("disc") == 0){
        int ivalue = std::stoi(*value);
        value = new std::string(std::to_string(ivalue));
    }
    
    return value;
}
//template <class KValue>
void FlacDemon::TrackListing::setValueForKey(std::string * value, std::string *key){
    
}
//void FlacDemon::TrackListing::setValueForKey(const unsigned char * value, const std::string *key){
//
//}
long FlacDemon::TrackListing::getTrackInfoForKey(const char * key){
    std::string tkey = key;
    return this->getTrackInfoForKey(&tkey);
}
long FlacDemon::TrackListing::getTrackInfoForKey(std::string * key){
    if(this->trackinfo->count(*key)){
        return this->trackinfo->at(*key);
    }
    return -1;
}
void FlacDemon::TrackListing::setTrackInfoForKey(const char * key, long value){
    std::string tkey = key;
    this->setTrackInfoForKey(&tkey, value);
}
void FlacDemon::TrackListing::setTrackInfoForKey(std::string * key, long value){
    std::cout << this->trackinfo->at(*key) << std::endl;
    this->trackinfo->at(*key) = value;
}
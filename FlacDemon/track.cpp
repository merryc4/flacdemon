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
FlacDemon::Track::Track(fd_keymap * keymap) : FlacDemon::TrackListing(keymap){

}
FlacDemon::Track::~Track(){
    
}
void FlacDemon::Track::setFile(FlacDemon::File * file){
    this->file = file;
    this->trackinfo->at("tracktime") = (long) this->file->mediaStreamInfo->duration / (0.001 * this->file->mediaStreamInfo->sampleRate);
    
    this->filepath = new std::string(*file->filepath);
}
void FlacDemon::Track::initInfo(){
    cout << time(nullptr) << endl;
    this->trackinfo->at("dateadded") = time(nullptr);
}
int FlacDemon::Track::openFilePath(){
    //will need to do more checks, path exists, relative pathname, file opened succesfully etc
    if(this->filepath){
        if(!this->file){
            this->file = new FlacDemon::File(this->filepath, false);
        } else {
            this->file->openFormatContext(true); //reset
        }
    }
    
    return this->file==nullptr ? 0 : 1;
}
std::string * FlacDemon::Track::keymapFileValue( std::string * key ){
    std::string * value = nullptr;
    if(this->keymap && this->keymap->count(*key))
        value = this->keymap->at(*key);
    else if(this->file){
        value = this->file->getMetaDataEntry(key);
    }
    return value;
}
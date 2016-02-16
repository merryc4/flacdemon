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
    this->trackinfo->at("dateadded") = time(nullptr);
    if( this->file ){
        this->trackinfo->at( "errorflags" ) = this->file->errorFlags;
    }
}
int FlacDemon::Track::openFilePath(){
    //will need to do more checks, path exists, relative pathname, file opened succesfully etc
    if(this->filepath){
        if(!this->file){
            this->file = new FlacDemon::File(this->filepath, false);
            this->file->parse();
        } else {
            this->file->openFormatContext(true); //reset
        }
    }
    
    return this->file==nullptr ? 0 : 1;
}
std::string FlacDemon::Track::keymapFileValue( std::string * key ){
    //this should not return a pointer
    std::string value = "";
    if(this->trackinfo && this->trackinfo->count(*key)){
        value = std::to_string( this->trackinfo->at( *key ) ) ;
    }
    else if(this->keymap && this->keymap->count(*key))
        value = *this->keymap->at(*key);
    else if(this->file){
        if(key->compare("albumuuid") == 0)
            value = *this->file->albumuuid;
        else
            value =  this->file->getMetaDataEntry( key );
    }
    return value;
}
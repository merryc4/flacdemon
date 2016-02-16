//
//  Album.cpp
//  FlacDemon
//
//  Created by merryclarke on 02/07/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "Album.h"

FlacDemon::Album::Album( std::string * iuuid ){
    this->trackCount = 0;
    this->multipleArtists = false;
    this->metadataInconsistency = false;
    
    this->path = "";
    this->uuid = "";
    if(iuuid && iuuid->length() ){
        this->uuid = *iuuid;
    }
}
FlacDemon::Album::~Album(){
    this->tracks.clear();
}
void FlacDemon::Album::addTrackListing ( FlacDemon::TrackListing * track ){
    this->tracks.push_back(track);
    this->trackCount = this->tracks.size();
    this->addMetaDataFromTrackListing( track );
}
void FlacDemon::Album::addMetaDataFromTrackListing( FlacDemon::TrackListing * track ) {
    std::string value, trackValue, key;
    fd_keymap::iterator * it = nullptr;
    while ( ( it = track->iterateMetadata( it ) ) ) {
        trackValue = (*it)->second;
        if( ! trackValue.length() ) continue;
        key = (*it)->first;
        value = this->valueForKey( &key );
        if( ! value.length() ) {
            this->setValueForKey( &trackValue , &key );
        } else if( trackValue != value && value != FlacDemonMetaDataMultipleValues ){
            this->setValueForKey( FlacDemonMetaDataMultipleValues , &key );
        }
    }
}
std::string FlacDemon::Album::valueForKey( std::string * key ) {
    std::string rvalue = "";
    if( this->metadata.count( *key ) ) {
        rvalue = this->metadata.at( *key );
    }
    return rvalue;
}
void FlacDemon::Album::setValueForKey( const char * value , std::string * key ) {
    std::string strValue = value;
    this->setValueForKey( &strValue, key );
}
void FlacDemon::Album::setValueForKey( std::string * value , const char * key ) {
    std::string strKey = key;
    this->setValueForKey(value, &strKey);
}
void FlacDemon::Album::setValueForKey( std::string * value , std::string * key ) {
    if( value && key ){
        if( this->metadata.count( *key ) ) {
            this->metadata.at( *key ) = *value;
        } else {
            this->metadata.insert( std::make_pair( *key , *value ));
        }
    }
}
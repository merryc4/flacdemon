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
    this->verified = true;
    
    if(iuuid && iuuid->length() ){
        this->uuid = *iuuid;
        this->setValueForKey( iuuid , "albumuuid" );
    }
}
FlacDemon::Album::~Album(){
    this->tracks.clear();
}
void FlacDemon::Album::addTrackListing ( FlacDemon::TrackListing * track ){
    this->tracks.push_back(track);
    this->trackCount = this->tracks.size();
    this->addMetaDataFromTrackListing( track );
    if( ! track->getTrackInfoForKey("verified") ){
        this->verified = false;
    }
}
void FlacDemon::Album::addMetaDataFromTrackListing( FlacDemon::TrackListing * track ) {
    std::string value, trackValue, key;
    for( fd_stringvector::iterator it = libraryTitlesAlbumCopyFromTrack->begin(); it != libraryTitlesAlbumCopyFromTrack->end(); it++ ) {
        key = (*it);
        trackValue = track->valueForKey( &key );
        if( ! trackValue.length() ) continue;
        value = this->valueForKey( &key );
        if( ! value.length() ) {
            if( trackValue != "-1" )
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
    } else {
        if( *key == "tracks" )
            rvalue = std::to_string( this->trackCount );
        else if( *key == "playcount" )
            rvalue = std::to_string( this->playcount() );
        else if( *key == "verified" )
            rvalue = std::to_string( (int) this->verified );
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
bool FlacDemon::Album::matchesSearch() {
    for( fd_tracklistingvector::iterator it = this->tracks.begin(); it != this->tracks.end(); it++ ){
        if( (*it)->matchesSearch() )
            return true;
    }
    return false;
}
float FlacDemon::Album::playcount(){
    float total = 0;
    for( fd_tracklistingvector::iterator it = this->tracks.begin(); it != this->tracks.end(); it++ ){
        total += (*it)->getTrackInfoForKey("playcount");
    }
    total /= this->tracks.size();
    return total;
}
/***********************************************************************
 * Album.cpp : Data for a single album
 * part of FlacDemon
 ************************************************************************
 *  Copyright (c) 2016 Meriadoc Clarke.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

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
        this->setValueForKey( *iuuid , "albumuuid" );
    }
}
FlacDemon::Album::~Album(){
    this->tracks.clear();
}
void FlacDemon::Album::addTrackListing ( FlacDemon::TrackListing & track ){
    this->tracks.push_back(&track);
    this->trackCount = this->tracks.size();
    this->addMetaDataFromTrackListing( track );
    if( ! track.getTrackInfoForKey("verified") ){
        this->verified = false;
    }
}
void FlacDemon::Album::addMetaDataFromTrackListing( FlacDemon::TrackListing & track ) {
    std::string value, trackValue, key;
    for( fd_stringvector::iterator it = libraryTitlesAlbumCopyFromTrack->begin(); it != libraryTitlesAlbumCopyFromTrack->end(); it++ ) {
        key = (*it);
        trackValue = track.valueForKey( key );
        if( ! trackValue.length() ) continue;
        value = this->valueForKey( key );
        if( ! value.length() ) {
            if( trackValue != "-1" )
                this->setValueForKey( trackValue , key );
        } else if( trackValue != value && value != FlacDemonMetaDataMultipleValues ){
            this->setValueForKey( FlacDemonMetaDataMultipleValues , key );
        }
    }
}
std::string FlacDemon::Album::valueForKey( const std::string & key ) {
    std::string rvalue = "";
    if( this->metadata.count( key ) ) {
        rvalue = this->metadata.at( key );
    } else {
        if( key == "tracks" )
            rvalue = std::to_string( this->trackCount );
        else if( key == "playcount" )
            rvalue = std::to_string( this->playcount() );
        else if( key == "verified" )
            rvalue = std::to_string( (int) this->verified );
    }
    return rvalue;
}
void FlacDemon::Album::setValueForKey( const char * value , std::string & key ) {
    std::string strValue = value;
    this->setValueForKey( strValue, key );
}
void FlacDemon::Album::setValueForKey( std::string & value , const char * key ) {
    std::string strKey = key;
    this->setValueForKey(value, strKey);
}
void FlacDemon::Album::setValueForKey( std::string & value , std::string & key ) {
    if( this->metadata.count( key ) ) {
        this->metadata.at( key ) = value;
    } else {
        this->metadata.insert( std::make_pair( key , value ));
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
fd_keymap * FlacDemon::Album::getScraperKeymap(){
    fd_keymap * rmap = new fd_keymap();
    for(fd_keymap::iterator it = this->metadata.begin(); it != this->metadata.end(); it++){
        if( it->second != FlacDemonMetaDataMultipleValues ){
            rmap->insert(fd_keypair{ it->first, it->second });
        }
    }
    return rmap;
}
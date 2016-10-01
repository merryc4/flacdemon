/***********************************************************************
 * TrackListing.cpp : Metadata (artist, title, etc) for a single audio track
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

#include "TrackListing.h"

FlacDemon::TrackListing::TrackListing(){
    this->init();
}
FlacDemon::TrackListing::TrackListing(fd_keymap & ikeymap){
    this->keymap = ikeymap;
    if(this->keymap.count("filepath"))
        this->filepath = this->keymap.at("filepath");
    this->init();

}
FlacDemon::TrackListing::~TrackListing(){
    
}
void FlacDemon::TrackListing::init(){
    this->playCount = 0;
    this->dateAdded = 0;
    this->trackTime = 0;
    
    this->isSearchMatch = false;
    
    this->trackinfo = new std::map<std::string, long>{
        { "playcount" , 0 },
        { "dateadded" , 0 },
        { "tracktime" , 0 },
        { "verified" , 0 },
        { "errorflags" , 0 },
        { "track" , 0 },
        { "disc", 0 }
    };
    if( this->keymap.size() ){
        std::string key;
        for(std::map<std::string, long>::iterator it = this->trackinfo->begin(); it != this->trackinfo->end(); it++){
            key = it->first;
            if (this->keymap.count(key)) {
                std::string value = this->keymap.at(key);
                int ivalue;
                fd_stringtoint(value, &ivalue);
                if(ivalue >= 0)
                    this->trackinfo->at(key) = ivalue;
            }
        }
        this->uuid = new std::string( this->valueForKey( "albumuuid" ) );
    }
}

std::string FlacDemon::TrackListing::valueForKey( const std::string & key){
    std::string value = "";
    if( key.compare("filepath") == 0 ){
        value = this->filepath;
    } else
        value = this->keymapFileValue( key );
    
    if( ! value.length() ){
        value = std::to_string( this->getTrackInfoForKey( key ) );
    }
    return value;
}
std::string FlacDemon::TrackListing::keymapFileValue( const std::string & key ){
    std::string value = "";
    if(this->trackinfo && this->trackinfo->count(key)){
        value = std::to_string( this->trackinfo->at( key ) );
        
    }
    else if(this->keymap.count( key )){
        value = this->keymap.at( key );
    }
    return value;
}
std::string FlacDemon::TrackListing::standardiseMetaValue(std::string & value, std::string & key){
    std::string rvalue = "";
    if(key.compare("disc") == 0){
        int ivalue = std::stoi( value );
        rvalue = std::to_string(ivalue);
    }
    return rvalue;
}
void FlacDemon::TrackListing::setValueForKey(std::string & value, std::string & key){
    
}
fd_keymap::iterator * FlacDemon::TrackListing::iterateMetadata( fd_keymap::iterator * it ) {
    //expects it to be a valid pointer
    if( it == nullptr ){
        it = new fd_keymap::iterator( this->keymap.begin() );
    } else {
        it->operator++();
        if( *it == this->keymap.end() ){
            delete it;
            it = nullptr;
        }
    }
    
    return it;
}

long FlacDemon::TrackListing::getTrackInfoForKey(const char * key){
    std::string tkey = key;
    return this->getTrackInfoForKey(tkey);
}
long FlacDemon::TrackListing::getTrackInfoForKey( const std::string & key ){
    if(this->trackinfo && this->trackinfo->count( key )){
        return this->trackinfo->at( key );
    }
    return -1;
}
void FlacDemon::TrackListing::setTrackInfoForKey(const char * key, long value){
    std::string tkey = key;
    this->setTrackInfoForKey(tkey, value);
}
void FlacDemon::TrackListing::setTrackInfoForKey(std::string & key, long value){
    this->trackinfo->at(key) = value;
}
bool FlacDemon::TrackListing::compareSearchStrings(std::vector < std::string > & sstrings, bool setMatch){
    std::string value;
    bool match = false;
    int matchCount = 0;
//    bool partMatch = false;
//    std::string::iterator strit;

    for ( std::vector < std::string > :: iterator it2 = sstrings.begin(); it2 != sstrings.end(); it2++){
//        partMatch = false;
        for ( fd_keymap::iterator it = this->keymap.begin(); it != this->keymap.end(); it++ ){
            value = it->second;
            if ( std::search(value.begin(), value.end(), (*it2).begin(), (*it2).end(), searchPredicate ) != value.end() ) {
                matchCount++;
                break;
            }
        }
    }
    if (matchCount >= sstrings.size()) {
        match = true;
    }
    if(setMatch)
        this->isSearchMatch = match;
    return match;
}
fd_keymap * FlacDemon::TrackListing::getKeymap(){
    return new fd_keymap( this->keymap );
}
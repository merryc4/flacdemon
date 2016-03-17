/***********************************************************************
 * Track.cpp : Data for a single audio track
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
    
    this->filepath = *file->filepath;
}
void FlacDemon::Track::initInfo(){
    this->trackinfo->at("dateadded") = time(nullptr);
    if( this->file ){
        this->trackinfo->at( "errorflags" ) = this->file->errorFlags;
    }
}
int FlacDemon::Track::openFilePath(){
    //will need to do more checks, path exists, relative pathname, file opened succesfully etc
    if( this->filepath.length() ){
        if(!this->file){
            this->file = new FlacDemon::File( &( this->filepath ) , false);
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
        value = this->keymap->at(*key);
    else if(this->file){
        if(key->compare("albumuuid") == 0)
            value = *this->file->albumuuid;
        else
            value = this->file->getMetaDataEntry( key );
    }
    return value;
}
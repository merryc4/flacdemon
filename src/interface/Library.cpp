/***********************************************************************
 * Library.cpp : Library data and management for interface
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

#include "Library.h"
#include "LibraryTitles.cpp"

FlacDemon::Library::Library(){
    this->searchDelayTime = -1;
    this->searching = false;
}
FlacDemon::Library::~Library(){
    
}
void FlacDemon::Library::libraryUpdate(fd_keymap_vector * values){
    for(fd_keymap_vector::iterator it = values->begin(); it != values->end(); it++){
        this->addTrackListing(*it);
    }
}
void FlacDemon::Library::addTrackListing( FlacDemon::TrackListing *trackListing ){
    this->tracks.insert( std::pair<std::string, FlacDemon::TrackListing * >( trackListing->valueForKey( "id" ) , trackListing ));
    FlacDemon::Album * album = this->getOrCreateAlbum( trackListing->uuid );
    if( album ) {
        album->addTrackListing ( trackListing );
    }
}
void FlacDemon::Library::addTrackListing( fd_keymap *keymap ){
    std::string idKey("id");
    if(!keymap->count(idKey)){
        std::cout << "Error: Malformed keymap, has no ID" << std::endl;
        return;
    }
    std::string id = keymap->at(idKey);
    if(this->tracks.count(id)){
        //update
    } else {
        FlacDemon::TrackListing * trackListing = new FlacDemon::TrackListing(keymap);
        this->addTrackListing( trackListing );
    }
}
FlacDemon::Album * FlacDemon::Library::getOrCreateAlbum ( std::string * albumuuid ) {
    if( albumuuid == nullptr ) return nullptr;
    FlacDemon::Album * album = nullptr;
    if( this->albums.count( *albumuuid ) ) {
        album = this->albums.at( *albumuuid );
    } else {
        FlacDemon::Album * album = new FlacDemon::Album( albumuuid );
        this->albums.insert( std::make_pair( album->uuid , album ) );
    }
    return album;
}
void FlacDemon::Library::sort( std::string sortKey , FlacDemonListingMode iListingMode ){
    if( sortKey.length() ) {
        this->currentSortKey = sortKey;
    }
    
    this->sortedTracks.clear();
    for( std::map < std::string, FlacDemon::TrackListing * > :: iterator it = this->tracks.begin(); it != this->tracks.end(); it++){
        this->sortedTracks.push_back(it->second);
    }
    TrackSorter < FlacDemon::TrackListing * > tracksorter( sortKey );
    std::sort(this->sortedTracks.begin(), this->sortedTracks.end(), tracksorter);
    
    this->sortedAlbums.clear();
    for( std::map < std::string, FlacDemon::Album * > :: iterator it = this->albums.begin(); it != this->albums.end(); it++){
        this->sortedAlbums.push_back(it->second);
    }
    TrackSorter < FlacDemon::Album * > albumsorter ( sortKey );
    std::sort( this->sortedAlbums.begin() , this->sortedAlbums.end() , albumsorter );
    
    if( iListingMode ) { //none default
        this->listingMode = iListingMode;
    }
    this->sortedListings.clear();
    switch ( this->listingMode ) {
        case FlacDemonListingModeDefault:
        case FlacDemonListingModeTracks:
            this->sortedListings.insert( this->sortedListings.begin(), this->sortedTracks.begin(), this->sortedTracks.end() );
            break;
        case FlacDemonListingModeAlbums:
            this->sortedListings.insert( this->sortedListings.begin(), this->sortedAlbums.begin(), this->sortedAlbums.end() );
            break;
    }
}
void FlacDemon::Library::search( fd_stringvector terms ){
    if( ! this->setSearchTerms( terms ) )
        return;
    this->searching = true;
    bool newSearch = ( this->searchDelayTime <= 0 );
    this->setSearchDelayTime(FLACDEMON_LIBRARY_SEARCH_DELAY);
    if( newSearch ){
        this->searchThread = new std::thread(&FlacDemon::Library::startSearchThread, this);
    }
}
void FlacDemon::Library::startSearchThread(){
    int sleepTime = 10;
    while (this->searchDelayTime > 0) {
        usleep(sleepTime * 100);
        this->setSearchDelayTime(this->searchDelayTime - 10);
    }
    this->runSearchThread();
}
void FlacDemon::Library::runSearchThread(){
    this->searchMutex.lock();
    for(std::map < std::string , FlacDemon::TrackListing * >::iterator it = this->tracks.begin(); it != this->tracks.end(); it++){
        it->second->compareSearchStrings(&this->searchTerms, true);
    }
    this->searching = false;
    this->searchTerms.clear();
    this->searchMutex.unlock();
}
void FlacDemon::Library::setSearchDelayTime(int time){
    this->searchMutex.lock();
    this->searchDelayTime = time;
    this->searchMutex.unlock();
}
bool FlacDemon::Library::setSearchTerms( fd_stringvector terms ){
    this->searchMutex.lock();
    bool diff = (this->searchTerms != terms);
    this->searchTerms = terms;
    this->searchMutex.unlock();
    return diff;
}
FlacDemon::TrackListing * FlacDemon::Library::trackListingForID(std::string ID){
    if(this->tracks.count(ID) == 0){
        return nullptr;
    }
    return this->tracks.at(ID);
}
fd_tracklistingvector & FlacDemon::Library::allTracks(){
    return this->sortedTracks;
}
fd_albumvector & FlacDemon::Library::allAlbums(){
    return this->sortedAlbums;
}
fd_librarylistingvector & FlacDemon::Library::allListings( FlacDemonListingMode iListingMode ) {
    
    return this->sortedListings;
}
size_t FlacDemon::Library::count(){
    return this->tracks.size();
}
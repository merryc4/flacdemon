//
//  Library.cpp
//  FlacDemon
//
//  Created by merryclarke on 12/02/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "Library.h"

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
void FlacDemon::Library::addTrackListing( FlacDemon::TrackListing *tracklisting ){
    
}
void FlacDemon::Library::addTrackListing( fd_keymap *keymap ){
    std::string idKey("id");
    if(!keymap->count(idKey)){
        std::cout << "Error: Malformed keymap, has no ID" << std::endl;
        return;
    }
    std::string * id = keymap->at(idKey);
    if(this->tracks.count(*id)){
        //update
    } else {
        FlacDemon::TrackListing * trackListing = new FlacDemon::TrackListing(keymap);
        this->tracks.insert(std::pair<std::string, FlacDemon::TrackListing * >(*id, trackListing));
    }
}
void FlacDemon::Library::sort( std::string sortKey ){
    this->currentSortKey = sortKey;
    this->sortedTracks.clear();
    for( std::map < std::string, FlacDemon::TrackListing * > :: iterator it = this->tracks.begin(); it != this->tracks.end(); it++){
        this->sortedTracks.push_back(it->second);
    }
    TrackSorter tracksorter( sortKey );
    std::sort(this->sortedTracks.begin(), this->sortedTracks.end(), tracksorter);
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
fd_tracklistingvector * FlacDemon::Library::allTracks(){
    return &this->sortedTracks;
}
size_t FlacDemon::Library::count(){
    return this->tracks.size();
}
//
//  Library.cpp
//  FlacDemon
//
//  Created by merryclarke on 12/02/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "Library.h"

FlacDemon::Library::Library(){
    this->sortKeys = fd_stringvector {"genre", "composer", "artist", "albumartist", "album", "disc", "track"};
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
    TrackSorter tracksorter;
    tracksorter.currentSortKey = sortKey;
    tracksorter.sortKeys = &this->sortKeys;
    std::sort(this->sortedTracks.begin(), this->sortedTracks.end(), tracksorter);
}
void FlacDemon::Library::search(std::string searchString){
    fd_stringvector components = fd_splitstring(&searchString, " ");
    for(std::map < std::string , FlacDemon::TrackListing * >::iterator it = this->tracks.begin(); it != this->tracks.end(); it++){
        it->second->compareSearchStrings(&components, true);
    }
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
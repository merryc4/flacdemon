//
//  Album.cpp
//  FlacDemon
//
//  Created by merryclarke on 02/07/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "Album.h"

FlacDemon::Album::Album(){
    this->tracks = new std::vector<FlacDemon::Track*>;
    this->trackCount = 0;
    this->multipleArtists = false;
    this->metadataInconsistency = false;
}
FlacDemon::Album::~Album(){
    this->tracks->clear();
    delete this->tracks;
}
void FlacDemon::Album::addTrack(FlacDemon::Track* track){
    this->tracks->push_back(track);
    this->trackCount = this->tracks->size();
}
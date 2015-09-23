//
//  Track.cpp
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "TrackFile.h"

FlacDemon::Track::Track(FlacDemon::File* file){
    this->playCount = 0;
    this->timeAdded = 0;
    this->trackTime = 0;
    if(file)
        this->setFile(file);
}
FlacDemon::Track::~Track(){

}
void FlacDemon::Track::setFile(FlacDemon::File * file){
    this->file = file;
}
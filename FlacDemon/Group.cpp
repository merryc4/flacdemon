//
//  Group.cpp
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "Group.h"

FlacDemon::Group::Group(string * iTitle, string * iFilepath, string * iTypeKey) {
    title = new string(*iTitle);
    typeKey = new string(*iTypeKey);
    filepath = new string(*iFilepath);
    
    flags = 0;
    
    tracks = new vector<FlacDemon::Track*>();
    groups = new vector<FlacDemon::Group*>();
}
FlacDemon::Group::~Group() {
    //remove all tracks groups
    
    delete title;
    delete typeKey;
    delete filepath;
    delete tracks;
    delete groups;
}

void FlacDemon::Group::add (FlacDemon::Track* track){
    if(this->contains(track))
        return;
    this->tracks->push_back(track);
//    for(vector<FlacDemon::Group*>::iterator it = track->groups->begin(); it != track->groups->end(); it++){
//        
//    }
    
}
void FlacDemon::Group::add (FlacDemon::Group* group){
    
}

void FlacDemon::Group::remove (FlacDemon::Track* track){
    
}
void FlacDemon::Group::remove (FlacDemon::Group* group){
    
}

bool FlacDemon::Group::contains (FlacDemon::Track* track){
    
    return false;
    
}
bool FlacDemon::Group::contains (FlacDemon::Group* group){
    
    return false;
}

void FlacDemon::Group::removeFromGroups(){
    
}
unsigned int FlacDemon::Group::trackCount(){
    
    return 0;
}

vector<FlacDemon::Track*> * FlacDemon::Group::allTracks(){
    
    return nullptr;
}
vector<FlacDemon::Group*> * FlacDemon::Group::allGroups(const char* key){
    
    return nullptr;
}
/***********************************************************************
 * Group.cpp : Group of Tracks
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
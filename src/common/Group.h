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

#ifndef __FlacDemon__Group__
#define __FlacDemon__Group__

#include <iostream>
#include <vector>
#include "Track.h"

#include "FlacDemonAll.h"

#define GroupTypeKeyArray ["Genre", "Composer", "Artist", "Album Artist", "Album", "Other"]
#define GroupTypeUKeyArray ["GENRE", "COMPOSER", "ARTIST", "ALBUMARTIST", "ALBUM", "OTHER"]


using namespace std;

class FlacDemon::Group {
protected:
    string * title;
    string * typeKey;
    string * filepath;
    uint flags;
    vector<FlacDemon::Track*> * tracks; //pointers?
    vector<FlacDemon::Group*> * groups;
    
    Group(string * iTitle = nullptr, string * iFilepath = nullptr, string * iTypeKey = nullptr);
    ~Group();
    
public:
    void add (FlacDemon::Track* track);
    void add (FlacDemon::Group* group);
    
    void remove (FlacDemon::Track* track);
    void remove (FlacDemon::Group* group);
    
    bool contains (FlacDemon::Track* track);
    bool contains (FlacDemon::Group* group);
    
    void removeFromGroups();
    unsigned int trackCount();
    
    vector<FlacDemon::Track*> * allTracks();
    vector<FlacDemon::Group*> * allGroups(const char* key=nullptr);
    
    //vector / arrays of tracks / groups
    
};

#endif /* defined(__FlacDemon__Group__) */

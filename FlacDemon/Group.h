//
//  Group.h
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__Group__
#define __FlacDemon__Group__

#include <iostream>
#include <vector>
#include "Track.h"

#include "FlacDemonNameSpace.h"

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
    
    Group(string * iTitle = NULL, string * iFilepath = NULL, string * iTypeKey = NULL);
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

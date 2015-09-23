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
#define GroupTypeKeyArray ["Genre", "Composer", "Artist", "Album Artist", "Album", "Other"]
#define GroupTypeUKeyArray ["GENRE", "COMPOSER", "ARTIST", "ALBUMARTIST", "ALBUM", "OTHER"]

using namespace std;

class FlacDemon::Group {
protected:
    const char* title;
    const char* typeKey;
    vector<FlacDemon::Track> tracks; //pointers?
    vector<FlacDemon::Group> groups;
    
public:
    void add (FlacDemon::Track* track);
    void add (FlacDemon::Group* group);
    
    void remove (FlacDemon::Track* track);
    void remove (FlacDemon::Group* group);
    
    void contains (FlacDemon::Track* track);
    void contains (FlacDemon::Group* group);
    
    void removeFromGroups();
    unsigned int trackCount();
    
    FlacDemon::Track* allTracks();
    FlacDemon::Group* allGroups(const char* key=nullptr);
    
    //vector / arrays of tracks / groups
    
};

#endif /* defined(__FlacDemon__Group__) */

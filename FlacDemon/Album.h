//
//  Album.h
//  FlacDemon
//
//  Created by merryclarke on 02/07/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__Album__
#define __FlacDemon__Album__

#include <stdio.h>
#include "FlacDemonAll.h"

class FlacDemon::Album {
    
private:
    
public:
    string* path;
    vector<FlacDemon::Track*> * tracks;
    unsigned long trackCount;
    
    bool multipleArtists;
    bool metadataInconsistency;
    
    Album();
    ~Album();
    
    void addTrack(FlacDemon::Track*);
    
    //playcount, date added, various flags, artist info (from scraper)
};

#endif /* defined(__FlacDemon__Album__) */

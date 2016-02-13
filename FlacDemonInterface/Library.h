//
//  Library.h
//  FlacDemon
//
//  Created by merryclarke on 12/02/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef __FlacDemon__Library__
#define __FlacDemon__Library__

#include <stdio.h>
#include "includes.h"
#include "FlacDemonNameSpace.h"
#include "typedefs.h"
#include "FlacDemonUtils.h"
#include "TrackListing.h"
#include "Group.h"
#include "TrackSorter.h"

#include <algorithm>

#define FLACDEMON_LIBRARY_SEARCH_DELAY 3000

class FlacDemon::Library{
private:
    std::map < std::string, FlacDemon::TrackListing * > tracks;
    fd_tracklistingvector sortedTracks;
    std::string currentSortKey;
    
    std::mutex searchMutex;
    
    std::thread * searchThread;
    int searchDelayTime;
    std::string searchString;
    
    void startSearchThread();
    void runSearchThread();
    void setSearchDelayTime(int time);
    void setSearchString(std::string search);
    
protected:
public:
    bool searching;

    Library();
    ~Library();

    void libraryUpdate( fd_keymap_vector * values );
    void addTrackListing( FlacDemon::TrackListing * tracklisting );
    void addTrackListing( fd_keymap * keymap );
    void sort( std::string sortKey );
    void search(std::string search);
    
    FlacDemon::TrackListing * trackListingForID(std::string ID);
    fd_tracklistingvector * allTracks();
    size_t count();

};


#endif /* defined(__FlacDemon__Library__) */

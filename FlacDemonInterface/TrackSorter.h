//
//  TrackSorter.h
//  FlacDemon
//
//  Created by merryclarke on 13/02/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#ifndef FlacDemon_TrackSorter_h
#define FlacDemon_TrackSorter_h

#include "includes.h"
#include "typedefs.h"
#include "TrackListing.h"
#include "FlacDemonUtils.h"

class TrackSorter {
public:
    std::string currentSortKey;
    std::string sortKey;
    
    TrackSorter( std::string sortKey );
    bool operator() ( FlacDemon::TrackListing * track1, FlacDemon::TrackListing * track2 );
};

#endif

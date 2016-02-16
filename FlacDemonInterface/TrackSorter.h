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

template < class T > class TrackSorter {
public:
    std::string currentSortKey;
    
    TrackSorter( std::string sortKey );
    
    bool operator()( T a, T b );
};

#endif

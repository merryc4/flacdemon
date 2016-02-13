//
//  TrackSorter.cpp
//  FlacDemon
//
//  Created by merryclarke on 13/02/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "TrackSorter.h"
fd_stringvector * sortKeys = new fd_stringvector {"genre", "composer", "albumartist", "album", "disc", "track"};

TrackSorter::TrackSorter(std::string sortKey){
    if(sortKey.compare("artist") == 0)
        sortKey = "albumartist";
    this->currentSortKey = sortKey;
}

bool TrackSorter::operator()( FlacDemon::TrackListing * track1, FlacDemon::TrackListing * track2 ){
    bool isDiff = false, nextKey = true;
    int diff;
    
    for( fd_stringvector::iterator it = std::find(sortKeys->begin(), sortKeys->end(), this->currentSortKey); it != sortKeys->end() && !isDiff && nextKey; it++){
        sortKey = (*it);
        if( ! (isDiff = ( ( diff = fd_strnumbercompare( track1->valueForKey(&sortKey) , track2->valueForKey(&sortKey) ) ) < 0 )) ){
            nextKey = (diff == 0);
        }
    }
    return isDiff;
};
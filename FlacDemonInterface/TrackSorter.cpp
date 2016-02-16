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
        std::string value1, value2;
        if( ! (isDiff = ( ( diff = fd_strnumbercompare( &( value1 = track1->valueForKey( &sortKey ) ) , &( value2 = track2->valueForKey( &sortKey ) ) ) ) < 0 )) ){
            nextKey = (diff == 0);
        }
    }
    return isDiff;
};
//
//  TrackSorter.cpp
//  FlacDemon
//
//  Created by merryclarke on 13/02/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

#include "TrackSorter.h"
static fd_stringvector * sortKeys = new fd_stringvector {"genre", "composer", "albumartist", "album", "disc", "track"};

template < class T >
TrackSorter < T > :: TrackSorter(std::string sortKey){
    if(sortKey.compare("artist") == 0)
        sortKey = "albumartist";
    this->currentSortKey = sortKey;
}


template < class T >
bool TrackSorter < T >::operator()( T a, T b ){
    bool isDiff = false, nextKey = true;
    int diff;
    std::string sortKey;
    
    for( fd_stringvector::iterator it = std::find(sortKeys->begin(), sortKeys->end(), this->currentSortKey); it != sortKeys->end() && !isDiff && nextKey; it++){
        sortKey = (*it);
        std::string value1, value2;
        if( ! (isDiff = ( ( diff = fd_strnumbercompare( &( value1 = a->valueForKey( &sortKey ) ) , &( value2 = b->valueForKey( &sortKey ) ) ) ) < 0 )) ){
            nextKey = (diff == 0);
        }
    }
    return isDiff;
};


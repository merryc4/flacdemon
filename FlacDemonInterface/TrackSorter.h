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

class TrackSorter {
public:
    std::string currentSortKey;
    fd_stringvector * sortKeys;
    std::string sortKey;
    
    TrackSorter() {};
    bool operator() ( FlacDemon::TrackListing * track1, FlacDemon::TrackListing * track2 ) {
        std::string value1, value2;
        bool isDiff = false, nextKey = true;
        int diff;
        
        for( fd_stringvector::iterator it = std::find(this->sortKeys->begin(), this->sortKeys->end(), this->currentSortKey); it != this->sortKeys->end() && !isDiff && nextKey; it++){
            sortKey = (*it);
            value1 = *track1->valueForKey(&sortKey);
            value2 = *track2->valueForKey(&sortKey);
            if( ! (isDiff = ( ( diff = fd_strnumbercompare(&value1, &value2) ) < 0 )) ){
                nextKey = (diff == 0);
            }
            //          isDiff = ( *track1->valueForKey(&sortKey) < *track2->valueForKey(&sortKey));
        }
        return isDiff;
    };
};

#endif

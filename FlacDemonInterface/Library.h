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

#include <algorithm>

class FlacDemon::Library{
private:
    std::map < std::string, FlacDemon::TrackListing * > tracks;
    fd_tracklistingvector sortedTracks;
    fd_stringvector sortKeys;
    std::string currentSortKey;
protected:
public:
    Library();
    ~Library();

    void libraryUpdate( fd_keymap_vector * values );
    void addTrackListing( FlacDemon::TrackListing * tracklisting );
    void addTrackListing( fd_keymap * keymap );
    void sort( std::string sortKey );
    void search(std::string searchString);

    
    FlacDemon::TrackListing * trackListingForID(std::string ID);
    fd_tracklistingvector * allTracks();

};
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

#endif /* defined(__FlacDemon__Library__) */

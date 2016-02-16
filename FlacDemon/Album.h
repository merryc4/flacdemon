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
#include "includes.h"
#include "typedefs.h"
#include "TrackListing.h"
#include "LibraryTitles.h"

class FlacDemon::Album {
    
private:
    void addMetaDataFromTrackListing( FlacDemon::TrackListing * track );
    
public:
    std::string path;
    std::string uuid;
    fd_tracklistingvector tracks;
    unsigned long trackCount;
    fd_keymap metadata;
    
    bool multipleArtists;
    bool metadataInconsistency;
    
    Album( std::string * iuuid = nullptr );
    ~Album();
    
//    void addTrack(FlacDemon::Track*);
    void addTrackListing( FlacDemon::TrackListing * );
    std::string valueForKey( std::string * key );
    void setValueForKey( const char * value , std::string * key );
    void setValueForKey( std::string * value , const char * key );
    void setValueForKey( std::string * value , std::string * key );
    
    bool matchesSearch();
    //playcount, date added, various flags, artist info (from scraper)
};

#endif /* defined(__FlacDemon__Album__) */

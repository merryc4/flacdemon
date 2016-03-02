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
#include "TrackSorter.cpp" //needed due to templates
#include "Album.h"
#include "LibraryTitles.h"
#include <algorithm>

#define FLACDEMON_LIBRARY_SEARCH_DELAY 3000

typedef enum {
    FlacDemonListingModeDefault,
    FlacDemonListingModeTracks,
    FlacDemonListingModeAlbums
} FlacDemonListingMode;

class FlacDemon::Library{
private:
    std::map < std::string, FlacDemon::TrackListing * > tracks;
    std::map < std::string, FlacDemon::Album * > albums;
    fd_tracklistingvector sortedTracks;
    fd_albumvector sortedAlbums;
    fd_librarylistingvector sortedListings;
    
    std::string currentSortKey;
    
    std::mutex searchMutex;
    
    std::thread * searchThread;
    int searchDelayTime;
    fd_stringvector searchTerms;
    
    void startSearchThread();
    void runSearchThread();
    void setSearchDelayTime(int time);
    bool setSearchTerms( fd_stringvector terms );
    
protected:
public:
    
    Library();
    ~Library();
    
    bool searching;
    FlacDemonListingMode listingMode;

    void libraryUpdate( fd_keymap_vector * values );
    void addTrackListing( FlacDemon::TrackListing * trackListing );
    void addTrackListing( fd_keymap * keymap );
    FlacDemon::Album * getOrCreateAlbum( std::string * albumuuid );
//    FlacDemon::Album * createAlbum( std::string * albumuuid );
    void sort( std::string sortKey , FlacDemonListingMode iListingMode = FlacDemonListingModeDefault );
    void search( fd_stringvector terms );
    
    FlacDemon::TrackListing * trackListingForID(std::string ID);
    fd_tracklistingvector & allTracks();
    fd_albumvector & allAlbums();
    fd_librarylistingvector & allListings( FlacDemonListingMode iListingMode = FlacDemonListingModeDefault );
    size_t count();

};


#endif /* defined(__FlacDemon__Library__) */

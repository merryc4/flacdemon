/***********************************************************************
 * Library.h : Library data and management for interface
 * part of FlacDemon
 ************************************************************************
 *  Copyright (c) 2016 Meriadoc Clarke.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

#ifndef __FlacDemon__Library__
#define __FlacDemon__Library__

#include <stdio.h>
#include "includes.h"
#include "FlacDemonNameSpace.h"
#include "typedefs.h"
#include "FlacDemonUtils.h"
#include "TrackListing.h"
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

    void libraryUpdate( fd_keymap_vector & values );
    void addTrackListing( FlacDemon::TrackListing & trackListing );
    void addTrackListing( fd_keymap & keymap );
    FlacDemon::Album * getOrCreateAlbum( std::string & albumuuid );
//    FlacDemon::Album * createAlbum( std::string * albumuuid );
    void sort( std::string sortKey = ""  , FlacDemonListingMode iListingMode = FlacDemonListingModeDefault );
    void search( fd_stringvector terms );
    
    FlacDemon::TrackListing * trackListingForID(std::string & Id);
    fd_tracklistingvector & allTracks();
    fd_albumvector & allAlbums();
    fd_librarylistingvector & allListings( FlacDemonListingMode iListingMode = FlacDemonListingModeDefault );
    size_t count();

};


#endif /* defined(__FlacDemon__Library__) */

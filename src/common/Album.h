/***********************************************************************
 * Album.h : Data for a single album
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
#ifndef __FlacDemon__Album__
#define __FlacDemon__Album__

#include "includes.h"
#include "typedefs.h"
#include "globals.h"
#include "TrackListing.h"
#include "LibraryTitles.h"

class FlacDemon::Album : public FlacDemon::LibraryListing {
    
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
    
    bool verified;
    
    Album( std::string * iuuid = nullptr );
    ~Album();
    
    void addTrackListing( FlacDemon::TrackListing * );
    std::string valueForKey( std::string * key );
    using FlacDemon::LibraryListing::valueForKey;

    void setValueForKey( const char * value , std::string * key );
    void setValueForKey( std::string * value , const char * key );
    void setValueForKey( std::string * value , std::string * key );
    
    bool matchesSearch();
    float playcount();
    //playcount, date added, various flags, artist info (from scraper)
};

#endif /* defined(__FlacDemon__Album__) */

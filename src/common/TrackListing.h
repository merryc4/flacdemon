/***********************************************************************
 * TrackListing.h : Metadata (artist, title, etc) for a single audio track
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
#ifndef __FlacDemon__TrackListing__
#define __FlacDemon__TrackListing__

#include "includes.h"
#include "FlacDemonNameSpace.h"
#include "typedefs.h"
#include "FlacDemonUtils.h"
#include "LibraryListing.h"

class FlacDemon::TrackListing : public FlacDemon::LibraryListing {
private:
protected:
    void init();
    time_t dateAdded;
    unsigned int playCount;
    time_t trackTime;
    
    std::string filepath;
    std::map<std::string, long> * trackinfo = nullptr;
    fd_keymap * keymap = nullptr;
    
    
public:
    std::string * uuid = nullptr;

    TrackListing();
    TrackListing (fd_keymap * keymap);
    ~TrackListing();
    
//    std::string valueForKey (const char * key);
    using FlacDemon::LibraryListing::valueForKey;
    std::string valueForKey (std::string * key);
    
    virtual std::string keymapFileValue(std::string * key);
    
    void setValueForKey(std::string * value, std::string* key);
    fd_keymap::iterator * iterateMetadata( fd_keymap::iterator * it );
    std::string standardiseMetaValue(std::string * value, std::string * key);
    
    long getTrackInfoForKey(const char * key);
    long getTrackInfoForKey(std::string * key);
    
    void setTrackInfoForKey(const char * key, long value);
    void setTrackInfoForKey(std::string * key, long value);
    
    bool compareSearchStrings(std::vector < std::string > * sstrings, bool setMatch = false);
};

#endif /* defined(__FlacDemon__TrackListing__) */

//
//  TrackListing.h
//  FlacDemon
//
//  Created by merryclarke on 28/01/2016.
//  Copyright (c) 2016 c4software. All rights reserved.
//

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

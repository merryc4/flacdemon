//
//  Track.h
//  FlacDemon
//
//  Created by merryclarke on 15/06/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__Track__
#define __FlacDemon__Track__

#include <iostream>
#include <time.h>
#include "FlacDemonNameSpace.h"
#include "File.h"

class FlacDemon::Track {
    
protected:
    time_t dateAdded;
    unsigned int playCount;
    time_t trackTime;
    
    std::string * filepath;
    
    std::map<std::string, long> * trackinfo;
    
public:
    string * uuid;
    FlacDemon::File* file;
    struct MediaStreamInfo * mediaStreamInfo;

    Track (FlacDemon::File* file = NULL);
    ~Track();
    
    void incrementPlaycount ();
    void decrementPlaycount ();
    
    const char * valueForKey (std::string * key);
    void setValueForKey(std::string * value, std::string* key);

    long getTrackInfoForKey(const char * key);
    long getTrackInfoForKey(std::string * key);
    
    void setFile(FlacDemon::File*);
    void setValuesFromAudioContext(AVCodecContext*);
    
    void initInfo();
    
//    void addGroup(&Group group);
//    void removeGroup(&Group group);
    
};

#endif /* defined(__FlacDemon__Track__) */

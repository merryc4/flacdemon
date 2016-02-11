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
#include "FlacDemonAll.h"
#include "File.h"
#include "TrackListing.h"

class FlacDemon::Track: public FlacDemon::TrackListing {
protected:
    
public:
    friend FlacDemon::File;
//    using FlacDemon::TrackListing::valueForKey;
    
    FlacDemon::File* file = nullptr;
    struct MediaStreamInfo * mediaStreamInfo = nullptr;

    Track (FlacDemon::File* file = nullptr);
    Track (fd_keymap * keymap);
    ~Track();
    
    void incrementPlaycount ();
    void decrementPlaycount ();
    
    void setFile(FlacDemon::File*);
    void setValuesFromAudioContext(AVCodecContext*);
    
    void initInfo();
    int openFilePath();
    
};

#endif /* defined(__FlacDemon__Track__) */

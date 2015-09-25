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
#include "FlacDemonNameSpace.h"
#include "File.h"

class FlacDemon::Track {
    
protected:
    time_t timeAdded;
    unsigned int playCount;
    time_t trackTime;

    
public:
    FlacDemon::File* file;
    struct MediaStreamInfo * mediaStreamInfo;

    Track (FlacDemon::File* file = NULL);
    ~Track();
    
    void incrementPlaycount ();
    void decrementPlaycount ();
    
    template <class KValue>
    KValue valueForKey (const char* key);
    
    template <class KValue>
    void setValueForKey(KValue value, char* key);
    
    void setFile(FlacDemon::File*);
    void setValuesFromAudioContext(AVCodecContext*);
    
//    void addGroup(&Group group);
//    void removeGroup(&Group group);
    
};

template <class KValue>
KValue FlacDemon::Track::valueForKey(const char* key){
    if(this->file){
        return this->file->getMetaDataEntry(key);
    }
    return NULL;
}

template <class KValue>
void FlacDemon::Track::setValueForKey(KValue value, char *key){
    
}

#endif /* defined(__FlacDemon__Track__) */

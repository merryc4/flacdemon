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

class FlacDemon::Track {
    
protected:
    char* fileLocation;
    time_t timeAdded;
    unsigned int playCount;
    time_t trackTime;
    
    AVCodecID codecID;
    string* codecName;
    int bitRate;
    //flags?
    int sampleRate;
    int channels;
    //groups
    
public:
    Track (char *fileLocation);
    
    void incrementPlaycount ();
    void decrementPlaycount ();
    
    template <class KValue>
    KValue valueForKey (char* key);
    
    template <class KValue>
    void setValueForKey(KValue value, char* key);
    
    void setValuesFromAudioContext(AVCodecContext*);
    
//    void addGroup(&Group group);
//    void removeGroup(&Group group);
    
};

template <class KValue>
KValue FlacDemon::Track::valueForKey(char* key){
    
}

template <class KValue>
void FlacDemon::Track::setValueForKey(KValue value, char *key){
    
}

#endif /* defined(__FlacDemon__Track__) */

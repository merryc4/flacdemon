//
//  Player.h
//  FlacDemon
//
//  Created by merryclarke on 28/09/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__Player__
#define __FlacDemon__Player__

#include <stdio.h>
#include <ao/ao.h>
#include "FlacDemonNameSpace.h"
#include "Database.h"

class FlacDemon::Player {
private:
    FlacDemon::Database * database;
    int defaultDriverID = -1;
    ao_device * device = NULL;
    ao_sample_format sampleFormat = {
        16,
        44100,
        2,
        AO_FMT_NATIVE, //revise whether we need to specify this
        0
    };
    
    std::thread * playerThread = NULL;
    int killPlaybackFlag = 0;
protected:
    
public:
    Player();
    ~Player();
    
    void stop();
    
    void setDatabase(FlacDemon::Database * database);
    void playTrackWithID(long ID);
    void playTrack(FlacDemon::Track * track);
    void playAudio(FlacDemon::Track * track, AVCodecContext * codecContext, AVPacket * packet, AVFrame * frame, int planar);
    void stopAudio();
    
    void startInterleave();
};

#endif /* defined(__FlacDemon__Player__) */

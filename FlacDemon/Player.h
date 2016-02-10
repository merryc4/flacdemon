//
//  Player.h
//  FlacDemon
//
//  Created by merryclarke on 28/09/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#ifndef __FlacDemon__Player__
#define __FlacDemon__Player__

#include "includes.h"
#include "avincludes.h"
#include <ao/ao.h>
#include "FlacDemonAll.h"
#include "Database.h"

class FlacDemon::Player {
private:
    FlacDemon::Database * database;
    SwrContext * audioResampleContext = nullptr;
    std::thread * playerThread = nullptr;
    std::mutex playTrackMutex;
    std::mutex playAudioMutex;
    FlacDemon::Track * currentTrack;
    
    int killPlaybackFlag = 0;
    bool audioPlaying = 0;
    int defaultDriverID = -1;
    float playbackProgress;
    float playbackDuration;
    ao_device * device = nullptr;
    ao_sample_format sampleFormat = {
        16,
        44100,
        2,
        AO_FMT_NATIVE, //revise whether we need to specify this
        0
    };
    
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
    float getProgress();
    uint8_t * interleave(AVFrame * frame, uint * size);
};

#endif /* defined(__FlacDemon__Player__) */

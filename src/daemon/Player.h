/***********************************************************************
 * Player.h : Audio playback
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
    std::thread * playbackUpdateThread;
    std::mutex playTrackMutex;
    std::mutex playAudioMutex;
    std::mutex updateMutex;
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
    uint8_t * interleave(AVFrame * frame, uint * size);
    void runPlaybackUpdateThread();
    float getProgress();

};

#endif /* defined(__FlacDemon__Player__) */

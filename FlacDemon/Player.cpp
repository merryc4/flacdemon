//
//  Player.cpp
//  FlacDemon
//
//  Created by merryclarke on 28/09/2015.
//  Copyright (c) 2015 c4software. All rights reserved.
//

#include "Player.h"

FlacDemon::Player::Player(){
    this->playerThread = nullptr;
    ao_initialize();
    this->defaultDriverID = ao_default_driver_id();
    this->playbackProgress = 0;
    this->playbackDuration = 1;
    cout << "default driver: " << defaultDriverID << endl;
}
FlacDemon::Player::~Player(){
    
}
void FlacDemon::Player::setDatabase(FlacDemon::Database * idatabase){
    this->database = idatabase;
}
void FlacDemon::Player::stop(){
    this->killPlaybackFlag = 1;
}
void FlacDemon::Player::playTrackWithID(long ID){
    FlacDemon::Track * track = this->database->trackForID(ID);
    if(track){
        this->playTrack(track);
    }
}
void FlacDemon::Player::playTrack(FlacDemon::Track * track){
    if(!track->openFilePath() || !track->file->isMediaFile()){
        return;
    }
    if(!this->playTrackMutex.try_lock()){
        return;
    }
        
    this->stopAudio();
    this->killPlaybackFlag = 0;
    
    /* find the mpeg audio decoder */
    AVCodec * codec = avcodec_find_decoder(track->file->codecID);
    if (!codec) {
        cout << "Codec not found" << endl;
        return;
    }
    AVCodecContext * codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        cout << "Could not allocate audio codec context" << endl;
        return;
    }
    /* open it */
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        cout << "could not open codec" << endl;

    }
    av_dump_format(track->file->formatContext, 0, "", 0);
    int bits=16;
    int planar = 0;
    switch (codecContext->sample_fmt) {
        case AV_SAMPLE_FMT_U8P:
            planar = 1;
        case AV_SAMPLE_FMT_U8:
            bits = 8;
            break;
            
        case AV_SAMPLE_FMT_S16P:
            planar = 1;
        case AV_SAMPLE_FMT_S16:
            bits = 16;
            break;
            
        case AV_SAMPLE_FMT_S32P:
            planar = 1;
        case AV_SAMPLE_FMT_S32:
            bits = 32;
            break;
            
        case AV_SAMPLE_FMT_NONE:
            break;
        default:
            cout << "unsupported sample format" <<endl;
            // should clean up before return;
            return;
            break;
    }
    this->sampleFormat.rate = (int)track->file->mediaStreamInfo->sampleRate;
    this->sampleFormat.bits = bits;
    
    AVFrame * frame = av_frame_alloc();
    AVPacket * packet = new AVPacket;
    av_init_packet(packet);
    packet->data = nullptr;
    packet->size = 0;
    
    this->playbackProgress = 0;
    this->playbackDuration = track->file->formatContext->duration / AV_TIME_BASE;
    this->currentTrack = track;

    this->playerThread = new std::thread(&FlacDemon::Player::playAudio, this, track, codecContext, packet, frame, planar);
    signalHandler->call("playingTrack", track);
    this->playTrackMutex.unlock();
}

void FlacDemon::Player::playAudio(FlacDemon::Track * track, AVCodecContext * codecContext, AVPacket * packet, AVFrame * frame, int planar){
    if(!this->playAudioMutex.try_lock())
        return;
    
    cout << "playing audio" << endl;
    int error, endoffile=0, gotFrame=0, samplelength=0;
    
    this->device = ao_open_live(this->defaultDriverID, &this->sampleFormat, nullptr);
    

    while(endoffile == 0 && error >=0 && this->killPlaybackFlag == 0){
        if((error = av_read_frame(track->file->formatContext, packet)) < 0){
            if(error == AVERROR_EOF){
                endoffile = 1;
            } else {
                cout << "error reading frame from file " << track->valueForKey("filepath") << endl;
                break;
            }
        }
        
        
        
        if((error = avcodec_decode_audio4(codecContext, frame, &gotFrame, packet)) < 0){
            char errbuf[255];
            av_strerror(error, errbuf, sizeof(errbuf));
            cout << "could not read decode audio frame: " << *track->valueForKey("filepath") << ". " << errbuf << endl;
            break;
        }
//        cout << "got audio frame " << endl;
        samplelength = error;
        uint size = frame->linesize[0];
        uint8_t * samples;
        if(planar){
            samples = this->interleave(frame, &size);
        } else {
            samples = frame->extended_data[0];
        }
        if(ao_play(this->device, (char*)samples, size) == 0){
            cout << "ao play error" << endl;
            //close device
            error = -1;
        }
        if(planar){
            av_freep(&samples);
        }
        this->playbackProgress += (float)frame->nb_samples / frame->sample_rate;
//        cout << "playback time " << this->playbackProgress << endl;
//        cout << "playback progress " << (this->playbackProgress / this->playbackDuration) << endl;
    }
    
    ao_close(this->device);
    av_frame_free(&frame);
    this->audioPlaying = false;
    this->playAudioMutex.unlock();
}
void FlacDemon::Player::stopAudio(){
    this->killPlaybackFlag = 1;
    if(this->playerThread && this->playerThread->joinable()){
        this->playerThread->join();
    }
}
uint8_t * FlacDemon::Player::interleave(AVFrame * frame, uint * size){
    //unfinished feature
    if(!this->audioResampleContext){
        this->audioResampleContext = swr_alloc();
        
        
        
        av_opt_set_channel_layout(this->audioResampleContext, "in_channel_layout",  AV_CH_LAYOUT_STEREO, 0);
        av_opt_set_channel_layout(this->audioResampleContext, "out_channel_layout", AV_CH_LAYOUT_STEREO,  0);
        av_opt_set_int(this->audioResampleContext, "in_sample_rate",     this->sampleFormat.rate,0);
        av_opt_set_int(this->audioResampleContext, "out_sample_rate",    this->sampleFormat.rate,0);
        av_opt_set_sample_fmt(this->audioResampleContext, "in_sample_fmt",      AV_SAMPLE_FMT_S16P,   0);
        av_opt_set_sample_fmt(this->audioResampleContext, "out_sample_fmt",     AV_SAMPLE_FMT_S16,    0);
        int error;
        if((error = swr_init(this->audioResampleContext)) < 0){
            cout << "open resampler failed" << endl;
            //free avr
        }
    }
    uint8_t * output;
    int out_linesize,
    samples_written;

    
    int num_samples = (int)swr_get_delay(this->audioResampleContext, frame->sample_rate) + frame->linesize[0];
    
    av_samples_alloc(&output, &out_linesize, frame->channels, num_samples, AV_SAMPLE_FMT_S16, 1);
    
    samples_written = swr_convert(this->audioResampleContext, &output, out_linesize, (const uint8_t**)&frame->data[0], frame->linesize[0]);

    *size = out_linesize / frame->channels;
    
    return output;
}
float FlacDemon::Player::getProgress(){
    return this->playbackProgress / this->playbackDuration;
}
